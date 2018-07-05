#ifndef _DATAHANDLE_H
#define _DATAHANDLE_H
#include<QtCore/QString>
#include<QtCore/QDebug>

unsigned int crc16Modbus(unsigned char *pData, unsigned int iNum);//crc������
bool crc16ModbusCheck(QByteArray *pData, int iNum);//��������ʱ����
int bytesToInt(QByteArray &bytes);
int bytes16ToInt(QByteArray &bytes);
float BytesToFloat(unsigned char *pch);
void FloatToChar(float Fvalue, unsigned char *pch);
QByteArray intToByte(int i);
QByteArray inttoByte16(int i);

#endif // !_DATA_HANDLE_H
