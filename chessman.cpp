#include "chessman.h"

extern qreal GRID_SIZE;

ChessMan::ChessMan(int id,int x,int y,Color color,
                   const QPixmap &pixmap, QGraphicsItem *parent)
    :QGraphicsPixmapItem(pixmap,parent),
      m_id(id),
      xpos(x),
      ypos(y),
      m_color(color)
{
    setOffset(-pixmap.width()/2,-pixmap.height()/2);
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
