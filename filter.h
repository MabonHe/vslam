/*************************************************************************
	> File Name: filter.h
	> Author: 
	> Mail: 
	> Created Time: 2020年06月23日 星期二 16时51分43秒
 ************************************************************************/

#ifndef _FILTER_H
#define _FILTER_H
#include<iostream>
#include<opencv2/core.hpp>
using namespace std;
using namespace cv;

class Filter
{
    public:
        Filter();
        ~Filter();
    public:
        void filter(Mat &src,Mat &dst,int filter_size);
        int y_average(Mat &src,int filter_size,int step);
};
#endif
