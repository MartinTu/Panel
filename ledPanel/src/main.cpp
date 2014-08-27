//============================================================================
// Name        : LEDPanel.cpp
// Author      : Martin Turetschek
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "ServerDisplayHandler.h"

using namespace std;

int main()
{
    ServerDisplayHandler* test = new ServerDisplayHandler();
    test->run();

    cout << "main: done" << endl;
    return 0;
}
