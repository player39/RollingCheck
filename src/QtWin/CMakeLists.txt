
FILE(GLOB UI_FILES ${PROJECT_SOURCE_DIR}/qtui/RollingCheck.ui)
FILE(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/include/QtWin/*.h)
FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB QRC_FILE_Rolling ${PROJECT_SOURCE_DIR}/resources/Rolling_Check.qrc)
qt5_wrap_ui(WRAP_FILES ${UI_FILES})
#将.ui文件编译成.h文件
qt5_add_resources(RCC ${QRC_FILE_Rolling})
#将qrc资源文件编译
add_executable(QtWin ${HEADER_FILES} ${SOURCE_FILES} ${WRAP_FILES} ${QRC_FILE_Rolling})#查一下这个命令具体效果，没有包含Check_Rolling.h链接的时候会报错
source_group(uis FILES ${UI_FILES})#将文件整理到一个文件夹
source_group(qrcs FILES ${QRC_FILE_Rolling})
source_group("Generated Files" FILES ${RCC} ${WRAP_FILES})
target_link_libraries(QtWin DataHandleLib)
install(TARGETS QtWin
       RUNTIME DESTINATION bin)

#名称和项目对应