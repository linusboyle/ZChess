#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <QGraphicsPixmapItem>

class ChessMan :public QGraphicsPixmapItem
{
public:
    enum Color{
        RED = 1,
        BLACK = 2,
    };

    ChessMan(int id,int x,int y,Color color,const QPixmap& pixmap,QGraphicsItem* parent = nullptr);

    enum {Type = UserType +1 };


    int id() const;
    int getXpos() const;
    int getYpos() const;

    void setPositon(int _x,int _y);

    int type() const override {
        return Type;
    }

    Color getColor() const;

private:
    int m_id;
    int xpos;
    int ypos;
    Color m_color;
};

#endif // CHESSMAN_H
