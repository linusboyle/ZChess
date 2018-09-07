#ifndef MUSICPLAYERDAEMON_H
#define MUSICPLAYERDAEMON_H

#include <QObject>
#include <QThread>

class MusicPlayerDaemon:public QObject
{
    Q_OBJECT
public:
    static MusicPlayerDaemon* instance (){
        static MusicPlayerDaemon* instance = new MusicPlayerDaemon();
        return instance;
    }

    inline void free(){
        this->deleteLater();
    }

    void playChessDownMusic();
    void playCheckedMusic();

private:
    MusicPlayerDaemon(QObject* parent = nullptr);
    MusicPlayerDaemon(const MusicPlayerDaemon& )=delete;

    ~MusicPlayerDaemon();

    QThread musicthread;

signals:
    void chessDownMusic();
    void checkMusic();
};

#endif // MUSICPLAYERDAEMON_H
