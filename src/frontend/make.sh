bison -d -o parser.tab.cpp parser.y
flex -o tokens.cpp tokens.l
g++ *.cpp -o frontend