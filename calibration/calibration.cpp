// calibration.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
/*------------------------------------------------------------------------------------------*\
This file contains material supporting chapter 9 of the cookbook:
Computer Vision Programming using the OpenCV Library.
by Robert Laganiere, Packt Publishing, 2011.

This program is free software; permission is hereby granted to use, copy, modify,
and distribute this source code, or portions thereof, for any purpose, without fee,
subject to the restriction that the copyright notice may not be removed
or altered from any source or altered source distribution.
The software is released on an as-is basis and without any warranties of any kind.
In particular, the software is not guaranteed to be fault-tolerant or free from failure.
The author disclaims all warranties with regard to this software, any use,
and any consequent failure, is purely the responsibility of the user.

Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "CameraCalibrator.h"
using namespace std;
using namespace cv;


int main()
{

	namedWindow("Image");
	Mat image;
	vector<string> filelist;

	// generate list of chessboard image filename
	for (int i = 1; i <= 13; i++) {

		stringstream str;
		str <<"left" << setw(2) << setfill('0') << i << ".jpg";
		cout << str.str() << endl;
		filelist.push_back(str.str());
		image = imread(str.str(), 0);
		imshow("Image", image);

		waitKey(100);
	}
	/*filelist.push_back("D:\\form\\Application\\left01.jpg");
	image = imread("D:\\form\\Application\\left01.jpg");
	imshow("Image", image);

	waitKey(100);*/

	// Create calibrator object
	CameraCalibrator cameraCalibrator;
	// add the corners from the chessboard
	Size boardSize(9, 6);
	cameraCalibrator.addChessboardPoints(
		filelist,	// filenames of chessboard image
		boardSize);	// size of chessboard
					// calibrate the camera
					//	cameraCalibrator.setCalibrationFlag(true,true);
	cameraCalibrator.calibrate(image.size());

	// Image Undistortion
	image = imread(filelist[2]);
	Mat uImage = cameraCalibrator.remap(image);
	imshow("Original Image", image);
	imshow("Undistorted Image", uImage);

	// display camera matrix
	Mat cameraMatrix = cameraCalibrator.getCameraMatrix();
	cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << endl;
	cout << cameraMatrix.at<double>(0, 0) << " " << cameraMatrix.at<double>(0, 1) << " " << cameraMatrix.at<double>(0, 2) << endl;
	cout << cameraMatrix.at<double>(1, 0) << " " << cameraMatrix.at<double>(1, 1) << " " << cameraMatrix.at<double>(1, 2) << endl;
	cout << cameraMatrix.at<double>(2, 0) << " " << cameraMatrix.at<double>(2, 1) << " " << cameraMatrix.at<double>(2, 2) << endl;

	Mat distCoeffs = cameraCalibrator.getDistCoeffs();
	cout << "畸变系数矩阵：" << distCoeffs.rows << "x" << distCoeffs.cols << endl;
	for (int i = 0; i<distCoeffs.rows; i++)
		for (int j = 0; j<distCoeffs.cols; j++)
			cout << distCoeffs.at<double>(i, j) << "\t";
	waitKey(0);

	waitKey();
	return 0;
}

