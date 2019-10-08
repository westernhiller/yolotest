#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    m_hiddenClasses.append("knife");
    m_hiddenClasses.append("gun");
    m_hiddenClasses.append("others");

    QString modeldir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + tr("\\models\\");
	m_detector = new YoloDetector(modeldir + "hidden.cfg", modeldir + "hidden.dat", m_hiddenClasses);
//	m_detector = new YoloDetector(modeldir + "yolov3-face.cfg", modeldir + "yolov3-face.weights", m_hiddenClasses);
	m_detector->start();
	connect(m_detector, SIGNAL(sendMessage(QString)), this, SLOT(onMessage(QString)));
	connect(this, SIGNAL(detectObject(QImage)), m_detector, SLOT(detect(QImage)));
	connect(m_detector, SIGNAL(objectDetected(QImage, quint32, float)), this, SLOT(onObjectDetected(QImage, quint32, float)));
    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowse()));
	connect(ui->btnDetect, SIGNAL(clicked()), this, SLOT(onDetect()));
    connect(this, SIGNAL(openImage(QImage)), ui->source, SLOT(onDisplay(QImage)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onBrowse()
{
	QFileDialog* fileDialog = new QFileDialog(this);
	fileDialog->setNameFilter(tr("*.png;;*.jpg;;*.bmp"));
	if (fileDialog->exec() == QDialog::Accepted)
	{
		QString filepath = fileDialog->selectedFiles()[0];
		filepath.replace(QChar('/'), QChar('\\'));

		ui->editFile->setText(filepath);
		m_srcImage = QImage(filepath);
		ui->source->display(m_srcImage);
 	}
}

void Dialog::processImage(cv::Mat &image)
{
	cv::resize(image, image, cv::Size(THZ_WIDTH, THZ_HEIGHT));
	cv::Mat matMask;
	cv::threshold(image, matMask, 0, 255, cv::THRESH_OTSU);
	cv::Mat structElement1 = getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9), cv::Point(-1, -1));
	dilate(matMask, matMask, structElement1);

	for (int i = 0; i < image.rows; i++)
	{
		uchar* ptr = (uchar*)image.ptr<uchar>(i);
		uchar* ptrM = (uchar*)matMask.ptr<uchar>(i);

		for (int j = 0; j < image.cols; j++)
		{
			if (ptrM[j] == 0)
			{
				ptr[j] = ptrM[j];
			}
		}
	}
}

void Dialog::onDetect()
{
	if (!m_srcImage.isNull())
	{
		cv::Mat matImage = qimage2mat(m_srcImage);
		cv::Mat matLevelAdjusted = levelAdjust(matImage);

		if(matLevelAdjusted.data)
			emit detectObject(mat2qimage(matLevelAdjusted));
	}
}

void Dialog::onObjectDetected(QImage imgResult, quint32 classid, float fprob)
{
	ui->result->display(imgResult);
}

void Dialog::onMessage(QString msg)
{
	ui->message->setText(msg);
}
