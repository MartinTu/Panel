/*
 * Share.h
 *
 *  Created on: 14.02.2013
 *      Author: martin
 */

#ifndef SHARE_H_
#define SHARE_H_

#include <string>
#include "MyException.h"
#include <errno.h>
#include <cstdlib>

unsigned long long parseULongLong(std::string str, std::string errorName);
unsigned long parseULong(std::string str, std::string errorName);
long parseLong(std::string str, std::string errorName );


const std::string sep1 = "$$"; // indicate end of a command
const std::string sep2 = "::"; // seperate command and parameterlist
const std::string sep3 = "#";  // seperate parameters of parameterlist
const std::string sep4 = ";";  // seperate elements of an array
const std::string sep5 = ".";  // seperate elements of a struct, in an array

#endif /* SHARE_H_ */
