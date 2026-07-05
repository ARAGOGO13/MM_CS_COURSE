CXX = g++
CXXFLAGS = -std=c++14 -O3 -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic \
 -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings \
 -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-property-attribute-mismatch

all: a02.out a03.out a05.out a06.out a07.out

a%.out: a%.o common_utils.o query_classes.o pattern_utils.o tasks.o
	$(CXX) $(CXXFLAGS) $^ -o $@

a%.o: main_%.cpp solve.h io.h
	$(CXX) -c $< $(CXXFLAGS) -o $@

common_utils.o: common_utils.cpp common_utils.h io.h
	$(CXX) -c $< $(CXXFLAGS) -o $@

query_classes.o: query_classes.cpp query_classes.h common_utils.h b_tree.h rb_tree.h avl_tree.h
	$(CXX) -c $< $(CXXFLAGS) -o $@

pattern_utils.o: pattern_utils.cpp pattern_utils.h common_utils.h
	$(CXX) -c $< $(CXXFLAGS) -o $@

tasks.o: tasks.cpp solve.h query_classes.h pattern_utils.h
	$(CXX) -c $< $(CXXFLAGS) -o $@

clean:
	rm -f *.o *.out
