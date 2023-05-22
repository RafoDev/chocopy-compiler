all: clean main

main:
	g++ -o main.exe src/main.cc src/Scanner.cc src/Mtoken.cc src/MessageGenerator.cc src/Parser.cc -Iinclude

test:
	@printf "\nTEST: scanning the file: test.py\n\n"
	@./main.exe tests/test.py
	@printf "\nTEST: scanning the file: test1.py\n\n"
	@./main.exe tests/test1.py
	@printf "\nTEST: scanning the file: test2.py\n\n"
	@./main.exe tests/test2.py
	@printf "\nTEST: scanning the file: test3.py\n\n"
	@./main.exe tests/test3.py
	@printf "\nTEST: scanning the file: test4.py\n\n"
	@./main.exe tests/test3.py
clean:
	rm -f *.exe
