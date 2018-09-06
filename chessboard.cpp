#include "chessboard.h"
#include "chessman.h"
#include "indicator.h"
#include <QDebug>

extern constexpr qreal GRID_SIZE = 60;

ChessBoard::ChessBoard(QObject *parent)
    :QGraphicsScene(parent),
      m_selected(nullptr)
{
    setBackgroundBrush(QColor(209,189,152));
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
    addRect(basex,basey,GRID_SIZE*(COLUMN_NUM-1),GRID_SIZE*(ROW_NUM-1),
            pen,brush);

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

    //TODO
    //draw the text
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
//    QHashIterator<QPair<int,int>,ChessMan*> iterator(m_chessmen);
//    while(iterator.hasNext()){
//        iterator.next();

//        removeItem(iterator.value());
//        delete iterator.value();
//    }

//    m_chessmen.clear();
//    clearSelection();
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

    switch (chess->id()) {
        case 0:
        case 1:
        case 16:
        case 17:
            displayShiHint(chess);
            break;
        case 2:
        case 18:
            displayGeneralHint(chess);
            break;
        case 3:
        case 4:
        case 19:
        case 20:
            displayXiangHint(chess);
            break;
        case 5:
        case 6:
        case 21:
        case 22:
            displayHorseHint(chess);
            break;
        case 7:
        case 8:
        case 23:
        case 24:
            displayJuHint(chess);
            break;
        case 9:
        case 10:
        case 25:
        case 26:
            displayPaoHint(chess);
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            displaySoldierHint(chess);
            break;
        default:
            displaySoldierHint(chess);
            break;
    }
}

void ChessBoard::displayShiHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

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

    display(valid);
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

void ChessBoard::displayGeneralHint(ChessMan* chess){
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
                valid.append(point);
            }
        }
        //TODO
        //general cannot be on the same column
        //or maybe they can,but just can  eat each other?
        else{
            if(point.x()>=3&&point.x()<=5&&point.y()>=7&&point.y()<=9
                    &&!checkSameColor(chess,point.x(),point.y())){
                valid.append(point);
            }
        }
    }

    display(valid);
}

void ChessBoard::displayXiangHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

    int dx[4] = {-2,-2,2,2};
    int dy[4] = {-2,2,-2,2};

    int dx2[4]= {-1,-1,1,1};
    int dy2[4]= {-1,1,-1,1};

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

    display(valid);
}

void ChessBoard::displayHorseHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

    int dx[8] = {-1,-2,1,2,2,1,-1,-2};
    int dy[8] = {-2,-1,-2,-1,1,2,2,1};

    int dx2[8] = {0,-1,0,1,1,0,0,-1};
    int dy2[8] = {-1,0,-1,0,0,1,1,0};

    for(int i=0;i<8;++i){
        QPoint point(x+dx[i],y+dy[i]);
        if(checkValid(point.x(),point.y())&&!exist(x+dx2[i],y+dy2[i])
                &&!checkSameColor(chess,point.x(),point.y())){
            valid.append(point);
        }
    }

    display(valid);
}

bool ChessBoard::exist(int x, int y){
    QPair<int,int> pair(x,y);
    if(m_chessmen.contains(pair)){
        return true;
    }
    return false;
}

void ChessBoard::displayJuHint(ChessMan* chess){
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

    display(valid);
}

void ChessBoard::displayPaoHint(ChessMan* chess){
    QList<QPoint> valid;

    int x = chess->getXpos();
    int y = chess->getYpos();

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

    display(valid);
}

void ChessBoard::displaySoldierHint(ChessMan* chess){
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
            if(checkValid(point.x(),point.y())){
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
            if(checkValid(point.x(),point.y())){
                valid.append(point);
            }
        }
    }
    display(valid);
}

void ChessBoard::display(QList<QPoint> valid){
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
    //cannot go further when moved
    setEnable(0);

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
        if(chess->id()>=0 && chess->id()<=15){ //red
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
    QHashIterator<QPair<int,int>,ChessMan*> iterator(m_chessmen);

    ChessMan* chess = nullptr;
    while(iterator.hasNext()){
        iterator.next();

        auto chessman = iterator.value();

        if(chessman->id() == id){
            chess = chessman;
            break;
        }
    }

    if(chess){
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
        return;
    }

    qDebug()<<"move chess that does not exist?";
    Q_UNREACHABLE();
}
