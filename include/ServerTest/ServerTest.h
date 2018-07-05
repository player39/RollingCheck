#ifndef _SERVERTEST_H
#define _SERVERTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_server.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
class jyServerTest : public QMainWindow
{
    Q_OBJECT

public:
  jyServerTest(QWidget *parent = Q_NULLPTR);

private:
  Ui::serverClass ui;
  QTcpServer* m_pServer;
  QTcpSocket* m_pSocket;
public slots:
  void slotServerNewConnect();
  void slotSocketReadData();
};
#endif // !_SERVERTEST_H
