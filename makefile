SRC = src/main.cpp src/cpu.cpp src/instructions.cpp
DST = bin/nes_emu

rule:
	g++ -Wall $(SRC) -o $(DST)

clean:
	rm -f bin/nes_emu
