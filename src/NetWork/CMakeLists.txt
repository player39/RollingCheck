SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
#设置lib文件生成路径
#add_library(m_comopen STATIC ${COM_FILE})
FILE(GLOB NET_CPP  *.cpp)
FILE(GLOB NET_HEAD  ${PROJECT_SOURCE_DIR}/include/NetWork/*.h )
#FILE(GLOB GTEST )
#拿到头文件和源文件
#add_executable(NetWork ${NET_CPP} ${NET_HEAD})
add_library(LinkLib STATIC ${NET_CPP} ${NET_HEAD})
target_link_libraries(LinkLib Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network Qt5::SerialPort)
#install(FILES ${NET_HEAD} DESTINATION src/NetWork/include)
#install(FILES ${NET_CPP} DESTINATION src/NetWork/src)
