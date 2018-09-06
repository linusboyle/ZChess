#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTcpServer;
class Game;
class QMessageBox;
class ConnectionDialog;
class QTcpSocket;
class ChessBoard;
class QGraphicsView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Host_Game_triggered();
    void on_actionConnect_to_Game_triggered();
    void on_action_Start_triggered();

    void onServerNewConnection();
    void onClientConnectionRequest(QString,int);

    void sendMessage(QByteArray message);
//    void initBoard();

private:
    Ui::MainWindow *ui;
    QTcpServer* m_server;
    Game* m_currentgame;
    QTcpSocket* m_socket;

    QMessageBox* m_waitingmessage;
    QMessageBox* m_connectingmessage;
    ConnectionDialog* m_connectdialog;

    ChessBoard* m_chessboard;
    QGraphicsView* m_centralwidget;

    void startWaiting();
    void informAbortion();
    void initGame();
    void abortGame(bool passive);
};

#endif // MAINWINDOW_H
