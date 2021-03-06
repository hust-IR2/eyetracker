cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_BUILD_TYPE Release)


find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

set(source
  src/main.cpp
  src/object.cpp
  src/camera.cpp
  src/serial.cpp
  src/lightsensor_datacontroler.cpp
  src/lightsensor_dataprocessor.cpp
  src/track_object.cpp
)

add_executable(demo ${source})

IF (WIN32)
target_link_libraries(demo ${OpenCV_LIBS})
ELSEIF (UNIX)
target_link_libraries(demo ${OpenCV_LIBS} pthread)
ENDIF ()
