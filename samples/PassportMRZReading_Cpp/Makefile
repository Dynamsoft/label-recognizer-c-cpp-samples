CC=gcc
CCFLAGS=-c -std=c++11

DLRLIB_PATH=../../lib/linux
LDFLAGS=-L $(DLRLIB_PATH) -Wl,-rpath=$(DLRLIB_PATH) -Wl,-rpath=./
DLRLIB=-lDynamsoftLabelRecognizer

STDLIB=-lstdc++

TARGET=PassportMRZReading
OBJECT=PassportMRZReading.o
SOURCE=PassportMRZReading.cpp

# build rule for target.
$(TARGET): $(OBJECT)
	$(CC) -o $(TARGET) $(OBJECT) $(STDLIB) $(DLRLIB) $(LDFLAGS)

# target to build an object file
$(OBJECT): $(SOURCE)
	$(CC) $(CCFLAGS) $(SOURCE)

# the clean target
.PHONY : clean
clean: 
	rm -f $(OBJECT) $(TARGET)