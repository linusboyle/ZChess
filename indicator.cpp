#include "indicator.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>

#define RADIUS_IND 10

extern qreal GRID_SIZE;

Indicator::Indicator(int x, int y, QGraphicsItem *parent)
     :QGraphicsItem(parent),x(x),y(y)
{
    setZValue(1);
    setFlags(ItemIsSelectable);
}

void Indicator::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget);

    QPen pen(Qt::NoPen);
    QBrush brush;

    if(option->state == QStyle::State_MouseOver){
        brush = Qt::darkGray;
    } else {
        brush = Qt::red;
    }

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(QPoint(x*GRID_SIZE,y*GRID_SIZE),RADIUS_IND,RADIUS_IND);
}

int Indicator::getX() const
{
    return x;
}

int Indicator::getY() const
{
    return y;
}

QRectF Indicator::boundingRect() const {
    return QRectF(x*GRID_SIZE-RADIUS_IND,y*GRID_SIZE-RADIUS_IND,RADIUS_IND*2,RADIUS_IND*2);
}

//void Indicator::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
//    Q_UNUSED(event);

//    qDebug()<<"double click";
//}