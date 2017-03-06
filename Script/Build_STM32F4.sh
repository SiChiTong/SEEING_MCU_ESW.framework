#!/bin/sh
cmake_build_type="Debug"
cmake_toolchain_file="/home/seeing/stm32-toolchain/cmake/stm32-cmake-stdperiph/gcc_stm32.cmake"
cmake_module_path="/home/seeing/stm32-toolchain/cmake/stm32-cmake-stdperiph/cmake/Modules"
toolchain_prefix="/home/seeing/stm32-toolchain/gcc-arm/gcc-arm-none-eabi-4_9-2015q3"
project_dir="/home/seeing/workspace/SEEING_MCU_ESW.framework"
stm32_startup_prefix="$project_dir/Source/Platform/Stm32f407/Startup/startup_stm32f"
stm32_cmsis_sources="$project_dir/Source/Platform/Stm32f407/Startup/system_stm32f4xx.c"
stm32_cmsis_linker_script="$project_dir/Source/Platform/Stm32f407/Startup/stm32f4flash.ld"
stm32_chip="STM32F407VG"
stm32_chip_type="40_41xxx"
stm32_cmake_path="/home/seeing/stm32-toolchain/cmake/stm32-cmake-stdperiph"
stm32_stdperiph_lib="$project_dir/Source/Platform/Stm32f407/Manufacturer/StdPeriphV161"


make_project() {
echo "---------------- build cmsis ------------------"
cd $stm32_cmake_path/cmsis
cmake -DCMAKE_TOOLCHAIN_FILE=$cmake_toolchain_file -DSTM32_FAMILY=F4 -DSTM32_CHIP_TYPE=$stm32_chip_type -DTOOLCHAIN_PREFIX=$toolchain_prefix -DSTM32F4_StdPeriphLib_DIR=$stm32_stdperiph_lib -DCMAKE_INSTALL_PREFIX=$toolchain_prefix/arm-none-eabi/ -DCMAKE_BUILD_TYPE=Release -DSTARTUP_PREFIX=$stm32_startup_prefix -DCMSIS_SOURCES=$stm32_cmsis_sources -DCMSIS_LINKER_SCRIPT=$stm32_cmsis_linker_script
make && make install

echo "---------------- build stdperiph lib ------------------"
cd $stm32_cmake_path/stdperiph
cmake -DCMAKE_TOOLCHAIN_FILE=$cmake_toolchain_file -DCMAKE_MODULE_PATH=$cmake_module_path -DSTM32_FAMILY=F4 -DTOOLCHAIN_PREFIX=$toolchain_prefix -DSTM32F4_StdPeriphLib_DIR=$stm32_stdperiph_lib -DCMAKE_INSTALL_PREFIX=$toolchain_prefix/arm-none-eabi/ -DCMAKE_BUILD_TYPE=Release
make && make install

echo "--------------------- build project --------------------"
rm -f $project_dir/CMakeLists.txt
cp -f $project_dir/Script/CMakeLists_Stm32f407.txt $project_dir/CMakeLists.txt
mkdir -p $project_dir/Build
cd $project_dir/Build
cmake -DSTM32_CHIP=$stm32_chip -DCMAKE_TOOLCHAIN_FILE=$cmake_toolchain_file -DCMAKE_BUILD_TYPE=$cmake_build_type -DCMAKE_MODULE_PATH=$cmake_module_path -DTOOLCHAIN_PREFIX=$toolchain_prefix $project_dir 
make seeing_mcu_esw.bin
cp -rf seeing_mcu_esw.bin /mnt/hgfs/vm_share/
}

clean_project() {
	cd $project_dir/Build
	make clean
	cd -
	rm -rf $project_dir/Build
	rm -rf $stm32_cmake_path/cmsis/CMakeCache.txt
	rm -rf $stm32_cmake_path/cmsis/CMakeFiles
	rm -rf $stm32_cmake_path/stdperiph/CMakeCache.txt
	rm -rf $stm32_cmake_path/stdperiph/CMakeFiles
}

if [ "$@"x = "all"x ]; then
	make_project
fi

if [ "$@"x = "clean"x ]; then
	clean_project
fi




