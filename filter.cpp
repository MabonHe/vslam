/*************************************************************************
	> File Name: filter.cpp
	> Author: 
	> Mail: 
	> Created Time: 2020年06月23日 星期二 16时51分52秒
 ************************************************************************/

#include"filter.h"

Filter::Filter()
{

}
Filter::~Filter()
{

}

void
Filter::filter(Mat &src,Mat &dst,int filter_size)
{
    int width = src.cols;
    int height = src.rows;
    int ysrc = 0;
    dst = src;
    cout << "step:" << src.step << endl;
    for (int y = 0;y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //ysrc = src.data[x+y*src.step];
            ysrc = y_average(src,filter_size,x+y*src.step);
            dst.data[x+y * dst.step] = ysrc;
        }
    }
}
int 
Filter::y_average(Mat &src,int filter_size,int step)
{
    int avera_y = 0,ysrc = 0,count = 0;
    int width = src.cols;
    int height = src.rows;
    for (int y = 0; y < filter_size; y++)
    {
        for (int x = 0; x < filter_size; x++ )
        {
            if ((step + x * y) > (width * height))
            {
            }
            else
            {
                count ++;
                ysrc +=src.data[step + x * y ];
            }
        }
    }
    avera_y = ysrc / count;

    return avera_y;
    
}
