#cmake最小需要版本
cmake_minimum_required(VERSION 2.8)

#项目名字
project(testmem)

aux_source_directory(. DIR_SRC)

#生成程序
add_executable(testmem ${DIR_SRC})

