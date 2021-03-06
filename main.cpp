/*************************************************************************
	> File Name: main.cpp
	> Author:
	> Mail:
	> Created Time: 2020年06月18日 星期四 17时41分58秒
 ************************************************************************/

#include <iostream>
#include "camera.h"
#include "filter.h"
#include "match.h"
#include "videorw.h"
#include <opencv2/features2d/features2d.hpp>
#include <getopt.h>
#include <string.h>
using namespace std;
using namespace cv;

bool savevideo = false;
bool readvideo = false;
char filename[64] = " ";
char testMode[32] = "";
int numbuf = 1;
static struct option const long_options[] =
{
    {"help", no_argument, NULL, 'H'},
    {"format", required_argument, NULL, 'f'},
    {"readvideo",required_argument, NULL, 'r'},
    {"testMode",required_argument,NULL,'m'},
    {"savevideo",required_argument,NULL,'s'},
    {NULL, 0, NULL, 0}
};
void usage()
{
    printf("Usage:\n"
        "--help -h\n"
        "--savevideo -s\n"
        "--readvideo -r\n"
        "--numbuf -n\n"
        "--testMode -m\n"
        "--filename -f\n");
}
int parse_args(int argc,char **argv);
int parse_args(int argc,char **argv)
{
    int c;
    int longindex=0;
    char format[64]="NV12";
    while ((c = getopt_long (argc, argv, "Hs:f:r:n:m:", long_options, &longindex)) != -1)
    {
        switch (c)
        {
            case 'f':
                memset(filename,0,sizeof(filename));
                sprintf(filename,"%s",optarg);
                break;
            case 's':
                savevideo = true;
                memset(filename,0,sizeof(filename));
                sprintf(filename,"%s",optarg);
                break;
            case 'r':
                readvideo = true;
                memset(filename,0,sizeof(filename));
                sprintf(filename,"%s",optarg);
                break;
            case 'n':
                numbuf = atoi(optarg);
                break;
            case 'm':
                memset(testMode,0,sizeof(testMode));
                sprintf(testMode,"%s",optarg);
                break;
            case 'H':
                usage();
                exit(0);
            case '?':
                usage();
                exit(0);
            default:
                usage();
                exit(0);
        }
    }
    return 0;
}
int
main(int argc,char *argv[])
{
    parse_args(argc,argv);
    cout << "Hello Vslam!" << endl;
    Mat frame,H;
    //Mat frame_bin = Mat::zeros(CV_8UC1);
    Mat frame_bin;
    struct FrameData framedata;
    Camera camera;
    Filter filter;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    if(readvideo)
    {
        cout << "filename:" << filename << endl;
        VideoRW videorw;
        vector<Mat> v;
        videorw.readvideo(filename,v);
        cout << "size:" << v.size() << endl;
        Mat E,R,t;
        findEssentialMatrix(v[3],v[20],E);
        computerPoseByEssentialMat(E,R,t);

        findEssentialMatrix(v[3],v[30],E);
        computerPoseByEssentialMat(E,R,t);
    }
    else if(savevideo)
    {
        cout << "save video record....." << endl;
        VideoRW videorw;
        videorw.savevideo(filename,numbuf);
    }
    else if(strcmp(testMode,"pose") == 0)
    {
        camera.start_camera_thread();
        while(true)
        {
            Mat dst;
            Mat E, R, t;
            camera.get_frame(framedata);
            bool ret = findEssentialMatrix(framedata.frame_queue[0],framedata.frame_queue[1],E);
            if (ret){
                computerPoseByEssentialMat(E,R,t);
            }
        }
    }
    else if(strcmp(testMode,"pose-H") == 0)
    {
        camera.start_camera_thread();
        Mat K = (Mat_<double>(3, 3) << 657.4, 0, 319.5, 0, 657.4, 239.5, 0, 0, 1);
        while(true)
        {
            Mat dst;
            Mat H, R, t;
            //int solutions = 0;
            vector<Mat> Rs_decomp, ts_decomp, normals_decomp;
            camera.get_frame(framedata);
            bool ret = CalculateHomographyMatrix(framedata.frame_queue[0],framedata.frame_queue[1],H);
            if (ret)
            {
                int solutions = decomposeHomographyMat(H,K,Rs_decomp,ts_decomp,normals_decomp);
                for(int i = 0; i < solutions; i++)
                {
                    double factor_d1 = 1.0;
                    Mat rvec_decomp;
                    Rodrigues(Rs_decomp[i], rvec_decomp);
                    cout << "Solution " << i << ":" << endl;
                    //cout << "rvec from homography decomposition: " << rvec_decomp.t() << endl;
                    //cout << "rvec from camera displacement: " << rvec_1to2.t() << endl;
                    cout << "tvec from homography decomposition: " << ts_decomp[i].t() << " and scaled by d: " << factor_d1 * ts_decomp[i].t() << endl;
                    //cout << "tvec from camera displacement: " << t_1to2.t() << endl;
                    //cout << "plane normal from homography decomposition: " << normals_decomp[i].t() << endl;
                    //cout << "plane normal at camera 1 pose: " << normal1.t() << endl << endl;
                }
            }
        }
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
