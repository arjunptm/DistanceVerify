// DistanceVerify.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <opencv2/aruco.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <cmath>


void setLabel(cv::Mat& im, const std::string label, cv::Point& loc ) {
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 1;
    int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::rectangle(im, loc +cv::Point(0, baseline), loc +cv::Point(text.width, -text.height), CV_RGB(0, 0, 0), cv::FILLED);
    cv::putText(im, label, loc , fontface, scale, CV_RGB(255, 255, 255), thickness, 8);
    loc.x += 25;
    loc.y += 25;
}

double distance(double x1, double x2, double y1, double y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

double computeArea(cv::Point2f a, cv::Point2f b, cv::Point2f c, cv::Point2f d) {
    double length = distance(a.x, b.x, a.y, b.y);
    double width = distance(a.x, c.x, a.y, c.y);
    return length * width;
}

void calibrator(int& current_state, cv::Mat& im, cv::Point tPos, std::vector<std::vector<cv::Point2f> > corners) {
    switch (current_state)
    {
    case 0: setLabel(im, "Place at closest distance and", tPos);
        setLabel(im, "press 'A' to save value.", tPos);
        if (GetKeyState('A') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
        {
            computeArea(corners[0][0], corners[0][1], corners[0][2], corners[0][3]);
            std::cout<< computeArea(corners[0][0], corners[0][1], corners[0][2], corners[0][3]);
        }
        break;
    default:
        break;
    }
}

int main() {
    cv::VideoCapture inputVideo;
    inputVideo.open(0);
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    int current_state = 0;
    cv::Point textPos = cv::Point(25, 25);

    while (inputVideo.grab()) {
        cv::Mat image, imageCopy;
        inputVideo.retrieve(image);
        image.copyTo(imageCopy);


        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f> > corners;
        cv::aruco::detectMarkers(image, dictionary, corners, ids);


        // if at least one marker detected
        if (ids.size() > 0) {
            cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
            calibrator(current_state, imageCopy, textPos, corners);
        }
        else {
            cv::Point tPos = textPos;
            setLabel(imageCopy, "No markers detected!", tPos);
        }


        cv::imshow("out", imageCopy);
        char key = (char)cv::waitKey(10);
        if (key == 27)
            break;
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
