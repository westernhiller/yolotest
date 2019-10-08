#ifndef YOLO_DETECTOR_H
#define YOLO_DETECTOR_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QTimer>
#include <QVariant>

#include "global.h"
#include "yolo_v2_class.hpp"

Q_DECLARE_METATYPE(bbox_t)

class YoloDetector : public QThread
{
    Q_OBJECT

public:
    explicit YoloDetector(QString config, QString weights, QStringList& classes, int classId = -1);
//    explicit YoloDetector(QString config, QString weights, QString classes, int classId = -1);
    ~YoloDetector() override;

signals:
    void objectDetected(QImage, quint32, float);
    void sendMessage(QString message);

protected:
    void run() override;

private:
    Detector* m_pYoloDetector;
    int m_classId;
    QMutex m_mutex;
    QVector<QImage> m_imageList;
    QString m_modelConfiguration;
    QString m_modelWeights;
    QString m_modelClasses;
    QStringList m_classes;

    bool m_bFilesValid;

    void loadYolo();
    bool checkFileExistence(QString filename);
    void drawBoxes(cv::Mat& mat_img,
                   std::vector<bbox_t> result_vec,
                   QStringList& obj_names);
    void drawBox(cv::Mat& mat_img, bbox_t result, std::string obj_name, cv::Scalar color);
    void clearImages();
public slots:
    void detect(QImage);
};

#endif // PERSON_DETECTOR_H
