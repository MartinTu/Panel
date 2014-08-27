/*
 * Share.cpp
 *
 *  Created on: 14.02.2013
 *      Author: martin
 */

#include "Share.h"

#include <errno.h>
#include <cstdlib>

using namespace std;

unsigned long long parseULongLong(string str, string errorName)
{
    char *end;

    unsigned long long value = strtoull(str.c_str(), &end,10);
    if((end == str)|| ( *end != '\0' ) || (errno == ERANGE))
    {
        throw MyException("unable to parse " + errorName);
    }

    return value;
}

unsigned long parseULong(string str, string errorName)
{
    char *end;

    unsigned long value = strtoul(str.c_str(), &end,10);
    if((end == str)|| ( *end != '\0' ) || (errno == ERANGE))
    {
        throw MyException("unable to parse " + errorName);
    }

    return value;
}


long parseLong(string str, string errorName)
{
    char *end;

    long value = strtol(str.c_str(), &end,10);
    if((end == str)|| ( *end != '\0' ) || (errno == ERANGE))
    {
        throw MyException("unable to parse " + errorName);
    }

    return value;
}


