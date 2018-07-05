#ifndef _TCPLINK_H
#define _TCPLINK_H

#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QByteArray>
#include "DataHandle/DataHandle.h"

class jyTcpLink : public QObject
{
  Q_OBJECT
public:
  explicit jyTcpLink(const QString& sipAddress, const int& iport, QObject *parent = nullptr);
  bool linkStart();
  bool closeLink();
  //传值和传引用和指针存在内存上的区别，传值本质是拷贝一份数据传送因此会加大内存开销，尽量使用引用和指针
  void sendOrder(const QString& sOrder);
  void writeData(unsigned char*Order, int OrderNum);
  bool getPortData(bool crcCheck);
  bool readAndCheckData(unsigned char *csendmsg, int iNum, bool crcCheck);
  QTcpSocket* returnTcpLink();
  QByteArray * returnTcpDataSp();
  QByteArray * returnTcpDataSpOrigin();
private:
  QTcpSocket *m_pSocket=nullptr;
  QString m_sIPAddress;
  int m_iHostPort;
  QByteArray *m_pTcpData;
  //处理完的数据的指针
  QByteArray *m_pTcpDataHandle;
  int m_ideepNum = 0;
signals:

public slots :

};

#endif //_TCPLINK_H