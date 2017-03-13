cmake_minimum_required(VERSION 2.8)
project( EUCALLtag )
SET("OpenCV_DIR" "/path/to/your/OpenCV/installation")
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable( EUCALLtag LinuxSourceCode.cpp )
target_link_libraries( EUCALLtag ${OpenCV_LIBS} ${ZBAR_LIBRARIES}zbar)



