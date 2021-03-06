# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/seeing/workspace/SEEING_MCU_ESW.framework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/seeing/workspace/SEEING_MCU_ESW.framework/Build

# Include any dependencies generated for this target.
include Source/Driver/Gpio/CMakeFiles/gpio.dir/depend.make

# Include the progress variables for this target.
include Source/Driver/Gpio/CMakeFiles/gpio.dir/progress.make

# Include the compile flags for this target's objects.
include Source/Driver/Gpio/CMakeFiles/gpio.dir/flags.make

Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj: Source/Driver/Gpio/CMakeFiles/gpio.dir/flags.make
Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj: ../Source/Driver/Gpio/Gpio.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/seeing/workspace/SEEING_MCU_ESW.framework/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj"
	cd /home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Driver/Gpio && /home/seeing/stm32-toolchain/gcc-arm/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gpio.dir/Gpio.c.obj   -c /home/seeing/workspace/SEEING_MCU_ESW.framework/Source/Driver/Gpio/Gpio.c

Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gpio.dir/Gpio.c.i"
	cd /home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Driver/Gpio && /home/seeing/stm32-toolchain/gcc-arm/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/seeing/workspace/SEEING_MCU_ESW.framework/Source/Driver/Gpio/Gpio.c > CMakeFiles/gpio.dir/Gpio.c.i

Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gpio.dir/Gpio.c.s"
	cd /home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Driver/Gpio && /home/seeing/stm32-toolchain/gcc-arm/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/seeing/workspace/SEEING_MCU_ESW.framework/Source/Driver/Gpio/Gpio.c -o CMakeFiles/gpio.dir/Gpio.c.s

Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj.requires:

.PHONY : Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj.requires

Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj.provides: Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj.requires
	$(MAKE) -f Source/Driver/Gpio/CMakeFiles/gpio.dir/build.make Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj.provides.build
.PHONY : Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj.provides

Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj.provides.build: Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj


# Object files for target gpio
gpio_OBJECTS = \
"CMakeFiles/gpio.dir/Gpio.c.obj"

# External object files for target gpio
gpio_EXTERNAL_OBJECTS =

Source/Driver/Gpio/libgpio.a: Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj
Source/Driver/Gpio/libgpio.a: Source/Driver/Gpio/CMakeFiles/gpio.dir/build.make
Source/Driver/Gpio/libgpio.a: Source/Driver/Gpio/CMakeFiles/gpio.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/seeing/workspace/SEEING_MCU_ESW.framework/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libgpio.a"
	cd /home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Driver/Gpio && $(CMAKE_COMMAND) -P CMakeFiles/gpio.dir/cmake_clean_target.cmake
	cd /home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Driver/Gpio && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gpio.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Source/Driver/Gpio/CMakeFiles/gpio.dir/build: Source/Driver/Gpio/libgpio.a

.PHONY : Source/Driver/Gpio/CMakeFiles/gpio.dir/build

Source/Driver/Gpio/CMakeFiles/gpio.dir/requires: Source/Driver/Gpio/CMakeFiles/gpio.dir/Gpio.c.obj.requires

.PHONY : Source/Driver/Gpio/CMakeFiles/gpio.dir/requires

Source/Driver/Gpio/CMakeFiles/gpio.dir/clean:
	cd /home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Driver/Gpio && $(CMAKE_COMMAND) -P CMakeFiles/gpio.dir/cmake_clean.cmake
.PHONY : Source/Driver/Gpio/CMakeFiles/gpio.dir/clean

Source/Driver/Gpio/CMakeFiles/gpio.dir/depend:
	cd /home/seeing/workspace/SEEING_MCU_ESW.framework/Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/seeing/workspace/SEEING_MCU_ESW.framework /home/seeing/workspace/SEEING_MCU_ESW.framework/Source/Driver/Gpio /home/seeing/workspace/SEEING_MCU_ESW.framework/Build /home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Driver/Gpio /home/seeing/workspace/SEEING_MCU_ESW.framework/Build/Source/Driver/Gpio/CMakeFiles/gpio.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Source/Driver/Gpio/CMakeFiles/gpio.dir/depend

