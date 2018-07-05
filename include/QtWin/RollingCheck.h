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
  //初始化参数
  void initParam();
  bool CheckLink();
  bool checkData(QString data,int flagModel);
  void initLastParam();
  void writeConfig(const QMap<QString,QString> &configinfo);

private:
  Ui::RollingCheckClass ui;
  //串口文件描述符
  jyComOpen *m_pCom=nullptr;
  //表示选择的串口
  int m_iCurrentIndex = -1;
  //tcplink
  jyTcpLink *m_pTcpLink=nullptr;
  //表示连接的目标ip
  QString m_sIpAddr = "0.0.0.0";
  //表示连接的端口
  int m_iIpPort=0;
  //表示要发送的数据
  QByteArray *m_bSendOrder;
 
  enum m_dataScope {unsignedfloatPrecision_0, except_0_floatPrecision_2, floatPrecision_2, floatPrecision_4, signfloatPrecision_0
  };
  //表地址
  unsigned char m_watchAddrNow = 1;
  unsigned char m_watchAddrNew = 0;

signals:

public slots :
  //打开串口
  bool slotOpenPort();
  //改变打开的串口号
  void slotChangeIndex(int i);
  void slotIpLink();
  //绑定读取参数按钮
  void slotInitParam();
  //绑定设置参数按钮
  void slotSetParam();
  //绑定复位按钮
  void slotReSet();
  //绑定设置表地址
  void slotSetAddress();
  //获取表地址
  void slotGetWatchAddr(const QString &addr);
};
#endif //_ROLLING_CHECK_H