#include <iostream>
#include <mpi.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define A 0.1140
#define B 0.5870
#define C 0.2989

const int MAXBYTES=8*1024*1024;
uchar buffer_send[MAXBYTES];

int main() {
  Mat image=imread("img.jpg",IMREAD_COLOR);
  unsigned char * pData = image.data;

  for(int i = 0; i < image.rows; i++)
  {
    for(int j = 0; j < image.cols; j++)
    {
      pData[i * image.cols + j]=(A*image.at<Vec3b>(i,j)[0] + B*image.at<Vec3b>(i,j)[1] + C*image.at<Vec3b>(i,j)[2]);
    }
  }

  Mat outImg(image.rows, image.cols, CV_8UC1);
  memcpy(outImg.data, pData, sizeof(unsigned char)*image.rows*image.cols);

  namedWindow("buffer",CV_WINDOW_AUTOSIZE);
  imshow("buffer", outImg);
  waitKey(0);

}

// http://study.marearts.com/2013/08/opencv-mat-data-point-access-and-memcpy.html
