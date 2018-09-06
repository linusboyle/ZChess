#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

class QLineEdit;

class ConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    ConnectionDialog(QWidget* parent = nullptr);

signals:
    void configurationSelected(QString,int);

private slots:
    void onConfirmed();

private:
    QLineEdit* m_ipedit;
    QLineEdit* m_portedit;
};

#endif // CONNECTIONDIALOG_H
