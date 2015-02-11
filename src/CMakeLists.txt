set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Unsu_SOURCE_DIR}/cmake)

include_directories(${Unsu_SOURCE_DIR}/include)
include_directories(${Unsu_SOURCE_DIR}/3rdparty)

SET(Boost_NO_BOOST_CMAKE ON)

find_package( TBB REQUIRED )
find_package( OpenCV REQUIRED)
FIND_PACKAGE( Boost COMPONENTS program_options filesystem system REQUIRED)


FILE (GLOB SRCS "*.cpp")
#FILE (GLOB_RECURSE SRCS "*.cpp")

add_executable(unsu "utility/main.cpp" ${SRCS})

include_directories(${TBB_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

target_link_libraries( unsu ${OpenCV_LIBS} ${TBB_LIBRARIES} ${Boost_LIBRARIES})
#target_link_libraries( distance ${OpenCV_LIBS} ${TBB_LIBRARIES} ${Boost_LIBRARIES})

