#include "canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget* parent)
    : QLabel(parent)
    , m_bHighlighted(false)
{
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet("background-color: rgb(0, 0, 0)");
    setAlignment(Qt::AlignCenter);
}

void Canvas::onDisplay(QImage image)
{
    display(image);
}

void Canvas::display(QImage image)
{
    if(image.isNull())
        m_imageDisplay = QImage();
    else {
        m_imageDisplay = image.scaled(size());
	}
    update();
}

void Canvas::highlight(bool bHighlight)
{
    m_bHighlighted = bHighlight;
    update();
}

void Canvas::reset()
{
    m_bHighlighted = false;
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
	emit clicked();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    //画出图像
    QPainter painter(this);

    if(m_imageDisplay.isNull())
    {
        painter.setBrush(QBrush(Qt::black));
        painter.drawRect(QRect(0, 0, width(), height()));
    }
    else {
        int x = (width() - m_imageDisplay.width()) / 2;
        int y = (height() - m_imageDisplay.height()) / 2;
        painter.drawImage(QPoint(x, y), m_imageDisplay);
    }
    if(m_bHighlighted)
    {
        painter.setPen(QPen(Qt::green, 5, Qt::SolidLine, Qt::RoundCap));
        painter.drawRect(QRect(0, 0, width(), height()));
    }
}
