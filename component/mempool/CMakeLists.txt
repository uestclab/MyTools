cmake_minimum_required(VERSION 3.5)

option(PUBLIC "This is a default option for PUBLIC" OFF) # OFF : PC , ON : Arm
message(STATUS "build public platform switch: "${PUBLIC})

option(BENCHMARK_MAIN "select to use test benchmark" ON) # OFF : not test , ON : test benchmark 
message("BENCHMARK_MAIN is ${BENCHMARK_MAIN}")

if(BENCHMARK_MAIN)
    add_definitions(-DBENCHMARK_MAIN)
    message(STATUS "start test benchmark")
else()
	 message(STATUS "start build library")	
endif(BENCHMARK_MAIN)

project(mempool)


if(${PUBLIC} STREQUAL "ON")
    message(STATUS "start to build ARM platform.")
	# cross complie
	SET(CMAKE_SYSTEM_NAME Linux)
	SET(CMAKE_SYSTEM_PROCESSOR arm) 
	SET(TOOLCHAIN_DIR "/home/gyl/petalinux/tools/linux-i386/gcc-arm-linux-gnueabi")
	SET(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR})
	SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

	SET(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/arm-linux-gnueabihf-gcc-6.2.1)
	SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-linux-gnueabihf-g++)
	# zlog
	link_directories("/home/gyl/liqingSpace/code/GW_arm_server/temp/lib/")
	include_directories("/home/gyl/liqingSpace/code/GW_arm_server/temp/include/")

elseif(${PUBLIC} STREQUAL "OFF") # stub test on pc
    message(STATUS "start to build PC platform.")
endif(${PUBLIC} STREQUAL "ON")

SET(CMAKE_BUILD_TYPE "Debug")  
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
SET(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/..")
SET(CMAKE_VERBOSE_MAKEFILE ON)    
SET(ROOT_PATH ".")

include_directories("${PROJECT_SOURCE_DIR}/../output/include")
link_directories("${PROJECT_SOURCE_DIR}/../output/lib")

if(BENCHMARK_MAIN)
	file(GLOB_RECURSE ALL_EXECUTABLES "mempool.c")

	add_executable(${PROJECT_NAME} ${ALL_EXECUTABLES})

	target_link_libraries(${PROJECT_NAME} buddy)
else()
	SET(LIB_NAME mempool)
	SET(ALL_LIBRARY_SRCS mempool.c)
	add_library(${LIB_NAME} SHARED ${ALL_LIBRARY_SRCS})
	if(${PUBLIC} STREQUAL "ON")
		message(STATUS "start to install ARM platform library.")
		install(TARGETS ${LIB_NAME} LIBRARY DESTINATION output/lib/)
	elseif(${PUBLIC} STREQUAL "OFF")
		message(STATUS "start to install PC platform library.")
		install(TARGETS ${LIB_NAME} LIBRARY DESTINATION output/lib/)
	endif(${PUBLIC} STREQUAL "ON")

	target_link_libraries(${LIB_NAME} buddy)

	install(FILES ${PROJECT_SOURCE_DIR}/mempool.h DESTINATION output/include/)
endif(BENCHMARK_MAIN)











