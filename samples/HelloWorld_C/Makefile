CC=gcc
CCFLAGS=-c

DLRLIB_PATH=../../lib/Linux
LDFLAGS=-L $(DLRLIB_PATH) -Wl,-rpath=$(DLRLIB_PATH) -Wl,-rpath=./
DLRLIB=-lDynamsoftLabelRecognizer

TARGET=HelloWorld
OBJECT=HelloWorld.o
SOURCE=HelloWorld.c

# build rule for target.
$(TARGET): $(OBJECT)
	$(CC) -o $(TARGET) $(OBJECT) $(DLRLIB) $(LDFLAGS)

# target to build an object file
$(OBJECT): $(SOURCE)
	$(CC) $(CCFLAGS) $(SOURCE)

# the clean target
.PHONY : clean
clean: 
	rm -f $(OBJECT) $(TARGET)