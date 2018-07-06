#include "DataHandle\DataHandle.h"


//����crc��
unsigned int crc16Modbus(unsigned char * pData , unsigned int iNum)
{
  unsigned char i, j;
  unsigned int wCrc = 0xFFFF;
  for (i = 0; i<iNum; i++)
  {
    wCrc ^= (unsigned int)(pData[i]);
    for (j = 0; j<8; j++)
    {
      if (wCrc & 1) { wCrc >>= 1; wCrc ^= 0xA001; }
      else
        wCrc >>= 1;
    }
  }
  union { unsigned int crcbuffer; unsigned char crccode[2]; }crc;//�������干���ڴ�ȡ�øߵ�λ
  crc.crcbuffer = wCrc;
	//qDebug() << crc.crccode[0] << crc.crccode[1];
  pData[iNum] = crc.crccode[0];
  pData[i + 1] = crc.crccode[1];
  return wCrc;
}
//���crc��
bool crc16ModbusCheck(QByteArray * pData , int iNum)
{
  if (iNum > 2)
  {
    unsigned char *_crcCheckData = (unsigned char*)pData->data();//����ת�� ��Ϊ֮��Ҫ����crcУ������Ҫ��������ƥ��
    unsigned int _crcCode = 0;
    _crcCode = crc16Modbus(_crcCheckData, iNum - 2);//crcУ�鳤��-2ȥ������֡������λcrc��
    union { unsigned int _ipDataCrc; unsigned char _cpDataCrc[2]; }crc;//ͨ�������干���ڴ�õ�ԭ���ݷ��͹�����crc��
    crc._ipDataCrc = 0;//4�ֽڿռ�����
    crc._cpDataCrc[1] = _crcCheckData[iNum - 1];
    crc._cpDataCrc[0] = _crcCheckData[iNum - 2];
    return (_crcCode == crc._ipDataCrc);//���жԱ�
  }
  return false;
}
//32bitת��
int bytesToInt(QByteArray &bytes)
{
  int result = bytes.at(3)&0x000000FF;
  qDebug() << bytes.toHex() << "bytes";
  result |= ((bytes.at(2) << 8) & 0x0000FF00);
  result |= ((bytes.at(1) << 16) & 0x00FF0000);
  result |= ((bytes.at(0) << 24) & 0xFF000000);
  return result;
}
//16bit
int bytes16ToInt(QByteArray & bytes)
{
  int result = bytes.at(1) & 0x000000FF;
  qDebug() << bytes.toHex() << "bytes";
  result |= ((bytes.at(0) << 8) & 0x0000FF00);
  return result;
}

float BytesToFloat(unsigned char * pch)
{
  float result;
  unsigned char *p;
  p = (unsigned char *)& result;
  *p = *pch; *(p + 1) = *(pch + 1); *(p + 2) = *(pch + 2); *(p + 3) = *(pch + 3);
  qDebug() << result << "  result";
  return result;
}

void FloatToChar(float Fvalue, unsigned char * pch)
{
  unsigned char *p;
  p = (unsigned char*)& Fvalue;
  *pch = *p;  
  *(pch + 1) = *(p + 1);  *(pch + 2) = *(p + 2);  *(pch + 3) = *(p + 3);
}

QByteArray intToByte(int i)
{
  QByteArray abyte0;
  abyte0.resize(4);
  abyte0[3] = (uchar)(0x000000ff & i);
  abyte0[2] = (uchar)((0x0000ff00 & i) >> 8);
  abyte0[1] = (uchar)((0x00ff0000 & i) >> 16);
  abyte0[0] = (uchar)((0xff000000 & i) >> 24);
  return abyte0;
}

QByteArray inttoByte16(int i)//���������1�ֳ�����
{
  QByteArray abyte0;
  abyte0.resize(2);
  abyte0[1] = (uchar)(0x000000ff & i);
  abyte0[0] = (uchar)((0x0000ff00 & i) >> 8);
  return abyte0;
}

/*bool checkData(QString data, int flagModel)
{

  QRegExp Check;
  switch (flagModel)
  {
  case unsignedfloatPrecision_0:
  {
    Check.setPattern("^[1-9]\\d*\\.\\d*|0\\.\\d*[1-9]\\d*|-[1-9]\\d{0,4}|[1-9]\\d{0,5}|0$");
    return Check.exactMatch(data);
    break;
  }
  case floatPrecision_2:
  {
    Check.setPattern("^([1-9]\\d{0,1}\\.\\d{1,2})|(0\\.\\d\\d{0,1})|([1-9]\\d{0,1})|0$");
    return Check.exactMatch(data);
    break;
  }
  case except_0_floatPrecision_2:
  {
    Check.setPattern("^([1-9]\\d{0,1}\\.\\d{1,2})|(0\\.\\d[1-9]{0,1})|([1-9]\\d{0,1})$");
    return Check.exactMatch(data);
    break;
  }
  case floatPrecision_4:
  {
    Check.setPattern("^[1-9]\\d{0,1}\\.\\d*|0\\.\\d*[1-9]\\d*|[1-9]\\d|[1-9]\\d{0,1}$");
    return Check.exactMatch(data);
    break;
  }
  case signfloatPrecision_0:
  {
    Check.setPattern("^-{0,1}[1-9]\\d*\\.\\d*|-0\\.\\d*[1-9]\\d*|-[1-9]\\d{0,4}|[1-9]\\d{0,5}$");
    return Check.exactMatch(data);
    break;
  }
  }
  return false;
}*/


