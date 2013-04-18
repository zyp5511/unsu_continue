set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Unsu_SOURCE_DIR}/cmake)
include_directories(${Unsu_SOURCE_DIR}/include)

find_package( TBB REQUIRED )
find_package( OpenCV REQUIRED)
FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED)

set(SRCS Transform.cpp Cluster.cpp Feature.cpp FeaturePCA.cpp Image.cpp main.cpp FeatureLoader.cpp FeatureWriter.cpp ImageCropper.cpp PatchDetector.cpp SVMDetector.cpp ExhaustiveCropper.cpp FeaturePartitioner.cpp FLANNDetector.cpp KNNDetector.cpp RandomCropper.cpp LatentDetector.cpp KMeanDetector.cpp)

add_executable(unsu ${SRCS})

include_directories(${TBB_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

target_link_libraries( unsu ${OpenCV_LIBS} ${TBB_LIBRARIES} ${Boost_LIBRARIES})

get_cmake_property(_variableNames VARIABLES)

#foreach (_variableName ${_variableNames})
#	message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11 ${CMAKE_CXX_FLAGS}")

if(APPLE)
	set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif(APPLE)
