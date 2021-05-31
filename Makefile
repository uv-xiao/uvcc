OBJ = entry.o regs.o stack.o vars.o utils.o ir.o

FLAGS = -std=c++14 -g # -Ofast

tigger: ${OBJ} 
	g++ $^ -o $@  ${FLAGS}

%.o: %.cpp %.h
	g++ -c $^ ${FLAGS}

.PHONY: clean clean_intermediate lines

clean_intermediate:
	- rm -f *.gch
	- rm -f *.tab.* lex.yy.*
	- rm -f ${OBJ}

clean:
	- rm -f *.gch
	- rm -f *.tab.* lex.yy.*
	- rm -f ${OBJ}
	- rm -f tigger

lines:
	- wc -l *.cpp *.h 
