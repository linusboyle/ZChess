#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include "chessboard.h"
#include "connectiondialog.h"
#include <QInputDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QGraphicsView>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

extern constexpr int DEFAULT_PORT = 6700;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_server(nullptr),
    m_currentgame(nullptr),
    m_socket(nullptr)
{
    ui->setupUi(this);
    ui->action_Abort_Connection->setEnabled(false);
    ui->action_Start->setEnabled(false);

    connect(ui->quitaction,&QAction::triggered,[this](){
        abortGame(false);

        qApp->quit();
    });

    m_chessboard = new ChessBoard(this);
    m_centralwidget = new QGraphicsView(this);
    m_centralwidget->setScene(m_chessboard);

    setCentralWidget(m_centralwidget);

    //message when hosting
    m_waitingmessage = new QMessageBox(QMessageBox::NoIcon,tr("Waiting"),
                                       tr("Waiting for Player..."),
                                       QMessageBox::Cancel,this,
                                       Qt::Dialog|Qt::FramelessWindowHint);
    //message when connecting
    m_connectingmessage=new QMessageBox(QMessageBox::Information, tr("Waiting"),
                                        tr("Waiting for Connection"),
                                        QMessageBox::Cancel,this,
                                        Qt::FramelessWindowHint|Qt::Dialog);

    //dialog of client
    m_connectdialog = new ConnectionDialog(this);

    connect(m_connectdialog,&ConnectionDialog::configurationSelected,
            this,&MainWindow::onClientConnectionRequest,Qt::QueuedConnection);

    connect(ui->action_Abort_Connection,&QAction::triggered, [this](){
                this->abortGame(false);
            });

    setFixedSize(650,650);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Start_triggered()
{
    //replace to game's start()
    m_currentgame->start();

    ui->action_Start->setEnabled(false);
}

//----------pre-game----------
//----------host--------------
void MainWindow::on_action_Host_Game_triggered()
{
    bool confirmed = false;
    int port = QInputDialog::getInt(this,tr("New Game"),tr("Input the Port you "
                                                           "want to host"),
                                    DEFAULT_PORT,0,10000,1,&confirmed);

    if(confirmed){
        if(!m_server){
            m_server = new QTcpServer(this);
        }
        m_server->listen(QHostAddress::Any,port);

        connect(m_server,&QTcpServer::newConnection,this,
                &MainWindow::onServerNewConnection);

        startWaiting();
    }
}

void MainWindow::startWaiting(){
    //you cannot host twice;
    ui->actionConnect_to_Game->setEnabled(false);
    ui->action_Host_Game->setEnabled(false);

    if(m_waitingmessage->exec() == QMessageBox::Cancel){
        //stop listening
        m_server->close();
        ui->actionConnect_to_Game->setEnabled(true);
        ui->action_Host_Game->setEnabled(true);
    }
}

//the server
void MainWindow::onServerNewConnection(){
    if(m_server->hasPendingConnections()){
        QTcpSocket* connection = m_server->nextPendingConnection();

        if(connection){
            if(m_socket)
                m_socket->deleteLater();
            m_socket = connection;
            //close messagebox
            m_waitingmessage->accept();
            //stop furthur listening
            m_server->close();

            initGame();
        }
    }
}

//-----client----------
void MainWindow::on_actionConnect_to_Game_triggered()
{
    m_connectdialog->exec();
}

void MainWindow::onClientConnectionRequest(QString ip,int port){
    QTcpSocket* connection = new QTcpSocket(this);

    connect(connection,&QTcpSocket::connected,[this,connection](){
        if(m_socket)
           m_socket->deleteLater();
        m_socket = connection;
        m_connectingmessage->accept();
        initGame();
    });

    connection->connectToHost(ip,port);
    if(m_connectingmessage->exec() == QMessageBox::Cancel){
        connection->disconnectFromHost();

        connection->deleteLater();
    }
}

//------------abortion
void MainWindow::abortGame(bool passive)
{
    if(!passive){
        //inform the other player
        informAbortion();
    }

    m_currentgame->resetGame();

    m_chessboard->clearBoard();

    ui->actionConnect_to_Game->setEnabled(true);
    ui->action_Host_Game->setEnabled(true);
    ui->action_Abort_Connection->setEnabled(false);
    ui->action_Start->setEnabled(false);
}

void MainWindow::initGame(){
    if(!m_currentgame){
        m_currentgame = new Game(this);
        connect(m_currentgame,&Game::opponentAborted, [this](){
                    this->abortGame(true);
                    QMessageBox::warning(this,
                                         tr("Aborted!"),
                                         tr("Your opponent has aborted the game!"),
                                         QMessageBox::Ok);
                });

        connect(m_currentgame,&Game::startGame,m_chessboard,&ChessBoard::initBoardState,
                Qt::QueuedConnection);
        connect(m_currentgame,&Game::switchSide,m_chessboard,&ChessBoard::setEnable,
                Qt::QueuedConnection);

        connect(m_currentgame,&Game::forwardMessage,
                this,&MainWindow::sendMessage);

        connect(m_chessboard,&ChessBoard::chessMoved,m_currentgame,&Game::onMoved);
        connect(m_currentgame,&Game::chessMoved,m_chessboard,&ChessBoard::moveChess);
    }

    connect(m_socket,&QTcpSocket::readyRead,[this](){
        m_currentgame->onIncomingMessage(m_socket->readAll());
    });


    //disable hosting and connecting
    ui->actionConnect_to_Game->setEnabled(false);
    ui->action_Host_Game->setEnabled(false);

    //enable disconnecting and game options
    ui->action_Abort_Connection->setEnabled(true);
    ui->action_Start->setEnabled(true);

    //information
    QMessageBox::information(this,tr("Connection Established"),
                             tr("the connection has been successfully "
                                "established,you can now start the game")
                             ,QMessageBox::Ok);
}

/*
 * send a message for abortion
 */

void MainWindow::informAbortion(){
    QJsonObject message;
    message.insert("Type",MESSAGE_ABORT);
    QJsonDocument document;
    document.setObject(message);

    qDebug()<<"message write:"<<document;
    m_socket->write(document.toBinaryData());
}

/*
 * handle game's request for sending message
 */

void MainWindow::sendMessage(QByteArray message){
    qDebug()<<"message write:"<<message;
    m_socket->write(message);
}
