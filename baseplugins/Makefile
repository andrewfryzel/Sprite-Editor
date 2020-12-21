## Here's a basic Makefile until I can figure out how to do this in qmake.

CXXFLAGS += -fPIC -shared -I. -I../ -I../pluginsdk

all: plus.so

plus.so:
	mkdir -p ../plugins
	$(CXX) $(CXXFLAGS) plus.cpp -o plus.so
	chmod -x plus.so
	mv plus.so ../plugins


clean:
	rm -rf ../plugins/plus.so
