#include "indicator.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>

#define RADIUS_IND 10

extern qreal GRID_SIZE;

Indicator::Indicator(int x, int y, QGraphicsItem *parent)
     :QGraphicsItem(parent),x(x),y(y)
{
    setPos(0,0);
    setZValue(1);
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

void Indicator::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget);

    painter->save();
    QPen pen;
    pen.setWidth(3);

    if(option->state & QStyle::State_MouseOver){
        pen = QColor(136, 14, 79);
    } else {
        pen = QColor(Qt::red);
    }

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->setRenderHint(QPainter::Antialiasing);

#define OFFSET_DEC 10
#define SIZE_DEC 40

    QPoint offset(x*GRID_SIZE-SIZE_DEC/2,y*GRID_SIZE-SIZE_DEC/2);
    int p_width =  SIZE_DEC;
    int p_height  = SIZE_DEC;

    painter->drawLine(offset,QPoint(offset.x()+OFFSET_DEC,offset.y()));
    painter->drawLine(offset,QPoint(offset.x(),offset.y()+OFFSET_DEC));

    painter->drawLine(offset.x()+p_width,offset.y(),
                      offset.x()+p_width-OFFSET_DEC,offset.y());
    painter->drawLine(offset.x()+p_width,offset.y(),
                      offset.x()+p_width,offset.y()+OFFSET_DEC);

    painter->drawLine(offset.x(),offset.y()+p_height,
                      offset.x(),offset.y()+p_height-OFFSET_DEC);
    painter->drawLine(offset.x(),offset.y()+p_height,
                      offset.x()+OFFSET_DEC,offset.y()+p_height);

    painter->drawLine(offset.x()+p_width,offset.y()+p_height,
                      offset.x()+p_width,offset.y()+p_height-OFFSET_DEC);
    painter->drawLine(offset.x()+p_width,offset.y()+p_height,
                      offset.x()+p_width-OFFSET_DEC,offset.y()+p_height);

    painter->restore();
}

QPainterPath Indicator::shape() const {
    QPainterPath path;
//    path.addEllipse(QPoint(x*GRID_SIZE,y*GRID_SIZE),RADIUS_IND,RADIUS_IND);
    path.addRect(x*GRID_SIZE-SIZE_DEC/2,y*GRID_SIZE-SIZE_DEC/2,SIZE_DEC,SIZE_DEC);
    return path;
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
//    return QRectF(x*GRID_SIZE-RADIUS_IND,y*GRID_SIZE-RADIUS_IND,RADIUS_IND*2,RADIUS_IND*2);
    return QRectF(x*GRID_SIZE-SIZE_DEC/2,y*GRID_SIZE-SIZE_DEC/2,SIZE_DEC,SIZE_DEC);
}
