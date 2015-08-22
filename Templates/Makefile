CC = clang
BOOST_HEADERS = /usr/local/include
BOOST_LIBS = /usr/local/lib

LFLAGS += -lstdc++

CPPFLAGS = --std=c++11 -Wall -stdlib=libc++ -O3

BINS += IsOneOf

%: %.cpp
	$(CC) $(CPPFLAGS) $(LFLAGS) -o $@ $<

all: $(BINS)
