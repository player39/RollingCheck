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
  QSerialPort *m_pCom;//����ָ��
  QByteArray *m_pData;
  QByteArray *m_pDataHandle;//����ָ��
  QMutex m_mPortMutex;//�߳���
  QString m_sComInfo="COM";//����ѡ����Ϣ
  int m_ideepNum=0;//���õݹ����

signals:


public slots:

};
#endif //_COMOPEN_H