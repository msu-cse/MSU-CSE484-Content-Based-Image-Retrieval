/*
 * parameters.hpp
 *
 *  Created on: Dec 9, 2010
 *      Author: zach
 */

#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <boost/program_options.hpp>

namespace po = boost::program_options;

/** Global program parameters */
extern po::variables_map params;

/**
 * Retrieves all of the parameters into the provided map.
 */
int getParameters(int ac, char* av[]);

#endif /* PARAMETERS_HPP_ */
