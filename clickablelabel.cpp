#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent)
{
    qDebug() << "Hi there";
}



void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    pressLocation = event->localPos();
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    releaseLocation = event->localPos();
    qDebug() << "press: " << pressLocation << "; release: " << releaseLocation;
    this->repaint();
}

void ClickableLabel::mouseMoveEvent(QMouseEvent *event)
{
    releaseLocation = event->localPos();
    this->repaint();
}

void ClickableLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::green);
    painter.drawRect(pressLocation.x(), pressLocation.y(), releaseLocation.x() - pressLocation.x(), releaseLocation.y() - pressLocation.y());
    painter.end();
}

void ClickableLabel::clearSelection()
{
    pressLocation.setX(0);
    pressLocation.setY(0);
    releaseLocation.setX(0);
    releaseLocation.setY(0);
    this->repaint();
}
