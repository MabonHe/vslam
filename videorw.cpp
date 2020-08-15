/*************************************************************************
	> File Name: videorw.cpp
	> Author:
	> Mail:
	> Created Time: 2020年08月08日 星期六 17时51分12秒
 ************************************************************************/

#include"videorw.h"
#include<iostream>
VideoRW::VideoRW()
{}
VideoRW::~VideoRW()
{}

int VideoRW::savevideo(Mat frame){

    return 0;
}
int VideoRW::savevideo(int framecount){

    return 0;
}
int VideoRW::readvideo(const char *filename,vector<Mat> &framelist){
    Mat frame;
    VideoCapture inputVideo(filename);
    if (!inputVideo.isOpened())
    {
        cout  << "Could not open the input video: " << filename << endl;
        return -1;
    }
    for(;;)
    {
        inputVideo.read(frame);
        if (frame.empty())
            break;
        framelist.push_back(frame);
    }
    return 0;
}

