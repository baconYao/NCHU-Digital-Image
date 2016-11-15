#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
  
  
using namespace cv;  
  
  
int main()  
{  
	//輸入三張影像(長寬皆相同)
    Mat img = imread("3744.jpg");
	Mat img2 = imread("3745.jpg");
	Mat img3 = imread("3746.jpg");
	int nChannels = img.channels();
	//影像高
	int nRows = img.rows;
	//影像寬
	int nCols = img.cols;

	//宣告輸出的image
	Mat dstMat( nRows, nCols, img.type());
	float fR, fG, fB;
	int iR, iG, iB;

	for( int j = 0; j < nRows; j++ ) {
        uchar* srcImg = img.ptr<uchar>(j);
        uchar* srcImg2 = img2.ptr<uchar>(j);
        uchar* srcImg3 = img3.ptr<uchar>(j);
		uchar* dstData = dstMat.ptr<uchar>(j);

        for( int i = 0; i < nCols; i++) {
			//將三張image的三個channel取平均值 (float型態，還需轉換成int，並處理outofbound)
			fR = (srcImg[nChannels*i + 2] + srcImg2[nChannels*i + 2] + srcImg3[nChannels*i + 2]) / 3;
			fG = (srcImg[nChannels*i + 1] + srcImg2[nChannels*i + 1] + srcImg3[nChannels*i + 1]) / 3;
			fB = (srcImg[nChannels*i + 0] + srcImg2[nChannels*i + 0] + srcImg3[nChannels*i + 0]) / 3;
			
			//float轉換成int
			iR = int (fR);
			iG = int (fG);
			iB = int (fB);

			//處理outofbound
			if(iR > 255){iR = 255;}
			if(iG > 255){iG = 255;}
			if(iB > 255){iB = 255;}

			//將出裡好的RGB value assign給dstMat(輸出的image)
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
	//輸出image
	imwrite("output.jpg",dstMat);
    
	//建立一個window視窗名為「遊戲畫面」
    namedWindow("輸出結果");  
    //在window顯示圖片
    imshow("輸出結果",dstMat);  
    //等待3s後自動關閉window
    waitKey(3000);  
}  