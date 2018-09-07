#include "chessboard.h"
#include "chessman.h"
#include "indicator.h"
#include "musicplayerdaemon.h"
#include <QDebug>
#include <QApplication>
#include <QFontDatabase>

extern constexpr qreal GRID_SIZE = 60;

ChessBoard::ChessBoard(QObject *parent)
    :QGraphicsScene(parent),
      m_selected(nullptr)
{
    setBackgroundBrush(QColor(209,189,152));
    font_id = QFontDatabase::addApplicationFont(":/resources/font.ttf");
    drawBoard();
    connect(this,&ChessBoard::selectionChanged,this,&ChessBoard::chessSelected,
            Qt::QueuedConnection);
}

void ChessBoard::drawBoard(){
    QPen pen(Qt::black);
    QBrush brush(Qt::NoBrush);

    qreal basex = 0;
    qreal basey = 0;

    pen.setWidth(6);
    addRect(basex-10,basey-10,(COLUMN_NUM-1)*GRID_SIZE+20,
            (TOTAL_ROW-1)*GRID_SIZE+20,
            pen,brush);

    pen.setWidth(4);

    //first half
    for(int i = 0;i<COLUMN_NUM-1;++i){
        for(int j = 0;j<ROW_NUM-1;++j){
            addRect(basex+i*GRID_SIZE,basey+j*GRID_SIZE,GRID_SIZE,GRID_SIZE,
                    pen,brush);
        }
    }
    pen.setWidth(2);
    addLine(basex+3*GRID_SIZE,basey,basex+5*GRID_SIZE,basey+2*GRID_SIZE,pen);
    addLine(basex+5*GRID_SIZE,basey,basex+3*GRID_SIZE,basey+2*GRID_SIZE,pen);
    pen.setWidth(4);

    basey+=(ROW_NUM-1)*GRID_SIZE;
    //river
    QFont font(QFontDatabase::applicationFontFamilies(font_id).at(0),30);

    addRect(basex,basey,GRID_SIZE*(COLUMN_NUM-1),GRID_SIZE,
            pen,brush);
    auto text = addText("楚河      漢界",font);

    auto rect = text->boundingRect();
    text->setPos(basex+GRID_SIZE*(COLUMN_NUM-1)/2-rect.width()/2,
                 basey+GRID_SIZE/2-rect.height()/2);

    basey+=GRID_SIZE;
    //second part
    for(int i = 0;i<COLUMN_NUM-1;++i){
        for(int j = 0;j<ROW_NUM-1;++j){
            addRect(basex+i*GRID_SIZE,basey+j*GRID_SIZE,GRID_SIZE,GRID_SIZE,
                    pen,brush);
        }
    }
    pen.setWidth(2);
    addLine(basex+3*GRID_SIZE,basey+2*GRID_SIZE,basex+5*GRID_SIZE,
            basey+4*GRID_SIZE,pen);
    addLine(basex+5*GRID_SIZE,basey+2*GRID_SIZE,basex+3*GRID_SIZE,
            basey+4*GRID_SIZE,pen);
    pen.setWidth(3);

    //decorations
    drawDecoration(1,2);
    drawDecoration(7,2);

    drawDecoration(0,3);
    drawDecoration(2,3);
    drawDecoration(4,3);
    drawDecoration(6,3);
    drawDecoration(8,3);

    drawDecoration(0,6);
    drawDecoration(2,6);
    drawDecoration(4,6);
    drawDecoration(6,6);
    drawDecoration(8,6);

    drawDecoration(1,7);
    drawDecoration(7,7);

}

void ChessBoard::drawDecoration(int x, int y){
    QPen pen(Qt::black);
    pen.setWidth(2);

    qreal pointx = x*GRID_SIZE;
    qreal pointy = y*GRID_SIZE;

#define D_OFFSET 4
    if(x!=0){
        addLine(pointx-D_OFFSET,pointy-D_OFFSET,pointx-GRID_SIZE/4,pointy-D_OFFSET,pen);
        addLine(pointx-D_OFFSET,pointy-D_OFFSET,pointx-D_OFFSET,pointy-GRID_SIZE/4,pen);

        addLine(pointx-D_OFFSET,pointy+D_OFFSET,pointx-GRID_SIZE/4,pointy+D_OFFSET,pen);
        addLine(pointx-D_OFFSET,pointy+D_OFFSET,pointx-D_OFFSET,pointy+GRID_SIZE/4,pen);
    }

    if(x!=COLUMN_NUM-1){
        addLine(pointx+D_OFFSET,pointy+D_OFFSET,pointx+GRID_SIZE/4,pointy+D_OFFSET,pen);
        addLine(pointx+D_OFFSET,pointy+D_OFFSET,pointx+D_OFFSET,pointy+GRID_SIZE/4,pen);

        addLine(pointx+D_OFFSET,pointy-D_OFFSET,pointx+GRID_SIZE/4,pointy-D_OFFSET,pen);
        addLine(pointx+D_OFFSET,pointy-D_OFFSET,pointx+D_OFFSET,pointy-GRID_SIZE/4,pen);
    }
}

void ChessBoard::initBoardState(){
    ChessMan* man = new ChessMan(0,3,9,ChessMan::RED,QPixmap(":/resources/仕.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(1,5,9,ChessMan::RED,QPixmap(":/resources/仕.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(2,4,9,ChessMan::RED,QPixmap(":/resources/帥.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    redgeneral.setX(4);
    redgeneral.setY(9);

    man = new ChessMan(3,2,9,ChessMan::RED,QPixmap(":/resources/相.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(4,6,9,ChessMan::RED,QPixmap(":/resources/相.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(5,1,9,ChessMan::RED,QPixmap(":/resources/马.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(6,7,9,ChessMan::RED,QPixmap(":/resources/马.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(7,0,9,ChessMan::RED,QPixmap(":/resources/車.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(8,8,9,ChessMan::RED,QPixmap(":/resources/車.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(9,1,7,ChessMan::RED,QPixmap(":/resources/炮.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(10,7,7,ChessMan::RED,QPixmap(":/resources/炮.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(11,0,6,ChessMan::RED,QPixmap(":/resources/兵.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(12,2,6,ChessMan::RED,QPixmap(":/resources/兵.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(13,4,6,ChessMan::RED,QPixmap(":/resources/兵.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(14,6,6,ChessMan::RED,QPixmap(":/resources/兵.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(15,8,6,ChessMan::RED,QPixmap(":/resources/兵.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);




    man = new ChessMan(16,3,0,ChessMan::BLACK,QPixmap(":/resources/士.png"));
    addItem(man);
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);

    man = new ChessMan(17,5,0,ChessMan::BLACK,QPixmap(":/resources/士.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(18,4,0,ChessMan::BLACK,QPixmap(":/resources/将.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);
    blackgeneral.setX(4);
    blackgeneral.setY(0);

    man = new ChessMan(19,2,0,ChessMan::BLACK,QPixmap(":/resources/黑象.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(20,6,0,ChessMan::BLACK,QPixmap(":/resources/黑象.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(21,1,0,ChessMan::BLACK,QPixmap(":/resources/黑马.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(22,7,0,ChessMan::BLACK,QPixmap(":/resources/黑马.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(23,0,0,ChessMan::BLACK,QPixmap(":/resources/黑車.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(24,8,0,ChessMan::BLACK,QPixmap(":/resources/黑車.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(25,1,2,ChessMan::BLACK,QPixmap(":/resources/黑炮.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(26,7,2,ChessMan::BLACK,QPixmap(":/resources/黑炮.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(27,0,3,ChessMan::BLACK,QPixmap(":/resources/卒.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(28,2,3,ChessMan::BLACK,QPixmap(":/resources/卒.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(29,4,3,ChessMan::BLACK,QPixmap(":/resources/卒.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(30,6,3,ChessMan::BLACK,QPixmap(":/resources/卒.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);

    man = new ChessMan(31,8,3,ChessMan::BLACK,QPixmap(":/resources/卒.png"));
    m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
    addItem(man);
}

void ChessBoard::clearBoard(){
    m_chessmen.clear();
    m_indicators.clear();
    clear();
    drawBoard();
}

void ChessBoard::chessSelected(){
    auto items = selectedItems();

    if(items.isEmpty()){
        display(QList<QPoint>());
        return;
    }

    Indicator* indicator = qgraphicsitem_cast<Indicator*>(items.first());
    if(indicator){
        move(m_selected,indicator->getX(),indicator->getY());

        m_selected = nullptr;
        display(QList<QPoint>());
        return;
    }


    ChessMan* chess = qgraphicsitem_cast<ChessMan*>(items.first());
    m_selected = chess;

    display(getHint(chess));
}

QList<QPoint> ChessBoard::getHint(ChessMan *chess){
    switch (chess->id()) {
        case 0:
        case 1:
        case 16:
        case 17:
            return getShiHint(chess);
        case 2:
        case 18:
            return getGeneralHint(chess);
        case 3:
        case 4:
        case 19:
        case 20:
            return getXiangHint(chess);
        case 5:
        case 6:
        case 21:
        case 22:
            return getHorseHint(chess);
        case 7:
        case 8:
        case 23:
        case 24:
            return getJuHint(chess);
        case 9:
        case 10:
        case 25:
        case 26:
            return getPaoHint(chess);
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            return getSoldierHint(chess);
        default:
            return getSoldierHint(chess);
    }
}

QList<QPoint> ChessBoard::getShiHint(ChessMan* chess){
    int x = chess->getXpos();
    int y = chess->getYpos();

    QList<QPoint> valid;

    if(redgeneral.x()==blackgeneral.x()){
        if(x==redgeneral.x()
           &&y>blackgeneral.y()&&y<redgeneral.y()
           &&countBetween(x,blackgeneral.y(),redgeneral.y())==1)
            return valid;
    }

    int dx[4] = {-1,-1,1,1};
    int dy[4] = {-1,1,-1,1};

    for(int i=0;i<4;++i){
        QPoint point(x+dx[i],y+dy[i]);
        if(chess->getColor() == ChessMan::BLACK){
            if(point.x()>=3&&point.x()<=5&&point.y()>=0&&point.y()<=2
                    &&!checkSameColor(chess,point.x(),point.y())){
                valid.append(point);
            }
        }
        else{
            if(point.x()>=3&&point.x()<=5&&point.y()>=7&&point.y()<=9
                    &&!checkSameColor(chess,point.x(),point.y())){
                valid.append(point);
            }
        }
    }

    return valid;
}

bool ChessBoard::checkValid(int x, int y)
{
    if(x<0||x>=COLUMN_NUM)
        return false;
    if(y<0||y>=TOTAL_ROW)
        return false;

    return true;
}

bool ChessBoard::checkSameColor(ChessMan *chess, int x, int y)
{
    QPair<int,int> pair(x,y);
    int count = m_chessmen.count(pair);
    if(count == 0){
        return false;
    }

    ChessMan* man = m_chessmen.value(pair);

    if(man->getColor() == chess->getColor()){
        return true;
    }

    return false;
}

QList<QPoint> ChessBoard::getGeneralHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

    int dx[4] = {-1,0,0,1};
    int dy[4] = {0,1,-1,0};

    for(int i=0;i<4;++i){
        QPoint point(x+dx[i],y+dy[i]);
        if(chess->getColor() == ChessMan::BLACK){
            if(point.x()>=3&&point.x()<=5&&point.y()>=0&&point.y()<=2
                    &&!checkSameColor(chess,point.x(),point.y())){

                    if(point.x()==redgeneral.x()
                            &&countBetween(point.x(),point.y(),redgeneral.y())==0)
                        continue;

                valid.append(point);
            }
        }
        else{
            if(point.x()>=3&&point.x()<=5&&point.y()>=7&&point.y()<=9
                    &&!checkSameColor(chess,point.x(),point.y())){

                    if(point.x()==blackgeneral.x()
                            &&countBetween(point.x(),blackgeneral.y(),point.y())==0)
                        continue;

                valid.append(point);
            }
        }
    }

    return valid;
}

QList<QPoint> ChessBoard::getXiangHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

    int dx[4] = {-2,-2,2,2};
    int dy[4] = {-2,2,-2,2};

    int dx2[4]= {-1,-1,1,1};
    int dy2[4]= {-1,1,-1,1};

    if(x==redgeneral.x()
       &&redgeneral.x()==blackgeneral.x()
       &&y>blackgeneral.y()&&y<redgeneral.y()
       &&countBetween(x,blackgeneral.y(),redgeneral.y())==1)
            return valid;

    for(int i=0;i<4;++i){
        QPoint point(x+dx[i],y+dy[i]);
        if(chess->getColor() == ChessMan::BLACK){
            //cannot across river
            if(checkValid(point.x(),point.y())&&point.y()<=4
                    &&!exist(x+dx2[i],y+dy2[i])
                    &&!checkSameColor(chess,point.x(),point.y())){

                valid.append(point);
            }
        }
        else{
            if(checkValid(point.x(),point.y())&&point.y()>=5
                    &&!exist(x+dx2[i],y+dy2[i])
                    &&!checkSameColor(chess,point.x(),point.y())){
                valid.append(point);
            }
        }
    }

    return valid;
}

QList<QPoint> ChessBoard::getHorseHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

    int dx[8] = {-1,-2,1,2,2,1,-1,-2};
    int dy[8] = {-2,-1,-2,-1,1,2,2,1};

    int dx2[8] = {0,-1,0,1,1,0,0,-1};
    int dy2[8] = {-1,0,-1,0,0,1,1,0};

    if(x==redgeneral.x()
       &&redgeneral.x()==blackgeneral.x()
       &&y>blackgeneral.y()&&y<redgeneral.y()
       &&countBetween(x,blackgeneral.y(),redgeneral.y())==1)
            return valid;

    for(int i=0;i<8;++i){
        QPoint point(x+dx[i],y+dy[i]);
        if(checkValid(point.x(),point.y())&&!exist(x+dx2[i],y+dy2[i])
                &&!checkSameColor(chess,point.x(),point.y())){
            valid.append(point);
        }
    }

    return valid;
}

bool ChessBoard::exist(int x, int y){
    QPair<int,int> pair(x,y);
    if(m_chessmen.contains(pair)){
        return true;
    }
    return false;
}

QList<QPoint> ChessBoard::getJuHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

    //the same column,down
    for(int i=y+1;i<TOTAL_ROW;++i){
        if(!exist(x,i))
            valid.append(QPoint(x,i));
        else{
            if(!checkSameColor(chess,x,i))
                valid.append(QPoint(x,i));
            break;
        }
    }
    //up
    for(int i=y-1;i>=0;--i){
        if(!exist(x,i))
            valid.append(QPoint(x,i));
        else{
            if(!checkSameColor(chess,x,i))
                valid.append(QPoint(x,i));
            break;
        }
    }

    if(x==redgeneral.x()
       &&redgeneral.x()==blackgeneral.x()
       &&y>blackgeneral.y()&&y<redgeneral.y()
       &&countBetween(x,blackgeneral.y(),redgeneral.y())==1){
        //if between two general,only up and down(well,I think in this circumstance
        //the owner of the chariot has won..)
        display(valid);
        return valid;
    }

    //right
    for(int i=x+1;i<COLUMN_NUM;++i){
        if(!exist(i,y))
            valid.append(QPoint(i,y));
        else{
            if(!checkSameColor(chess,i,y))
                valid.append(QPoint(i,y));
            break;
        }
    }
    //left
    for(int i=x-1;i>=0;--i){
        if(!exist(i,y))
            valid.append(QPoint(i,y));
        else{
            if(!checkSameColor(chess,i,y))
                valid.append(QPoint(i,y));
            break;
        }
    }

    return valid;
}

QList<QPoint> ChessBoard::getPaoHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

    if(x==redgeneral.x()
       &&redgeneral.x()==blackgeneral.x()
       &&y>blackgeneral.y()&&y<redgeneral.y()
       &&countBetween(x,blackgeneral.y(),redgeneral.y())==1){
        //the problem here is cannon can jump through the general
        //although it's very rare,we should take it into consideration
            for(int i=y+1;i<TOTAL_ROW;++i){
                if(!exist(x,i))
                    valid.append(QPoint(x,i));
            }
            //up
            for(int i=y-1;i>=0;--i){
                if(!exist(x,i))
                    valid.append(QPoint(x,i));
            }
            return valid;
    }

    //the same column,down
    for(int i=y+1;i<TOTAL_ROW;++i){
        if(!exist(x,i))
            valid.append(QPoint(x,i));
        else {
            for(int j=i+1;j<TOTAL_ROW;++j) {
                if(exist(x,j)){
                    if(!checkSameColor(chess,x,j))
                        valid.append(QPoint(x,j));
                    break;
                }
            }
            break;
        }
    }
    //up
    for(int i=y-1;i>=0;--i){
        if(!exist(x,i))
            valid.append(QPoint(x,i));
        else {
            for(int j=i-1;j>=0;--j) {
                if(exist(x,j)){
                    if(!checkSameColor(chess,x,j))
                        valid.append(QPoint(x,j));
                    break;
                }
            }
            break;
        }
    }

    //right
    for(int i=x+1;i<COLUMN_NUM;++i){
        if(!exist(i,y))
            valid.append(QPoint(i,y));
        else {
            for(int j=i+1;j<COLUMN_NUM;++j) {
                if(exist(j,y)){
                    if(!checkSameColor(chess,j,y))
                        valid.append(QPoint(j,y));
                    break;
                }
            }
            break;
        }
    }
    //left
    for(int i=x-1;i>=0;--i){
        if(!exist(i,y))
            valid.append(QPoint(i,y));
        else {
            for(int j=i-1;j>=0;--j) {
                if(exist(j,y)){
                    if(!checkSameColor(chess,j,y))
                        valid.append(QPoint(j,y));
                    break;
                }
            }
            break;
        }
    }

    return valid;
}

QList<QPoint> ChessBoard::getSoldierHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

    //cannot go back;
    int dx[3] = {0,1,-1};
    int dy[3] = {-1,0,0};

    int dxb[3] = {0,1,-1};
    int dyb[3] = {1,0,0};

    int limit = 0;
    if(chess->getColor() == ChessMan::RED){
        if(y<=4){
            limit = 3;
        }else {
            limit = 1;
        }
        for(int i=0;i<limit;++i){
            QPoint point(x+dx[i],y+dy[i]);
            if(x==redgeneral.x()
               &&redgeneral.x()==blackgeneral.x()
               &&y>blackgeneral.y()&&y<redgeneral.y()
               &&countBetween(x,blackgeneral.y(),redgeneral.y())==1
               &&dx[i]!=0)
                    continue;

            if(checkValid(point.x(),point.y())&&!checkSameColor(chess,point.x(),point.y()))
            {
                valid.append(point);
            }
        }
    } else {
        if(y>=5){
            limit = 3;
        }else {
            limit = 1;
        }
        for(int i=0;i<limit;++i){
            QPoint point(x+dxb[i],y+dyb[i]);
            if(x==redgeneral.x()
               &&redgeneral.x()==blackgeneral.x()
               &&y>blackgeneral.y()&&y<redgeneral.y()
               &&countBetween(x,blackgeneral.y(),redgeneral.y())==1
               &&dx[i]!=0)
                    continue;
            if(checkValid(point.x(),point.y())&&!checkSameColor(chess,point.x(),point.y()))
            {
                valid.append(point);
            }
        }
    }
    return valid;
}

void ChessBoard::display(const QList<QPoint>& valid){
    int size=m_indicators.size();

    for(int i=0;i<size;++i){
        auto indicator = m_indicators.at(i);
        removeItem(indicator);
        delete indicator;
    }
    m_indicators.clear();

    //add
    foreach (const QPoint& point, valid) {
        auto indicator = new Indicator(point.x(),point.y());
        addItem(indicator);

        m_indicators.push_back(indicator);
    }
}

void ChessBoard::move(ChessMan *chess, int x, int y){

    //update general positon
    if(chess->id() == 2){//red general
        redgeneral.setX(x);
        redgeneral.setY(y);
    } else if(chess->id() == 18){
        blackgeneral.setX(x);
        blackgeneral.setY(y);
    }

    m_chessmen.remove(QPair<int,int>(chess->getXpos(),chess->getYpos()));

    chess->setPositon(x,y);
    QPair<int,int> pair(x,y);
    if(m_chessmen.contains(pair)){
        //eat enemy
        ChessMan* eaten = m_chessmen.take(pair);
        removeItem(eaten);

        emit chessEaten(eaten->id());
        delete eaten;
    }
    m_chessmen.insert(pair,chess);

    emit chessMoved(chess->id(),x,y);
    MusicPlayerDaemon::instance()->playChessDownMusic();

    //check if the general is checked;
    QList<QPoint> nextHint = getHint(chess);
    if(nextHint.contains(redgeneral)||nextHint.contains(blackgeneral)){
        MusicPlayerDaemon::instance()->playCheckedMusic();
    }
}

//switch control actually
void ChessBoard::setEnable(int side){

    /* 0~15 is red
     * 16-31 is black
     */

    //side :0 ->nothing
    //1->red
    //2->black

    QHashIterator<QPair<int,int>,ChessMan*> iterator(m_chessmen);
    while(iterator.hasNext()){
        iterator.next();
        auto chess = iterator.value();
        if(chess->getColor() == ChessMan::RED){ //red
            chess->setFlag(QGraphicsItem::ItemIsSelectable,side == 1 ? true:false);
        } else {
            chess->setFlag(QGraphicsItem::ItemIsSelectable,side == 2 ? true:false);
        }
    }

    //clear selection and render
    display(QList<QPoint>());
    m_selected = nullptr;
}

void ChessBoard::moveChess(int id, int x, int y){

    //update general position
    if(id == 2){//red general
        redgeneral.setX(x);
        redgeneral.setY(y);
    } else if(id == 18){
        blackgeneral.setX(x);
        blackgeneral.setY(y);
    }

    QHashIterator<QPair<int,int>,ChessMan*> iterator(m_chessmen);

    ChessMan* chess = nullptr;
    while(iterator.hasNext()){
        iterator.next();

        auto chessman = iterator.value();

        if(chessman->id() == id){
            chess = m_chessmen.take(QPair<int,int>(chessman->getXpos(),
                                                   chessman->getYpos()));
            break;
        }
    }

    if(chess){
        chess->setPositon(x,y);

        QPair<int,int> pair(x,y);
        if(m_chessmen.contains(pair)){
            //eat enemy
            ChessMan* eaten = m_chessmen.take(pair);
            removeItem(eaten);

            emit chessEaten(eaten->id());
            delete eaten;
        }

        m_chessmen.insert(pair,chess);
        MusicPlayerDaemon::instance()->playChessDownMusic();

        QList<QPoint> nextHint = getHint(chess);
        if(nextHint.contains(redgeneral)||nextHint.contains(blackgeneral)){
            MusicPlayerDaemon::instance()->playCheckedMusic();
        }

        return;
    }

    qDebug()<<"move chess that does not exist?";
    Q_UNREACHABLE();
}

QPoint ChessBoard::getPos(int id){
    QHashIterator<QPair<int,int>,ChessMan*> iterator(m_chessmen);
    while(iterator.hasNext()){
        iterator.next();

        if(iterator.value()->id() == id){
            return QPoint(iterator.key().first,iterator.key().second);
        }
    }
    //if does not exist

    qDebug()<<"get position of chess that does not exist";
    Q_UNREACHABLE();
}

int ChessBoard::countBetween(int column, int y1, int y2){
    int retval=0;
    QHashIterator<QPair<int,int>,ChessMan*> iterator(m_chessmen);
    while(iterator.hasNext()){
        iterator.next();

        if(iterator.value()->id() != 2 && iterator.value()->id()!= 18){
            if(iterator.value()->getXpos() == column){
                int y = iterator.value()->getYpos();
                if(y>=y1&&y<=y2){
                    retval++;
                } else{
                    continue;
                }
            } else {
                continue;
            }
        }
    }

    return retval;
}

void ChessBoard::initBoardState(QString state){

    if(state.isEmpty()){
        this->initBoardState();
        return;
    }

    ChessMan* man;
    QStringList lines = state.split("\n",QString::SkipEmptyParts);
    //here,no matter who is first,red will be the first to move;

    while(!lines.isEmpty()){
        if(lines.first()=="red"){
            lines.pop_front();
            int number;
            QStringList tokens;
            QString line;

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(2,pos.x(),pos.y(),ChessMan::RED,
                                   QPixmap(":/resources/帥.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i,pos.x(),pos.y(),ChessMan::RED,
                                   QPixmap(":/resources/仕.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+3,pos.x(),pos.y(),ChessMan::RED,
                                   QPixmap(":/resources/相.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+5,pos.x(),pos.y(),ChessMan::RED,
                                   QPixmap(":/resources/马.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+7,pos.x(),pos.y(),ChessMan::RED,
                                   QPixmap(":/resources/車.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+9,pos.x(),pos.y(),ChessMan::RED,
                                   QPixmap(":/resources/炮.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+11,pos.x(),pos.y(),ChessMan::RED,
                                   QPixmap(":/resources/兵.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }
        } else {
            lines.pop_front();
            int number;
            QStringList tokens;
            QString line;

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(18,pos.x(),pos.y(),ChessMan::BLACK,
                                   QPixmap(":/resources/将.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+16,pos.x(),pos.y(),ChessMan::BLACK,
                                   QPixmap(":/resources/士.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+19,pos.x(),pos.y(),ChessMan::BLACK,
                                   QPixmap(":/resources/黑象.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+21,pos.x(),pos.y(),ChessMan::BLACK,
                                   QPixmap(":/resources/黑马.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+23,pos.x(),pos.y(),ChessMan::BLACK,
                                   QPixmap(":/resources/黑車.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+25,pos.x(),pos.y(),ChessMan::BLACK,
                                   QPixmap(":/resources/黑炮.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }

            line = lines.takeFirst();
            tokens = line.split(' ',QString::SkipEmptyParts);
            number = tokens.takeFirst().toInt();
            for(int i =0;i<number;++i){
                QString position = tokens.takeFirst();
                QPoint pos = mapToBoard(QString(position.at(1)).toInt(),
                                        QString(position.at(3)).toInt());
                man = new ChessMan(i+27,pos.x(),pos.y(),ChessMan::BLACK,
                                   QPixmap(":/resources/卒.png"));
                addItem(man);
                m_chessmen.insert(QPair<int,int>(man->getXpos(),man->getYpos()),man);
            }
        }
    }
}

QPoint ChessBoard::mapToBoard(int x, int y){
    return QPoint(x,TOTAL_ROW-1-y);
}

QString ChessBoard::dumpMap(){
    QMap<int,ChessMan*> chessmen;

    QHashIterator<QPair<int,int>,ChessMan*> iterator(m_chessmen);
    while(iterator.hasNext()){
        iterator.next();

        chessmen.insert(iterator.value()->id(),iterator.value());
    }

#define APPEND(ID) if(chessmen.contains(ID)){ \
                        line.append(QString(" <%1,%2>")\
                                    .arg(chessmen.value(ID)->getXpos())\
                                    .arg(TOTAL_ROW-1-chessmen.value(ID)->getYpos()));\
                        count++;\
                   }\

    int count = 0;
    QString retval;

    retval.append("red\n");
    retval.append("1 ").append(QString("<%1,%2>\n").arg(chessmen.value(2)->getXpos())
                               .arg(TOTAL_ROW-1-chessmen.value(2)->getYpos()));

    QString line;
    APPEND(0);
    APPEND(1);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(3);
    APPEND(4);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(5);
    APPEND(6);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(7);
    APPEND(8);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(9);
    APPEND(10);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(11);
    APPEND(12);
    APPEND(13);
    APPEND(14);
    APPEND(15);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    retval.append("black\n");
    retval.append("1 ").append(QString("<%1,%2>\n").arg(chessmen.value(18)->getXpos())
                               .arg(TOTAL_ROW-1-chessmen.value(18)->getYpos()));

    APPEND(16);
    APPEND(17);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(19);
    APPEND(20);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(21);
    APPEND(22);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(23);
    APPEND(24);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(25);
    APPEND(26);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    APPEND(27);
    APPEND(28);
    APPEND(29);
    APPEND(30);
    APPEND(31);
    retval.append(QString::number(count)+line).append('\n');
    count = 0;
    line.clear();

    return retval;
}
