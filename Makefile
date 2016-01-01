APP=CodeParser
DRIVER=CodeParser_driver
TEST=test

C=gcc
CFLAGS= -Wall

CXX=g++
CXXFLAGS= -std=c++11 -g -I./include -Wall

APP_FLAGS=-fPIC
APP_OBJDIR=./obj/lib
APP_SRCDIR=./src/lib
APP_BINDIR=./bin/lib

DRIVER_OBJDIR=./obj/driver
DRIVER_SRCDIR=./src/driver
DRIVER_BINDIR=./bin/driver
DRIVER_LDFLAGS=-L$(APP_BINDIR)
DRIVER_LFLAGS=-l$(APP)

TEST_OBJDIR=./obj/test
TEST_SRCDIR=./src/test
TEST_BINDIR=./bin/test

APP_SRCS=$(shell find $(APP_SRCDIR) -maxdepth 1 -type f -name "*.cpp")
APP_OBJS=$(patsubst $(APP_SRCDIR)/%.cpp, $(APP_OBJDIR)/%.o, $(APP_SRCS))

DRIVER_SRCS=$(shell find $(DRIVER_SRCDIR) -maxdepth 1 -type f -name "*.cpp")
DRIVER_OBJS=$(patsubst $(DRIVER_SRCDIR)/%.cpp, $(DRIVER_OBJDIR)/%.o, $(DRIVER_SRCS))

TEST_SRCS=$(shell find $(TEST_SRCDIR) -maxdepth 1 -type f -name "*.c")
TEST_OBJS=$(patsubst $(TEST_SRCDIR)/%.c, $(TEST_OBJDIR)/%.o, $(TEST_SRCS))

all: $(APP) driver testBinary

lib: $(APP)
driver: $(DRIVER)
test: $(TEST_BINARY)

$(APP): $(APP_OBJS)
	$(CXX) -shared -Wl,-soname,$(APP).so $(APP_OBJS) -lc -o $(APP_BINDIR)/lib$(APP).so
	
$(APP_OBJS): $(APP_OBJDIR)/%.o : $(APP_SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(APP_FLAGS) $(LDFLAGS) -c $< -o $@
	
$(DRIVER): $(DRIVER_OBJS)
	$(CXX) $(CXXFLAGS) $(DRIVER_LDFLAGS) $(DRIVER_LFLAGS) $^ -o $(DRIVER_BINDIR)/$(DRIVER)
	
$(DRIVER_OBJDIR)/%.o : $(DRIVER_SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@
	
$(TEST): $(TEST_OBJS)
	$(C) $(CFLAGS) $^ -o $(TEST_BINDIR)/$(TEST)

$(TEST_OBJDIR)/%.o : $(TEST_SRCDIR)/%.c
	$(C) $(CFLAGS) -c $< -o $@


	
.PHONY: clean

clean:
	rm -f $(APP_OBJDIR)/*.o
	rm -f $(APP_BINDIR)/*
	rm -f $(DRIVER_OBJDIR)/*.o
	rm -f $(DRIVER_BINDIR)/*
	rm -f $(TEST_OBJDIR)/*.o
	rm -f $(TEST_BINDIR)/*
	
	