#include "game.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#ifdef QT_DEBUG
#include <QDebug>
#endif

extern constexpr int TIMEOUT_MS = 60000; //60s

Game::Game(QObject *parent):
    QObject(parent),
    m_state(G_BEFORE_START),
    m_side(P_MAX)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(TIMEOUT_MS);

    connect(m_timer,&QTimer::timeout,this,&Game::timeout);
}

void Game::resetGame(){
    m_state = G_BEFORE_START;
    m_side  = P_MAX;
    m_timer->stop();
}

void Game::start()
{
    QJsonObject object;
    QJsonDocument document;
    //if it's the first to start,be the red side
    if(m_state == G_BEFORE_START) {
        object.insert("Type",MESSAGE_START);
        //TODO
        //maybe the color is useless here..

        object.insert("Color","RED");
        document.setObject(object);

        emit forwardMessage(document.toBinaryData());
        m_state = G_WAIT_FOR_CONFIRM;
    } else {
        object.insert("Type",MESSAGE_START);
        object.insert("Color","BLACK");
        document.setObject(object);

        emit forwardMessage(document.toBinaryData());

        //the game starts,from the red side
        m_state = G_RED;
        m_side = P_BLACK;

        //draw the board gui
        emit startGame();
        emit switchSide(0);

        m_timer->start();
    }
}

void Game::onIncomingMessage(QByteArray message){
    auto document = QJsonDocument::fromBinaryData(message);
    QJsonObject object = document.object();

#ifdef QT_DEBUG
    qDebug()<<"incoming message:"<<object;
#endif

    switch (object.value("Type").toInt()) {
    case MESSAGE_ABORT:
        emit opponentAborted();
        break;
    case MESSAGE_START:
        onRequestStart();
        break;
    case MESSAGE_MOVE:
    {
        int id=object.value("id").toInt();
        int x=object.value("x").toInt();
        int y=object.value("y").toInt();
        onOpponentMoved(id,x,y);
    }
        break;
    case MESSAGE_SURRENDER:
        emit opponentSurrender();
        break;
    default:
        qDebug()<<"other...";
        Q_UNREACHABLE();
        break;
    }
}

void Game::onRequestStart(){
    if (m_state == G_BEFORE_START){
        m_state = G_WAIT_FOR_CONFIRM;
    } else {
        m_state = G_RED;
        m_side = P_RED;

        emit startGame();
        emit switchSide(1);

        m_timer->start();
    }
}

void Game::changeTurn(){
    qDebug()<<"switch side";

    if(m_side == P_RED){
        if(m_state == G_RED){
            emit switchSide(0);
            m_state = G_BLACK;
        }
        else{
            emit switchSide(1);
            m_state = G_RED;
        }
    }
    else if(m_side == P_BLACK){
        if(m_state == G_BLACK){
            emit switchSide(0);
            m_state = G_RED;
        }
        else{
            emit switchSide(2);
            m_state = G_BLACK;
        }
    }
}

void Game::onMoved(int id, int newx, int newy){
    m_timer->stop();

    //send the package
    QJsonObject object;
    object.insert("Type",MESSAGE_MOVE);
    object.insert("id",id);
    object.insert("x",newx);
    object.insert("y",newy);

    QJsonDocument document;
    document.setObject(object);

    forwardMessage(document.toBinaryData());

    //restart the timer,and switch side
    this->changeTurn();
    m_timer->start();
}

void Game::onOpponentMoved(int id, int x, int y){
    m_timer->stop();

    //request the board to update
    emit chessMoved(id,x,y);

    //switch side
    this->changeTurn();
    m_timer->start();
}

void Game::onEaten(int id){
    if(id == 2){
        //black win
        emit endGame(false);
        return;
    } else if(id == 18){
        //red win
        emit endGame(true);
        return;
    }

    //TODO
    //do something when chess is eaten
}

void Game::timeout(){
    m_timer->stop();

    if(m_state == G_RED){
        //red timeout,black win
        emit endTimeout(false);
    } else {
        //red win
        emit endTimeout(true);
    }

    //IDEA
    //don't intend to tell the other the timeout,it will timeout when receiving no
    //move fallback from the other side
}
