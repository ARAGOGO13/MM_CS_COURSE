CXX = g++
CXXFLAGS = -std=c++14 -O3 -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic \
           -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings \
           -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-property-attribute-mismatch

SOURCES = main.cpp record.cpp command.cpp list.cpp
HEADERS = record.h command.h list.h condition.h io.h ordering.h operation.h command_type.h

all: a.out

a.out: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

clean:
	rm -f a.out