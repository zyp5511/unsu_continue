set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Unsu_SOURCE_DIR}/cmake)

include_directories(${Unsu_SOURCE_DIR}/include)

SET(Boost_NO_BOOST_CMAKE ON)

find_package( TBB REQUIRED )
find_package( OpenCV REQUIRED)
FIND_PACKAGE( Boost COMPONENTS program_options filesystem system REQUIRED)


FILE (GLOB_RECURSE SRCS "*.cpp")

add_executable(unsu ${SRCS})

include_directories(${TBB_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

target_link_libraries( unsu ${OpenCV_LIBS} ${TBB_LIBRARIES} ${Boost_LIBRARIES})

#get_cmake_property(_variableNames VARIABLES)


#foreach (_variableName ${_variableNames})
#	message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()


