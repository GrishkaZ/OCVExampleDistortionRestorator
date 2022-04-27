#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>


using namespace std;
using namespace cv;


int main() {

	String FILE_PATH = "../out_camera_data.xml";

	FileStorage fs;
	fs.open(FILE_PATH, FileStorage::READ);

	if (!fs.isOpened()) {
		cerr << "Failed to open " << FILE_PATH << endl;
		return 1;
	}

	Mat cameraMatrix, distCoeffs;
	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distCoeffs;

	bool useFisheye;
	fs["fisheye_model"] >> useFisheye;

	cout << endl
		<< "cameraMatrix = " << cameraMatrix << endl;
	cout << "distCoeffs = " << distCoeffs << endl << endl;


	Mat src = imread("../images/1.jpeg");

	Mat res, map1, map2;
	Size imageSize = src.size();

	if (useFisheye)
	{
		Mat newCamMat;
		fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix, distCoeffs, imageSize,
			Matx33d::eye(), newCamMat, 1);
		fisheye::initUndistortRectifyMap(cameraMatrix, distCoeffs, Matx33d::eye(), newCamMat, imageSize,
			CV_16SC2, map1, map2);
	}
	else
	{
		initUndistortRectifyMap(
			cameraMatrix, distCoeffs, Mat(),
			getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0), imageSize,
			CV_16SC2, map1, map2);
	}


	remap(src, res, map1, map2, INTER_LINEAR);

	imwrite("../res.png", res);

	imshow("Image View", res);
	int k = waitKey(0);

	



	return 0;
}










