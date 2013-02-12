OBJS = main.o Image.o Feature.o PatchDetector.o SVMDetector.o ExhaustiveCropper.o ImageCropper.o RandomCropper.o KNNDetector.o
OBJS_SCRIPT = script.o Feature.o PatchDetector.o SVMDetector.o ExhaustiveCropper.o ImageCropper.o RandomCropper.o KNNDetector.o 
CC = clang++
DEBUG = -O2 -std=c++11 -stdlib=libc++
ROOT = /usr/local
LD = -L$(ROOT)/lib
LDLIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_flann
LFLAGS = $(DEBUG)
CFLAGS = $(DEBUG) -I$(ROOT)/include -c
#TARGET = features.exe
TARGET = unsu
TARGET_SCRIPT = script

all: $(TARGET) $(TARGET_SCRIPT)

$(TARGET): $(OBJS) 
	$(CC) $(OBJS) $(LFLAGS) $(LD) $(LDLIBS) -o $(TARGET)

$(TARGET_SCRIPT): $(OBJS_SCRIPT) 
	$(CC) $(OBJS_SCRIPT) $(LFLAGS) $(LD) $(LDLIBS) -o $(TARGET_SCRIPT)

main.o: main.cpp Image.h Image.cpp Feature.h Feature.cpp PatchDetector.h PatchDetector.cpp SVMDetector.h SVMDetector.cpp ExhaustiveCropper.h ExhaustiveCropper.cpp ImageCropper.h ImageCropper.cpp RandomCropper.h RandomCropper.cpp KNNDetector.h KNNDetector.cpp 
	$(CC) $(CFLAGS) main.cpp      

script.o: script.cpp script.h Feature.h Feature.cpp PatchDetector.h PatchDetector.cpp SVMDetector.h SVMDetector.cpp ExhaustiveCropper.h ExhaustiveCropper.cpp ImageCropper.h ImageCropper.cpp RandomCropper.h RandomCropper.cpp KNNDetector.h KNNDetector.cpp  
	$(CC) $(CFLAGS) script.cpp      

Feature.o: Feature.cpp Feature.h
	$(CC) $(CFLAGS) Feature.cpp      

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

SVMDetector.o: SVMDetector.cpp SVMDetector.h 
	$(CC) $(CFLAGS) SVMDetector.cpp          


clean:
	rm -rf *o $(TARGET)

