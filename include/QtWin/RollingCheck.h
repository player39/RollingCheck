#ifndef _ROLLING_CHECK_H
#define _ROLLING_CHECK_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "NetWork/ComOpen.h"
#include "NetWork/TcpLink.h"
#include "NetWork/SendThread.h"
#include "DataHandle/dataHandle.h"
#include "ui_RollingCheck.h"

class jyRollingCheck : public QMainWindow
{
  Q_OBJECT
	
public:
  jyRollingCheck(QWidget *parent = Q_NULLPTR);
  //��ʼ������
  void initParam();
  bool CheckLink();
  bool checkData(QString data,int flagModel);
  void initLastParam();
  void writeConfig(const QMap<QString,QString> &configinfo);

private:
  Ui::RollingCheckClass ui;
  //�����ļ�������
  jyComOpen *m_pCom=nullptr;
  //��ʾѡ��Ĵ���
  int m_iCurrentIndex = -1;
  //tcplink
  jyTcpLink *m_pTcpLink=nullptr;
  //��ʾ���ӵ�Ŀ��ip
  QString m_sIpAddr = "0.0.0.0";
  //��ʾ���ӵĶ˿�
  int m_iIpPort=0;
  //��ʾҪ���͵�����
  QByteArray *m_bSendOrder;
 
  enum m_dataScope {unsignedfloatPrecision_0, except_0_floatPrecision_2, floatPrecision_2, floatPrecision_4, signfloatPrecision_0
  };
  //���ַ
  unsigned char m_watchAddrNow = 1;
  unsigned char m_watchAddrNew = 0;

signals:

public slots :
  //�򿪴���
  bool slotOpenPort();
  //�ı�򿪵Ĵ��ں�
  void slotChangeIndex(int i);
  void slotIpLink();
  //�󶨶�ȡ������ť
  void slotInitParam();
  //�����ò�����ť
  void slotSetParam();
  //�󶨸�λ��ť
  void slotReSet();
  //�����ñ��ַ
  void slotSetAddress();
  //��ȡ���ַ
  void slotGetWatchAddr(const QString &addr);
};
#endif //_ROLLING_CHECK_H