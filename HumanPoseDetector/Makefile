OBJS = main.o Feature.o HumanDetector.o ImageCropper.o NegativeCollector.o KNNDetector.o
CC = g++
DEBUG = -O2 "-std=c++11"
ROOT = /usr/local
LD = -L$(ROOT)/lib
LDLIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_flann
LFLAGS = $(DEBUG)
CFLAGS = $(DEBUG) -I$(ROOT)/include -c
#TARGET = features.exe
TARGET = unsu

all: $(TARGET) 

$(TARGET): $(OBJS) 
	$(CC) $(OBJS) $(LFLAGS) $(LD) $(LDLIBS) -o $(TARGET)

main.o: main.cpp Feature.cpp Feature.h KNNDetector.cpp KNNDetector.h NegativeCollector.cpp NegativeCollector.h HumanDetector.cpp HumanDetector.h ImageCropper.cpp ImageCropper.h
	$(CC) $(CFLAGS) main.cpp      

Feature.o: Feature.cpp Feature.h
	$(CC) $(CFLAGS) Feature.cpp      

KNNDetector.o: KNNDetector.cpp KNNDetector.h HumanDetector.cpp HumanDetector.h
	$(CC) $(CFLAGS) KNNDetector.cpp          

NegativeCollector.o: NegativeCollector.cpp NegativeCollector.h
	$(CC) $(CFLAGS) NegativeCollector.cpp 

HumanDetector.o: HumanDetector.cpp HumanDetector.h 
	$(CC) $(CFLAGS) HumanDetector.cpp          

ImageCropper.o: ImageCropper.cpp ImageCropper.h
	$(CC) $(CFLAGS) ImageCropper.cpp          

clean:
	rm -rf *o $(TARGET)

