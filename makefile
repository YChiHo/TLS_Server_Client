C = gcc
CXX = g++
OBJS = Client.cpp
TARGET = run
LIBS = -lssl -lcrypto -L/usr/local/ssl/lib
HEADER = Clientheader.hpp
CXXFLAGS= -g -o
.SUFFIXES : .cpp .o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) -std=c++11 $(HEADER) $(OBJS) $(CXXFLAGS) $(TARGET) $(LIBS)
#std = c++
#	$(CXX) $(OBJS) $(CXXFLAGS) $(TARGET) $(LIBS) -std=c++11

clean:
	rm  -f $(TARGET) core*
