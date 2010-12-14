/*
 * Log.h
 *
 *  Created on: Dec 12, 2010
 *      Author: zach
 */

#ifndef LOG_H_
#define LOG_H_

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/defaultconfigurator.h>

#define root		log4cxx::Logger::getLogger("cbir")
#define debug(x)	LOG4CXX_DEBUG(root,x)
#define info(x)		LOG4CXX_INFO(root,x)
#define warn(x)		LOG4CXX_WARN(root,x)
#define error(x)	LOG4CXX_ERROR(root,x)

#endif /* LOG_H_ */
