#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <QGraphicsItem>

class ChessMan :public QGraphicsItem
{
public:
    enum Color{
        RED = 1,
        BLACK = 2,
    };

    ChessMan(int id,int x,int y,Color color,const QPixmap& pixmap,
             QGraphicsItem* parent = nullptr);

    enum {Type = UserType +1 };

    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget) override;
    QRectF boundingRect() const override;

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
    QPixmap pixmap;
    QPoint offset;
};

#endif // CHESSMAN_H
