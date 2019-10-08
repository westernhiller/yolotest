#include "yolodetector.h"
#include <QDebug>
#include <QFileInfo>

using namespace cv;
//using namespace dnn;
using namespace std;

YoloDetector::YoloDetector(QString config, QString weights, QStringList& classes, int classId)
    : m_classId(classId)
    , m_modelConfiguration(config)
    , m_modelWeights(weights)
{
    m_classes = classes;
	qDebug() << QStringLiteral("正在调入深度学习模型。。。");
    emit sendMessage(QStringLiteral("正在调入深度学习模型。。。"));

    qRegisterMetaType<QVariant>("QVariant");
    m_bFilesValid = checkFileExistence(m_modelConfiguration) &&
            checkFileExistence(m_modelWeights);

    qDebug() << "YoloDetector::YoloDetector: m_bFileValid = " << m_bFilesValid;
    if(m_bFilesValid)
        loadYolo();
}

YoloDetector::~YoloDetector()
{
    requestInterruption();
    if(m_pYoloDetector)
        delete m_pYoloDetector;
    clearImages();
    quit();
    wait();
}

void YoloDetector::clearImages()
{
    m_mutex.lock();
    while(!m_imageList.empty())
        m_imageList.pop_front();
    m_mutex.unlock();
}

void YoloDetector::loadYolo()
{
    m_pYoloDetector = new Detector(m_modelConfiguration.toStdString(), m_modelWeights.toStdString());
    emit sendMessage(QStringLiteral("深度学习模型调入完毕."));
	qDebug() << QStringLiteral("深度学习模型调入完毕.");
}

bool YoloDetector::checkFileExistence(QString filename)
{
    QFileInfo file(filename);
    if(file.exists()==false)
    {
        emit sendMessage(QStringLiteral("文件%1不存在！").arg(filename));
        return false;
    }

    return true;
}

void YoloDetector::drawBoxes(Mat& mat_img, vector<bbox_t> result_vec, QStringList& obj_names)
{
    int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

    for (auto &i : result_vec)
    {
        if(m_classId == -1)
        {
            for (auto &i : result_vec)
            {
                cv::Scalar color = obj_id_to_color(i.obj_id);
                cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
                if (obj_names.size() > i.obj_id)
                {
                    std::string obj_name = obj_names[i.obj_id].toStdString();
                    drawBox(mat_img, i, obj_name, color);
                }
            }
        }
        else if(m_classId == i.obj_id)
        {
            cv::Scalar color = obj_id_to_color(i.obj_id);
            cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
            if (obj_names.size() > i.obj_id)
            {
                std::string obj_name = obj_names[i.obj_id].toStdString();
                drawBox(mat_img, i, obj_name, color);
            }
        }
    }
}

void YoloDetector::drawBox(Mat& mat_img, bbox_t result, string obj_name, Scalar color)
{
    cv::rectangle(mat_img, cv::Rect(result.x, result.y, result.w, result.h), color, 2);
/*
    if (result.track_id > 0)
            obj_name += " - " + std::to_string(result.track_id);
    cv::Size const text_size = getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
    int const max_width = (text_size.width > result.w + 2) ? text_size.width : (result.w + 2);
    cv::rectangle(mat_img, cv::Point2f(std::max((int)result.x - 1, 0), std::max((int)result.y - 30, 0)),
                      cv::Point2f(std::min((int)result.x + max_width, mat_img.cols - 1), std::min((int)result.y, mat_img.rows - 1)),
                      color, CV_FILLED, 8, 0);
    putText(mat_img, obj_name, cv::Point2f(result.x, result.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
*/
}

void YoloDetector::run()
{
    int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

    if(!m_bFilesValid)
        return;

    while (!isInterruptionRequested())
    {
        if(m_imageList.size() > 0)
        {
            m_mutex.lock();
            QImage image = m_imageList.front();
            m_imageList.pop_front();
            m_mutex.unlock();

            Mat frame = qimage2mat(image);
			if (1 == frame.channels())
				cvtColor(frame, frame, COLOR_GRAY2RGB);

            const int64 start = cv::getTickCount();
            vector<bbox_t> result_vec = m_pYoloDetector->detect(frame);
            double duration = (cv::getTickCount()-start)/ cv::getTickFrequency();
            qDebug() << "GPU detection took " << duration << " second.";

            if(result_vec.empty())
            {
                emit objectDetected(image, 0, 0.01f);
            }
            else
            {
                float highprob = 0;
                quint32 highobjid = 0;

				for (auto &i : result_vec)
                {
                    if(i.prob > highprob)
                    {
                        highprob = i.prob;
                        highobjid = i.obj_id;
                    }
                    cv::Scalar color = cv::Scalar(255, 0, 0);  // obj_id_to_color(i.obj_id);
                    cv::rectangle(frame, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
                    QString qobj = m_classes[i.obj_id];
                    drawBox(frame, i, qobj.toStdString(), color);
                }
				qDebug() << "highobjid = " << highobjid << ", high prob = " << highprob;
                emit objectDetected(mat2qimage(frame), highobjid, highprob);
            }
        }
        else
            msleep(5);
    }
}

void YoloDetector::detect(QImage image)
{
    m_mutex.lock();
    // waiting list no longer than 3
    int size = m_imageList.size();
    if(size >= 5)
    {
        for(int i = 0; i < size - 4; i++)
            m_imageList.pop_front();
    }
    m_imageList.push_back(image);
    m_mutex.unlock();
}

