/*
 * common.h
 *
 *  Created on: Dec 12, 2010
 *      Author: zach
 */

#ifndef COMMON_H_
#define COMMON_H_


#include <flann/flann.hpp> // FLANN
#include <string>		// strings
#include <fstream>		// file IO
#include <iostream>		// more IO
#include <vector>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <sstream>
#include <map>
#include <iso646.h>		// and, or, not, etc.
#include <assert.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>	// String manip
#include <boost/foreach.hpp>			// FOR_EACH macro
#include <boost/algorithm/string/replace.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

/**
 * Each feature is made up of 128 numbers, each number is less than
 * 256.
 */
typedef float feat;


namespace fs = boost::filesystem;
namespace po = boost::program_options;
namespace ip = boost::asio::ip;
namespace asio = boost::asio;

typedef flann::Matrix<feat> dataset;

#include "parameters.hpp"
#include "log.hpp"
#include "parameters.hpp"
//#include "indexer.hpp"
#include "CBIR.hpp"
//#include "cluster.hpp"
#include "macros.hpp"

#endif /* COMMON_H_ */
