// #include <iostream>
#include "../include/Scanner.h"
// using namespace std;

int main(int argc, char *argv[])
{
	Scanner scanner(argv[1]);
	scanner.scan();



	MToken token = scanner.getToken();
	cout<<token;
	token = scanner.getToken();
}
