#include "gameplayarea.h"
#include <QGraphicsView>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QLCDNumber>
#include <QTimer>
#include <QDebug>
#include <QScrollBar>

extern int TIMEOUT_MS;

static int TIMEOUT_S;

GamePlayArea::GamePlayArea(QWidget *parent)
    :QWidget(parent)
{
    TIMEOUT_S = TIMEOUT_MS/1000;

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);

    connect(m_timer,&QTimer::timeout,this,&GamePlayArea::onTimerElapsed);

    remaining = TIMEOUT_S;

    m_playview = new QGraphicsView(this);
    m_playview->setFixedSize(650,650);
    m_playview->setFrameStyle(QFrame::NoFrame);

    m_log = new QTextEdit(this);
    m_log->setReadOnly(true);
    m_log->setStyleSheet("background-color:#D1BD98;");
    m_log->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_log->setFrameStyle(QFrame::NoFrame);
    m_log->verticalScrollBar()->setStyleSheet(
                "QScrollBar:vertical {"
                "width: 9px;"
                "margin: 0px 0 0px 0;"
                "}"
                "QScrollBar::handle:vertical {"
                    "background: rgb(195, 195, 195);"
                    "min-height: 20px;"
                    "margin: 0 1px 0 2px;"
                    "border-radius: 3px;"
                    "border: none;"
                "}"
                "QScrollBar::add-line:vertical {"
                    "height: 0px;"
                    "subcontrol-position: bottom;"
                    "subcontrol-origin: margin;"
                "}"
                "QScrollBar::sub-line:vertical {"
                    "height: 0px;"
                    "subcontrol-position: top;"
                    "subcontrol-origin: margin;"
                "}"
                "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                    "border: 1px solid grey;"
                    "width: 3px;"
                    "height: 3px;"
                    "background: white;"
                "}"
                "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                    "background: none;"
                "}"
                );

    m_lcd = new QLCDNumber(this);
    m_lcd->setSegmentStyle(QLCDNumber::Flat);
    m_lcd->setDigitCount(10);
    m_lcd->setStyleSheet("color: Red; background:#D1BD98;border: 0px;");
    m_lcd->display(TIMEOUT_S);
    m_lcd->setMinimumHeight(30);

    QVBoxLayout* m_rightlayout = new QVBoxLayout();
    m_rightlayout->setMargin(0);
    m_rightlayout->setMargin(0);
    m_rightlayout->addWidget(m_lcd);
    m_rightlayout->addWidget(m_log);

    QHBoxLayout* m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_playview);
    m_layout->addLayout(m_rightlayout);

    setLayout(m_layout);
}

void GamePlayArea::setScene(QGraphicsScene *scene){
    m_playview->setScene(scene);
}

void GamePlayArea::log(QString text){
    QString time = QString("<i>")+QDateTime::currentDateTime().time().toString()+QString("</i>");
    time.append(" : ");
    time.append(text);

    m_log->append(time);
}

void GamePlayArea::start(){
    red = false; //false because of  the way of using it with recalc,see MainWindow.cpp
    m_timer->stop();

    remaining = TIMEOUT_S;

    m_lcd->setStyleSheet("color: Red; background:#D1BD98;border: 0px;");
    m_lcd->display(TIMEOUT_S);

    m_timer->start();
}

void GamePlayArea::recalc(){
    m_timer->stop();

    remaining = TIMEOUT_S;

    if(red){
        m_lcd->setStyleSheet("color: Black; background:#D1BD98;border: 0px;");
        red = false;
    } else {
        m_lcd->setStyleSheet("color: Red; background:#D1BD98;border: 0px;");
        red = true;
    }

    m_lcd->display(TIMEOUT_S);

    m_timer->start();
}

void GamePlayArea::onTimerElapsed(){
    m_lcd->display(--remaining);

    //afraid the endgame logic is too much,and messagebox may block the signal
    //as I use the queuedconnection

    if(remaining==0){
        m_timer->stop();
    }
}

void GamePlayArea::reset(){
    m_timer->stop();
    remaining = TIMEOUT_S;
    m_lcd->display(TIMEOUT_S);
}
