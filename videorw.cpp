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
int VideoRW::savevideo(const char *filename="live.avi",int framecount=1){
    Mat frame;
    cout << "filename:" << filename << "  framecount:" << framecount << endl;
    Size frameSize(640,480);
    bool isColor = true;
    VideoWriter writer;
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    double fps = 30.0;                          // framerate of the created video stream
    char _filename[64] = "./live.avi";             // name of the output video file
    writer.open(filename, codec, fps, frameSize, isColor);
    // check if we succeeded
    if (!writer.isOpened()) {
        cerr << "Could not open the output video file for write\n";
        return -1;
      }
    VideoCapture cap;
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    cap.open(deviceID, apiID);
    cap.set(CAP_PROP_FRAME_WIDTH,640);
    cap.set(CAP_PROP_FRAME_HEIGHT,480);
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    for(int i = 0;i < framecount;i++)
    {
        cap.read(frame);
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        writer.write(frame);
    }
    cap.release();
    writer.release();
}

