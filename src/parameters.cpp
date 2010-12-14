/*
 * parameters.c
 *
 *  Created on: Dec 9, 2010
 *      Author: zach
 */

#include "common.hpp"
#include <boost/program_options.hpp>

using namespace std;

namespace cbir {

string usage_string =
"MSU Cluster-Based Image Recognition";
po::variables_map params;
po::options_description help("Help");
po::options_description args("General Arguments");
po::options_description formats("File Formats");
po::options_description clustering("Clustering");
po::options_description bag("Bag of Words");
po::options_description cmdline_options;

void print_usage() {
	cout << usage_string << endl << endl << cmdline_options;
}

void getParameters (int ac, char* av[]) {
	help.add_options()
			("help,h", "Show this message")
	;
	args.add_options()
			// -- Parameters
			("feature,f",
					po::value<string>()->default_value("esp.feature.hdf5"),
					"Feature file (Text, HDF5)")
			("index,i",
					po::value<string>()->default_value("esp.feature.index.hdf5"),
					"Index file (HDF5)")
			("cluster,c",
					po::value<string>()->default_value("esp.feature.clusters.hdf5"),
					"Cluster file (HDF5)")
	;
	formats.add_options()
			// -- Actions
			("convert-to-hdf5",
					"Convert <feature> file to HDF5 format, store as <feature>.hdf5")
	;
	clustering.add_options()
			("build-clusters",
					"Build K-Means clusters from <feature>, save to <clusters>")
			("build-cluster-index",
					"Load clusters from <clusters> or build from <feature>, then"
					"index the clusters, save the index to <index>")
			("load-cluster-index",
					"Load clusters from <clusters> or build from <feature>, then"
					"load the pre-indexed clusters from <index>")
			("num-clusters,n",
					po::value<int>()->default_value(150000),
					"Number of clusters")
			("num-iters,t",
					po::value<int>()->default_value(15),
					"K-Means iterations")
	;

	bag.add_options()
			("bag-of-words",
					po::value<string>(),
					"Build bag-of-words representation from <features> with "
					"clusters optionally from <clusters>, store resulting text files in <dir>")
			("img-list,m",
					po::value<string>()->default_value("imglist.txt"),
					"Image list")
			("size-list,s",
					po::value<string>()->default_value("esp.size"),
					"Feature size list")
	;

	cmdline_options.add(help).add(args).add(formats).add(clustering).add(bag);

	po::store(po::parse_command_line(ac, av, cmdline_options),  params);
//	po::store(po::parse_command_line(ac, av, args), params);
//	po::store(po::parse_command_line(ac, av, formats), params);
//	po::store(po::parse_command_line(ac, av, clustering), params);
//	po::store(po::parse_command_line(ac, av, bag), params);
	po::notify(params);
}

}
