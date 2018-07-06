FILE(GLOB GTEST_HEADER_FILES ${PROJECT_SOURCE_DIR}/include/GTest/*.h)
FILE(GLOB GTEST_CPP_FILES *.cpp)
FILE(GLOB GTEST_LIB ${GTEST_LIB_PATH}/*.lib)
#FILE(GLOB TEST_CPP ${PROJECT_SOURCE_DIR}/src/QtWin/RollingCheck.cpp)
MESSAGE(STATUS ${GTEST_LIB})
add_executable(GTest ${GTEST_CPP_FILES})
#-----------------------
#if(CMAKE_BUILD_TYPE MATCHES "Release" )
#FILE(GLOB GTEST_LIB ${GTEST_LIB_PATH}/release)
#elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
#FILE(GLOB GTEST_LIB ${GTEST_LIB_PATH}/debug)
#endif()
#---------------------如何链接debug版本
target_link_libraries(GTest DataHandleLib ${GTEST_LIB} QtLib)