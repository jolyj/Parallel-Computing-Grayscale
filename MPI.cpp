#include <iostream>
#include <mpi.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

using namespace std;
using namespace cv;

#define A 0.1140
#define B 0.5870
#define C 0.2989

const int MAXBYTES= 8 * 1080 * 1920;
uchar buffer_send[MAXBYTES];

void matsnd(Mat &m, int dest, int id, int totalnodes) {
      int startval = m.rows * id / totalnodes + 1;
      int endval = m.rows * (id + 1) / totalnodes;

      uchar * pData = m.data;
      for(int i = startval; i < endval; i++)
      {
        for(int j = 0; j < m.cols; j++)
        {
          pData[(i - startval) * m.cols + j]=(A*m.at<Vec3b>(i,j)[0] + B*m.at<Vec3b>(i,j)[1] + C*m.at<Vec3b>(i,j)[2]);
        }
      }

      int bytes = (endval - startval) * m.cols;
      Mat outImg(m.rows, m.cols, CV_8UC1);
      memcpy(outImg.data, pData, bytes);
      MPI_Send(outImg.data, bytes, MPI_UNSIGNED_CHAR, dest, 0, MPI_COMM_WORLD);
}

uchar *matrcv(int src) {
      MPI_Status status;
      int count;
      uchar *buffer = new uchar[MAXBYTES];
      MPI_Recv(buffer, MAXBYTES * sizeof(uchar), MPI_UNSIGNED_CHAR, src, 0, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status,MPI_UNSIGNED_CHAR,&count);
      return buffer;
}

int main ( int argc, char *argv[] )
{
   MPI::Init (argc,argv);

   int totalnodes;
   int id = MPI::COMM_WORLD.Get_rank();
   MPI_Comm_size(MPI_COMM_WORLD, &totalnodes); // get totalnodes
   Mat image=imread("img.jpg",IMREAD_COLOR);

   std::clock_t start;
   double duration;
   start = std::clock();

   if (id == 0)
   {
      int startval = image.rows * id / totalnodes + 1;
      int endval = image.rows * (id + 1) / totalnodes;

      uchar * pData = image.data;
      for(int i = startval; i < endval; i++)
      {
        for(int j = 0; j < image.cols; j++)
        {
          pData[i * image.cols + j]=(A*image.at<Vec3b>(i,j)[0] + B*image.at<Vec3b>(i,j)[1] + C*image.at<Vec3b>(i,j)[2]);
        }
      }

      int bytes = (endval - startval) * image.cols;
      Mat outImg(image.rows, image.cols, CV_8UC1);
      memcpy(outImg.data, pData, bytes);

      uchar *buf1 = matrcv(1);
      uchar *buf2 = matrcv(2);
      uchar *buf3 = matrcv(3);

      memcpy(outImg.data + bytes, buf1, bytes);
      memcpy(&outImg.data[2 * bytes], buf2, bytes);
      memcpy(&outImg.data[3 * bytes], buf3, bytes);

      duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

      std::cout<<"MPI grayscale duration: "<< duration <<'\n';

      namedWindow("MPI grayscale",CV_WINDOW_AUTOSIZE);
      imshow("MPI grayscale", outImg);
      waitKey(0);

      delete buf1;
      delete buf2;
      delete buf3;
   }
   else {
      matsnd(image, 0, id, totalnodes);
   }

   MPI::Finalize();
}
