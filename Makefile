APP=common
CXX=g++
CXXFLAGS= -std=c++11 -g
SRCDIR=./src

SRCS=$(shell find ./src -maxdepth 1 -type f -name "*.cpp")
OBJS=$(patsubst src/%.cpp, obj/%.o, $(SRCS))

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(APP) $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f common
	rm -f *~ .depend
	rm -f ./bin/Debug/common
	rm -rf ./obj/*

include .depend

