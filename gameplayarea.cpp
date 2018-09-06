#include "gameplayarea.h"
#include <QGraphicsView>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QDateTime>

GamePlayArea::GamePlayArea(QWidget *parent)
    :QWidget(parent)
{
    m_playview = new QGraphicsView(this);
    m_playview->setFixedSize(650,650);

    m_log = new QTextEdit(this);
    m_log->setReadOnly(true);
    m_log->setStyleSheet("background-color:#D1BD98;");
    m_log->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout* m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_layout->addWidget(m_playview);
    m_layout->addWidget(m_log);

    setLayout(m_layout);
}

void GamePlayArea::setScene(QGraphicsScene *scene){
    m_playview->setScene(scene);
}

void GamePlayArea::log(QString text){
    QString time = QDateTime::currentDateTime().time().toString();
    time.append(':');
    time.append(text);

    m_log->append(time);
}
