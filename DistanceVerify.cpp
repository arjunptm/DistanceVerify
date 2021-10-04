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
#include <fstream>

using namespace std;


void setLabel(cv::Mat& im, const string label, cv::Point& loc ) {
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
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double computeAreaAndScale(cv::Point2f a, cv::Point2f b, cv::Point2f c, cv::Point2f d) {
    double length = distance(a.x, b.x, a.y, b.y)/10;
    double width = distance(a.x, c.x, a.y, c.y)/10;
    return length * width;
}

void calibrator(int& current_state, cv::Mat& im, cv::Point tPos, vector<vector<cv::Point2f> > corners, map<string, double>& calibVals) {
    ofstream myfile;
    switch (current_state)
    {
    case 1: setLabel(im, "Place at closest distance and", tPos);
        setLabel(im, "press 'A' to save value.", tPos);
        if (GetKeyState('A') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
        {
            //computeAreaAndScale(corners[0][0], corners[0][1], corners[0][2], corners[0][3]);
            calibVals.insert(pair<string, double>("First", computeAreaAndScale(corners[0][0], corners[0][1], corners[0][2], corners[0][3])));
            current_state++;
        }
        break;
    case 2: setLabel(im, "Place at second distance and", tPos);
        setLabel(im, "press 'B' to save value.", tPos);
        if (GetKeyState('B') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
        {
            //computeAreaAndScale(corners[0][0], corners[0][1], corners[0][2], corners[0][3]);
            calibVals.insert(pair<string, double>("Second", computeAreaAndScale(corners[0][0], corners[0][1], corners[0][2], corners[0][3])));
            current_state++;
        }
        break;
    case 3: setLabel(im, "Place at third distance and", tPos);
        setLabel(im, "press 'C' to save value.", tPos);
        if (GetKeyState('C') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
        {
            //computeAreaAndScale(corners[0][0], corners[0][1], corners[0][2], corners[0][3]);
            calibVals.insert(pair<string, double>("Third", computeAreaAndScale(corners[0][0], corners[0][1], corners[0][2], corners[0][3])));
            current_state++;
        }
        break;
    case 4: setLabel(im, "Place at farthest distance and", tPos);
        setLabel(im, "press 'D' to save value.", tPos);
        if (GetKeyState('D') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
        {
            //computeAreaAndScale(corners[0][0], corners[0][1], corners[0][2], corners[0][3]);
            calibVals.insert(pair<string, double>("Fourth", computeAreaAndScale(corners[0][0], corners[0][1], corners[0][2], corners[0][3])));
            current_state++;
        }
        break;
    case 5:   myfile.open("calibVals.txt", ios::out);
        myfile << calibVals.at("First") << "\n";
        myfile << calibVals.at("Second") << "\n";
        myfile << calibVals.at("Third") << "\n";
        myfile << calibVals.at("Fourth") << "\n";
        myfile.close();
        current_state++;
        break;
    default:
        break;
    }
}

int main() {

    int current_state = 1;
    cv::Point textPos = cv::Point(25, 25);
    map<string, double> calibVals;


    cv::VideoCapture inputVideo;
    inputVideo.open(0);
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    //cv::namedWindow("Display", cv::WINDOW_NORMAL);

    while (inputVideo.grab()) {
        cv::Mat image, imageCopy;
        inputVideo.retrieve(image);
        image.copyTo(imageCopy);


        vector<int> ids;
        vector<vector<cv::Point2f> > corners;
        cv::aruco::detectMarkers(image, dictionary, corners, ids);


        // if at least one marker detected
        if (ids.size() > 0 && current_state < 6) {
            cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
            calibrator(current_state, imageCopy, textPos, corners, calibVals);
            rectangle(imageCopy, corners[0][0], corners[0][2],
                cv::Scalar(0, 0, 0),
                -1, cv::LINE_8);
        }
        else if (current_state < 6) {
            cv::Point tPos = textPos;
            setLabel(imageCopy, "No markers detected!", tPos);
        }
        else {
            cv::Point tPos = textPos;
            setLabel(imageCopy, "Press ESC to quit.", tPos);
        }


        cv::imshow("Display", imageCopy);
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
