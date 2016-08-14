#include <iostream>

#include "../Shared/crypto++/rsa.h"
#include "../Shared/crypto++/integer.h"
#include "../Shared/crypto++/osrng.h"
#include "../Shared/crypto++/base64.h"
#include "../Shared/crypto++/files.h"

using namespace CryptoPP;
using namespace std;

int main() {
	// Keys
	AutoSeededRandomPool prng;
	InvertibleRSAFunction privKey;
	privKey.Initialize(prng, 1024);
	RSAFunction pubKey(privKey);

	Integer n(privKey.GetModulus()), e(pubKey.GetPublicExponent()), d(privKey.GetPrivateExponent());

	std::cout << "n: 0x" << n << endl;
	std::cout << "e: 0x" << e << endl;
	std::cout << "d: 0x" << d << endl;

	// Encryption
	std::string message = "secret";
	CryptoPP::Integer m((const byte *)message.data(), message.size());
	std::cout << "m: " << m << std::endl;
	// m: 126879297332596.

	CryptoPP::Integer c = pubKey.ApplyFunction(m);
	std::cout << "c: " << std::hex << c << std::endl;
	// c: 3f47c32e8e17e291h

	// Decryption
	CryptoPP::Integer r = privKey.CalculateInverse(prng, c);
	std::cout << "r: " << std::hex << r << std::endl;

	// r: 736563726574h
	std::string recovered;
	recovered.resize(r.MinEncodedSize());

	r.Encode((byte *)recovered.data(), recovered.size());
	std::cout << "recovered: " << recovered << std::endl;

	// recovered: secret
	return 0;
}