#include <iostream>
#include "sha256.h"


using std::string;
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	char *str = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
	string output=sha256(str);
	cout << "sha256('"<< str << "'):" << output << endl;
	return 0;
}
