#ifndef _COMOPEN_H
#define _COMOPEN_H

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QtSerialPort>
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtWidgets/QMessageBox>
#include <QtCore/qstring.h>
#include <Windows.h>
#include "DataHandle/dataHandle.h"

class jyComOpen : public QObject
{
  Q_OBJECT

public:
  explicit jyComOpen(QObject *parent = Q_NULLPTR);
  void initPort();
  QSerialPort * returnSp();
  bool spOpen();
  bool closeCom();
  void addComInfo(int i);
  void writeData(unsigned char*Order, int OrderNum);
  bool getPortData(bool crcCheck);
  bool readAndCheckData(unsigned char *csendmsg, int iNum, bool crcCheck);
  QByteArray * returnDataSp();
  QByteArray * returnDataSpOrigin();

private:
  QSerialPort *m_pCom;//串口指针
  QByteArray *m_pData;
  QByteArray *m_pDataHandle;//数据指针
  QMutex m_mPortMutex;//线程锁
  QString m_sComInfo="COM";//串口选择信息
  int m_ideepNum=0;//设置递归深度

signals:


public slots:

};
#endif //_COMOPEN_H