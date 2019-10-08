#include "global.h"
#include <QTime>
#include <QDebug>
//#include <opencv2/xphoto.hpp>

using namespace std;
using namespace cv;

void msleep(unsigned int msec)
{
	QTime dueTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dueTime);
}

QImage mat2qimage(const Mat& mat)
{
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);          // Set the color table (used to translate colour indexes to qRgb values)          image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar* pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar* pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}      // 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}

Mat qimage2mat(const QImage& qimage)
{
	Mat mat, mat2;
	int from_to[] = { 0, 0, 1, 1, 2, 2, 3, 3 };
	switch (qimage.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.constBits(), qimage.bytesPerLine());
		mat2 = Mat(mat.rows, mat.cols, CV_8UC4);
		mixChannels(&mat, 1, &mat2, 1, from_to, 4);
		return mat2;
		break;
	case QImage::Format_RGB888:
		mat = Mat(qimage.height(), qimage.width(), CV_8UC3, (uchar*)qimage.constBits(), qimage.bytesPerLine());
		mat2 = Mat(mat.rows, mat.cols, CV_8UC3);
		mixChannels(&mat, 1, &mat2, 1, from_to, 3);
		cvtColor(mat2, mat2, COLOR_BGR2RGB);
		return mat2;
		break;
	case QImage::Format_Indexed8:
		mat = Mat(qimage.height(), qimage.width(), CV_8UC1, (uchar*)qimage.constBits(), qimage.bytesPerLine());
		break;
	default:
		break;
	}
	return mat;
}

/**
 * @brief 区域生长算法，输入图像应为三通道图像（RGB、HSV、YUV等）
 * @param srcImage 区域生长的源图像
 * @param pt 区域生长点
 * @param ch1Thres ch2Thres ch3Thres 三个通道的生长限制阈值，临近像素符合±chxThres范围内才能进行生长
 * @param ch1LowerBind ch1LowerBind ch1LowerBind 三个通道的最小值阈值
 * @param ch1UpperBind ch2UpperBind ch3UpperBind 三个通道的最大值阈值，在这个范围外即使临近像素符合±chxThres也不能生长
 * @return 生成的区域图像（二值类型）
 */
Mat RegionGrow(Mat srcImage, Point pt, int ch1Thres, int ch2Thres, int ch3Thres,
	int ch1LowerBind, int ch1UpperBind, int ch2LowerBind,
	int ch2UpperBind, int ch3LowerBind, int ch3UpperBind)
{
	Point pToGrowing;                       //待生长点位置
	int pGrowValue = 0;                             //待生长点灰度值
	Scalar pSrcValue = 0;                               //生长起点灰度值
	Scalar pCurValue = 0;                               //当前生长点灰度值
	Mat growImage = Mat::zeros(srcImage.size(), CV_8UC1);   //创建一个空白区域，填充为黑色
	//生长方向顺序数据
	int DIR[8][2] = { {-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0} };
	vector<Point> growPtVector;                     //生长点栈
	growPtVector.push_back(pt);                         //将生长点压入栈中
	growImage.at<uchar>(pt.y, pt.x) = 255;              //标记生长点
	pSrcValue = srcImage.at<Vec3b>(pt.y, pt.x);         //记录生长点的灰度值

	while (!growPtVector.empty())                       //生长栈不为空则生长
	{
		pt = growPtVector.back();                       //取出一个生长点
		growPtVector.pop_back();

		//分别对八个方向上的点进行生长
		for (int i = 0; i < 8; ++i)
		{
			pToGrowing.x = pt.x + DIR[i][0];
			pToGrowing.y = pt.y + DIR[i][1];
			//检查是否是边缘点
			if (pToGrowing.x < 0 || pToGrowing.y < 0 ||
				pToGrowing.x >(srcImage.cols - 1) || (pToGrowing.y > srcImage.rows - 1))
				continue;

			pGrowValue = growImage.at<uchar>(pToGrowing.y, pToGrowing.x);       //当前待生长点的灰度值
			pSrcValue = srcImage.at<Vec3b>(pt.y, pt.x);
			if (pGrowValue == 0)                    //如果标记点还没有被生长
			{
				pCurValue = srcImage.at<Vec3b>(pToGrowing.y, pToGrowing.x);
				if (pCurValue[0] <= ch1UpperBind && pCurValue[0] >= ch1LowerBind &&   //限制生长点的三通道上下界
					pCurValue[1] <= ch2UpperBind && pCurValue[1] >= ch2LowerBind &&
					pCurValue[2] <= ch3UpperBind && pCurValue[2] >= ch3LowerBind)
				{
					if (abs(pSrcValue[0] - pCurValue[0]) < ch1Thres &&
						abs(pSrcValue[1] - pCurValue[1]) < ch2Thres &&
						abs(pSrcValue[2] - pCurValue[2]) < ch3Thres)                    //在阈值范围内则生长
					{
						growImage.at<uchar>(pToGrowing.y, pToGrowing.x) = 255;      //标记为白色
						growPtVector.push_back(pToGrowing);                 //将下一个生长点压入栈中
					}
				}
			}
		}
	}
	return growImage.clone();
}

Mat levelAdjust(const Mat& imgIn, uchar highlight, uchar shadow, double midtones)
{
	Mat imgOut;

	int diff = highlight - shadow;
	if (diff <= 1)
		return imgOut;

	imgOut.create(imgIn.size(), CV_8UC1);
	double coef = 1.0 / diff;
	double exponent = 1.0f / midtones;

	for (int i = 0; i < imgIn.rows; i++)
	{
		uchar* ptrIn = (uchar*)imgIn.ptr<uchar>(i);
		uchar* ptrOut = (uchar*)imgOut.ptr<uchar>(i);
		for (int j = 0; j < imgIn.cols; j++)
		{
			if (ptrIn[j] >= highlight)
				ptrOut[j] = 255;
			else if (ptrIn[j] <= shadow)
				ptrOut[j] = 0;
			else
			{
				//				ptrOut[j] = static_cast<uchar>((ptrIn[j] - shadow) * coef * 255);
				double v = ceil(pow((ptrIn[j] - shadow) * coef, exponent) * 255.0);
				//				printf("v = %f\n", v);
				ptrOut[j] = static_cast<uchar>(v);
			}
		}
	}
	return imgOut;
}

Mat levelAdjust(const Mat& imgIn, double clipPercentage)
{
	int size = imgIn.rows * imgIn.cols;
	double thresh = clipPercentage * size;

	int bin[256];
	for (int i = 0; i < 256; i++)
		bin[i] = 0;

	for (int i = 0; i < imgIn.rows; i++)
	{
		uchar* ptr = (uchar*)imgIn.ptr<uchar>(i);
		for (int j = 0; j < imgIn.cols; j++)
		{
			bin[ptr[j]]++;
		}
	}
	int shadow = 0;
	int sum = 0;

	while (shadow < 256)
	{
		sum += bin[shadow];
		if (sum > thresh)
			break;
		shadow++;
	}
	
	int highlight = 255;
	sum = 0;
	while (highlight >= 0)
	{
		sum += bin[highlight];
		if (sum > thresh)
			break;
		highlight--;
	}

	//	printf("highlight = %d, shadow = %d", highlight, shadow);
	return levelAdjust(imgIn, highlight, shadow);
}
