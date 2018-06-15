cmake_minimum_required(VERSION 2.8)


project( Curtain )

find_package( OpenCV REQUIRED )
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
add_executable( ${PROJECT_NAME} ./src/Curtain.cpp ./src/CurtainGrain.cpp ./src/main.cpp)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
