/*************************************************************************
	> File Name: main.cpp
	> Author:
	> Mail:
	> Created Time: 2020年06月18日 星期四 17时41分58秒
 ************************************************************************/

#include<iostream>
#include"camera.h"
#include"filter.h"
#include"match.h"
#include"videorw.h"
#include<opencv2/features2d/features2d.hpp>
using namespace std;
using namespace cv;
int
main(int argc,char *argv[])
{
    cout << "Hello Vslam!" << endl;
    Mat frame,H;
    //Mat frame_bin = Mat::zeros(CV_8UC1);
    Mat frame_bin;
    struct FrameData framedata;
    Camera camera;
    Filter filter;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    if(argc > 1)
    {
        cout << "argc:" << argc << endl;
        cout << "argv:" << argv[1] << endl;
        VideoRW videorw;
        vector<Mat> v;
        videorw.readvideo(argv[1],v);
        cout << "size:" << v.size() << endl;
    }
    //camera.start_camera_thread();
    while (false)
    {
        Mat dst;
        camera.get_frame(framedata);
        cvtColor(framedata.frame_queue[0],frame,COLOR_RGB2GRAY);
        cout << framedata.frame_queue.size() << endl;
        CalculateHomographyMatrix(framedata.frame_queue[0],framedata.frame_queue[1],H);


        filter.filter(frame,dst,5);

        cout << "width:" << frame.cols << "hieght:" << frame.rows << endl;
        unsigned ysrc = 0, yave = 0;
        for (int y = 0;y < frame.rows; y++ )
        {
            for (int x = 0; x < frame.cols; x ++)
               ysrc += (int)frame.data[x+y*frame.step];
        }
        yave = ysrc/(frame.rows*frame.cols);
        cout << "ysrc:" << ysrc << " yave:" << yave << endl;

        threshold(frame,frame_bin,yave,255,THRESH_BINARY);
        vector<vector<Point> > contours0;
        findContours(frame_bin,contours0,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);
        contours.resize(contours0.size());
        cout << "contours0 size:"<< contours0.size() <<endl;

        imshow("process image filter",dst);
        imshow("process image",frame_bin);
        waitKey(1);
        sleep(15);


    }
    return 0;
}
