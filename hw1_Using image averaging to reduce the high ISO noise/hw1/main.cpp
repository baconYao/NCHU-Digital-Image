#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
  
  
using namespace cv;  
  
  
int main()  
{  
	//��J�T�i�v��(���e�ҬۦP)
    Mat img = imread("3744.jpg");
	Mat img2 = imread("3745.jpg");
	Mat img3 = imread("3746.jpg");
	int nChannels = img.channels();
	//�v����
	int nRows = img.rows;
	//�v���e
	int nCols = img.cols;

	//�ŧi��X��image
	Mat dstMat( nRows, nCols, img.type());
	float fR, fG, fB;
	int iR, iG, iB;

	for( int j = 0; j < nRows; j++ ) {
        uchar* srcImg = img.ptr<uchar>(j);
        uchar* srcImg2 = img2.ptr<uchar>(j);
        uchar* srcImg3 = img3.ptr<uchar>(j);
		uchar* dstData = dstMat.ptr<uchar>(j);

        for( int i = 0; i < nCols; i++) {
			//�N�T�iimage���T��channel�������� (float���A�A�ٻ��ഫ��int�A�óB�zoutofbound)
			fR = (srcImg[nChannels*i + 2] + srcImg2[nChannels*i + 2] + srcImg3[nChannels*i + 2]) / 3;
			fG = (srcImg[nChannels*i + 1] + srcImg2[nChannels*i + 1] + srcImg3[nChannels*i + 1]) / 3;
			fB = (srcImg[nChannels*i + 0] + srcImg2[nChannels*i + 0] + srcImg3[nChannels*i + 0]) / 3;
			
			//float�ഫ��int
			iR = int (fR);
			iG = int (fG);
			iB = int (fB);

			//�B�zoutofbound
			if(iR > 255){iR = 255;}
			if(iG > 255){iG = 255;}
			if(iB > 255){iB = 255;}

			//�N�X�̦n��RGB value assign��dstMat(��X��image)
			dstData[nChannels*i + 2] = iR;
            dstData[nChannels*i + 1] = iG;
            dstData[nChannels*i + 0] = iB;

			/*
            srcImg[nChannels*i + 2] = 0;				//R channel
			srcImg[nChannels*i + 1] = 128;				//G channel
            srcImg[nChannels*i + 0] = 128;				//B channel
			*/
		}
        
    }
	//��Ximage
	imwrite("output.jpg",dstMat);
    
	//�إߤ@��window�����W���u�C���e���v
    namedWindow("��X���G");  
    //�bwindow��ܹϤ�
    imshow("��X���G",dstMat);  
    //����3s��۰�����window
    waitKey(3000);  
}  