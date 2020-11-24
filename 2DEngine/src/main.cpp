#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

int main(int argc, char* argv[]) {

	cv::Mat img = cv::imread("Test.png");

	cv::imshow("Naja", img);

	int k = cv::waitKey(0);
}