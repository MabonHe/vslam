/*************************************************************************
	> File Name: match.h
	> Author:
	> Mail:
	> Created Time: 2020年06月26日 星期五 14时15分56秒
 ************************************************************************/

#ifndef _MATCH_H
#define _MATCH_H
#include<opencv2/features2d.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/calib3d.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/core/mat.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
using namespace cv;
using namespace std;
bool CalculateHomographyMatrix(Mat &full,Mat &frame,Mat &H);
void crossCheckMatching(Ptr<DescriptorMatcher>& descriptorMatcher,const Mat& descriptors1, const Mat& descriptors2,vector<DMatch>& filteredMatches12, int knn);
void simpleMatching(Ptr<DescriptorMatcher>& descriptorMatcher,
    const Mat& descriptors1, const Mat& descriptors2,
    vector<DMatch>& matches12);
bool estimatePoseRANSAC(Mat &full,Mat &frame);
bool findFundamentalMatrix(Mat &full,Mat &frame,Mat &F);
bool findEssentialMatrix(Mat &full,Mat &frame,Mat &E);
bool computeCorrespondEpipolarLines(Mat &F);
bool computerPoseByEssentialMat(Mat &E);
#endif
