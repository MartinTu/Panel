/*
 * Utils.h
 *
 *  Created on: 06.05.2013
 *      Author: martin
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <unistd.h>//file path with "proc/self/exe"
#include <stdio.h>
#include <wiringPi.h>

using namespace std;

class Utils
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
        static void printStars();
        static void display_thread_sched_attr();
        static bool isInRange(int var, int min, int max);
        static int resize(int val, int min_val, int max_val);
        static string getSelfPath();
        static bool fileExists(std::string path, std::string file);
        static void split(const std::string& message, const std::string& delimeter, std::vector<std::string> &tokens);
        static void ledInit();
        static void ledOn();
        static void ledOff();
};

#endif /* UTILS_H_ */
