#include "NetWork/ComOpen.h"
#include <QtSerialPort/QtSerialPort>
#include <iostream>

jyComOpen::jyComOpen(QObject *parent):QObject(parent)
{
  m_pCom = new QSerialPort;
  m_pData = new QByteArray;
  m_pDataHandle = new QByteArray;
  //串口打开的时候发出readyread信号，要不断的调用读取数据槽函数接受不然读不到数据
  //不要用信号槽，不能保证一次读完一个数据帧  connect(m_pCom, &QSerialPort::readyRead, this, &jyComOpen::slotReadyRead);
}
//初始化串口设置
void jyComOpen::initPort()
{
  m_pCom->setPort(QSerialPortInfo(m_sComInfo));
  //波特率
  m_pCom->setBaudRate(QSerialPort::Baud9600);
  //8位1读
  m_pCom->setDataBits(QSerialPort::Data8);
  //设置停止位为1
  m_pCom->setStopBits(QSerialPort::OneStop);
  //设置流控制为无
  m_pCom->setFlowControl(QSerialPort::NoFlowControl);
  //设置奇偶校验为无
  m_pCom->setParity(QSerialPort::NoParity);
}
//返回串口指针
QSerialPort * jyComOpen::returnSp()
{
  return m_pCom;
}
//打开串口，并且发送数据请求完成数据初始化
bool jyComOpen::spOpen()
{
  if (!m_pCom->open(QIODevice::ReadWrite))
  {
    return false;
  }
  return m_pCom->isOpen();
}
bool jyComOpen::closeCom()
{
  m_pCom->close();
  return m_pCom->isOpen();
}
//做串口选择，根据传入参数打开1234.。。。串口
void jyComOpen::addComInfo(int i)
{
  m_sComInfo.append(QString::number(i));
}
//在这里实际进行write动作
void jyComOpen::writeData(unsigned char *Order,int OrderNum)
{
  unsigned int _icrcCode = 0;
  //crc校验
  _icrcCode = crc16Modbus(Order , OrderNum);
  QByteArray _sendMsg;
  for (int i = 0; i < OrderNum + 2 ; ++i)
  {
    _sendMsg.append(Order[i]);
  }
  qDebug() << _sendMsg.toHex() << " check sendorder data";
  qDebug() << OrderNum + 2 << "check order num";
  m_pCom->write(_sendMsg, OrderNum + 2);
}
//进行数据的获取
bool jyComOpen::getPortData(bool crcCheck)
{
  qDebug() << m_pData->toHex() << "before clear";
  m_pData->clear();
  qDebug() << m_pData->toHex() << "after clear" << m_pData->size() << "size num";
  //单位为毫秒 等待50毫秒 判断是否正确写入 写入则返回true
  if (m_pCom->waitForBytesWritten(100))
  {
    //等待50毫秒的时间如果期间有可读数据则返回true超时则返回false
    while (m_pCom->waitForReadyRead(150))
    {
      m_pData->append(m_pCom->readAll());
    }

    int sizetest;
    sizetest = m_pData->size();
    qDebug() << m_pData->toHex() << "get order msg";
    qDebug() << sizetest << "get order msg size";
    //如果是设置参数指令则不对返回值进行crccheck
    if(!crcCheck)
    {
      //得到的数据进行crc校验,返回false准备进行重传
      return crc16ModbusCheck(m_pData, sizetest);
    }
    else
    {
      return true;
    }
  }
  else
  {
    //写入失败
    return false;
  }
}
//写入和读取CRC校验重传都在里面
bool jyComOpen::readAndCheckData(unsigned char *csendmsg,int iNum,bool crcCheck)
{
  //递归出口
  if (m_ideepNum > 10)
  {
    //清零
    m_ideepNum = 0;
    return false;
  }
  //递归深度
  ++m_ideepNum;
  //数据写入
  writeData(csendmsg, iNum);
  //获取数据并且进行crc校验
  if (getPortData(crcCheck))
  {
    //写入命令都不做返回数据处理
    if (!crcCheck)
    {
      qDebug() << m_pDataHandle->toHex() << "before clear handle";
      m_pDataHandle->clear();
      qDebug() << m_pDataHandle->toHex() << "after clear handle" << m_pDataHandle->size() << "size num handle";

      for (int i = 3; i < m_pData->size() - 2; ++i)
      {
        m_pDataHandle->append(m_pData->at(i));
      }
      qDebug() << m_pDataHandle->toHex() << "get data handle";
      qDebug() << m_pDataHandle->size() << "get data size";
      //深度清零
      m_ideepNum = 0;
      return true;
    }
    else
    {
      //清零
      m_ideepNum = 0;
      return true;
    }
  }
  //数据重传,这样会变成递归的形式使用计数的形式设置递归深度 到达则退出
  else
  {
    return readAndCheckData(csendmsg, iNum, crcCheck);
  }
}

QByteArray * jyComOpen::returnDataSp()
{
  return m_pDataHandle;
}

QByteArray * jyComOpen::returnDataSpOrigin()
{
  return m_pData;
}
