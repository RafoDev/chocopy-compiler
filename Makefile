all: clean main

main:
	g++ -o main.exe src/main.cc src/Scanner.cc src/Mtoken.cc -Iinclude

test:
	@printf "\nTEST: scanning the file: test.py\n\n"
	@./main.exe test.py
	@printf "\nTEST: scanning the file: test1.py\n\n"
	@./main.exe test1.py
	@printf "\nTEST: scanning the file: test2.py\n\n"
	@./main.exe test2.py
	@printf "\nTEST: scanning the file: test2.py\n\n"
	@./main.exe test3.py
clean:
	rm -f *.exe
