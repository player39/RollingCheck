#include "QtWin/IPLineEdit.h"

jyipLineEdit::jyipLineEdit(QObject *parent)
{
  m_pHLayout = new QHBoxLayout(this);
  m_pHLayout->setSpacing(10);
  m_pHLayout->setContentsMargins(2, 2, 2, 2);
  for (int i = 0; i < 4; i++)
  {
    m_pLine[i] = new QLineEdit(this);
    m_pLine[i]->setFrame(false);
    m_pLine[i]->setMaxLength(3);
    m_pLine[i]->setAlignment(Qt::AlignCenter);
    m_pLine[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_pHLayout->addWidget(m_pLine[i]);
  }
  this->setReadOnly(true);
  this->setFrame(false);
}

QString jyipLineEdit::returnIP()
{
  QString IPAddr = "";
  for(int i=0;i<4;i++)
  {
    IPAddr.append(m_pLine[i]->text());
    IPAddr.append(".");
  }
  IPAddr.chop(1);
  return IPAddr;
}

bool jyipLineEdit::CheckIPAddr(QString IPAddr)
{
  QString _pattern = "((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))";
  QRegExp _CheckIP(_pattern);
  return _CheckIP.exactMatch(IPAddr);
}

void jyipLineEdit::initIPLine(QString &IPAddr)
{
  QStringList _IPAddrGroup = IPAddr.split(".");
  for (int i = 0; i < 4; i++)
  {
    m_pLine[i]->setText(_IPAddrGroup[i]);
  }
}

void jyipLineEdit::paintEvent(QPaintEvent *event)
{
  __super::paintEvent(event);
  QPainter _painter(this);
  QBrush _brush;
  _brush.setColor(Qt::black);
  _brush.setStyle(Qt::BrushStyle::SolidPattern);
  _painter.setBrush(_brush);
  int _width = 0;
  for (int i = 0; i < 3; i++)
  {
    _width += m_pLine[i]->width() + (i == 0 ? 3 : 10);
    _painter.drawEllipse(_width, height() / 1.3, 1, 1);
  }
  QPainter _painterLine(this);
  _painterLine.setPen(QPen(QColor::fromRgb(142, 142, 142, 255)));
  _painterLine.drawLine(0, 0, this->width() - 1, 0);
  _painterLine.drawLine(0, 0, 0, this->height() - 1);
  _painterLine.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
  _painterLine.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1);
}