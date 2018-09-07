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
class GamePlayArea;

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
    void surrender();

    void sendMessage(QByteArray message);
//    void initBoard();

    void on_action_Load_Map_triggered();

private:
    Ui::MainWindow *ui;
    QTcpServer* m_server;
    Game* m_currentgame;
    QTcpSocket* m_socket;

    QMessageBox* m_waitingmessage;
    QMessageBox* m_connectingmessage;
    ConnectionDialog* m_connectdialog;

    ChessBoard* m_chessboard;
    GamePlayArea* m_centralwidget;

    QString mapinfo;

    static QString getChessName(int id){
    switch (id) {
        case 0:
        case 1:
            return tr("Red Advisor");
        case 2:
            return tr("Red General");
        case 3:
        case 4:
            return tr("Red Elephant");
        case 5:
        case 6:
            return tr("Red Horse");
        case 7:
        case 8:
            return tr("Red Chariot");
        case 9:
        case 10:
            return tr("Red Cannon");
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            return tr("Red Soldier");
        case 16:
        case 17:
            return tr("Black Advisor");
        case 18:
            return tr("Black General");
        case 19:
        case 20:
            return tr("Black Elephant");
        case 21:
        case 22:
            return tr("Black Horse");
        case 23:
        case 24:
            return tr("Black Chariot");
        case 25:
        case 26:
            return tr("Black Cannon");
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            return tr("Black Soldier");
        default:
            Q_UNREACHABLE();
            break;
    }
}

    void startWaiting();
    void informAbortion();
    void initGame();
    void abortGame(bool passive);
};

#endif // MAINWINDOW_H
