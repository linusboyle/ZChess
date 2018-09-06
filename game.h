#ifndef GAME_H
#define GAME_H

enum MessageType{
    MESSAGE_ABORT,
    MESSAGE_START,
    MESSAGE_MOVE,
};

class QTcpSocket;
class QTimer;

#include <QObject>

class Game:public QObject
{
    Q_OBJECT
public:
    Game(QObject* parent = nullptr);

    void resetGame();

public slots:
    void start();
    void onIncomingMessage(QByteArray message);
    void onMoved(int id,int newx,int newy);

signals:
    void startGame();
    void opponentAborted();
    void forwardMessage(QByteArray message);
    void switchSide(int side);
    void chessMoved(int id,int x,int y);

private:
    enum GameState{
        G_RED,
        G_BLACK,
        G_BEFORE_START,
        G_WAIT_FOR_CONFIRM,
    };

    enum PlayerColor{
        P_RED,
        P_BLACK,
        P_MAX,
    };

    void onRequestStart();
    void onOpponentMoved(int id,int x,int y);

    GameState m_state;
    PlayerColor m_side;
    QTimer* m_timer;

private slots:
    void onTimerElapsed();
};


#endif // GAME_H
