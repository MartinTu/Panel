/*
 * Utile.h
 *
 *  Created on: 06.05.2013
 *      Author: martin
 */

#ifndef UTILE_H_
#define UTILE_H_

#include <sys/time.h>
#include <string.h>
#include <dirent.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Utile
{
    public:
		static int invert(int val, int maxval);
        static unsigned long getTime();
//        static int round(float val);
        static int pow(int base, int exp);
        static int absolute(int val);
        static float absolute(float val);
        static int min(int a, int b);
        static int max(int a, int b);
        static bool isOdd(int val);
        static bool isEven(int val);
        static void display_thread_sched_attr();
        static int resize(int val, int min_val, int max_val);
        static bool fileExists(std::string path, std::string file);
        static void split(const std::string& message, const std::string& delimeter, std::vector<std::string> &tokens);
};

#endif /* UTILE_H_ */
