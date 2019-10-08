#ifndef GLOBAL_H
#define GLOBAL_H

//#define THZSIMULATE

#include <QImage>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"

#define THZ_WIDTH   360
#define THZ_HEIGHT  720

void msleep(unsigned int msec);
QImage mat2qimage(const cv::Mat &mat);
cv::Mat qimage2mat(const QImage &qimage);
cv::Mat RegionGrow(cv::Mat srcImage, cv::Point pt, int ch1Thres,int ch2Thres, int ch3Thres,
               int ch1LowerBind=0,int ch1UpperBind=255,int ch2LowerBind=0,
               int ch2UpperBind=255,int ch3LowerBind=0,int ch3UpperBind=255);

cv::Mat levelAdjust(const cv::Mat& imgIn, uchar highlight, uchar shadow, double midtones = 0.4);
cv::Mat levelAdjust(const cv::Mat& imgIn, double clipPercentage = 0.0);

#endif // GLOBAL_H
