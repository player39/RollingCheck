#include "DataHandle\DataHandle.h"
#include "QtWin\RollingCheck.h"
#include<gtest\gtest.h>
#include<iostream>

//ȫ���¼������а���ִ��ǰ��ִ�к� ִ��
class RollingCheck_Test :public testing::Environment {

public:
  RollingCheck_Test(int argc, char **argv) :m_iargc(argc), m_pargv(argv)
  {
  }
  //ִ��ǰ
  virtual void SetUp()
  {
    m_pApp = new QApplication(m_iargc, m_pargv);
  }
  //ִ�к�
  virtual void TearDown()
  {
    m_pApp->exit();
    delete m_pApp;
    m_pApp = 0;
  }
  //ִ��ǰ����QApplication����
  QApplication *m_pApp;
  int m_iargc;
  char **m_pargv;
};
//����ȷ��������ȷ��ֵ���ᱨ��
TEST(RollingCheck, case1)
{
  
  float _testNum = 1;
  jyRollingCheck _testRollingCheck;
 
  for (int i = 0; i < 5; ++i)
  {

    EXPECT_TRUE(_testRollingCheck.checkData(QString("%1").arg(_testNum), 0)) << "false_num:" << i;
    _testNum = _testNum * 10;
    std::cout<<_testNum<<"\n";
  }

  QString _QStringFloat = "0.1";
  for (int i = 0; i < 8; ++i)
  {
    EXPECT_TRUE(_testRollingCheck.checkData(_QStringFloat.append("1"), 0)) << "false_num:" << i;
    _testNum = _testNum / 10;
  }
}

TEST(ComOpen, case2)
{
  jyComOpen _testCom;
  //�ֱ�Ϊ��λ�������ݣ���������2�ֳ� 1,16,0,30,0,1,2
  unsigned char _testOrder1[12] = { 1, 0x10, 0, 0x1c, 0, 1, 2, 0, 1 ,0x65,0xcc};
  unsigned char _testOrder2[9] = { 1, 3, 0, 0, 0, 0x21,0x85,0xd2 };
  unsigned char _testOrder3[25] = { 1, 16, 0, 0, 0, 4, 8, 0, 0, 0, 1, 0, 0, 0, 2, 0x0a, 0x7b };

  ASSERT_EQ(crc16Modbus(_testOrder1, 9), 52325)<<"crc_false_num_1";
  ASSERT_EQ(crc16Modbus(_testOrder2, 6), 53893)<< "crc_false_num_2";
  ASSERT_EQ(crc16Modbus(_testOrder3, 15), 2683)<< "crc_false_num_3";

  EXPECT_TRUE(_testCom.readAndCheckData(_testOrder1,11,true))<<"com_false_num_1";
  EXPECT_TRUE(_testCom.readAndCheckData(_testOrder1, 8, false)) << "com_false_num_2";
  EXPECT_TRUE(_testCom.readAndCheckData(_testOrder1, 17, true)) << "com_false_num_3";


}

int main(int argc, char* argv[])
{
  testing::AddGlobalTestEnvironment(new RollingCheck_Test(argc, argv));
  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
    while (1);
  return 0;
}
