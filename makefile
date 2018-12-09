SRC = src/main.cpp src/cpu.cpp src/instructions.cpp src/nes.cpp src/ppu.cpp src/opcodes/*
DST = bin/nes_emu

rule:
	g++ -Wall $(SRC) -o $(DST)

clean:
	rm -f bin/nes_emu
