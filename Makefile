all: coralccc

OBJS = parser.o  \
       main.o    \
       tokens.o  \
       ASTNode.o

CXX = g++
CXXFLAGS = -g


clean:
	$(RM) -rf parser.cpp parser.hpp coralccc tokens.cpp *.o $(OBJS)

parser.cpp: parser.y
	bison -d -o $@ $^
	
parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	flex -o $@ $^

parser.o : parser.cpp parser.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tokens.o : tokens.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<	

ASTNode.o : ASTNode.cpp ASTNode.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<	

main.o : main.cpp ASTNode.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<	

coralccc: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) 


