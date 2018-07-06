#include "NetWork/Tcplink.h"

jyTcpLink::jyTcpLink(const QString& sipAddress,const int& iport,QObject *parent)
{
  //初始化连接ip
  m_sIPAddress = sipAddress;
  //初始化连接端口
  m_iHostPort = iport;
  m_pTcpData = new QByteArray;
  m_pTcpDataHandle = new QByteArray;
  qDebug() << m_iHostPort;
}
//开启连接
bool jyTcpLink::linkStart()
{
  m_pSocket = new QTcpSocket();
  m_pSocket->connectToHost(m_sIPAddress, m_iHostPort);
  //测试连接是否成功
  if (!m_pSocket->waitForConnected(30000))
  {
    return false;
  }
  else 
  {
    //指令代码
    uchar sendbuf_r[9] = { 1,3,0,0,0,33 };
    readAndCheckData(sendbuf_r, 6, false);
    return true;
  }
}
//关闭连接
bool jyTcpLink::closeLink()
{
  m_pSocket->close();
  return !m_pSocket->isOpen();
}
//发送指令
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
  unsigned int _icrcCode = 0;//局部变量自动清空
  _icrcCode = crc16Modbus(Order, OrderNum);//crc校验
  qDebug() << _icrcCode << "crcode";
  QByteArray _sendMsg;//局部变量自动清空
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
  if (m_pSocket->waitForBytesWritten(100))//单位为毫秒 等待100毫秒 判断是否正确写入 写入则返回true
  {
    while (m_pSocket->waitForReadyRead(150))//等待150毫秒的时间如果期间有可读数据则返回true超时则返回false
    {
      m_pTcpData->append(m_pSocket->readAll());
    }//读取完毕
    qDebug() << m_pTcpData->toHex() << "get order msg";
    qDebug() << m_pTcpData->size() << "get order msg size";
    if (!crcCheck)//如果是设置参数指令则不对返回值进行crccheck
    {
      return (crc16ModbusCheck(m_pTcpData, m_pTcpData->size()));//得到的数据进行crc校验
    }
    else
    {
      return true;
    }
		//qDebug() << m_pTcpData->toHex()<<"hanshu";
  }
  else
  {
    return false;//写入失败
  }
}

bool jyTcpLink::readAndCheckData(unsigned char * csendmsg, int iNum, bool crcCheck)
{
  if (m_ideepNum > 10)//递归出口
  {
    m_ideepNum = 0;//清零
    return false;
  }
  //递归深度
  m_ideepNum++;
  //数据写入
  writeData(csendmsg, iNum);
  //获取数据并且进行crc校验
  if (getPortData(crcCheck))
  {
    //写入命令都不做返回数据处理
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
      //清零
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
