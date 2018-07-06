
#include "QtWin/RollingCheck.h"

jyRollingCheck::jyRollingCheck(QWidget *parent)
  :QMainWindow(parent)
{
  ui.setupUi(this);
  //��ʼ���ϴ�һ����
  initLastParam();
  //�󶨴��ڴ򿪰�ť
  connect(ui.pushButton_SerialPort, &QPushButton::clicked, this, &jyRollingCheck::slotOpenPort);
  //��ip���Ӱ�ť
  connect(ui.pushButton_IPLink, &QPushButton::clicked, this, &jyRollingCheck::slotIpLink);
  //�󶨳�ʼ������
  connect(ui.pushButton_GetParam, &QPushButton::clicked, this, &jyRollingCheck::slotInitParam);
  //�����ò�����ť
  connect(ui.pushButton_SetParam, &QPushButton::clicked, this, &jyRollingCheck::slotSetParam);
  //�󶨸�λ��ť
  connect(ui.pushButton_ReSet, &QPushButton::clicked, this, &jyRollingCheck::slotReSet);
  //���趨��ַ��ť
  connect(ui.pushButton_SetAddr, &QPushButton::clicked, this, &jyRollingCheck::slotSetAddress);
  //�󶨻�ȡ���ַ����LineEdit���ݸ���ʱ�ͻ�ȡ
	connect(ui.lineEdit_WatchAddr, &QLineEdit::textChanged, this, &jyRollingCheck::slotGetWatchAddr);
}
//���ò�����ť
void jyRollingCheck::slotSetParam()
{
  if (CheckLink())//������֤��û������ û�������򷵻�true
  {
    return;
  }
  m_bSendOrder = new QByteArray;
  m_bSendOrder->clear();//���һ��ָ��5
//ͨ��������ʽ����������֤��������ʽ��һ����ȷ
  if (!checkData(ui.lineEdit_Set2->text(), unsignedfloatPrecision_0))
  {
    QMessageBox::about(this, "message", "please check data correct");
    return;
  }
  if (!checkData(ui.lineEdit_Set1->text(), unsignedfloatPrecision_0))
  {
    QMessageBox::about(this, "message", "please check data correct");
    return;
  }
  if (!checkData(ui.lineEdit_Out2->text(), except_0_floatPrecision_2))
  {
    QMessageBox::about(this, "message", "please check data correct");
    return;
  }
  if (!checkData(ui.lineEdit_Out1->text(), floatPrecision_2))
  {
    QMessageBox::about(this, "message", "please check data correct");
    return;
  }
  if (!checkData(ui.lineEdit_PSCL->text(), floatPrecision_4))
  {
    QMessageBox::about(this, "message", "please check data correct");
    return;
  }
  if (!checkData(ui.lineEdit_HLOP->text(), signfloatPrecision_0))
  {
    QMessageBox::about(this, "message", "please check data correct");
    return;
  }
  if (!checkData(ui.lineEdit_LLOP->text(), signfloatPrecision_0))
  {
    QMessageBox::about(this, "message", "please check data correct");
    return;
  }
//��ʼ��ָ��ͷ
  unsigned char _sendMsgHead[9] = { 1,16,0,0,0,14,28 };
  if (ui.lineEdit_WatchAddr->text() != "")
  {
    //�ѵ�һλ�趨Ϊ���ַ
    _sendMsgHead[0] = m_watchAddrNow;
  }
  else
  {
    QMessageBox::about(this, "message", "please enter watchAddrss");
    return;
  }
  //��ָ��ͷ���ӵ�ָ����
  for (int i = 0 ; i < 7 ; ++i)
  {
    m_bSendOrder->append(_sendMsgHead[i]);
  }
  //���Ҫ�趨�Ĳ���ǰ7�����Ǹ����ͣ�ȡ����
  unsigned char _floatToByteBuf[7][4];
  FloatToChar(ui.lineEdit_Set2->text().toFloat(), _floatToByteBuf[0]);
  FloatToChar(ui.lineEdit_Set1->text().toFloat(), _floatToByteBuf[1]);
  FloatToChar(ui.lineEdit_Out2->text().toFloat(), _floatToByteBuf[2]);
  FloatToChar(ui.lineEdit_Out1->text().toFloat(), _floatToByteBuf[3]);
  FloatToChar(ui.lineEdit_PSCL->text().toFloat(), _floatToByteBuf[4]);
  FloatToChar(ui.lineEdit_HLOP->text().toFloat(), _floatToByteBuf[5]);
  FloatToChar(ui.lineEdit_LLOP->text().toFloat(), _floatToByteBuf[6]);
  //��Ҫ���õ�ǰ7λ��������ָ����
  for (int j=0 ; j<7 ; ++j)
  {
    for (int i = 0 ; i < 4 ; ++i)
    {
      m_bSendOrder->append(_floatToByteBuf[j][i]);
    }
  }
  //��ΪcrcУ����Ҫunsigned char����������һ������ת��
  unsigned char *_ucSendMsg = (unsigned char*)(m_bSendOrder->data());
  //7λ+28λ���� λָ2λ16������ �õ��������������Ժ�д����λ��
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen() )
  {
    m_pCom->readAndCheckData(_ucSendMsg, 35, true);
  }
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    m_pTcpLink->readAndCheckData(_ucSendMsg, 35, true);
  }
	//��7������д����Ϊ�ֳ���ͬ���Է���������д
  m_bSendOrder->clear();
  //ͷΪ7���ֳ�14�ֽ�
  unsigned char _sendMsgHead16[9] = { 1,16,0,20,0,7,14 };
  _sendMsgHead16[0] = m_watchAddrNow;
  //д��ͷ
  for (int i = 0; i < 7 ; ++i)
  {
    m_bSendOrder->append(_sendMsgHead16[i]);
  }
  //д��Ϊint���� ָ���ʽΪ����0 1 0 1 0 1����һλ������8bit ��16bitΪһ������
  m_bSendOrder->append(inttoByte16(ui.comboBox_InputModel->currentIndex()));
	
  m_bSendOrder->append(inttoByte16(ui.comboBox_Output->currentIndex()));
	
  m_bSendOrder->append(inttoByte16(ui.comboBox_CountingRate->currentIndex()));
	
  m_bSendOrder->append(inttoByte16(ui.comboBox_Float->currentIndex()));
	
  m_bSendOrder->append(inttoByte16(ui.comboBox_TransmittingRate->currentIndex()));

  m_bSendOrder->append(inttoByte16(ui.comboBox_Input->currentIndex()));

  m_bSendOrder->append(inttoByte16(ui.comboBox_PowerMemory->currentIndex()));

  qDebug() << m_bSendOrder->toHex() << "this is write 16";
  //д��ָ��
  unsigned char *_ucSendMsg16 = (unsigned char*)(m_bSendOrder->data());
  //7λͷ+14λ����
  if(m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  { 
    m_pCom->readAndCheckData(_ucSendMsg16, 21, true);
  }
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    m_pTcpLink->readAndCheckData(_ucSendMsg16, 21, true);
  }
}
//��λ�ۺ���������ֵ��������û�о���Ч��
void jyRollingCheck::slotReSet()
{
  //������֤��û������ û�������򷵻�true
  if (CheckLink())
  {
    return;
  }
  //��λָ��
  unsigned char _reSetOrder[13] = { 1,16,0,28,0,1,2,0,1 };
  if (ui.lineEdit_WatchAddr->text() != "")
  {
    _reSetOrder[0] = m_watchAddrNow;
  }
  else
  {
    QMessageBox::about(this, "message", "please enter watchAddr");
    return;
  }
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  {
    m_pCom->readAndCheckData(_reSetOrder, 9, true);
  }
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    m_pTcpLink->readAndCheckData(_reSetOrder, 9, true);
  }
}
//���ñ��ַ�ۺ�������������ʹ��
void jyRollingCheck::slotSetAddress()
{
  //������֤��û������ û�������򷵻�true
  if (CheckLink())
  {
    return;
  }
  unsigned char _setAddrOrder[13] = { 1,16,0,30,0,1,2 };
  //��ȡ�¾ɱ��ַ
  if ((ui.lineEdit_WatchAddr->text()!="")&&(ui.lineEdit_ChangeWatchAddr->text()!=""))
  {
    _setAddrOrder[0] = m_watchAddrNow;
    m_watchAddrNew = ui.lineEdit_ChangeWatchAddr->text().toInt();
  }
  else
  {
    QMessageBox::about(this, "message", "please enter address");
    return;
  }
  //��������ת�������ݴ���֮����ӵ�������
  QByteArray *_intbuf=new QByteArray;
  for (int i = 0; i < 7; ++i)
  {
    _intbuf->append(_setAddrOrder[i]);
  }
  _intbuf->append(inttoByte16(ui.lineEdit_ChangeWatchAddr->text().toInt()));

  unsigned char *_sendOrder = (unsigned char*)_intbuf->data();
  //����ָ��
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  {
    m_pCom->readAndCheckData(_sendOrder, 11, true);
  }
  
  if (m_pTcpLink != nullptr&&m_pTcpLink->returnTcpLink() != nullptr&&m_pTcpLink->returnTcpLink()->isOpen())
  {
    m_pTcpLink->readAndCheckData(_sendOrder, 11, true);
  }
}
void jyRollingCheck::slotGetWatchAddr(const QString & addr)
{
  qDebug() << addr;
  m_watchAddrNow = addr.toInt();
}
//�򿪶˿�
bool jyRollingCheck::slotOpenPort()
{
  //�����û��tcp����
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    QMessageBox::about(this, "message", "please close ip link");
    return false;
  }
  //�����û�д��������еĻ��ر��Լ����ٶ���
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  {
    m_pCom->closeCom();
    delete m_pCom;
    m_pCom = nullptr;
    QMessageBox::about(this, "message","close link success" );
    return true;
  }
  //ȡ���ں�
  m_iCurrentIndex = (ui.comboBox_SelectPort->currentText()).right(1).toInt();
  m_pCom = new jyComOpen;
  //ѡ�񴮿�
  m_pCom->addComInfo(m_iCurrentIndex);
  //��ʼ��������Ϣ
  m_pCom->initPort();
  //�򿪴���
  if (m_pCom->spOpen())
  {
    //������ڴ򿪳ɹ�����г�ʼ��
    slotInitParam();
    //������һ�����ӵ�������Ϣ
    QMessageBox::about(this, "message", "open success");
    QString _list = "SERIALPORTCONFIG/SerialPort";
    QMap<QString, QString> configinfo;
    configinfo.insert(_list, ui.comboBox_SelectPort->currentText());
    writeConfig(configinfo);
    return true;
  }
  else
  {
	  QMessageBox::about(this, "message", "open fail");
    delete m_pCom;
    m_pCom = nullptr;
	  return false;
  }
}

//��������ʼ��
void jyRollingCheck::initParam()
{
  //ȥ��ͷβ��ʵ������
  QByteArray *_pData;
  if (m_pCom!=nullptr && m_pCom->returnSp()!=nullptr && m_pCom->returnSp()->isOpen())
  {
    _pData = m_pCom->returnDataSp();
  }
  if (m_pTcpLink!=nullptr && m_pTcpLink->returnTcpLink()!=nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    _pData = m_pTcpLink->returnTcpDataSp();
  }
  qDebug() << _pData->toHex()<<"yuanshuju";
  //��Ŵ�����֮�������
  int _dataGroup[100];
  //�����������ݸ�ʽת��
  QByteArray _Data;
  //��ʾ��ǰ�ǵڼ�����ŵ�����
  int j = 0;
  //��ռ����ô�С
  _Data.clear();
  _Data.resize(2);
  //����1�ֳ���
  for (int i = 40; i < 53 ; ++j)
  {
    _Data[0] = _pData->at(i);
    _Data[1] = _pData->at(++i);
    ++i;
    qDebug() << _Data.toHex() << "_data";
    _dataGroup[j] = bytes16ToInt(_Data);
    qDebug() << _dataGroup[j];
  }
  //�����ò�����
  _Data[0] = _pData->at(58);
  _Data[1] = _pData->at(59);
  _dataGroup[j] = bytes16ToInt(_Data);
  //����Լ����ô�С
  _Data.clear();
  _Data.resize(4);
  unsigned char *_cData;
  //��Ÿ�������
  float _fDataGroup[8];
  //ͬ��������ݸ���
  j = 0;
  //��������
  for (int i = 0; i < 32 ; ++i)
  {
    _Data[0] = _pData->at(i);
    _Data[1] = _pData->at(++i);
    _Data[2] = _pData->at(++i);
    _Data[3] = _pData->at(++i);
    _cData = (unsigned char *)_Data.data();
    _fDataGroup[j] = BytesToFloat(_cData);
    ++j;
  }
  //��ʼ��ʼ������
  ui.lineEdit_Set2->setText(QString::number(_fDataGroup[0]));
  ui.lineEdit_Set1->setText(QString::number(_fDataGroup[1]));
  ui.lineEdit_Out2->setText(QString::number(_fDataGroup[2]));
  ui.lineEdit_Out1->setText(QString::number(_fDataGroup[3]));
  ui.lineEdit_PSCL->setText(QString::number(_fDataGroup[4]));
  ui.lineEdit_HLOP->setText(QString::number(_fDataGroup[5]));
  ui.lineEdit_LLOP->setText(QString::number(_fDataGroup[6]));
  ui.lineEdit_CheckNum->setText(QString::number(_fDataGroup[7]));
  ui.comboBox_InputModel->setCurrentIndex(_dataGroup[0]);
  ui.comboBox_Output->setCurrentIndex(_dataGroup[1]);
  ui.comboBox_CountingRate->setCurrentIndex(_dataGroup[2]);
  ui.comboBox_Float->setCurrentIndex(_dataGroup[3]);
  ui.comboBox_TransmittingRate->setCurrentIndex(_dataGroup[4]);
  ui.comboBox_Input->setCurrentIndex(_dataGroup[5]);
  ui.comboBox_PowerMemory->setCurrentIndex(_dataGroup[6]);
  ui.comboBox_BaudRate->setCurrentIndex(_dataGroup[7]);
  //���ԭʼ���ݱ������һ�λ�ȡ���������Ⱦ
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  {
    m_pCom->returnDataSpOrigin()->clear();
  }
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    m_pTcpLink->returnTcpDataSpOrigin()->clear();
  }
}
//��������Ƿ�������ӣ��������������Ϣ
bool jyRollingCheck::CheckLink()
{
  bool _ComFlag = (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen());
  bool _IPFlag = (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen());
  //������Ϊ0�򷵻�
  if (!(_ComFlag || _IPFlag))
  {
    QMessageBox::about(this, "message", "creat link");
    return true;
  }
  else
  {
    return false;
  }
}
//ͨ��������ʽ�����ò�����ʱ���������У��
bool jyRollingCheck::checkData(QString data,int flagModel)
{

  QRegExp Check;
  switch (flagModel)
  {
  case unsignedfloatPrecision_0: 
  {
    Check.setPattern("^[1-9]\\d{0,5}\\.\\d*|0\\.\\d*|[1-9]\\d{0,5}|0$");
    return Check.exactMatch(data);
    break;
  }
  case floatPrecision_2:
  {  
    Check.setPattern("^([1-9]\\d{0,1}\\.\\d{1,2})|(0\\.\\d\\d{0,1})|([1-9]\\d{0,1})$");
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
}
//����Ĭ������ͨ��д��ini�ļ�
void jyRollingCheck::writeConfig(const QMap<QString,QString> &configinfo)
{
  QSettings _configFile("config.ini", QSettings::IniFormat);
  QMap<QString, QString>::const_iterator i;
  for (i = configinfo.constBegin(); i != configinfo.constEnd(); ++i)
  {
    _configFile.setValue(i.key(), i.value());
  }
}
//��ȡ��һ�ε����ò�����ip��ַ�ȣ��Լ�ɨ����Կ����Ĵ��ڣ�û�������ļ�����Զ�����
void jyRollingCheck::initLastParam()
{
  //��һ�׶�ɨ������õĴ��ڳ�ʼ��combobox
  int _num = 0;
  //foreachͨ��Ԥ����ʵ�ֵ�һ�������Ǻ�һ��������ʵ���Զ������ڶ�������
  foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
  {
    ui.comboBox_SelectPort->addItem(info.portName());
    //��ʾ�м���
    ++_num;
  }
  if (!_num)
  {
    QMessageBox::about(this, "message", "0 com can use");
  }
  //�ڶ��׶ο�ʼ��ȡ������Ϣ������ini�ļ��򴴽�����д��Ĭ��ֵ
  qDebug() << QDir::currentPath();
  //QSetting�����ṩ������ƽ̨�ĳ־û���Ӧ�����ã�������Ϣ�ȵȣ��������ֹ��캯���ĳ�ʼ����������������config.ini�ļ���ᴴ��һ��config.ini
  QSettings *_initparam=new QSettings("config.ini", QSettings::IniFormat);
//������������������������д��
  if (!(_initparam->contains("IPCONFIG")&&_initparam->contains("SERIALPORTCONFIG")))
  {
    _initparam->setValue("IPCONFIG/IPAddress", "192.168.1.254");
    _initparam->setValue("IPCONFIG/IPPort", "30000");
    _initparam->setValue("SERIALPORTCONFIG/SerialPort", "COM3");
    _initparam->setValue("SERIALPORTCONFIG/WatchAddress", "1");
  }
  //ǿ������ļ���������һ����������֮��Ŵ����ļ�
  _initparam->sync();
  //����ini�ļ��ṹ������[section]
  QStringList _paramList = _initparam->childGroups();
  QString _param="";
  foreach(QString _paramGroup, _paramList)
  {
    //����Ĭ��·��������Ϊ[section]��ʾ��section�µļ�ֵ�ԡ�
    _initparam->beginGroup(_paramGroup);
    //ȡ���м�
    QStringList _paramKeyList = _initparam->childKeys();

    foreach(QString _paramKeys, _paramKeyList)
    {
      //ȡֵ
      _param += tr(_initparam->value(_paramKeys).toString().toStdString().c_str())+" ";
    }
    //����ָ��
    _initparam->endGroup();
  }
  _param.chop(1);
  //�и�ȡֵ
  QStringList _paramEnd = _param.split(" ");
  //��ʼ��ui
  ui.jyipLineEdit_IPLine->initIPLine(_paramEnd[0]);
  ui.lineEdit_PortNum->setText(_paramEnd[1]);
  for (int i = 0; i < _num; ++i )
  {
   if( ui.comboBox_SelectPort->currentData(i).toString()==_paramEnd[2])
   {
     ui.comboBox_SelectPort->setCurrentIndex(i);
   }
  }
  ui.lineEdit_WatchAddr->setText(_paramEnd[3]);
}

void jyRollingCheck::slotChangeIndex(int i)
{
  //���Ĵ���ComboBox���ѡ��
  m_iCurrentIndex = i + 1;
}
//ip����
void jyRollingCheck::slotIpLink()
{
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  {
    QMessageBox::about(this, "message", "please close com open");
    return;
  }
  //�����������ر����Ӳ��ҷ���
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())//�����������ر�ԭ������ ��δ����
  {
    m_pTcpLink->closeLink();
    delete m_pTcpLink;
    m_pTcpLink = nullptr;
    QMessageBox::about(this, "message", "close link success");
    return ;
  }
  /*ȡ��ip��ַ*/
  QString _ipAddr="";
  _ipAddr = ui.jyipLineEdit_IPLine->returnIP();
  if (!(ui.jyipLineEdit_IPLine->CheckIPAddr(_ipAddr)))
  {
    QMessageBox::about(this, "message", "IP error");
    return;
  }
  qDebug() << _ipAddr;
  //ʵ����TCP_LINK�����Ҵ�����������
  m_iIpPort = ui.lineEdit_PortNum->text().toInt();

  m_pTcpLink = new jyTcpLink(_ipAddr,m_iIpPort);

  m_watchAddrNow = ui.lineEdit_WatchAddr->text().toInt();
  qDebug() << m_watchAddrNow;
  if (m_pTcpLink->linkStart())
  {
    slotInitParam();
    QString _list = "/IPCONFIG/IPAddress";
    QString _list2 = "/IPCONFIG/IPPort";
    QMap<QString, QString> configinfo;
    configinfo.insert(_list, ui.jyipLineEdit_IPLine->returnIP());
    configinfo.insert(_list2, ui.lineEdit_PortNum->text());
    writeConfig(configinfo);
    QMessageBox::about(this, "message", "link success");
    return;
  }
  else
  {
    QMessageBox::about(this, "message", "link fail");
    delete m_pTcpLink;
    m_pTcpLink = nullptr;
    return;
  }
}
//ÿ���������¼���ַ�����ӿ�����ʱ����Ҫ���ַ��������ֻ��Ҫ�򿪴��ڲ����漰���ַ�����ַ�ھ��巢��ָ���ʱ����м�¼�͸�д
void jyRollingCheck::slotInitParam()
{
  unsigned char sendbuf_r[9] = { 1,3,0,0,0,33 };
  if (ui.lineEdit_WatchAddr->text() != "")
  {
    sendbuf_r[0] = m_watchAddrNow;
  }
  else
  {
    QMessageBox::about(this, "message", "please enter watchAddr");
    return;
  }
  //�ж϶�ȡ���ݹ������Ƿ������⣬�Դ˾���Ҫ��Ҫ��ʼ��
  bool _initFlag = false;
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  {
    _initFlag = m_pCom->readAndCheckData(sendbuf_r, 6 , false);
  }
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    _initFlag = m_pTcpLink->readAndCheckData(sendbuf_r, 6 , false);
  }
  if (_initFlag)
  {
    initParam();
  }
  else
  {
    QMessageBox::about(this, "error", "get data error");
  }
}


