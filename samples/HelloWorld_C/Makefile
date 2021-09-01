CC=gcc
CCFLAGS=-c

DLRMODEL_PATH=../../CharacterModel
DLRLIB_PATH=../../lib/linux
LDFLAGS=-L $(DLRLIB_PATH) -Wl,-rpath=$(DLRLIB_PATH) -Wl,-rpath=./
DLRLIB=-lDynamsoftLabelRecognizer

TARGET=HelloWorld
OBJECT=HelloWorld.o
SOURCE=HelloWorld.c

# build rule for target.
$(TARGET): $(OBJECT)
	$(CC) -o $(TARGET) $(OBJECT) $(DLRLIB) $(LDFLAGS)
	cp -r $(DLRMODEL_PATH) $(DLRLIB_PATH)

# target to build an object file
$(OBJECT): $(SOURCE)
	$(CC) $(CCFLAGS) $(SOURCE)

# the clean target
.PHONY : clean
clean: 
	rm -f $(OBJECT) $(TARGET) -r $(DLRLIB_PATH)/CharacterModel