/*************************************************************************
	> File Name: videorw.h
	> Author:
	> Mail:
	> Created Time: 2020年08月08日 星期六 17时51分24秒
 ************************************************************************/

#ifndef _VIDEORW_H
#define _VIDEORW_H
#include<vector>
#include<opencv2/core.hpp>
#include<opencv2/videoio.hpp>
using namespace cv;
using namespace std;
class VideoRW
{
public:
    VideoRW();
    ~VideoRW();
    int savevideo(Mat frame);
    int savevideo(int framecount);
    int readvideo(const char *filename,vector<Mat> &framelist);

};
#endif
