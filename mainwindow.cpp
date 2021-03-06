#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include "chessboard.h"
#include "connectiondialog.h"
#include "gameplayarea.h"
#include "musicplayerdaemon.h"

#include <QInputDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QFileDialog>
#include <QFontDatabase>

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
    ui->actionS_urrender->setEnabled(false);

    connect(ui->quitaction,&QAction::triggered,[this](){
        if(m_socket&&m_socket->state() == QTcpSocket::ConnectedState){
            abortGame(false);
            m_socket->waitForBytesWritten(3000);
        }
        qApp->quit();
    });

    /* init UI
     */

    m_chessboard = new ChessBoard(this);
    m_chessboard->initBoardState();
    m_chessboard->setEnable(0);
    m_centralwidget = new GamePlayArea(this);
    m_centralwidget->setScene(m_chessboard);

    setCentralWidget(m_centralwidget);

    //message when hosting
    m_waitingmessage = new QMessageBox(QMessageBox::NoIcon,tr("Waiting"),
                                       tr("Waiting for Player..."),
                                       QMessageBox::Cancel,this,
                                       Qt::Dialog|Qt::FramelessWindowHint);
    //message when connecting
    m_connectingmessage = new QMessageBox(QMessageBox::Information, tr("Waiting"),
                                        tr("Waiting for Connection"),
                                        QMessageBox::Cancel,this,
                                        Qt::FramelessWindowHint|Qt::Dialog);

    //dialog of client
    m_connectdialog = new ConnectionDialog(this);

    setFixedSize(900,650);

    connect(m_connectdialog,&ConnectionDialog::configurationSelected,
            this,&MainWindow::onClientConnectionRequest,Qt::QueuedConnection);

    connect(ui->action_Abort_Connection,&QAction::triggered, [this](){
                m_centralwidget->log(tr("You Aborted the game"));
                this->abortGame(false);
            });
    connect(ui->actionS_urrender,&QAction::triggered,this,&MainWindow::surrender);

    m_centralwidget->log(tr("The Program Starts Successfully"));
}

MainWindow::~MainWindow()
{
    delete ui;
    MusicPlayerDaemon::instance()->free();
}

void MainWindow::on_action_Start_triggered()
{
    m_currentgame->start(mapinfo);
    m_centralwidget->log(tr("You are ready for playing,waiting for game start..."));

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
        m_centralwidget->log(tr("You start to host game at port %1").arg(port));
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

        m_centralwidget->log(tr("You cancelled the host"));
    }
}

//the server
void MainWindow::onServerNewConnection(){
    if(m_server->hasPendingConnections()){
        QTcpSocket* connection = m_server->nextPendingConnection();

        if(connection){
//            if(m_socket)
//                m_socket->deleteLater();
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
//        if(m_socket)
//           m_socket->deleteLater();
        m_socket = connection;
        m_connectingmessage->accept();
        initGame();
    });

    m_centralwidget->log(tr("You start to connect to host %1 : %2").arg(ip).arg(port));

    connection->connectToHost(ip,port);
    if(m_connectingmessage->exec() == QMessageBox::Cancel){
        connection->disconnectFromHost();
        connection->deleteLater();
        m_centralwidget->log(tr("You cancelled the connection"));
    }
}

//------------abortion
void MainWindow::abortGame(bool passive)
{
    if(!passive){
        //inform the other player
        informAbortion();
    }

    m_centralwidget->reset();

    if(m_currentgame)
        m_currentgame->resetGame();

    m_chessboard->clearBoard();

    ui->actionConnect_to_Game->setEnabled(true);
    ui->action_Host_Game->setEnabled(true);
    ui->action_Abort_Connection->setEnabled(false);
    ui->action_Start->setEnabled(false);
    ui->action_Load_Map->setEnabled(true);
    ui->actionS_urrender->setEnabled(false);

    m_centralwidget->log(tr("Game ends"));

    if(m_socket && m_socket->state() == QTcpSocket::ConnectedState
                && m_socket->waitForBytesWritten()){
        m_socket->deleteLater();
        m_socket = nullptr;
    }

    mapinfo = QString();
}

void MainWindow::initGame(){
    if(!m_currentgame){
        m_currentgame = new Game(this);
        connect(m_currentgame,&Game::opponentAborted,this,[this](){
                    m_centralwidget->log(tr("Your opponent has aborted the game"));
                    QMessageBox::warning(this,
                                         tr("Aborted!"),
                                         tr("Your opponent has aborted the game!"),
                                         QMessageBox::Ok);
                    this->abortGame(true);
                },Qt::QueuedConnection);

        connect(m_currentgame,&Game::opponentSurrender,this,[this](){
                    m_centralwidget->log(tr("Your opponent has surrendered"));
                    QMessageBox::information(this,
                                         tr("Surrender!"),
                                         tr("Your opponent has surrendered!You Win!"),
                                         QMessageBox::Ok);
                    this->abortGame(true);
                },Qt::QueuedConnection);

        connect(m_currentgame,&Game::startGame,this,[this](QString map){
                    ui->actionS_urrender->setEnabled(true);
                    ui->action_Start->setEnabled(false);
                    ui->action_Load_Map->setEnabled(false);

                    if(map != mapinfo){
                        m_chessboard->clearBoard();
                        m_chessboard->initBoardState(map);
                        mapinfo = map;
                    }

                    m_centralwidget->start();
                    m_centralwidget->log(">>>>>New Game<<<<<");
                    m_centralwidget->log(tr("The Game Starts!"));

                },Qt::QueuedConnection);

        connect(m_currentgame,&Game::switchSide,this,[this](int side){
                    m_chessboard->setEnable(side);
                    if(side == 0){
                        m_centralwidget->log(tr("Now it's opponent's turn"));
                    } else {
                        m_centralwidget->log(tr("Now it's your turn"));
                    }
                    m_centralwidget->recalc();
                }, Qt::QueuedConnection);

        connect(m_currentgame,&Game::forwardMessage,
                this,&MainWindow::sendMessage);

        connect(m_chessboard,&ChessBoard::chessMoved,this,[this](int id,int x,int y){
                    m_currentgame->onMoved(id,x,y);
                    m_centralwidget->log(tr("Chess %1 moved to (%2,%3)")
                                         .arg(getChessName(id))
                                         .arg(x).arg(y));
                },Qt::QueuedConnection);

        connect(m_chessboard,&ChessBoard::chessEaten,this,[this](int id){
                    m_currentgame->onEaten(id);
                    m_centralwidget->log(tr("Chess %1 eaten")
                                         .arg(getChessName(id)));
                },Qt::QueuedConnection);

        connect(m_currentgame,&Game::chessMoved,this,[this](int id,int x,int y){
                    m_chessboard->moveChess(id,x,y);
                    m_centralwidget->log(tr("Chess %1 moved to (%2,%3)")
                                         .arg(getChessName(id))
                                         .arg(x).arg(y));
                }, Qt::QueuedConnection);

        connect(m_currentgame,&Game::endGame,this,[this](bool redwin){
                    m_centralwidget->reset();
                    m_centralwidget->log(redwin ?
                                             tr("The Red Wins") : tr("The Black wins"));
                    QMessageBox message(QMessageBox::NoIcon,tr("Game End"),
                                        redwin?tr("The Red Wins!"):tr("The Black Wins!"),
                                        QMessageBox::Ok,this);
                    message.exec();
                    this->abortGame(true);
                },Qt::QueuedConnection);

        connect(m_currentgame,&Game::endTimeout,this,[this](bool redwin){
                    m_centralwidget->reset();
                    m_centralwidget->log(redwin ?
                                        tr("The Black Timeout") : tr("The Red Timeout"));
                    m_centralwidget->log(redwin ?
                                        tr("The Red Wins") : tr("The Black wins"));

                    QMessageBox message(QMessageBox::NoIcon,tr("Game End"),
                                        redwin?tr("The Black Timeout\nThe Red Wins!"):
                                               tr("The Red Timeout\nThe Black Wins!"),
                                        QMessageBox::Ok,this);
                    message.exec();
                    this->abortGame(true);
                },Qt::QueuedConnection);

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
    m_centralwidget->log(tr("Connection Established"));
    m_centralwidget->log(tr("Host:%1 : %2").arg(m_socket->localAddress().toString())
                         .arg(m_socket->localPort()));
    m_centralwidget->log(tr("Client:%1 : %2").arg(m_socket->peerAddress().toString())
                         .arg(m_socket->peerPort()));
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
    if(m_socket)
        m_socket->write(document.toBinaryData());
}

/*
 * handle game's request for sending message
 */

void MainWindow::sendMessage(QByteArray message){
    qDebug()<<"message write:"<<message;
    if(m_socket)
        m_socket->write(message);
}

void MainWindow::surrender(){
    QMessageBox message(QMessageBox::Warning,tr("Surrender"),tr("Are you sure?"),
                        QMessageBox::Ok|QMessageBox::Cancel,this);

    if(message.exec() == QMessageBox::Ok){
        QJsonObject message;
        message.insert("Type",MESSAGE_SURRENDER);
        QJsonDocument document;
        document.setObject(message);

        qDebug()<<"message write:"<<document;
        m_socket->write(document.toBinaryData());

        m_centralwidget->log(tr("You Surrendered!"));
        abortGame(true);
    }
}

void MainWindow::on_action_Load_Map_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Map File"),
                                 QCoreApplication::applicationDirPath(),
                                 tr("Text (*.txt)"));
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QFile::ReadOnly)) {
            QTextStream stream(&file);
            m_chessboard->clearBoard();
            mapinfo = stream.readAll();
            m_chessboard->initBoardState(mapinfo);
            m_chessboard->setEnable(0);
        }
    }

}

void MainWindow::on_action_Save_Map_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Map File"),
                                QCoreApplication::applicationDirPath());

    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QFile::WriteOnly|QFile::Text)){
            QTextStream stream(&file);
            stream << m_chessboard->dumpMap();
        }
    }
}
