CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(TEST_PROJECT_NAME testesw)

PROJECT(${TEST_PROJECT_NAME})

#PKG_CHECK_MODULES(GTEST REQUIRED gtest>=1.7.0)
#PKG_CHECK_MODULES(GMOCK REQUIRED gmock>=1.7.0)

#FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

ADD_DEFINITIONS(-DMY_TEST) 

SET(SEEING_MCU_ESW_DIR /home/seeing/workspace/SEEING_MCU_ESW.framework/Source)

SET(GTEST_INCLUDE_DIRS /home/seeing/googletest/googletest/include)
SET(GMOCK_INCLUDE_DIRS /home/seeing/googletest/googlemock/include)

SET(GTEST_LIBRARIES /home/seeing/googletest/googlemock/gtest/libgtest.a)
SET(GTEST_MAIN_LIBRARIES /home/seeing/googletest/googlemock/gtest/libgtest_main.a)
SET(GMOCK_LIBRARIES /home/seeing/googletest/googlemock/libgmock.a)
SET(GMOCK_MAIN_LIBRARIES /home/seeing/googletest/googlemock/libgmock_main.a)

INCLUDE_DIRECTORIES(
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
    ${SEEING_MCU_ESW_DIR}
    ${SEEING_MCU_ESW_DIR}/Global
)

AUX_SOURCE_DIRECTORY(. TEST_SOURCES)
AUX_SOURCE_DIRECTORY(Mocks MOCK_SOURCES)

AUX_SOURCE_DIRECTORY(${SEEING_MCU_ESW_DIR}/Application ESW_APPLICATION_SOURCES)
AUX_SOURCE_DIRECTORY(${SEEING_MCU_ESW_DIR}/Application/Universal ESW_APPLICATION_UNIVERSAL_SOURCES)
AUX_SOURCE_DIRECTORY(${SEEING_MCU_ESW_DIR}/Application/Network ESW_APPLICATION_NETWORK_SOURCES)
AUX_SOURCE_DIRECTORY(${SEEING_MCU_ESW_DIR}/Application/Network/Adapter ESW_APPLICATION_NETWORK_ADAPTER_SOURCES)
AUX_SOURCE_DIRECTORY(${SEEING_MCU_ESW_DIR}/Application/Network/Protocol ESW_APPLICATION_NETWORK_PROTOCOL_SOURCES)
AUX_SOURCE_DIRECTORY(${SEEING_MCU_ESW_DIR}/Application/Terminal ESW_APPLICATION_TERMINAL_SOURCES)
AUX_SOURCE_DIRECTORY(${SEEING_MCU_ESW_DIR}/Common/Structure ESW_COMMON_STRUCTURE_SOURCES)
AUX_SOURCE_DIRECTORY(${SEEING_MCU_ESW_DIR}/Common/Utilities ESW_COMMON_UTILITIES_SOURCES)

#set(ESW_APPLICATION_NETWORK_SOURCES 
 #   ${SEEING_MCU_ESW_DIR}/Application/Network/Network.c
#)

#file(GLOB ESW_APPLICATION_NETWORK_SOURCES ${SEEING_MCU_ESW_DIR}/Application/Network/*.c)

ADD_EXECUTABLE(${TEST_PROJECT_NAME} 
     ${ESW_APPLICATION_SOURCES}
    ${ESW_APPLICATION_UNIVERSAL_SOURCES}
    ${ESW_APPLICATION_NETWORK_SOURCES}
    ${ESW_APPLICATION_NETWORK_ADAPTER_SOURCES}
    ${ESW_APPLICATION_NETWORK_PROTOCOL_SOURCES}
    ${ESW_APPLICATION_TERMINAL_SOURCES}
    ${ESW_COMMON_STRUCTURE_SOURCES}
    ${ESW_COMMON_UTILITIES_SOURCES}
    ${MOCK_SOURCES}
    ${TEST_SOURCES}
 )

TARGET_LINK_LIBRARIES(${TEST_PROJECT_NAME} 
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    ${GMOCK_LIBRARIES}
    ${GMOCK_MAIN_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT} 
)
