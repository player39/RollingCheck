#include "NetWork/Tcplink.h"

jyTcpLink::jyTcpLink(const QString& sipAddress,const int& iport,QObject *parent)
{
  //��ʼ������ip
  m_sIPAddress = sipAddress;
  //��ʼ�����Ӷ˿�
  m_iHostPort = iport;
  m_pTcpData = new QByteArray;
  m_pTcpDataHandle = new QByteArray;
  qDebug() << m_iHostPort;
}
//��������
bool jyTcpLink::linkStart()
{
  m_pSocket = new QTcpSocket();
  m_pSocket->connectToHost(m_sIPAddress, m_iHostPort);
  //���������Ƿ�ɹ�
  if (!m_pSocket->waitForConnected(30000))
  {
    return false;
  }
  else 
  {
    //ָ�����
    uchar sendbuf_r[9] = { 1,3,0,0,0,33 };
    readAndCheckData(sendbuf_r, 6, false);
    return true;
  }
}
//�ر�����
bool jyTcpLink::closeLink()
{
  m_pSocket->close();
  return !m_pSocket->isOpen();
}
//����ָ��
void jyTcpLink::sendOrder(const QString& Order)
{
  m_pSocket->write("test");
  m_pSocket->flush();
}

void jyTcpLink::writeData(unsigned char * Order, int OrderNum)
{
  for (int i = 0; i < OrderNum; ++i)
  {
    qDebug() << Order[i] << "char";
  }
  unsigned int _icrcCode = 0;//�ֲ������Զ����
  _icrcCode = crc16Modbus(Order, OrderNum);//crcУ��
  qDebug() << _icrcCode << "crcode";
  QByteArray _sendMsg;//�ֲ������Զ����
  for (int i = 0; i < OrderNum + 2; ++i)
  {
    _sendMsg.append(Order[i]);
  }
  qDebug() << _sendMsg.toHex() << " check sendorder data";
  qDebug() << OrderNum + 2 << "check order num";
  m_pSocket->write(_sendMsg , OrderNum + 2);
}

bool jyTcpLink::getPortData(bool crcCheck)
{
  qDebug() << m_pTcpData->toHex() << "before clear";
  m_pTcpData->clear();
  qDebug() << m_pTcpData->toHex() << "after clear" << m_pTcpData->size() << "size num";
  if (m_pSocket->waitForBytesWritten(100))//��λΪ���� �ȴ�100���� �ж��Ƿ���ȷд�� д���򷵻�true
  {
    while (m_pSocket->waitForReadyRead(150))//�ȴ�150�����ʱ������ڼ��пɶ������򷵻�true��ʱ�򷵻�false
    {
      m_pTcpData->append(m_pSocket->readAll());
    }//��ȡ���
    qDebug() << m_pTcpData->toHex() << "get order msg";
    qDebug() << m_pTcpData->size() << "get order msg size";
    if (!crcCheck)//��������ò���ָ���򲻶Է���ֵ����crccheck
    {
      return (crc16ModbusCheck(m_pTcpData, m_pTcpData->size()));//�õ������ݽ���crcУ��
    }
    else
    {
      return true;
    }
		//qDebug() << m_pTcpData->toHex()<<"hanshu";
  }
  else
  {
    return false;//д��ʧ��
  }
}

bool jyTcpLink::readAndCheckData(unsigned char * csendmsg, int iNum, bool crcCheck)
{
  if (m_ideepNum > 10)//�ݹ����
  {
    m_ideepNum = 0;//����
    return false;
  }
  //�ݹ����
  m_ideepNum++;
  //����д��
  writeData(csendmsg, iNum);
  //��ȡ���ݲ��ҽ���crcУ��
  if (getPortData(crcCheck))
  {
    //д����������������ݴ���
    if (!crcCheck)
    {
      qDebug() << m_pTcpDataHandle->toHex() << "before clear handle";
      m_pTcpDataHandle->clear();
      qDebug() << m_pTcpDataHandle->toHex() << "after clear handle" << m_pTcpDataHandle->size() << "size num handle";

      for (int i = 3; i < m_pTcpData->size() - 2; ++i)
      {
        m_pTcpDataHandle->append(m_pTcpData->at(i));
      }
      qDebug() << m_pTcpDataHandle->toHex() << "get data handle";
      qDebug() << m_pTcpDataHandle->size() << "get data size";
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
  return false;
}

QTcpSocket * jyTcpLink::returnTcpLink()
{
  return m_pSocket;
}

QByteArray * jyTcpLink::returnTcpDataSp()
{
  return m_pTcpDataHandle;
}

QByteArray * jyTcpLink::returnTcpDataSpOrigin()
{
  return m_pTcpData;
}
