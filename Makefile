CC  = gcc
CXX = g++
HOME = /home/apps
ROOT = .
INCLUDE = -I$(ROOT)/include -I$(HOME)/mylib/include
SRC = ./src
INC = ./include
BIN = ./bin
RM = rm -rf
MV = mv
LIBS = -L$(HOME)/mylib/lib -lglog -lpthread
CFLAGS   = -Wall -O2 -D_FILE_OFFSET_BITS=64
CXXFLAGS = -Wall -O2 -D_FILE_OFFSET_BITS=64 -DSIZEOF_LOG_INT=8

TARGET = ftrl_main

all : $(TARGET)
	$(RM) $(BIN)/*
	$(MV) $(TARGET) $(BIN)

ftrl_main : ftrl_main.o ftrl.o config.o splitString.o
	$(CXX) -o $@ $^ $(LIBS) -static

ftrl_main.o : $(SRC)/ftrl_main.cpp $(INC)/ftrl.h $(INC)/ftrl_common.h $(INC)/config.h
	$(CXX) -c $(CXXFLAGS) $< $(INCLUDE)

ftrl.o : $(SRC)/ftrl.cpp $(INC)/ftrl.h $(INC)/ftrl_common.h
	$(CXX) -c $(CXXFLAGS) $< $(INCLUDE)

config.o : $(SRC)/config.cpp $(INC)/config.h $(INC)/splitString.h
	$(CXX) -c $(CXXFLAGS) $< $(INCLUDE)

splitString.o : $(SRC)/splitString.cpp $(INC)/splitString.h
	$(CXX) -c $(CXXFLAGS) $< $(INCLUDE)

.PHONY : clean
clean :
	$(RM) $(TARGET) *.o core
