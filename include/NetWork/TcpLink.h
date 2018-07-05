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
  //��ֵ�ʹ����ú�ָ������ڴ��ϵ����𣬴�ֵ�����ǿ���һ�����ݴ�����˻�Ӵ��ڴ濪��������ʹ�����ú�ָ��
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
  //����������ݵ�ָ��
  QByteArray *m_pTcpDataHandle;
  int m_ideepNum = 0;
signals:

public slots :

};

#endif //_TCPLINK_H