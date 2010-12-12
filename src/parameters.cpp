/*
 * parameters.c
 *
 *  Created on: Dec 9, 2010
 *      Author: zach
 */

#include <boost/program_options.hpp>
#include <map>
#include <string>
#include <iostream>
#include "parameters.hpp"

using namespace std;

po::variables_map params;

int getParameters (int ac, char* av[]) {
	po::options_description desc("Allowed options");
	desc.add_options()
			("help,h", "Show this message")
			("feature,f", po::value<string>(), "Feature file")
			("index,i", po::value<string>(), "Index file")
			("cluster,c", po::value<string>(), "Cluster file")
			("words,w", po::value<string>(), "Bag-of-words Directory")
	;

	po::store(po::parse_command_line(ac, av, desc), params);
	po::notify(params);

	if(params.count("help")) {
		cout << desc << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
