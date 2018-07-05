#include "ServerTest/ServerTest.h"

jyServerTest::jyServerTest(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  m_pServer = new QTcpServer();
  //listen����ָ��������Ŀ��������ַ�ͱ����˿ڣ�������ַΪAnyʱ�������������ӿ�
  qDebug()<<m_pServer->listen(QHostAddress::Any, 8080);
  //newConnection�źŻ��ڷ�������������ʱ���ͣ��󶨲ۺ���Ϊ���������ӽ���socketͨ�Žӿ�
  connect(m_pServer, &QTcpServer::newConnection, this, &jyServerTest::slotServerNewConnect);

}
void jyServerTest::slotServerNewConnect()
{
  //������һ�����ڵȴ������� socket��Ϊ���������ӽڵ㱻���� QTcpServer��������ʱ�������Զ�ɾ�����ڷ�����û�б����������ʱ���ô˺����᷵��0
  m_pSocket = m_pServer->nextPendingConnection();
  //��������Ҫ��ȡʱ����readyRead�źţ������ݶ�ȡ���� һֱ��ȡ����
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
