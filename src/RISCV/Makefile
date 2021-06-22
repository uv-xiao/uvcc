

all: riscv

riscv: riscv.cpp entry.cpp riscv.h entry.h
	g++ riscv.cpp entry.cpp -o riscv -g -std=c++14

.PHONY: clean check

clean:
	rm -f riscv

