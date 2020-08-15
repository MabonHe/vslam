/*************************************************************************
	> File Name: camera.cpp
	> Author: 
	> Mail: 
	> Created Time: 2020年06月18日 星期四 17时37分29秒
 ************************************************************************/

#include"camera.h"
using namespace std;
using namespace cv;
struct FrameData frame_data;
Camera::Camera()
{
    frame_data.is_empty=true;
}
Camera::~Camera()
{

}
void 
Camera::get_frame(struct FrameData &framedata)
{
    frame_data.frame_queue.clear();
    frame_data.is_empty = true;
    int count = 0;
    while(true)
    {
        sleep(1);
        count++;
        if (frame_data.is_empty == false){
            framedata.frame_queue = frame_data.frame_queue;
            break;
        }

        if (count >= 5){
            cerr << "cannot get a frame data" << endl;
            break;
        }

    }
}
void
Camera::start_camera_thread()
{
    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    ret = pthread_create(&pid,&attr,&startcamera,&frame_data);
    
}
void * Camera::startcamera(void *args)
{
    struct FrameData *framedata = (struct FrameData *)args;
    Mat frame;
    VideoCapture cap;
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    cap.open(deviceID, apiID);
    cap.set(CAP_PROP_FRAME_WIDTH,640);
    cap.set(CAP_PROP_FRAME_HEIGHT,480);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return (void *)-1;
    }
    
    for(;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        if (frame_data.is_empty){
            frame_data.frame_queue.push_back(frame);
            if (frame_data.frame_queue.size() >= 2)
                frame_data.is_empty = false;
        }
        // show live and wait for a key with timeout long enough to show images
        imshow("Live", frame);
        waitKey(5);
    }
}

