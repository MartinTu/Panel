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
#include <cmath>
#include <wiringPi.h>
#include "LUT.h"

using namespace std;

enum direction_t
{
    dirLeft = 0x00,
    dirLeftBot = 0x01,
    dirBottom = 0x02,
    dirRightBot = 0x03,
    dirRight = 0x04,
    dirRightTop = 0x05,
    dirTop = 0x06,
    dirLeftTop = 0x07
};

class Utils
{
    public:
		static float sinf(float x);
		static float sinn(float x);
		static float cosf(float x);
		static float cosn(float x);
		static float distance(float x1, float y1, float x2, float y2);
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
        static int addInBounds(int val1, int val2, int min_val = 0x00, int max_val = 0xff);
        static int resize(int val, int min_val, int max_val);
        static string getSelfPath();
        static bool fileExists(std::string path, std::string file);
        static void split(const std::string& message, const std::string& delimeter, std::vector<std::string> &tokens);
        static void ledInit();
        static void ledOn();
        static void ledOff();
};

#endif /* UTILS_H_ */
