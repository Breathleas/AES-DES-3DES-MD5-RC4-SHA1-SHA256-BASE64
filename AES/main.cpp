#include "AES.h"

int main(int argc, char const *argv[])
{
	for (int type=128;type<=256;type+=64)
		for (int klen=128;klen<=256;klen+=64)
			AES::testAES(type,klen);
	AES::demoAES();
	return 0;
}