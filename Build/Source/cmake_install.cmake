# Install script for directory: /home/seeing/workspace/SEEING_MCU_ESW.framework/Source

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Application/cmake_install.cmake")
  include("/home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Common/cmake_install.cmake")
  include("/home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Driver/cmake_install.cmake")
  include("/home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Middleware/cmake_install.cmake")
  include("/home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Platform/cmake_install.cmake")
  include("/home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/System/cmake_install.cmake")

endif()

