FILE(GLOB CPP_FILES *.cpp)
FILE(GLOB HEAD_FILES ${PROJECT_SOURCE_DIR}/include/ServerTest/*.h)
FILE(GLOB UI_FILE_TEST ${PROJECT_SOURCE_DIR}/qtui/server.ui)
FILE(GLOB QRC_FILE_TEST "resources/server.qrc")
qt5_wrap_ui(WRAP_UI ${UI_FILE_TEST})
qt5_add_resources(RCC_TEST ${QRC_FILE_TEST})
add_executable(ServerTest ${CPP_FILES} ${HEAD_FILES} ${WRAP_UI} ${QRC_FILE_TEST})
source_group(uis FILES ${UI_FILE_TEST})
source_group(qrcs FILES ${QRC_FILE_TEST})
source_group("Generated Files" FILES ${UI_FILE_TEST} ${QRC_FILE_TEST})
target_link_libraries(ServerTest Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network Qt5::SerialPort)

install(TARGETS ServerTest
        RUNTIME DESTINATION bin
)