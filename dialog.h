#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QImage>
#include "canvas.h"
#include "yolodetector.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
signals:
    void openImage(QImage);
	void detectObject(QImage);

private:
    Ui::Dialog *ui;
    YoloDetector* m_detector;
    QStringList m_hiddenClasses;
	QImage m_srcImage;

	void processImage(cv::Mat &image);

public slots:
	void onBrowse();
	void onDetect();
	void onMessage(QString);
	void onObjectDetected(QImage, quint32, float);
};

#endif // DIALOG_H
