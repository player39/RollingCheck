
#include "QtWin/RollingCheck.h"

jyRollingCheck::jyRollingCheck(QWidget *parent)
  :QMainWindow(parent)
{
  ui.setupUi(this);
  //初始化上次一配置
  initLastParam();
  //绑定串口打开按钮
  connect(ui.pushButton_SerialPort, &QPushButton::clicked, this, &jyRollingCheck::slotOpenPort);
  //绑定ip连接按钮
  connect(ui.pushButton_IPLink, &QPushButton::clicked, this, &jyRollingCheck::slotIpLink);
  //绑定初始化参数
  connect(ui.pushButton_GetParam, &QPushButton::clicked, this, &jyRollingCheck::slotInitParam);
  //绑定设置参数按钮
  connect(ui.pushButton_SetParam, &QPushButton::clicked, this, &jyRollingCheck::slotSetParam);
  //绑定复位按钮
  connect(ui.pushButton_ReSet, &QPushButton::clicked, this, &jyRollingCheck::slotReSet);
  //绑定设定地址按钮
  connect(ui.pushButton_SetAddr, &QPushButton::clicked, this, &jyRollingCheck::slotSetAddress);
  //绑定获取表地址，当LineEdit数据更改时就获取
	connect(ui.lineEdit_WatchAddr, &QLineEdit::textChanged, this, &jyRollingCheck::slotGetWatchAddr);
}
//设置参数按钮
void jyRollingCheck::slotSetParam()
{
  if (CheckLink())//函数验证有没有连接 没有连接则返回true
  {
    return;
  }
  m_bSendOrder = new QByteArray;
  m_bSendOrder->clear();//清空一下指令5
//通过正则表达式进行数据验证，正则表达式不一定正确
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
//初始化指令头
  unsigned char _sendMsgHead[9] = { 1,16,0,0,0,14,28 };
  if (ui.lineEdit_WatchAddr->text() != "")
  {
    //把第一位设定为表地址
    _sendMsgHead[0] = m_watchAddrNow;
  }
  else
  {
    QMessageBox::about(this, "message", "please enter watchAddrss");
    return;
  }
  //把指令头附加到指令中
  for (int i = 0 ; i < 7 ; ++i)
  {
    m_bSendOrder->append(_sendMsgHead[i]);
  }
  //存放要设定的参数前7个都是浮点型，取数据
  unsigned char _floatToByteBuf[7][4];
  FloatToChar(ui.lineEdit_Set2->text().toFloat(), _floatToByteBuf[0]);
  FloatToChar(ui.lineEdit_Set1->text().toFloat(), _floatToByteBuf[1]);
  FloatToChar(ui.lineEdit_Out2->text().toFloat(), _floatToByteBuf[2]);
  FloatToChar(ui.lineEdit_Out1->text().toFloat(), _floatToByteBuf[3]);
  FloatToChar(ui.lineEdit_PSCL->text().toFloat(), _floatToByteBuf[4]);
  FloatToChar(ui.lineEdit_HLOP->text().toFloat(), _floatToByteBuf[5]);
  FloatToChar(ui.lineEdit_LLOP->text().toFloat(), _floatToByteBuf[6]);
  //把要设置的前7位参数加入指令中
  for (int j=0 ; j<7 ; ++j)
  {
    for (int i = 0 ; i < 4 ; ++i)
    {
      m_bSendOrder->append(_floatToByteBuf[j][i]);
    }
  }
  //因为crc校验需要unsigned char类型所以做一次类型转换
  unsigned char *_ucSendMsg = (unsigned char*)(m_bSendOrder->data());
  //7位+28位数据 位指2位16进制数 拿到了完整的命令以后写入下位机
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen() )
  {
    m_pCom->readAndCheckData(_ucSendMsg, 35, true);
  }
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    m_pTcpLink->readAndCheckData(_ucSendMsg, 35, true);
  }
	//后7个数据写入因为字长不同所以分两个命令写
  m_bSendOrder->clear();
  //头为7个字长14字节
  unsigned char _sendMsgHead16[9] = { 1,16,0,20,0,7,14 };
  _sendMsgHead16[0] = m_watchAddrNow;
  //写入头
  for (int i = 0; i < 7 ; ++i)
  {
    m_bSendOrder->append(_sendMsgHead16[i]);
  }
  //写入为int类型 指令格式为数据0 1 0 1 0 1这里一位数代表8bit ，16bit为一组数据
  m_bSendOrder->append(inttoByte16(ui.comboBox_InputModel->currentIndex()));
	
  m_bSendOrder->append(inttoByte16(ui.comboBox_Output->currentIndex()));
	
  m_bSendOrder->append(inttoByte16(ui.comboBox_CountingRate->currentIndex()));
	
  m_bSendOrder->append(inttoByte16(ui.comboBox_Float->currentIndex()));
	
  m_bSendOrder->append(inttoByte16(ui.comboBox_TransmittingRate->currentIndex()));

  m_bSendOrder->append(inttoByte16(ui.comboBox_Input->currentIndex()));

  m_bSendOrder->append(inttoByte16(ui.comboBox_PowerMemory->currentIndex()));

  qDebug() << m_bSendOrder->toHex() << "this is write 16";
  //写入指令
  unsigned char *_ucSendMsg16 = (unsigned char*)(m_bSendOrder->data());
  //7位头+14位数据
  if(m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  { 
    m_pCom->readAndCheckData(_ucSendMsg16, 21, true);
  }
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    m_pTcpLink->readAndCheckData(_ucSendMsg16, 21, true);
  }
}
//复位槽函数，返回值正常但是没有具体效果
void jyRollingCheck::slotReSet()
{
  //函数验证有没有连接 没有连接则返回true
  if (CheckLink())
  {
    return;
  }
  //复位指令
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
//设置表地址槽函数，可以正常使用
void jyRollingCheck::slotSetAddress()
{
  //函数验证有没有连接 没有连接则返回true
  if (CheckLink())
  {
    return;
  }
  unsigned char _setAddrOrder[13] = { 1,16,0,30,0,1,2 };
  //获取新旧表地址
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
  //进行数据转换和数据处理之后添加到命令中
  QByteArray *_intbuf=new QByteArray;
  for (int i = 0; i < 7; ++i)
  {
    _intbuf->append(_setAddrOrder[i]);
  }
  _intbuf->append(inttoByte16(ui.lineEdit_ChangeWatchAddr->text().toInt()));

  unsigned char *_sendOrder = (unsigned char*)_intbuf->data();
  //发送指令
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
//打开端口
bool jyRollingCheck::slotOpenPort()
{
  //检测有没有tcp连接
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    QMessageBox::about(this, "message", "please close ip link");
    return false;
  }
  //检测有没有串口连接有的话关闭以及销毁对象
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  {
    m_pCom->closeCom();
    delete m_pCom;
    m_pCom = nullptr;
    QMessageBox::about(this, "message","close link success" );
    return true;
  }
  //取串口号
  m_iCurrentIndex = (ui.comboBox_SelectPort->currentText()).right(1).toInt();
  m_pCom = new jyComOpen;
  //选择串口
  m_pCom->addComInfo(m_iCurrentIndex);
  //初始化串口信息
  m_pCom->initPort();
  //打开串口
  if (m_pCom->spOpen())
  {
    //如果串口打开成功则进行初始化
    slotInitParam();
    //保存这一次连接的配置信息
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

//做参数初始化
void jyRollingCheck::initParam()
{
  //去掉头尾的实际数据
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
  //存放处理完之后的数据
  int _dataGroup[100];
  //用来进行数据格式转换
  QByteArray _Data;
  //表示当前是第几个存放的数据
  int j = 0;
  //清空加重置大小
  _Data.clear();
  _Data.resize(2);
  //处理1字长的
  for (int i = 40; i < 53 ; ++j)
  {
    _Data[0] = _pData->at(i);
    _Data[1] = _pData->at(++i);
    ++i;
    qDebug() << _Data.toHex() << "_data";
    _dataGroup[j] = bytes16ToInt(_Data);
    qDebug() << _dataGroup[j];
  }
  //单独拿波特率
  _Data[0] = _pData->at(58);
  _Data[1] = _pData->at(59);
  _dataGroup[j] = bytes16ToInt(_Data);
  //清空以及重置大小
  _Data.clear();
  _Data.resize(4);
  unsigned char *_cData;
  //存放浮点数据
  float _fDataGroup[8];
  //同样标记数据个数
  j = 0;
  //处理浮点数
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
  //开始初始化数据
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
  //清空原始数据避免对下一次获取数据造成污染
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  {
    m_pCom->returnDataSpOrigin()->clear();
  }
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())
  {
    m_pTcpLink->returnTcpDataSpOrigin()->clear();
  }
}
//用来检查是否存在连接，用于输出错误信息
bool jyRollingCheck::CheckLink()
{
  bool _ComFlag = (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen());
  bool _IPFlag = (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen());
  //两个都为0则返回
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
//通过正则表达式在设置参数的时候进行数据校验
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
//保存默认配置通过写入ini文件
void jyRollingCheck::writeConfig(const QMap<QString,QString> &configinfo)
{
  QSettings _configFile("config.ini", QSettings::IniFormat);
  QMap<QString, QString>::const_iterator i;
  for (i = configinfo.constBegin(); i != configinfo.constEnd(); ++i)
  {
    _configFile.setValue(i.key(), i.value());
  }
}
//读取上一次的配置参数如ip地址等，以及扫描可以开启的串口，没有配置文件则会自动创建
void jyRollingCheck::initLastParam()
{
  //第一阶段扫描可以用的串口初始化combobox
  int _num = 0;
  //foreach通过预处理实现第一个参数是后一个参数的实例自动遍历第二个参数
  foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
  {
    ui.comboBox_SelectPort->addItem(info.portName());
    //表示有几个
    ++_num;
  }
  if (!_num)
  {
    QMessageBox::about(this, "message", "0 com can use");
  }
  //第二阶段开始读取配置信息，若无ini文件则创建并且写入默认值
  qDebug() << QDir::currentPath();
  //QSetting用于提供独立于平台的持久化的应用设置，配置信息等等，采用这种构造函数的初始化方法，若不存在config.ini文件则会创建一个config.ini
  QSettings *_initparam=new QSettings("config.ini", QSettings::IniFormat);
//如果不存在这两条配置则进行写入
  if (!(_initparam->contains("IPCONFIG")&&_initparam->contains("SERIALPORTCONFIG")))
  {
    _initparam->setValue("IPCONFIG/IPAddress", "192.168.1.254");
    _initparam->setValue("IPCONFIG/IPPort", "30000");
    _initparam->setValue("SERIALPORTCONFIG/SerialPort", "COM3");
    _initparam->setValue("SERIALPORTCONFIG/WatchAddress", "1");
  }
  //强制输出文件，不加这一条会在析构之后才创建文件
  _initparam->sync();
  //返回ini文件结构中所有[section]
  QStringList _paramList = _initparam->childGroups();
  QString _param="";
  foreach(QString _paramGroup, _paramList)
  {
    //设置默认路径，参数为[section]表示在section下的键值对。
    _initparam->beginGroup(_paramGroup);
    //取所有键
    QStringList _paramKeyList = _initparam->childKeys();

    foreach(QString _paramKeys, _paramKeyList)
    {
      //取值
      _param += tr(_initparam->value(_paramKeys).toString().toStdString().c_str())+" ";
    }
    //重置指针
    _initparam->endGroup();
  }
  _param.chop(1);
  //切割取值
  QStringList _paramEnd = _param.split(" ");
  //初始化ui
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
  //更改串口ComboBox里的选项
  m_iCurrentIndex = i + 1;
}
//ip连接
void jyRollingCheck::slotIpLink()
{
  if (m_pCom != nullptr && m_pCom->returnSp() != nullptr && m_pCom->returnSp()->isOpen())
  {
    QMessageBox::about(this, "message", "please close com open");
    return;
  }
  //如果有连接则关闭连接并且返回
  if (m_pTcpLink != nullptr && m_pTcpLink->returnTcpLink() != nullptr && m_pTcpLink->returnTcpLink()->isOpen())//如果有连接则关闭原有连接 还未测试
  {
    m_pTcpLink->closeLink();
    delete m_pTcpLink;
    m_pTcpLink = nullptr;
    QMessageBox::about(this, "message", "close link success");
    return ;
  }
  /*取得ip地址*/
  QString _ipAddr="";
  _ipAddr = ui.jyipLineEdit_IPLine->returnIP();
  if (!(ui.jyipLineEdit_IPLine->CheckIPAddr(_ipAddr)))
  {
    QMessageBox::about(this, "message", "IP error");
    return;
  }
  qDebug() << _ipAddr;
  //实例化TCP_LINK对象并且传入两个参数
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
//每次在这里记录表地址，连接开启的时候不需要表地址，物理上只需要打开串口并不涉及表地址，表地址在具体发送指令的时候进行记录和改写
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
  //判断读取数据过程中是否有问题，以此决定要不要初始化
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


