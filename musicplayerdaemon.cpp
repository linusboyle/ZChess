#include "musicplayerdaemon.h"
#include <QMediaPlayer>

MusicPlayerDaemon::MusicPlayerDaemon(QObject *parent)
    :QObject(parent)
{
    QMediaPlayer* chessdown = new QMediaPlayer();
    chessdown->setMedia(QUrl("qrc:/resources/chessdown.mp3"));
    chessdown->moveToThread(&musicthread);

    QMediaPlayer* chesschecked = new QMediaPlayer();
    chesschecked->setMedia(QUrl("qrc:/resources/check.wav"));
    chesschecked->moveToThread(&musicthread);

    connect(&musicthread,&QThread::finished,chessdown,&QMediaPlayer::deleteLater);
    connect(&musicthread,&QThread::finished,chesschecked,&QMediaPlayer::deleteLater);
    connect(this,&MusicPlayerDaemon::chessDownMusic,chessdown,&QMediaPlayer::play);
    connect(this,&MusicPlayerDaemon::checkMusic,chesschecked,&QMediaPlayer::play);

    musicthread.start();
}

void MusicPlayerDaemon::playChessDownMusic(){
    emit chessDownMusic();
}

MusicPlayerDaemon::~MusicPlayerDaemon(){
    musicthread.quit();
    musicthread.wait();
}

void MusicPlayerDaemon::playCheckedMusic(){
    emit checkMusic();
}
