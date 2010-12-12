/*
 * main.cc
 *
 *  Created on: Dec 9, 2010
 *      Author: zach
 */

//#include "flann-cpp-src/flann.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "parameters.hpp"

#define FILENAME "it"
#define null NULL
/**
 * Feature buffer size.  Each number is <= 3 chars, plus spaces.
 * 128 of them, leave gratuitous amounts of room.  Plus the newline.
 */
#define FEAT_BUF_SIZE (4*128)

/**
 * Each feature is made up of 128 numbers, each number is less than
 * 256.
 */
typedef unsigned char feat;

using namespace std;

int main(int ac, char* av[]) {
	//	string buffer;
	//	ifstream fin(FILENAME);
	//	vector<vector<feat> > features;
	//	char *pToken = null;
	//
	//	// Read each line until we hit EOF
	//	while( ! (getline(fin, buffer).eof()) ) {
	//		vector<string> strs;
	//		vector<feat> feature_points;
	//		boost::split(strs, "string to split", boost::is_any_of("\t "));
	//
	//		BOOST_FOREACH( string s, strs ) {
	//			cout << s << endl;
	//		}

	//		features.push_back(vector<feat>());
	//
	//
	//		// Tokenize
	//		pToken = buffer.c_str();
	////		while( strep(&pToken, " ") ) {
	////			feat value = atoi(pToken);
	////
	////		}
	//	}
	int retVal = 0;
	try {
		retVal = getParameters(ac, av);
		cout << "Feature file is " << params["feature"].as<string> () << endl;
	} catch (exception& e) {
		cerr << "error: " << e.what() << "\n";
		return 1;
	} catch (...) {
		cerr << "Exception of unknown type!\n";
	}

	return retVal;
}

