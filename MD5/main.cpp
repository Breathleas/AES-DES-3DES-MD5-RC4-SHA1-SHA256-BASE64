#include <iostream>
#include "md5.h"

using std::cout;
using std::endl;

void printMD5(const string& message) {
  cout << "md5(\"" << message << "\") = "
       << MD5(message).toStr() << endl;
}

int main() {
  printMD5("");
  printMD5("a");
  printMD5("abc");
  printMD5("message digest");
  printMD5("abcdefghijklmnopqrstuvwxyz");
  printMD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	return 0;
}
