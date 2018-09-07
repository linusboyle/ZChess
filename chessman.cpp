#include "chessman.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

extern qreal GRID_SIZE;

//TODO
//no longer inherit pixmapitem...

ChessMan::ChessMan(int id,int x,int y,Color color,
                   const QPixmap &pixmap, QGraphicsItem *parent)
    :QGraphicsItem(parent),
      m_id(id),
      xpos(x),
      ypos(y),
      m_color(color),
      pixmap(pixmap)
{
    offset = QPoint(-pixmap.width()/2,-pixmap.height()/2);

    setPos(x*GRID_SIZE,y*GRID_SIZE);
}

int ChessMan::id() const
{
    return m_id;
}

int ChessMan::getXpos() const
{
    return xpos;
}

int ChessMan::getYpos() const
{
    return ypos;
}

ChessMan::Color ChessMan::getColor() const
{
    return m_color;
}

void ChessMan::setPositon(int _x, int _y){
    xpos = _x;
    ypos = _y;

    setPos(xpos*GRID_SIZE,ypos*GRID_SIZE);
}

void ChessMan::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
    Q_UNUSED(widget);

    painter->save();

    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    painter->drawPixmap(this->offset,this->pixmap);

    if(option->state & QStyle::State_Selected){
        //draw decoration

#define OFFSET_DEC 10
        int p_width =  pixmap.width();
        int p_height  = pixmap.height();

        QPen pen(QColor(109,76,65));
        pen.setWidth(4);

        painter->setPen(pen);
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
    }

    painter->restore();
}

QRectF ChessMan::boundingRect() const
{
    if (pixmap.isNull())
        return QRectF();

    return QRectF(offset, pixmap.size() / pixmap.devicePixelRatio());
}
