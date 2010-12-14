/*
 * CBIR.cpp
 *
 *  Created on: Dec 13, 2010
 *      Author: zach
 */

#include "CBIR.hpp"
#include <flann/io/hdf5.h>

namespace cbir {

using namespace std;

const string CBIR::DATA_SET_NAME = "dataset";

CBIR::CBIR() {
	features.data = NULL;
	clusters.data = NULL;
	clusterIndex = NULL;
}
CBIR::~CBIR() {
}

bool CBIR::haveFeatures() {
	return features.data == NULL ? false : true;
}

bool CBIR::haveClusters() {
	return clusters.data == NULL ? false : true;
}

bool CBIR::haveClusterIndex() {
	return clusterIndex == NULL ? false : true;
}

int CBIR::loadFeatures(fs::path file) {
	ENSURE_EXISTS(file, "Feature file");
	info("Loading features from " << file);
	load(features, file);
	return EXIT_SUCCESS;
}

int CBIR::countLines(fs::path file) {
	ifstream fin(file.string().c_str());

	if (!fin.is_open())
		return 0;
	string line;

	int numLines = 0;
	while (getline(fin, line))
		numLines++;

	fin.close();
	return numLines;
}

void CBIR::save(dataset& data, fs::path file) {

	if (IS_HDF5(file)) {
		flann::save_to_file(data, file.string(), CBIR::DATA_SET_NAME);
	} else {
		error("Cannot save to " << file << " because it does not end in .hdf5");
	}
}

void CBIR::load(dataset &data, fs::path file) {
	info("Attempting to load data set from " << file << " into " << &data);

	if (file.extension() == ".hdf5") {
		loadHdf(data, file);
	} else {
		loadTxt(data, file);
	}

	info("Loaded " << data.rows << " rows x " << data.cols
			<< " columns of data");
}

void CBIR::loadHdf(dataset &data, fs::path file) {
	debug("Loading HDF data");
	flann::load_from_file(data, file.string(), CBIR::DATA_SET_NAME);
}

void CBIR::loadTxt(dataset &data, fs::path file) {
	debug("Loading TXT data");

	debug("Determining number of rows...");
	int numLines = countLines(file);
	debug(file << " contains " << numLines << " rows");

	if (!numLines)
		return;

	ifstream fin(file.string().c_str());
	string line;
	int tmp;
	feat *rawData = new feat[numLines * NUM_COLUMNS];
	debug("Allocated rawData at " << rawData);

	for (int curLine = 0; getline(fin, line); curLine++) {
		istringstream s(line);

		if (curLine % 10000 == 0)
			debug("Loading line " << curLine);

		for (int curToken = 0; !s.eof(); curToken++) {
			s >> rawData[(curLine * NUM_COLUMNS) + (curToken)];
		}
	}

	data.data = (feat*) rawData;
	data.rows = numLines;
	data.cols = NUM_COLUMNS;

	fin.close();
}

int CBIR::saveFeatures(fs::path file) {
	// -- Save to HDF5?
	ENSURE_HDF5(file, "Index file");

	info("Saving index to " << file);
	save(features, file);
	return EXIT_SUCCESS;
}

int CBIR::loadClusters(fs::path file) {
	ENSURE_HDF5(file, "Cluster file");

	info("Loading clusters from " << file);
	load(clusters, file);
	return EXIT_SUCCESS;
}

int CBIR::buildClusters(int numClusters, int numIterations) {
	if (not haveFeatures())
		loadFeatures(FEATURE_FILE);

	// -- Do the clustering
	info("Performing cluster operation " << numClusters << " clusters, "
			<< numIterations << " iterations");

	FLANNParameters params;
	{
		params.algorithm = KMEANS;
		params.checks = 2048;
		params.cb_index = 0.6;
		params.branching = 10;
		params.iterations = numIterations;
		params.centers_init = CENTERS_GONZALES;
		params.target_precision = -1;
		params.build_weight = 0.01;
		params.memory_weight = 1;
	}
	if (clusters.data) {
		delete[] clusters.data;
		clusters.data = NULL;
	}

	clusters.data = new float[numClusters * CBIR::NUM_COLUMNS];
	clusters.rows = numClusters;
	clusters.cols = CBIR::NUM_COLUMNS;

	if (flann_compute_cluster_centers(features.data, features.rows,
			features.cols, numClusters, clusters.data, &params) <= 0)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int CBIR::saveClusters(fs::path file) {
	ENSURE_HDF5(file, "Cluster file");

	info("Saving clusters to " << file);
	save(clusters, file);
	return EXIT_SUCCESS;
}

int CBIR::buildClusterIndex() {

	if (not haveClusters()) {
		if (fs::exists(CLUSTER_FILE)) {
			if (OK != loadClusters(CLUSTER_FILE))
				return EXIT_FAILURE;
		} else if (OK != buildClusters(NUM_CLUSTERS, NUM_ITER))
			return EXIT_FAILURE;
	}

	info("Indexing clusters (" << clusters.rows << " rows)");
	FLANNParameters indexParams;
	float speedup = 0.0f;
	{
		indexParams.algorithm = KDTREE;
		indexParams.checks = 2048;
		indexParams.trees = 8;
		indexParams.target_precision = -1;
		indexParams.build_weight = 0.01;
		indexParams.memory_weight = 1;
	}
	clusterIndex = flann_build_index(clusters.data, clusters.rows,
			clusters.cols, &speedup, &indexParams);

	if (clusterIndex == NULL) {
		error("Could not build cluster index");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int CBIR::loadClusterIndex(fs::path file) {
	ENSURE_HDF5(file, "Index file");

	clusterIndex = flann_load_index((char*) file.string().c_str(),
			features.data, features.rows, features.cols);
	if (not clusterIndex) {
		error("Could not save index file");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int CBIR::saveClusterIndex(fs::path file) {
	ENSURE_HDF5(file, "Index file");

	info("Saving cluster index to " << file);
	if (OK != flann_save_index(clusterIndex, (char*) file.string().c_str())) {
		error("Could not save index file");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int CBIR::computeBagOfWords(fs::path featureFile, fs::path directory,
		fs::path imageList, fs::path featureList) {
	ENSURE_EXISTS(directory,"Output dictory");
	ENSURE_EXISTS(featureFile,"Feature file");
	ENSURE_EXISTS(imageList, "Image list");
	ENSURE_EXISTS(featureList, "Image feature count list");

	if (not fs::is_directory(directory)) {
		error(directory << " is not a directory");
		return EXIT_FAILURE;
	}

	// -- Ensure the clustering and indexing operation have already been done
	if (not haveClusters()) {
		if (fs::exists(CLUSTER_FILE))
			loadClusters(CLUSTER_FILE);
		else {
			debug("Cluster file " << CLUSTER_FILE << " does not exist. Building clusters.")
			buildClusters(NUM_CLUSTERS, NUM_ITER);
		}
	}
	if (not haveClusterIndex()) {
		if (fs::exists(INDEX_FILE))
			loadClusterIndex(INDEX_FILE);
		else
			buildClusterIndex();
	}

	// -- Compute nearest neighbors
	int *indices = new int[features.rows];
	float *distances = new float[features.rows];
	FLANNParameters flannParams;
	{
		flannParams.log_level = LOG_NONE;
		//		flannParams.log_destination=NULL;
		flannParams.random_seed = CENTERS_RANDOM;
	}
	info("Computing nearest neighbors");
	flann_find_nearest_neighbors_index(clusterIndex, features.data,
			features.rows, indices, distances, 1, &flannParams);

	ImageFeatureCount imageFeatureCount;
	string imageName;
	int featureCount;

	// -- Load the image list and the corresponding feature counts
	ifstream fImageList(imageList.string().c_str());
	ifstream fFeatureList(featureList.string().c_str());

	info("Reading image feature list " << imageList);
	info("Reading image feature counts from " << featureList);
	while (not fImageList.eof()) {
		fImageList >> imageName;
		fFeatureList >> featureCount;

		imageFeatureCount[imageName] = featureCount;
	}
	fImageList.close();
	fImageList.close();

	debug("Have feature info for " << imageFeatureCount.size() << " images");

	// -- Iterate over each
	int totalFeatures = 0;
	ImageFeatureCount::iterator iter;
	for (iter = imageFeatureCount.begin(); iter != imageFeatureCount.end()
			&& totalFeatures < features.rows; iter++) {

		// Open the document, delete existing contents
		fs::path documentPath = directory / iter->first;
		ofstream doc(documentPath.string().c_str(), ios::out | ios::trunc);

		debug("Writing " << iter->second << " 'words' to " << iter->first);

		if (not doc.is_open()) {
			error("Could not open " << iter->first);
			return EXIT_FAILURE;
		}

		/*
		 * Lemur Format:
		 * <DOC>
		 * <DOCNO>imgB</DOCNO>
		 * <TEXT>
		 * w3 w3 w1
		 * </TEXT>
		 * </DOC>
		 */

		doc << "<DOC>" << endl << "<DOCNO>" << iter->first << "</DOCNO>"
				<< endl << "<TEXT>" << endl;

		// Iterate over all of the features for this doc
		for (int docFeature = 0; docFeature < iter->second && totalFeatures
				< features.rows; docFeature++, totalFeatures++) {
			//			debug("Feature number " << totalFeatures << "(" << docFeature << "/" << iter->second << ") for " << iter->first);
			doc << indices[totalFeatures] << " ";
		}

		doc << "</TEXT>" << endl << "</DOC>" << endl;

		doc.close();
	}

	info("Wrote " << totalFeatures << " 'words'");
}


#define check_error(error) \
	if (error == boost::asio::error::eof) { \
		info("Connection closed"); \
		continue; \
	} \
	else if (error) \
		throw boost::system::system_error(error);

int CBIR::startServer(fs::path clusterFile, int port) {

	ENSURE_EXISTS(clusterFile, "Cluster file");
	ENSURE_HDF5(clusterFile, "Cluster file");

	info("Starting server");

	loadClusters(clusterFile);
	buildClusterIndex();

	// I Love Boost.
	// Makes things so much easier.
	asio::io_service io_service;
	ip::tcp::acceptor acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), port));
	boost::system::error_code error;
	char *buffer = new char[1024 * 1024 * 10]; // 10MB buffer
	int read;
	for (;;) {
		// -- Accept the connection
		info("Waiting for a connection...")
		ip::tcp::socket socket(io_service);
		acceptor.accept(socket);
		info("Accepted a connection from " << socket.remote_endpoint().address().to_string());

		// -- Read the first 4 bytes for size
		read = socket.read_some(asio::buffer(buffer, 4), error);
		check_error(error);
		info("Read " << read << " bytes");

		// -- Calculate the size, read the file
		long size = ntohl(*(long*)buffer);
		info("Waiting on " << size << " bytes")
		read = socket.read_some(asio::buffer(buffer, size), error);
		info("Read " << read << " bytes");
		check_error(error);
		if(read != size) {
			error("Did not get expected byte-count");
			continue;
		}

		// TODO: We are writing out to a file, then reading it back in.
		// Technically, we could parse this information from the buffer...
		// ... but that's hard.

		// -- Write out the file
		fs::path tempFile( "server.tmp" );
		info("Writing data to " << tempFile);
		ofstream fout(tempFile.string().c_str(), ios::trunc);
		fout.write(buffer, size);
		fout.flush();
		fout.close();

		// -- Read back *in* the file
		loadFeatures(tempFile);

		// -- Nearest neighbor
		// TODO: THis is a copy-paste, refactor code
		int *indices = new int[features.rows];
		float *distances = new float[features.rows];
		FLANNParameters flannParams;
		{
			flannParams.log_level = LOG_NONE;
			//		flannParams.log_destination=NULL;
			flannParams.random_seed = CENTERS_RANDOM;
		}
		info("Computing nearest neighbors");
		flann_find_nearest_neighbors_index(clusterIndex, features.data,
				features.rows, indices, distances, 1, &flannParams);

		// -- Load the data into a single string
		stringstream ss;
		for(int i = 0; i < features.rows; i++)
			ss << indices[i] << " ";

		// -- Send the data back over the wire
		info("Got features: " << ss.str());

		socket.close();
	}
}

}
