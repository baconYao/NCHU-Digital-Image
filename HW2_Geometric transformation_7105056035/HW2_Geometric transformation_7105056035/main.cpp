/* 
	HW2: Geometric transformation
	�m�W: �i����
	�Ǹ�: 7105056035

	ref: 
		http://yester-place.blogspot.tw/2008/06/opencv.html
		http://monkeycoding.com/?p=653
	

*/

#include <iostream>
#include "cv.h"  																			//for �q����ı�禡�w
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>								//for GUI�������禡�w

using namespace std;     
using namespace cv;

int points = 0;																		//�p��ϥΪ̿�ܪ��u�I�v���ƶq�A�̦h��|�I
int outputWidth;																	//��Ximage���e
int outputHeight;																	//��Ximage����
int sortX[4];																			//�ƧǹL��X�I
int sortY[4];																			//�ƧǹL��Y�I

Mat showImg;
Point selectedPoint[4] = {0};

double inverseMappingArr[16] = {0};								//for Inverse Mapping
double fourPointsArr[8] = {0};										//�s��user��ܪ��|�Ӯy��

// Mouse�ƥ�
void onMouse(int Event, int x,int y, int flags, void* param )
{	
	if(Event == EVENT_LBUTTONDOWN)
	{
		circle(showImg, Point(x, y), 6, Scalar(0, 0, 255),2);
		imshow("��J�v��-��|�I", showImg);
		selectedPoint[points] = Point(x, y);		

		cout << "�ҿ諸�I(" << points << "): "<< selectedPoint[points] << endl;		
		points++;
	}
}


int main()  
{  
    // Ū����l�v��
	showImg = imread("test.jpg");
  imshow("��J�v��-��|�I", showImg);
	
	// originalImg - ��J�Ψӭp�⪺��; matchImg - �e�X�������ϤW�I;
  Mat originalImg = imread("test.jpg");
	Mat matchImg = imread("test.jpg");

	while(1)
	{
		setMouseCallback("��J�v��-��|�I", onMouse, NULL);
		waitKey(1);
		
		if (points == 4)
		{
			cout << "�w�ﺡ" << endl;
			
			//x, y�y�ХѤp��j�Ƨ�
			for(int i = 0; i < points; i++)
			{
				sortX[i] = selectedPoint[i].x;
				sortY[i] = selectedPoint[i].y;
			}
			sort(sortX, sortX + 4);
			sort(sortY, sortY + 4);
			// �p��e��
			if (sortY[3] - sortY[0] != 0){
				outputHeight = sortY[3] - sortY[0];
			}
			else{
				outputHeight = sortY[0];
			}
			// �p�Ⱚ��
			if (sortX[3] - sortX[0] != 0){
				outputWidth = sortX[3] - sortX[0];
			}
			else{
				outputWidth = sortX[0];
			}

			// �]�w��X�v�����榡�Τj�p
			Mat Output_Img( outputHeight, outputWidth, CV_8UC3, Scalar(0,0,0));

			//Inverse Mapping�A���X�Ϥ���[x y xy 1] ��J�}�CinverseMappingArr��
			inverseMappingArr[0] = 0;            
			inverseMappingArr[1] = 0; 
			inverseMappingArr[2] = inverseMappingArr[0] * inverseMappingArr[1];  
			inverseMappingArr[3] = 1; 
			
			inverseMappingArr[4] = outputWidth; 
			inverseMappingArr[5] = 0; 
			inverseMappingArr[6] = inverseMappingArr[4] * inverseMappingArr[5];  
			inverseMappingArr[7] = 1; 
			
			inverseMappingArr[8] = outputWidth; 
			inverseMappingArr[9] = outputHeight; 
			inverseMappingArr[10] = inverseMappingArr[8] * inverseMappingArr[9];  
			inverseMappingArr[11] = 1; 
			
			inverseMappingArr[12] = 0;           
			inverseMappingArr[13] = outputHeight; 
			inverseMappingArr[14] = inverseMappingArr[12] * inverseMappingArr[13];  
			inverseMappingArr[15] = 1; 


			// ��ҿ諸4�Ӯy��(x, y)�ȩ�J�}�CfourPointsArr
			fourPointsArr[0] = selectedPoint[0].x;  
			fourPointsArr[1] = selectedPoint[0].y;
			
			fourPointsArr[2] = selectedPoint[1].x;  
			fourPointsArr[3] = selectedPoint[1].y;
			
			fourPointsArr[4] = selectedPoint[2].x;  
			fourPointsArr[5] = selectedPoint[2].y;
			
			fourPointsArr[6] = selectedPoint[3].x;  
			fourPointsArr[7] = selectedPoint[3].y;
						

			Mat Matrix_A = Mat(4, 4, CV_64F, inverseMappingArr);
			Mat Matrix_InvA;
			Mat Matrix_B = Mat(4, 2, CV_64F, fourPointsArr);
			Mat Matrix_Para;

			// ��SVD����k��XA���ϯx�}
			invert(Matrix_A, Matrix_InvA, DECOMP_SVD);

			// �ѥXTransformation���Ѽ�
			Matrix_Para = Matrix_InvA * Matrix_B;

			// ��X�����I
			int a, b;
			float Match_X = 0;
			float Match_Y = 0;

			for(int i = 0; i < Output_Img.rows; i++)
			{
				for(int j = 0; j < Output_Img.cols; j++)
				{	
					// ������I
					Match_X = Matrix_Para.at<double>(0, 0)*j + Matrix_Para.at<double>(1, 0)*i + Matrix_Para.at<double>(2, 0)*i*j + Matrix_Para.at<double>(3, 0);
					Match_Y = Matrix_Para.at<double>(0, 1)*j + Matrix_Para.at<double>(1, 1)*i + Matrix_Para.at<double>(2, 1)*i*j + Matrix_Para.at<double>(3, 1);			
					
					circle(matchImg, Point(int(Match_X), int(Match_Y)), 1, Scalar(0, 0, 255),1);
					
					// bilinear interpolation
					a = Match_X-(int)Match_X;
					b = Match_Y-(int)Match_Y;
					// B
					Output_Img.at<Vec3b>(i, j)[0] = (1-a) * (1-b) * originalImg.at<Vec3b>(Match_Y, Match_X)[0]+ a * (1-b) * originalImg.at<Vec3b>(Match_Y+1, Match_X)[0]+ (1-a) * b * originalImg.at<Vec3b>(Match_Y, Match_X+1)[0] + a * b * originalImg.at<Vec3b>(Match_Y+1, Match_X+1)[0];
					// G
					Output_Img.at<Vec3b>(i, j)[1] = (1-a)*(1-b)*originalImg.at<Vec3b>(Match_Y,Match_X)[1] + a * (1-b) * originalImg.at<Vec3b>(Match_Y+1,Match_X)[1] + (1-a) * b * originalImg.at<Vec3b>(Match_Y,Match_X+1)[1] + a * b * originalImg.at<Vec3b>(Match_Y+1,Match_X+1)[1];
					// R
					Output_Img.at<Vec3b>(i, j)[2] = (1-a) * (1-b) * originalImg.at<Vec3b>(Match_Y,Match_X)[2] + a * (1-b) * originalImg.at<Vec3b>(Match_Y+1,Match_X)[2] + (1-a) * b * originalImg.at<Vec3b>(Match_Y,Match_X+1)[2] + a * b * originalImg.at<Vec3b>(Match_Y+1,Match_X+1)[2];
				}
			}
			imshow("�����I", matchImg);			
			imshow("��X���v��", Output_Img);
			imwrite("output.jpg", Output_Img);
			break;
		}
	}
    waitKey(0);
	return 0;
}



