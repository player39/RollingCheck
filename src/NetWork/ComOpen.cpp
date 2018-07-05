#include "NetWork/ComOpen.h"
#include <QtSerialPort/QtSerialPort>
#include <iostream>

jyComOpen::jyComOpen(QObject *parent):QObject(parent)
{
  m_pCom = new QSerialPort;
  m_pData = new QByteArray;
  m_pDataHandle = new QByteArray;
  //���ڴ򿪵�ʱ�򷢳�readyread�źţ�Ҫ���ϵĵ��ö�ȡ���ݲۺ������ܲ�Ȼ����������
  //��Ҫ���źŲۣ����ܱ�֤һ�ζ���һ������֡  connect(m_pCom, &QSerialPort::readyRead, this, &jyComOpen::slotReadyRead);
}
//��ʼ����������
void jyComOpen::initPort()
{
  m_pCom->setPort(QSerialPortInfo(m_sComInfo));
  //������
  m_pCom->setBaudRate(QSerialPort::Baud9600);
  //8λ1��
  m_pCom->setDataBits(QSerialPort::Data8);
  //����ֹͣλΪ1
  m_pCom->setStopBits(QSerialPort::OneStop);
  //����������Ϊ��
  m_pCom->setFlowControl(QSerialPort::NoFlowControl);
  //������żУ��Ϊ��
  m_pCom->setParity(QSerialPort::NoParity);
}
//���ش���ָ��
QSerialPort * jyComOpen::returnSp()
{
  return m_pCom;
}
//�򿪴��ڣ����ҷ�����������������ݳ�ʼ��
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
//������ѡ�񣬸��ݴ��������1234.����������
void jyComOpen::addComInfo(int i)
{
  m_sComInfo.append(QString::number(i));
}
//������ʵ�ʽ���write����
void jyComOpen::writeData(unsigned char *Order,int OrderNum)
{
  unsigned int _icrcCode = 0;
  //crcУ��
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
//�������ݵĻ�ȡ
bool jyComOpen::getPortData(bool crcCheck)
{
  qDebug() << m_pData->toHex() << "before clear";
  m_pData->clear();
  qDebug() << m_pData->toHex() << "after clear" << m_pData->size() << "size num";
  //��λΪ���� �ȴ�50���� �ж��Ƿ���ȷд�� д���򷵻�true
  if (m_pCom->waitForBytesWritten(100))
  {
    //�ȴ�50�����ʱ������ڼ��пɶ������򷵻�true��ʱ�򷵻�false
    while (m_pCom->waitForReadyRead(150))
    {
      m_pData->append(m_pCom->readAll());
    }

    int sizetest;
    sizetest = m_pData->size();
    qDebug() << m_pData->toHex() << "get order msg";
    qDebug() << sizetest << "get order msg size";
    //��������ò���ָ���򲻶Է���ֵ����crccheck
    if(!crcCheck)
    {
      //�õ������ݽ���crcУ��,����false׼�������ش�
      return crc16ModbusCheck(m_pData, sizetest);
    }
    else
    {
      return true;
    }
  }
  else
  {
    //д��ʧ��
    return false;
  }
}
//д��Ͷ�ȡCRCУ���ش���������
bool jyComOpen::readAndCheckData(unsigned char *csendmsg,int iNum,bool crcCheck)
{
  //�ݹ����
  if (m_ideepNum > 10)
  {
    //����
    m_ideepNum = 0;
    return false;
  }
  //�ݹ����
  ++m_ideepNum;
  //����д��
  writeData(csendmsg, iNum);
  //��ȡ���ݲ��ҽ���crcУ��
  if (getPortData(crcCheck))
  {
    //д����������������ݴ���
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
      //�������
      m_ideepNum = 0;
      return true;
    }
    else
    {
      //����
      m_ideepNum = 0;
      return true;
    }
  }
  //�����ش�,�������ɵݹ����ʽʹ�ü�������ʽ���õݹ���� �������˳�
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
