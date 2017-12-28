CC=g++
PROGRAM_2=fractal2

all: p2

p2:
	$(CC) --std=c++17 $(PROGRAM_2).cpp -o $(PROGRAM_2).exe
	time ./$(PROGRAM_2).exe < data.in
	echo "expected 1984683"
