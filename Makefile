all: clean main

main:
	g++ -o main.exe src/main.cc

test:
	@printf "\nTEST: scanning the file: test.py\n\n"
	@./main.exe test.py
	@printf "\nTEST: scanning the file: test1.py\n\n"
	@./main.exe test1.py
	@printf "\nTEST: scanning the file: test2.py\n\n"
	@./main.exe test2.py
clean:
	rm -f *.exe
