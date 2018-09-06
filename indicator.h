#ifndef INDICATOR_H
#define INDICATOR_H

#include <QGraphicsItem>

class Indicator :public QGraphicsItem
{
public:
    Indicator(int x,int y,QGraphicsItem* parent = nullptr);
    enum {Type = UserType +2};
    int type() const override{
        return Type;
    }
    QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    int getX() const;
    int getY() const;

private:
    int x;
    int y;
};

#endif // INDICATOR_H
