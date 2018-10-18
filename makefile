SRC = src/main.cpp
DST = bin/nes_emu

rule:
	g++ -Wall -o $(DST) $(SRC)

clean:
	rm -f bin/nes_emu