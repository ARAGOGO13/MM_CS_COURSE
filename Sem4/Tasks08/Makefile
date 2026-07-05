CXX = g++
CXXFLAGS = -std=c++14 -O3 -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic \
           -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings \
           -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-property-attribute-mismatch

SOURCES = record.cpp hash_index.cpp list.cpp command.cpp main.cpp
HEADERS = record.h command.h list.h condition.h io.h ordering.h operation.h command_type.h hash_index.h

all: a.out

a.out: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

clean:
	rm -f a.out