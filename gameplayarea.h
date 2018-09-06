#ifndef GAMEPLAYAREA_H
#define GAMEPLAYAREA_H

#include <QWidget>

class QGraphicsView;
class QGraphicsScene;
class QTextEdit;
class QLCDNumber;
class QTimer;

class GamePlayArea :public QWidget
{
    Q_OBJECT
public:
    GamePlayArea(QWidget* parent = nullptr);

    void setScene(QGraphicsScene* scene);
    void log(QString text);
    void start();
    void recalc();
    void reset();

private:
    QGraphicsView* m_playview;
    QTextEdit* m_log;
    QLCDNumber* m_lcd;
    QTimer* m_timer;

    int remaining;
    bool red;

private slots:
    void onTimerElapsed();
};

#endif // GAMEPLAYAREA_H
