cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
project(2DEngine)

#list(APPEND CMAKE_PREFIX_PATH "C:/Users/Georg/Documents/Code/libraries/opencv")
set(OpenCV_DIR $ENV{OPENCV_DIR})

find_package(OpenCV REQUIRED )

set(SOURCES
	"src/main.cpp"
)

add_executable(2DEngine ${SOURCES})
target_include_directories(2DEngine PRIVATE  ${OpenCV_INCLUDE_DIRS} "include")
target_link_libraries( 2DEngine ${OpenCV_LIBS})