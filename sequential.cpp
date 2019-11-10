
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

#define A 0.1140
#define B 0.5870
#define C 0.2989

using namespace std;
using namespace cv;

int main()
{
	Mat image;
	string fname = "img.jpg";
	image = imread(fname,CV_LOAD_IMAGE_COLOR);
	Mat grayscale(image.rows,image.cols,CV_8UC1,Scalar(0));

	std::clock_t start;
	double duration;
	start = std::clock();

	for(int i=0; i<image.rows;i++)
	{
		for(int j=0;j<image.cols;j++)
		{
			grayscale.at<uchar>(i,j)=(A*image.at<Vec3b>(i,j)[0] + B*image.at<Vec3b>(i,j)[1] + C*image.at<Vec3b>(i,j)[2]);
		}
	}

	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

	std::cout<<"sequential grayscale duration: "<< duration <<'\n';

	namedWindow("sequential grayscale",CV_WINDOW_AUTOSIZE);
	imshow("sequential grayscale",grayscale);
	waitKey(0);
	return 0;
}
