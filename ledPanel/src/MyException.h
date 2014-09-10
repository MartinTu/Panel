/*
 * MyException.h
 *
 *  Created on: 24.05.2013
 *      Author: martin
 */

#ifndef MYEXCEPTION_H_
#define MYEXCEPTION_H_

#include <string>

class MyException
{
    public:
        MyException(std::string message);
        virtual ~MyException();
        std::string what();

    private:
        std::string message;
};

#endif /* MYEXCEPTION_H_ */
