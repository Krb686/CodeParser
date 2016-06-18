LIB=CodeParser
DRIVER=CodeParser_driver
TEST=test_binary

C=gcc
CFLAGS= -Wall

CXX=g++
CXXFLAGS= -std=c++11 -g -I./include -Wall

LIB_FLAGS=-fPIC
LIB_OBJDIR=./obj/lib
LIB_SRCDIR=./src/lib
LIB_BINDIR=./bin/lib

DRIVER_OBJDIR=./obj/driver
DRIVER_SRCDIR=./src/driver
DRIVER_BINDIR=./bin/driver
DRIVER_LDFLAGS=-L$(LIB_BINDIR)
DRIVER_LFLAGS=-l$(LIB)

TEST_OBJDIR=./obj/test
TEST_SRCDIR=./src/test
TEST_BINDIR=./bin/test

LIB_SRCS=$(shell find $(LIB_SRCDIR) -maxdepth 1 -type f -name "*.cpp")
LIB_OBJS=$(patsubst $(LIB_SRCDIR)/%.cpp, $(LIB_OBJDIR)/%.o, $(LIB_SRCS))

DRIVER_SRCS=$(shell find $(DRIVER_SRCDIR) -maxdepth 1 -type f -name "*.cpp")
DRIVER_OBJS=$(patsubst $(DRIVER_SRCDIR)/%.cpp, $(DRIVER_OBJDIR)/%.o, $(DRIVER_SRCS))

TEST_SRCS=$(shell find $(TEST_SRCDIR) -maxdepth 1 -type f -name "*.c")
TEST_OBJS=$(patsubst $(TEST_SRCDIR)/%.c, $(TEST_OBJDIR)/%.o, $(TEST_SRCS))

all: $(LIB) $(DRIVER) $(TEST)

lib: $(LIB)
driver: $(DRIVER)
test: $(TEST)

$(LIB): $(LIB_OBJS)
	$(CXX) -shared -Wl,-soname,lib$(LIB).so $(LIB_OBJS) -lc -o $(LIB_BINDIR)/lib$(LIB).so

$(LIB_OBJS): $(LIB_OBJDIR)/%.o : $(LIB_SRCDIR)/%.cpp LIB_PRINT
	$(CXX) $(CXXFLAGS) $(LIB_FLAGS) $(LDFLAGS) -c $< -o $@

.PHONY: LIB_PRINT

LIB_PRINT:
	@printf "\nMaking lib ojects...\n"

$(DRIVER): $(DRIVER_OBJS)
	$(CXX) $(CXXFLAGS) $(DRIVER_LDFLAGS) $^ $(DRIVER_LFLAGS) -o $(DRIVER_BINDIR)/$(DRIVER)

$(DRIVER_OBJDIR)/%.o : $(DRIVER_SRCDIR)/%.cpp DRIVER_PRINT
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

.PHONY: DRIVER_PRINT

DRIVER_PRINT:
	@printf "\nMaking driver objects...\n"

$(TEST): $(TEST_OBJS)
	$(C) $(CFLAGS) $^ -o $(TEST_BINDIR)/$(TEST)

$(TEST_OBJDIR)/%.o : $(TEST_SRCDIR)/%.c TEST_PRINT
	$(C) $(CFLAGS) -c $< -o $@

.PHONY: TEST_PRINT

TEST_PRINT:
	@printf "\nMaking test objects...\n"



.PHONY: clean

clean:
	rm -f $(LIB_OBJDIR)/*.o
	rm -f $(LIB_BINDIR)/*
	rm -f $(DRIVER_OBJDIR)/*.o
	rm -f $(DRIVER_BINDIR)/*
	rm -f $(TEST_OBJDIR)/*.o
	rm -f $(TEST_BINDIR)/*


