/*
 * macros.hpp
 *
 *  Created on: Dec 13, 2010
 *      Author: zach
 */

#ifndef MACROS_HPP_
#define MACROS_HPP_

#include "common.hpp"

/** Return OK */
#define 	OK		EXIT_SUCCESS
#define		FAIL	EXIT_FAILURE

#define IS_HDF5(pathString)	\
	(fs::path(pathString).extension() == ".hdf5")

#define ENSURE_HDF5(pathString,desc) \
	if(not IS_HDF5(pathString)) { \
		error(desc << " " << pathString << " must end in HDF5."); \
		print_usage(); \
		return FAIL; \
	}

#define ENSURE_EXISTS(pathString,desc) \
	if(not fs::exists(pathString)) { \
		error(desc << " " << pathString << " does not exist."); \
		print_usage(); \
		return FAIL; \
	}

#endif /* MACROS_HPP_ */
