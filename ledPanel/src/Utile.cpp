/*
 * Utile.cpp
 *
 *  Created on: 06.05.2013
 *      Author: martin
 */

#include "Utile.h"

using namespace std;

unsigned long Utile::getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return 1000000 * tv.tv_sec + tv.tv_usec;
}

//int Utile::round(float val)
//{
//    int rt = val;
////    val -= rt; /* val represents now only the numbers behind the point */
////
////    if (val >= 0.5)
////        rt++;
////
//    return rt;
//
//
//
//}

int Utile::absolute(int val)
{
    return ((val < 0) ? val * (-1) : val);
}

float Utile::absolute(float val)
{
    return ((val < 0) ? val * (-1) : val);
}

int Utile::resize(int val, int min_val, int max_val)
{
    if (val < min_val)
        return min_val;

    if (val > max_val)
        return max_val;

    return val;
}

bool Utile::fileExists(string path, string file)
{
    DIR *dir;
    struct dirent *ent;
    bool rc = false;

    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (!file.compare(ent->d_name))
            {
                rc = true;
                break;
            }
        }
        closedir(dir);
        return rc;
    }
    else
    {
        /* could not open directory */
        return false;
    }
}

void Utile::split(const string& message, const string& delimeter, vector<string> &tokens)
{
    string::size_type start = 0;
    string::size_type end;

    tokens.clear();

    while (1)
    {
        end = message.find(delimeter, start);
        tokens.push_back(message.substr(start, end - start));

        if (end == string::npos)
            break;

        /*Exclude the delimiter in the next search*/
        start = end + delimeter.size();
    }
}

