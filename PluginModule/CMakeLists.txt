# CMakeList.txt: CMakeProject 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

#-----------------TODO 自定义编译选项
# set(CMAKE_CXX_FLAGS “-std=c++11 -O2 -g”)

STRING( REGEX REPLACE ".*/(.*)" "\\1" PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR}) 
#-----------------TODO 添加模块变量
set(_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../${PROJECT_NAME}/include)
set(_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../${PROJECT_NAME}/src)

#将各种文件归类到指定变量
FILE(GLOB_RECURSE HEADER_FILES ${_INC_PATH}/*.h)
FILE(GLOB_RECURSE SOURCE_FILES ${_SRC_PATH}/*.h)
FILE(GLOB_RECURSE SOURCE_FILES ${_SRC_PATH}/*.cpp)

#查找当前目录下所有的.h和.cpp文件
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})

#-----------------TODO 创建工程内的库
#-----------------STATIC表示创建静态库，目标文件为 libmylib.a，如果是 SHARED， 则为创建动态库.
IF(OUTPUT_TYPE STREQUAL "dll")
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
ELSEIF(OUTPUT_TYPE STREQUAL "lib")
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
ELSE()
add_executable (${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
ENDIF()
#-----------------TODO EXE编译连接
#add_executable (${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

#-----------------TODO添加头文件的搜索目录
include_directories(${_INC_PATH}
					${_SRC_PATH})
					
#-----------------TODO 添加lib文件的搜索目录
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../${PROJECT_NAME}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib)


find_package(ExtendStructure)
if(ExtendStructure_FOUND)
  include_directories(${ExtendStructure_DIR}/include)
  link_directories(${ExtendStructure_DIR}/lib)
  target_link_libraries(${PROJECT_NAME} debug ${ExtendStructure_LIBRARY_DEBUG})
  target_link_libraries(${PROJECT_NAME} optimized ${ExtendStructure_LIBRARY})
endif(ExtendStructure_FOUND)
find_package(Library)
if(Library_FOUND)
  include_directories(${Library_DIR}/include)
  link_directories(${Library_DIR}/lib)
  target_link_libraries(${PROJECT_NAME} debug ${Library_LIBRARY_DEBUG})
  target_link_libraries(${PROJECT_NAME} optimized ${Library_LIBRARY})
endif(Library_FOUND)
