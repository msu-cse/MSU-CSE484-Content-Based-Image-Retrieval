/*
 * main.cc
 *
 *  Created on: Dec 9, 2010
 *      Author: zach
 */

//#include "flann-cpp-src/flann.h"
#include "common.hpp"

namespace cbir {

using namespace std;

#define FIRST_ROW_OF_DATA \
		{}

int main(int ac, char* av[]) {

	// -- Configure Logging
	log4cxx::PropertyConfigurator::configure("log4cxx.properties");

	// -- Load parameters, help
	getParameters(ac, av);
	if (params.count("help")) {
		print_usage();
		return EXIT_FAILURE;
	}

	CBIR c;

	if (OK != c.loadFeatures(FEATURE_FILE))
		return EXIT_FAILURE;

	if(SANITY_CHECK) {
		float firstRow [CBIR::NUM_COLUMNS] = FIRST_ROW_OF_DATA;
		for(int i = 0; i < CBIR::NUM_COLUMNS; i++) {
			if( firstRow[i] != c.features.data[i] ) {
				error("Row 1 at " << i << " does not match!");
				return EXIT_FAILURE;
			}
		}
	}

	if (CONVERT_TO_HDF5)
		if (OK != c.saveFeatures(FEATURE_FILE + ".hdf5"))
			return EXIT_FAILURE;

	if (BUILD_CLUSTERS)
		if (OK != c.buildClusters() or OK != c.saveClusters(CLUSTER_FILE))
			return EXIT_FAILURE;

	if (BUILD_CLUSTER_INDEX)
		if (OK != c.buildClusterIndex() or OK != c.saveClusterIndex(INDEX_FILE))
			return EXIT_FAILURE;

	if (LOAD_CLUSTER_INDEX)
		if (OK != c.loadClusterIndex(INDEX_FILE))
			return EXIT_FAILURE;

	if (BAG_OF_WORDS)
		if (OK != c.computeBagOfWords(FEATURE_FILE, BAG_OF_WORDS_DIR,
				IMAGE_LIST_FILE, FEATURE_COUNT_FILE))
			return EXIT_FAILURE;

	//	// -- Load features
	//	if( fs::exists(FEATURE_FILE) ) {
	//		info("Loading features from " << FEATURE_FILE);
	//		load(features, FEATURE_FILE);
	//	} else {
	//		error("Feature file " << FEATURE_FILE << " does not exist");
	//		print_usage();
	//		return EXIT_FAILURE;
	//	}
	//
	//	// -- Save to HDF5?
	//	if( CONVERT_TO_HDF5 ) {
	//		string hdf5file = FEATURE_FILE + ".hdf5";
	//		info("Saving to " << hdf5file);
	//		save(features, hdf5file);
	//	}
	//
	//	// -- Cluster the points if we were told to, or if we are indexing
	//	// and the cluster file doesn't exit.
	//	if( BUILD_CLUSTERS ) {
	//		// -- Verify HDF5 extension
	//		if(fs::path(CLUSTER_FILE).extension() != ".hdf5") {
	//			error("Cluster file " << CLUSTER_FILE << " must end in .hdf5");
	//			return EXIT_FAILURE;
	//		}
	//
	//		// -- Do the clustering
	//		info("Performing cluster operation " << NUM_CLUSTERS << " clusters, " << NUM_ITER << " iterations")
	//		cluster(features,clusters,NUM_CLUSTERS,NUM_ITER);
	//
	//		// -- Save clusters
	//		info("Saving cluster data to " << CLUSTER_FILE)
	//		save(clusters, CLUSTER_FILE);
	//	}
	//
	//
	//	// -- Load the clusters if we are indexing and the file exists
	//	if( BUILD_CLUSTER_INDEX and fs::exists(CLUSTER_FILE)) {
	//		load(clusters, CLUSTER_FILE);
	//	}
	//
	//	// -- Index the cluster
	//	if( BUILD_CLUSTER_INDEX ) {
	//
	//		if(not IS_HDF5(INDEX_FILE)) {
	//			error("Index file " << INDEX_FILE << " must end in .hdf5");
	//			return EXIT_FAILURE;
	//		}
	//
	//		clusterIndex = index(clusters);
	//
	//		if(clusterIndex == NULL) {
	//			error("Could not build cluster index");
	//			return EXIT_FAILURE;
	//		}
	//
	//		if(OK != flann_save_index(clusterIndex, INDEX_FILE)) {
	//			error("Could not save index file");
	//			return EXIT_FAILURE;
	//		}
	//	}
	//
	//	// -- Nearest neighbors
	//	if( BAG_OF_WORDS ) {
	//		// Make sure the index file exists.  Even if we JUST calculated it,
	//		// the file should still exist.
	//		if(not IS_HDF5(INDEX_FILE) or not fs::exists(INDEX_FILE)) {
	//			error("Index file " << INDEX_FILE << " does not exist or does not end in .hdf5");
	//			return EXIT_FAILURE;
	//		}
	//
	//		if(!clusterIndex) {
	//			clusterIndex = flann_load_index(INDEX_FILE,clusters,)
	//		}
	//	}

	return 0;
}

}

int main(int ac, char* av[]) {
	cbir::main(ac, av);
}
