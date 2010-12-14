/**
 * \file CBIR.h
 *
 *  Created on: Dec 13, 2010
 *      Author: zach
 */

#ifndef CBIR_H_
#define CBIR_H_

#include "common.hpp"

namespace cbir {

using namespace std;

typedef map<string,int> ImageFeatureCount;

/**
 *
 */
class CBIR {
public:
	CBIR();
	virtual ~CBIR();

	static const string DATA_SET_NAME;
	static const int NUM_COLUMNS = 128;

	dataset features;
	dataset clusters;
	flann_index_t clusterIndex;

	/**
	 * Count the number of lines in the file.
	 * \param file File to check
	 * \return Number of lines in file.
	 */
	static int countLines(fs::path file);

	/**
	 * Saves the dataset to file in HDF5 format.
	 * \param data Dataset to save
	 * \param file File to save to
	 */
	static void save(dataset& data, fs::path file);

	/**
	 * Loads a dataset from file.
	 *
	 * \param data Dataset to store the information into
	 * \param file File to load from
	 *
	 * \remarks This function allocates data.data to be the proper dimensions
	 */
	static void load(dataset &data, fs::path file);

	/**
	 * Loads a dataset from an HDF file.
	 *
	 * \see load
	 */
	static void loadHdf(dataset &data, fs::path file);

	/**
	 *
	 */
	static void loadTxt(dataset &data, fs::path file);

	bool haveFeatures();
	bool haveClusters();
	bool haveClusterIndex();

	int loadFeatures(fs::path file);
	int saveFeatures(fs::path file);


	int buildClusters(int numClusters=150000,int numIterations=15);
	int saveClusters(fs::path file);
	int loadClusters(fs::path file);

	int buildClusterIndex();
	int saveClusterIndex(fs::path file);
	int loadClusterIndex(fs::path file);

	int computeBagOfWords(fs::path featureFile, fs::path directory, fs::path imageList, fs::path featureList);
};

}

#endif /* CBIR_H_ */
