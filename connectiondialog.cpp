#include "connectiondialog.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QIntValidator>

extern int DEFAULT_PORT;

ConnectionDialog::ConnectionDialog(QWidget *parent):
    QDialog(parent)
{
    m_ipedit = new QLineEdit(this);
    m_ipedit->setInputMask("000.000.000.000;_");

    m_portedit = new QLineEdit(this);
    QIntValidator* validator = new QIntValidator(0,10000,this);
    m_portedit->setValidator(validator);
    m_portedit->setText(QString::number(DEFAULT_PORT));

    QLabel* m_iplabel = new QLabel(tr("IP:"),this);
    QLabel* m_portlabel = new QLabel(tr("PORT:"),this);

    QHBoxLayout* m_inputlayout = new QHBoxLayout();
    m_inputlayout->addWidget(m_iplabel);
    m_inputlayout->addWidget(m_ipedit);

    QHBoxLayout* m_portlayout = new QHBoxLayout();
    m_portlayout->addWidget(m_portlabel);
    m_portlayout->addWidget(m_portedit);

    QPushButton* m_ok = new QPushButton(tr("OK"),this);
    QPushButton* m_cancel = new QPushButton(tr("Cancel"),this);

    QHBoxLayout* m_buttonlayout = new QHBoxLayout();
    m_buttonlayout->addWidget(m_ok);
    m_buttonlayout->addWidget(m_cancel);

    QVBoxLayout* m_layout = new QVBoxLayout();
    m_layout->addLayout(m_inputlayout);
    m_layout->addLayout(m_portlayout);
    m_layout->addLayout(m_buttonlayout);

    connect(m_ok,&QPushButton::clicked,this,&ConnectionDialog::onConfirmed);
    connect(m_cancel,&QPushButton::clicked,this,&ConnectionDialog::reject);

    setLayout(m_layout);
}

void ConnectionDialog::onConfirmed(){
    emit configurationSelected(m_ipedit->text(),m_portedit->text().toInt());
    accept();
}
