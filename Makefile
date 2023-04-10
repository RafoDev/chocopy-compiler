all: clean main

main:
	g++ -o main.exe src/main.cc

test:
	./main.exe test.py
clean:
	rm -f *.exe
