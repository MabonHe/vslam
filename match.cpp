/*************************************************************************
	> File Name: match.cpp
	> Author:
	> Mail:
	> Created Time: 2020年06月26日 星期五 14时15分42秒
 ************************************************************************/

#include"match.h"
#include"RobustMatcher.h"
vector<Point2f> g_points1;
vector<Point2f> g_points2;
bool CalculateHomographyMatrix(Mat &full,Mat &frame,Mat &H)
{
    double ransacReprojThreshold = 5;
    Ptr<ORB> detector = ORB::create(2000);
    Ptr<DescriptorExtractor> descriptorExtractor;
    Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce");
    vector<KeyPoint> keypoints1;
    detector->detect(full, keypoints1);
    Mat descriptors1;
    detector->compute(full, keypoints1, descriptors1);

    vector<KeyPoint> keypoints2;
    detector->detect(frame, keypoints2);
    cout << "keypointsize:" << keypoints2.size() << endl;
    Mat descriptors2;
    detector->compute(frame, keypoints2, descriptors2);
    //draw keypoints
    Mat img_keypoints1, img_keypoints2,img_match;
    drawKeypoints(full,keypoints1,img_keypoints1,Scalar::all(-1));
    drawKeypoints(frame,keypoints2,img_keypoints2,Scalar::all(-1));
    char outputfile[128]="/home/maobin/workspace/vslam/full.bmp";
    //imwrite(outputfile,img_keypoints1);
    //imshow("full",img_keypoints1);
    //imshow("frame",img_keypoints2);
    char outputfile_frame[128]="/home/maobin/workspace/vslam/frame.bmp";
    //imwrite(outputfile_frame,img_keypoints2);

    if (keypoints1.size() < 20 || keypoints2.size() < 20)
    {
        cout << "Cannot find enough key points" << endl;
        return false;
    }
    vector<DMatch> filteredMatches;
    crossCheckMatching(descriptorMatcher, descriptors1, descriptors2, filteredMatches, 1);
    drawMatches(full,keypoints1,frame,keypoints2,filteredMatches,img_match,Scalar::all(-1),Scalar::all(-1));
    //imshow("match",img_match);
    //waitKey(1);
    char outputfile_match[128]="/home/maobin/workspace/vslam/match.bmp";
    //imwrite(outputfile_match,img_match);
    vector<int> queryIdxs(filteredMatches.size()), trainIdxs(filteredMatches.size());
    for (size_t i = 0; i < filteredMatches.size(); i++)
    {
        queryIdxs[i] = filteredMatches[i].queryIdx;
        trainIdxs[i] = filteredMatches[i].trainIdx;
    }
    vector<Point2f> points1;
    KeyPoint::convert(keypoints1, points1, queryIdxs);

    vector<Point2f> points2;
    Mat noArray;
    KeyPoint::convert(keypoints2, points2, trainIdxs);
    H = findHomography(Mat(points1), Mat(points2), noArray, RANSAC, ransacReprojThreshold);
    if (H.empty())
    {
        return false;
    }
        double H00 = *(double*)H.ptr(0, 0);
        double H01 = *(double*)H.ptr(0, 1);
        double H02 = *(double*)H.ptr(0, 2);
        double H10 = *(double*)H.ptr(1, 0);
        double H11 = *(double*)H.ptr(1, 1);
        double H12 = *(double*)H.ptr(1, 2);
        double H20 = *(double*)H.ptr(2, 0);
        double H21 = *(double*)H.ptr(2, 1);
        double H22 = *(double*)H.ptr(2, 2);
        cout <<"H00:" << H00 <<endl;
        cout <<"H01:" << H01 <<endl;
        cout <<"H02:" << H02 <<endl;
        cout <<"H10:" << H10 <<endl;
        cout <<"H11:" << H11 <<endl;
        cout <<"H12:" << H12 <<endl;
        cout <<"H20:" << H20 <<endl;
        cout <<"H21:" << H21 <<endl;
        cout <<"H22:" << H22 <<endl;
    return true;
}
void crossCheckMatching(Ptr<DescriptorMatcher>& descriptorMatcher,
    const Mat& descriptors1, const Mat& descriptors2,
    vector<DMatch>& filteredMatches12, int knn = 1)
{
    filteredMatches12.clear();
    vector<vector<DMatch> > matches12, matches21;
    descriptorMatcher->knnMatch(descriptors1, descriptors2, matches12, knn);
    descriptorMatcher->knnMatch(descriptors2, descriptors1, matches21, knn);

    DMatch forward;
    DMatch backward;

    for (size_t m = 0; m < matches12.size(); m++)
    {
        bool findCrossCheck = false;
        for (size_t fk = 0; fk < matches12[m].size(); fk++)
        {
            forward = matches12[m][fk];

            for (size_t bk = 0; bk < matches21[forward.trainIdx].size(); bk++)
            {
                backward = matches21[forward.trainIdx][bk];
                if (backward.trainIdx == forward.queryIdx)
                {
                    filteredMatches12.push_back(forward);
                    findCrossCheck = true;
                    break;
                }
            }
            if (findCrossCheck) break;
        }
    }
}
void simpleMatching(Ptr<DescriptorMatcher>& descriptorMatcher,
    const Mat& descriptors1, const Mat& descriptors2,
    vector<DMatch>& matches12)
{
    vector<DMatch> matches;
    descriptorMatcher->match(descriptors1, descriptors2, matches12);
}

bool estimatePoseRANSAC(Mat &full,Mat &frame)
{
    double ransacReprojThreshold = 5;
    Ptr<ORB> detector = ORB::create(2000);
    Ptr<DescriptorExtractor> descriptorExtractor;
    Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce");
    vector<KeyPoint> keypoints1;
    detector->detect(full, keypoints1);
    Mat descriptors1;
    detector->compute(full, keypoints1, descriptors1);

    vector<KeyPoint> keypoints2;
    detector->detect(frame, keypoints2);
    cout << "keypointsize:" << keypoints2.size() << endl;
    Mat descriptors2;
    detector->compute(frame, keypoints2, descriptors2);
    //draw keypoints
    Mat img_keypoints1, img_keypoints2,img_match;
    drawKeypoints(full,keypoints1,img_keypoints1,Scalar::all(-1));
    drawKeypoints(frame,keypoints2,img_keypoints2,Scalar::all(-1));
    char outputfile[128]="/home/maobin/workspace/vslam/full.bmp";
    //imwrite(outputfile,img_keypoints1);
    //imshow("full",img_keypoints1);
    //imshow("frame",img_keypoints2);
    char outputfile_frame[128]="/home/maobin/workspace/vslam/frame.bmp";
    //imwrite(outputfile_frame,img_keypoints2);

    if (keypoints1.size() < 20 || keypoints2.size() < 20)
    {
        cout << "Cannot find enough key points" << endl;
        return false;
    }
    vector<DMatch> filteredMatches;
    crossCheckMatching(descriptorMatcher, descriptors1, descriptors2, filteredMatches, 1);
    drawMatches(full,keypoints1,frame,keypoints2,filteredMatches,img_match,Scalar::all(-1),Scalar::all(-1));
    //imshow("match",img_match);
    char outputfile_match[128]="/home/maobin/workspace/vslam/match.bmp";
    //imwrite(outputfile_match,img_match);
    vector<int> queryIdxs(filteredMatches.size()), trainIdxs(filteredMatches.size());
    for (size_t i = 0; i < filteredMatches.size(); i++)
    {
        queryIdxs[i] = filteredMatches[i].queryIdx;
        trainIdxs[i] = filteredMatches[i].trainIdx;
    }
    vector<Point2f> points1;
    KeyPoint::convert(keypoints1, points1, queryIdxs);

    vector<Point2f> points2;
    Mat noArray;
    KeyPoint::convert(keypoints2, points2, trainIdxs);

    int iterationsCount = 500;        // number of Ransac iterations.
    float reprojectionError = 2.0;    // maximum allowed distance to consider it an inlier.
    float confidence = 0.95;          // RANSAC successful confidence.

    Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64FC1);    // vector of distortion coefficients
    Mat rvec = cv::Mat::zeros(3, 1, CV_64FC1);          // output rotation vector
    Mat tvec = cv::Mat::zeros(3, 1, CV_64FC1);          // output translation vector
    bool useExtrinsicGuess = false;   // if true the function uses the provided rvec and tvec values as
                                      // initial approximations of the rotation and translation vectors

    vector<cv::Point3f> list_points3d_model_match;    // container for the model 3D coordinates found in the scene
    vector<cv::Point2f> list_points2d_scene_match;    // container for the model 2D coordinates found in the scene

    for(unsigned int match_index = 0; match_index < filteredMatches.size(); ++match_index)
    {
        //Point3f point3d_model = list_points3d_model[ filteredMatches[match_index].trainIdx ];   // 3D point from model
        //Point2f point2d_scene = keypoints_scene[ filteredMatches[match_index].queryIdx ].pt;    // 2D point from the scene
        //list_points3d_model_match.push_back(point3d_model);                                      // add 3D point
        //list_points2d_scene_match.push_back(point2d_scene);                                      // add 2D point
    }
}
bool findFundamentalMatrix(Mat &full,Mat &frame,Mat &F)
{
    int numKeyPoints = 1000;
    RobustMatcher rmatcher;                                                          // instantiate RobustMatcher
    Ptr<FeatureDetector> detector = ORB::create(numKeyPoints);
    //cv::FeatureDetector * detector = new OrbFeatureDetector(numKeyPoints);       // instantiate ORB feature detector
    Ptr<DescriptorExtractor> extractor = ORB::create();
    //cv::DescriptorExtractor * extractor = new OrbDescriptorExtractor();          // instantiate ORB descriptor extractor
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    rmatcher.setFeatureDetector(detector);                                           // set feature detector
    rmatcher.setDescriptorExtractor(extractor);                                      // set descriptor extractor
    rmatcher.setDescriptorMatcher(matcher);
    vector<DMatch> good_match;
    vector<KeyPoint> keypoints_frame,keypoints_ref;
    rmatcher.robustMatch(frame,good_match,keypoints_frame,full,keypoints_ref);

    if (good_match.size() < 10)
    {
        cout << "[ERROR] keypoints less than 10:" << good_match.size() << endl;
        return false;
    }
    vector<int> queryIdxs(good_match.size()), trainIdxs(good_match.size());
    for (size_t i = 0; i < good_match.size(); i++)
    {
        queryIdxs[i] = good_match[i].queryIdx;
        trainIdxs[i] = good_match[i].trainIdx;
    }
    vector<Point2f> points1;
    KeyPoint::convert(keypoints_frame, points1, queryIdxs);

    vector<Point2f> points2;
    KeyPoint::convert(keypoints_ref, points2, trainIdxs);
    F = findFundamentalMat(points1,points2,FM_RANSAC,3,0.99);

    return true;

}
bool findEssentialMatrix(Mat &full,Mat &frame,Mat &E){
    int numKeyPoints = 1000;
    RobustMatcher rmatcher;                                                          // instantiate RobustMatcher
    Ptr<FeatureDetector> detector = ORB::create(numKeyPoints);
    //cv::FeatureDetector * detector = new OrbFeatureDetector(numKeyPoints);       // instantiate ORB feature detector
    Ptr<DescriptorExtractor> extractor = ORB::create();
    //cv::DescriptorExtractor * extractor = new OrbDescriptorExtractor();          // instantiate ORB descriptor extractor
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    rmatcher.setFeatureDetector(detector);                                           // set feature detector
    rmatcher.setDescriptorExtractor(extractor);                                      // set descriptor extractor
    rmatcher.setDescriptorMatcher(matcher);
    vector<DMatch> good_match;
    vector<KeyPoint> keypoints_frame,keypoints_ref;
    rmatcher.robustMatch(frame,good_match,keypoints_frame,full,keypoints_ref);

    if (good_match.size() < 10)
    {
        cout << "[ERROR] keypoints less than 10:" << good_match.size() << endl;
        return false;
    }
    vector<int> queryIdxs(good_match.size()), trainIdxs(good_match.size());
    for (size_t i = 0; i < good_match.size(); i++)
    {
        queryIdxs[i] = good_match[i].queryIdx;
        trainIdxs[i] = good_match[i].trainIdx;
    }
    vector<Point2f> points1;
    KeyPoint::convert(keypoints_frame, points1, queryIdxs);
    g_points1 = points1;

    vector<Point2f> points2;
    KeyPoint::convert(keypoints_ref, points2, trainIdxs);
    g_points2 = points2;
    //logic camera matrix
    Mat K = (Mat_<double>(3, 3) << 657.4, 0, 319.5, 0, 657.4, 239.5, 0, 0, 1);
    E = findEssentialMat(points1,points2,K,RANSAC,0.99,1.0);
    return true;
}
bool computeCorrespondEpipolarLines(Mat &F)
{


    return true;
}
bool computerPoseByEssentialMat(Mat &E,Mat &R,Mat &t)
{
    Mat K = (Mat_<double>(3, 3) << 657.4, 0, 319.5, 0, 657.4, 239.5, 0, 0, 1);
    recoverPose(E,g_points1,g_points2,K,R,t);
    cout << "R is" <<endl << R << endl;
    cout << "t is" << endl << t << endl;
    return true;
}
