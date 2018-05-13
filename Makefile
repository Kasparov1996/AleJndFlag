CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wextra
LDFLAGS = -lcrypto -lcurl -ljsoncpp
LIBS = -lcrypto -lcurl -ljsoncpp
INCS = -I./include
PROG = get_flag
SRCS = $(wildcard *.cpp)
SUBDIRS = single multi clean

single:
	@$(CXX) $(SRCS) $(CXXFLAGS) -o $(PROG) $(INCS) $(LDFLAGS)

multi:
	@$(CXX) $(SRCS) $(CXXFLAGS) -o $(PROG) $(INCS) $(LDFLAGS) -DMULTISERVICE

.PHONY: $(SUBDIRS)

clean:
	@rm -f *.o $(PROG)
