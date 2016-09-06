cmake_minimum_required(VERSION 3.5)
project(Proyecto_SO2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Block.cpp Block.h FreeBlockBitMap.cpp FreeBlockBitMap.h Program.cpp Program.h Partition.cpp Partition.h INode.cpp INode.h bitmap.cpp bitmap.h)
add_executable(Proyecto_SO2 ${SOURCE_FILES})