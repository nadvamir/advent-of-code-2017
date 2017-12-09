CC=g++-7
PROGRAM_1=garbage
PROGRAM_2=garbage2

all: p1 p2

p1:
	$(CC) --std=c++17 $(PROGRAM_1).cpp -o $(PROGRAM_1).exe
	cat data.in | ./$(PROGRAM_1).exe

p2:
	$(CC) --std=c++17 $(PROGRAM_2).cpp -o $(PROGRAM_2).exe
	cat data.in | ./$(PROGRAM_2).exe
