#ifndef GAMEPLAYAREA_H
#define GAMEPLAYAREA_H

#include <QWidget>

class QGraphicsView;
class QGraphicsScene;
class QTextEdit;

class GamePlayArea :public QWidget
{
    Q_OBJECT
public:
    GamePlayArea(QWidget* parent = nullptr);

    void setScene(QGraphicsScene* scene);
    void log(QString text);

private:
    QGraphicsView* m_playview;
    QTextEdit* m_log;
};

#endif // GAMEPLAYAREA_H
