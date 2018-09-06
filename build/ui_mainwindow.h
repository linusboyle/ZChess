/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Host_Game;
    QAction *actionConnect_to_Game;
    QAction *action_Abort_Connection;
    QAction *action_Start;
    QAction *quitaction;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuConnection;
    QMenu *menu_Game;
    QMenu *menuGeneral;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        action_Host_Game = new QAction(MainWindow);
        action_Host_Game->setObjectName(QStringLiteral("action_Host_Game"));
        actionConnect_to_Game = new QAction(MainWindow);
        actionConnect_to_Game->setObjectName(QStringLiteral("actionConnect_to_Game"));
        action_Abort_Connection = new QAction(MainWindow);
        action_Abort_Connection->setObjectName(QStringLiteral("action_Abort_Connection"));
        action_Start = new QAction(MainWindow);
        action_Start->setObjectName(QStringLiteral("action_Start"));
        quitaction = new QAction(MainWindow);
        quitaction->setObjectName(QStringLiteral("quitaction"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 23));
        menuConnection = new QMenu(menuBar);
        menuConnection->setObjectName(QStringLiteral("menuConnection"));
        menu_Game = new QMenu(menuBar);
        menu_Game->setObjectName(QStringLiteral("menu_Game"));
        menuGeneral = new QMenu(menuBar);
        menuGeneral->setObjectName(QStringLiteral("menuGeneral"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuGeneral->menuAction());
        menuBar->addAction(menuConnection->menuAction());
        menuBar->addAction(menu_Game->menuAction());
        menuConnection->addAction(action_Host_Game);
        menuConnection->addAction(actionConnect_to_Game);
        menuConnection->addAction(action_Abort_Connection);
        menu_Game->addAction(action_Start);
        menuGeneral->addAction(quitaction);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_Host_Game->setText(QApplication::translate("MainWindow", "&Host Game", nullptr));
        actionConnect_to_Game->setText(QApplication::translate("MainWindow", "&Connect to Game", nullptr));
        action_Abort_Connection->setText(QApplication::translate("MainWindow", "&Abort Connection", nullptr));
        action_Start->setText(QApplication::translate("MainWindow", "&Start", nullptr));
        quitaction->setText(QApplication::translate("MainWindow", "&Quit", nullptr));
        menuConnection->setTitle(QApplication::translate("MainWindow", "&Connection", nullptr));
        menu_Game->setTitle(QApplication::translate("MainWindow", "&Game", nullptr));
        menuGeneral->setTitle(QApplication::translate("MainWindow", "Gene&ral", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
