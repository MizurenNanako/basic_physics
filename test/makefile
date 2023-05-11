CXX 		= 	g++
CXXFLAGS	=	-std=c++20 \
				-g \
				-Wall \
				-I'../include/'
LIBS		=	-lm

test_concepts: test_concepts.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: *.o
	rm $^