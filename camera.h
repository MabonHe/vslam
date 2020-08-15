/*************************************************************************
	> File Name: camera.h
	> Author: 
	> Mail: 
	> Created Time: 2020年06月18日 星期四 17时45分28秒
 ************************************************************************/

#ifndef _CAMERA_H
#define _CAMERA_H
#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<vector>
#include<opencv2/core.hpp>
#include<opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;
struct FrameData
{
    Mat frame;
    vector<Mat> frame_queue;
    bool is_empty;
};
class Camera
{
    public:
        Camera();
        ~Camera();
        void get_frame(struct FrameData &framedata);
        void start_camera_thread();
    private:
        static void * startcamera(void *args);
    private:
        pthread_t pid;

};
#endif
