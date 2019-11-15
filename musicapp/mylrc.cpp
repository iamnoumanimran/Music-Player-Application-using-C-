#include "mylrc.h"
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>


MyLrc::MyLrc(QWidget *parent) :
    QLabel(parent)
{
//    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	setWindowFlags(Qt::FramelessWindowHint);
	this->move(450, 330);
    setAttribute(Qt::WA_TranslucentBackground);
    setText(tr("MusicPlayer"));
    setMaximumSize(800, 60);
    setMinimumSize(800, 60);

    linear_gradient.setStart(0, 10);
    linear_gradient.setFinalStop(0, 40);
    linear_gradient.setColorAt(0.1, QColor(14, 179, 255));
    linear_gradient.setColorAt(0.5, QColor(114, 232, 255));
    linear_gradient.setColorAt(0.9, QColor(14, 179, 255));

    mask_linear_gradient.setStart(0, 10);
    mask_linear_gradient.setFinalStop(0, 40);

	mask_linear_gradient.setColorAt(0.1, QColor(0, 1, 4));
	mask_linear_gradient.setColorAt(0.5, QColor(0, 1, 16));
	mask_linear_gradient.setColorAt(0.9, QColor(0, 1, 4));

    font.setFamily("Helvetica");
    font.setBold(true);
    font.setPointSize(15);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    lrc_mask_width = 0;
    lrc_mask_width_interval = 0;
}


void MyLrc::start_lrc_mask(qint64 intervaltime)
{
    qreal count = intervaltime / 30;
    lrc_mask_width_interval = 800 / count;
    lrc_mask_width = 0;
    timer->start(30);
}

void MyLrc::stop_lrc_mask()
{
    timer->stop();
    lrc_mask_width = 0;
    update();
}

void MyLrc::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(font);

    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(1, 1, 800, 60, Qt::AlignLeft, text());

    painter.setPen(QPen(linear_gradient, 0));
    painter.drawText(0, 0, 800, 60, Qt::AlignLeft, text());

    painter.setPen(QPen(mask_linear_gradient, 0));
    painter.drawText(0, 0, lrc_mask_width, 60, Qt::AlignLeft, text());

}

void MyLrc::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        offset = event->globalPos() - frameGeometry().topLeft();
}


void MyLrc::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        setCursor(Qt::PointingHandCursor);
        move(event->globalPos() - offset);
       }
}


void MyLrc::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(tr("hide"), this, SLOT(hide()));
    menu.exec(event->globalPos());
}


void MyLrc::timeout()
{
    lrc_mask_width += lrc_mask_width_interval;
    update();
}
