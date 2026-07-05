CXX      = g++
CXXFLAGS = -std=c++14 -O3 -fstack-protector-all -W -Wall -Wextra -Wunused \
           -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal \
           -Wpointer-arith -Wformat-security -Wmissing-format-attribute \
           -Wformat=2 -Wwrite-strings -Wcast-align -Wno-long-long \
           -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual \
           -Wno-property-attribute-mismatch

HEADERS = record.h command.h list.h condition.h io.h ordering.h \
          operation.h command_type.h hash_index.h net_utils.h

SERVER_SOURCES = record.cpp hash_index.cpp list.cpp command.cpp \
                 net_utils.cpp server.cpp

CLIENT_SOURCES = net_utils.cpp client.cpp

all: server client

server: $(SERVER_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SERVER_SOURCES)

client: $(CLIENT_SOURCES) net_utils.h io.h
	$(CXX) $(CXXFLAGS) -o $@ $(CLIENT_SOURCES)

clean:
	rm -f server client

.PHONY: all clean