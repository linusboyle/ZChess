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
    void clearBoard();
    void setEnable(int side);
    void moveChess(int id,int x,int y);

private:
    void drawBoard();
    void drawDecoration(int x,int y);

    void displayShiHint(ChessMan* chess);
    void displayGeneralHint(ChessMan*);
    void displayXiangHint(ChessMan*);
    void displayHorseHint(ChessMan*);
    void displayJuHint(ChessMan*);
    void displayPaoHint(ChessMan*);
    void displaySoldierHint(ChessMan*);

    void display(QList<QPoint>);

    void move(ChessMan* chess,int x,int y);

    bool exist(int x,int y);
    bool checkValid(int x,int y);
    bool checkInPalace(int x,int y);
    bool checkSameColor(ChessMan* chess,int x,int y);

    QHash<QPair<int,int>,ChessMan*> m_chessmen;
    QVector<Indicator*> m_indicators;
    ChessMan* m_selected;

private slots:
    void chessSelected();

signals:
    void chessMoved(int chessid,int newx,int newy);
    void chessEaten(int chessid);
};

#endif // CHESSBOARD_H
