#include "ServerTest/ServerTest.h"

jyServerTest::jyServerTest(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  m_pServer = new QTcpServer();
  //listen函数指定监听的目标主机地址和本机端口，主机地址为Any时会监听所有网络接口
  qDebug()<<m_pServer->listen(QHostAddress::Any, 8080);
  //newConnection信号会在服务器有新连接时发送，绑定槽函数为新来的连接建立socket通信接口
  connect(m_pServer, &QTcpServer::newConnection, this, &jyServerTest::slotServerNewConnect);

}
void jyServerTest::slotServerNewConnect()
{
  //返回下一个正在等待的连接 socket作为服务器的子节点被创建 QTcpServer对象被销毁时，他将自动删除，在服务器没有被挂起的连接时调用此函数会返回0
  m_pSocket = m_pServer->nextPendingConnection();
  //有数据需要读取时触发readyRead信号，绑定数据读取函数 一直读取数据
  connect(m_pSocket, &QTcpSocket::readyRead, this, &jyServerTest::slotSocketReadData);
}

void jyServerTest::slotSocketReadData()
{
  QByteArray buffer;
  buffer = m_pSocket->readAll();
  QString buffer_string = buffer;
  if (buffer_string==("test"))
  {
    m_pSocket->write("this is test");
    m_pSocket->flush();
  }
  
  qDebug() << "this is receive data "+buffer;
}
