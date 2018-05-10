CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wextra
LDFLAGS = -ljsoncpp -lcurl -ldl
LIBS = -ljsoncpp -lcurl -ldl
INCS = -I. -I./include
PROG = get_flag
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
SUBDIRS = all clean

all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) $(INCS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< $(LIBS) $(INCS) -o $@

.PHONY: $(SUBDIRS)

clean:
	rm -f *.o $(PROG)
