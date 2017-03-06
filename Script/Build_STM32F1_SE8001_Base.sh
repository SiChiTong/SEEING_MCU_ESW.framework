#!/bin/sh
cmake_build_type="Debug"
cmake_toolchain_file="/home/seeing/stm32-toolchain/cmake/stm32-cmake-stdperiph/gcc_stm32.cmake"
cmake_module_path="/home/seeing/stm32-toolchain/cmake/stm32-cmake-stdperiph/cmake/Modules"
toolchain_prefix="/home/seeing/stm32-toolchain/gcc-arm/gcc-arm-none-eabi-4_9-2015q3"
project_dir="/home/seeing/workspace/SEEING_MCU_ESW.framework"
stm32_startup_prefix="$project_dir/Source/Platform/Stm32f107/Startup/startup_stm32f10x_"
stm32_cmsis_sources="$project_dir/Source/Platform/Stm32f107/Startup/system_stm32f10x.c"
stm32_cmsis_linker_script="$project_dir/Source/Platform/Stm32f107/Startup/stm32f1flash.ld"
stm32_chip="STM32F103RC"
stm32_chip_type="HD"
stm32_cmake_path="/home/seeing/stm32-toolchain/cmake/stm32-cmake-stdperiph"
stm32_stdperiph_lib="$project_dir/Source/Platform/Stm32f107/Manufacturer/StdPeriphV350"
stm32_cmsis_path="$toolchain_prefix/arm-none-eabi/include/stm32f1"

make_project() {
echo "---------------- build cmsis ------------------"
cd $stm32_cmake_path/cmsis
cmake -DCMAKE_TOOLCHAIN_FILE=$cmake_toolchain_file -DSTM32_FAMILY=F1 -DSTM32_CHIP_TYPE=$stm32_chip_type -DTOOLCHAIN_PREFIX=$toolchain_prefix -DSTM32F1_StdPeriphLib_DIR=$stm32_stdperiph_lib -DCMAKE_INSTALL_PREFIX=$toolchain_prefix/arm-none-eabi/ -DCMAKE_BUILD_TYPE=Release -DSTARTUP_PREFIX=$stm32_startup_prefix -DCMSIS_INCLUDE_DIR=$stm32_cmsis_path -DCMSIS_SOURCES=$stm32_cmsis_sources -DCMSIS_LINKER_SCRIPT=$stm32_cmsis_linker_script
make && make install

echo "---------------- build stdperiph lib ------------------"
cd $stm32_cmake_path/stdperiph
cmake -DCMAKE_TOOLCHAIN_FILE=$cmake_toolchain_file -DCMAKE_MODULE_PATH=$cmake_module_path -DSTM32_FAMILY=F1 -DTOOLCHAIN_PREFIX=$toolchain_prefix -DSTM32F1_StdPeriphLib_DIR=$stm32_stdperiph_lib -DCMAKE_INSTALL_PREFIX=$toolchain_prefix/arm-none-eabi/  -DCMSIS_INCLUDE_DIR=$stm32_cmsis_path -DCMAKE_BUILD_TYPE=Release
make && make install

echo "--------------------- build project --------------------"
rm -f $project_dir/CMakeLists.txt
cp -f $project_dir/Script/CMakeLists_Stm32f107_SE8001_Base.txt $project_dir/CMakeLists.txt
mkdir -p $project_dir/Build
cd $project_dir/Build
cmake -DSTM32_FAMILY=F1 -DSTM32_CHIP=$stm32_chip -DCMAKE_TOOLCHAIN_FILE=$cmake_toolchain_file -DCMAKE_BUILD_TYPE=$cmake_build_type  -DCMSIS_INCLUDE_DIR=$stm32_cmsis_path -DCMAKE_MODULE_PATH=$cmake_module_path -DTOOLCHAIN_PREFIX=$toolchain_prefix $project_dir 
#make seeing_mcu_esw.bin
#cp -rf seeing_mcu_esw.bin /mnt/hgfs/vm_share/
make seeing_mcu_esw.hex
cp -rf seeing_mcu_esw.hex /mnt/hgfs/vm_share/
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




