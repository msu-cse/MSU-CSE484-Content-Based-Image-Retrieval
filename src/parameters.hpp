/*
 * parameters.hpp
 *
 *  Created on: Dec 9, 2010
 *      Author: zach
 */

#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include "common.hpp"

namespace po = boost::program_options;

#define GET_PARAM(name,type,default_val) \
	(params.count(name) ? params[name].as<type>() : ((type) default_val))
#define PARAM_SET(name)	(params.count(name) ? true : false)

#define FEATURE_FILE			GET_PARAM("feature",string,"")
#define CLUSTER_FILE			GET_PARAM("cluster",string,"")
#define INDEX_FILE				GET_PARAM("index",string,"")
#define HDF5_FILE				GET_PARAM("hdf5",string,"") // params["hdf5"].as<string>()
#define NUM_CLUSTERS			params["num-clusters"].as<int>()
#define NUM_ITER				params["num-iters"].as<int>()
#define BAG_OF_WORDS_DIR		GET_PARAM("bag-of-words",string,"")

#define BAG_OF_WORDS			PARAM_SET("bag-of-words")
#define CONVERT_TO_HDF5			PARAM_SET("convert-to-hdf5")
#define BUILD_CLUSTERS			PARAM_SET("build-clusters")

#define IMAGE_LIST_FILE			GET_PARAM("img-list",string,"")
#define FEATURE_COUNT_FILE		GET_PARAM("size-list",string,"")

/* Cluster indexes */
#define BUILD_CLUSTER_INDEX		PARAM_SET("build-cluster-index")
#define LOAD_CLUSTER_INDEX		PARAM_SET("save-cluster-index")
#define SAVE_CLUSTER_INDEX		PARAM_SET("load-cluster-index")

namespace cbir {


/** Global program parameters */
extern po::variables_map params;

/**
 * Retrieves all of the parameters into the provided map.
 */
void getParameters(int ac, char* av[]);

/**
 * Prints usage
 */
void print_usage();

}

#endif /* PARAMETERS_HPP_ */
