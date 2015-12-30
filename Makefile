APP=CodeParser
DRIVER=driver
CXX=g++
CXXFLAGS= -std=c++11 -g -I./include
APP_FLAGS=-fPIC
APP_OBJDIR=./obj/lib
APP_SRCDIR=./src/lib
APP_BINDIR=./bin/lib
DRIVER_OBJDIR=./obj/driver
DRIVER_SRCDIR=./src/driver

APP_SRCS=$(shell find $(APP_SRCDIR) -maxdepth 1 -type f -name "*.cpp")
APP_OBJS=$(patsubst $(APP_SRCDIR)/%.cpp, $(APP_OBJDIR)/%.o, $(APP_SRCS))

DRIVER_SRCS=$(shell find $(DRIVER_SRCDIR) -maxdepth 1 -type f -name "*.cpp")
DRIVER_OBJS=$(patsubst $(DRIVER_SRCDIR)/%.cpp, $(DRIVER_OBJDIR)/%.o, $(DRIVER_SRCS))

all: $(APP)
driver: $(DRIVER)

$(APP): $(APP_OBJS)
	$(CXX) -shared -Wl,-soname,$(APP).so.1 $(APP_OBJS) -lc -o $(APP_BINDIR)/$(APP).so.1.0.1
	
$(APP_OBJS): $(APP_OBJDIR)/%.o : $(APP_SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(APP_FLAGS) $(LDFLAGS) -Wall -c $< -o $@
	
$(DRIVER): $(DRIVER_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(DRIVER) $(DRIVER_OBJS) $(LDLIBS)
	
.PHONY: clean

clean:
	rm -f $(APP_OBJDIR)/*.o
	rm -f $(DRIVER_OBJDIR)/*.o
	
	