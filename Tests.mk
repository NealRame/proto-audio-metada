CFLAGS    += -O0 -g
CXXFLAGS  += -O0 -g

all: $(OBJECTS) libgtest.a gtest_main.o
	$(CXX) $(CXXFLAGS) -pthread -o proto-audio-metadata-test $^

gtest_main.o: $(CURDIR)/../vendors/gtest/src/gtest_main.cc
	$(CXX) $(CXXFLAGS) -pthread -c $(CURDIR)/../vendors/gtest/src/gtest_main.cc

libgtest.a:
	$(CXX) $(CXXFLAGS) -pthread -c $(CURDIR)/../vendors/gtest/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

include $(DEPS)
