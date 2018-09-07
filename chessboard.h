#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>

#define COLUMN_NUM 9
#define ROW_NUM 5
#define TOTAL_ROW ROW_NUM*2

class ChessMan;
class Indicator;

class ChessBoard:public QGraphicsScene
{
    Q_OBJECT
public:
    ChessBoard(QObject* parent = nullptr);

public slots:
    void initBoardState();
    void initBoardState(QString state);
    void clearBoard();
    void setEnable(int side);
    void moveChess(int id,int x,int y);

private:
    void drawBoard();
    void drawDecoration(int x,int y);

    QList<QPoint> getHint(ChessMan* chess);

    QList<QPoint> getShiHint(ChessMan* chess);
    QList<QPoint> getGeneralHint(ChessMan*);
    QList<QPoint> getXiangHint(ChessMan*);
    QList<QPoint> getHorseHint(ChessMan*);
    QList<QPoint> getJuHint(ChessMan*);
    QList<QPoint> getPaoHint(ChessMan*);
    QList<QPoint> getSoldierHint(ChessMan*);

    void display(const QList<QPoint> &);

    void move(ChessMan* chess,int x,int y);

    bool exist(int x,int y);
    bool checkValid(int x,int y);
    bool checkSameColor(ChessMan* chess,int x,int y);
    int countBetween(int column,int y1,int y2);
    QPoint mapToBoard(int x,int y);
    QPoint mapToFile(int x,int y);
    QPoint getPos(int id);

    QHash<QPair<int,int>,ChessMan*> m_chessmen;
    QVector<Indicator*> m_indicators;
    ChessMan* m_selected;
    QPoint redgeneral;
    QPoint blackgeneral;

private slots:
    void chessSelected();

signals:
    void chessMoved(int chessid,int newx,int newy);
    void chessEaten(int chessid);
};

#endif // CHESSBOARD_H
