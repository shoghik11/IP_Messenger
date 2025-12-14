CXX=g++
CXXFLAGS=-std=c++11 -pthread

all: ip_messenger

ip_messenger: ip_messenger.cpp
	$(CXX) $(CXXFLAGS) ip_messenger.cpp -o ip_messenger

clean:
	rm -f ip_messenger
