#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QImage>
#include <QTimer>

class Canvas : public QLabel
{
    Q_OBJECT
public:
    explicit Canvas(QWidget* parent = nullptr);

	inline QImage getImage() { return m_imageDisplay; }
    void display(QImage);
    void highlight(bool);
    void reset();// display initial image
signals:
	void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
    QImage m_imageDisplay;
    bool m_bHighlighted;

public slots:
    void onDisplay(QImage);
};

#endif // CANVAS_H
