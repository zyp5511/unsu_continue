OBJS = main.o FeaturePCA.o Cluster.o FeaturePartitioner.o FeatureWriter.o FeatureLoader.o Image.o Feature.o PatchDetector.o SVMDetector.o ExhaustiveCropper.o ImageCropper.o RandomCropper.o FLANNDetector.o KNNDetector.o
CC = g++
#CC = icpc
#CC = clang++
MKL = -DEIGEN_USE_MKL_ALL 
DEBUG = $(MKL) -O2 -g -std=c++11 
#DEBUG =  -O2 -g -std=c++11
#DEBUG = -O2 $(MKL) -std=c++11 -stdlib=libc++
LOCAL = /usr/local
EIGEN = /usr/local/include/eigen3
LD = -L$(LOCAL)/lib
LDLIBS = -ltbb -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_flann $(MKLLIBSOMP)
MKLLIBS = -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm
MKLLIBSOMP = -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm
LFLAGS = $(DEBUG)
CFLAGS = $(DEBUG) -I$(LOCAL)/include -I$(EIGEN) -c
#TARGET = features.exe
TARGET = unsu

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CC) $(OBJS) $(LFLAGS) $(LD) $(LDLIBS) -o $(TARGET)

main.o: main.cpp Cluster.cpp Cluster.h FeatureWriter.cpp FeatureWriter.h FeaturePartitioner.cpp FeaturePartitioner.h FeatureLoader.cpp FeatureLoader.h Image.h Image.cpp Feature.h Feature.cpp PatchDetector.h PatchDetector.cpp SVMDetector.h SVMDetector.cpp ExhaustiveCropper.h ExhaustiveCropper.cpp ImageCropper.h ImageCropper.cpp RandomCropper.h RandomCropper.cpp KNNDetector.h KNNDetector.cpp FLANNDetector.h FLANNDetector.cpp FeaturePCA.cpp FeaturePCA.h
	$(CC) $(CFLAGS) main.cpp      


Feature.o: Feature.cpp Feature.h
	$(CC) $(CFLAGS) Feature.cpp      

FeatureLoader.o: FeatureLoader.cpp FeatureLoader.h
	$(CC) $(CFLAGS) FeatureLoader.cpp      

FeaturePCA.o: FeaturePCA.cpp FeaturePCA.h
	$(CC) $(CFLAGS) FeaturePCA.cpp

FeaturePartitioner.o: FeaturePartitioner.cpp FeaturePartitioner.h
	$(CC) $(CFLAGS) FeaturePartitioner.cpp      

Cluster.o: Cluster.cpp Cluster.h
	$(CC) $(CFLAGS) Cluster.cpp      

FeatureWriter.o: FeatureWriter.cpp FeatureWriter.h
	$(CC) $(CFLAGS) FeatureWriter.cpp      

Image.o: Image.cpp Image.h
	$(CC) $(CFLAGS) Image.cpp      

RandomCropper.o: RandomCropper.cpp RandomCropper.h Feature.cpp Feature.h
	$(CC) $(CFLAGS) RandomCropper.cpp 

ImageCropper.o: ImageCropper.cpp ImageCropper.h
	$(CC) $(CFLAGS) ImageCropper.cpp 

ExhaustiveCropper.o: ExhaustiveCropper.cpp ExhaustiveCropper.h
	$(CC) $(CFLAGS) ExhaustiveCropper.cpp          

PatchDetector.o: PatchDetector.cpp PatchDetector.h 
	$(CC) $(CFLAGS) PatchDetector.cpp          

KNNDetector.o: KNNDetector.cpp KNNDetector.h 
	$(CC) $(CFLAGS) KNNDetector.cpp          

FLANNDetector.o: FLANNDetector.cpp FLANNDetector.h 
	$(CC) $(CFLAGS) FLANNDetector.cpp          

SVMDetector.o: SVMDetector.cpp SVMDetector.h 
	$(CC) $(CFLAGS) SVMDetector.cpp          


clean:
	rm -rf *o $(TARGET)

