/*
 * MyException.cpp
 *
 *  Created on: 24.05.2013
 *      Author: martin
 */

#include "MyException.h"

/**
 * CTOR
 */
MyException::MyException(std::string what)
{
    this->message = what;
}

/**
 * DTOR
 */
MyException::~MyException()
{
}

/**
 * returns the error message
 */
std::string MyException::what()
{
    return this->message;
}

