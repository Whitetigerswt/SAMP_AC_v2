#include "CAESManager.h"
#include "../Shared/Crypto++/modes.h"
#include "../Shared/Crypto++/aes.h"
#include "../Shared/Crypto++/filters.h"
#include <iostream>
#include <string>

#include "../Shared/Crypto++/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "../Shared/Crypto++/cryptlib.h"
using CryptoPP::Exception;

#include "../Shared/Crypto++/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

using CryptoPP::AES;

#include "ccm.h"
using CryptoPP::CBC_Mode;

std::string CAESManager::Encrypt(std::string plaintext)
{
	//Key and IV setup
	//AES encryption uses a secret key of a variable length (128-bit, 196-bit or 256-   
	//bit). This key is secretly exchanged between two parties before communication   
	//begins. DEFAULT_KEYLENGTH= 16 bytes
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
	memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

	std::string ciphertext;

	//
	// Create Cipher Text
	//

	CBC_Mode< AES >::Encryption e;
	e.SetKeyWithIV(key, sizeof(key), iv);

	// The StreamTransformationFilter removes
	//  padding as required.
	StringSource s(plaintext, true,
		new StreamTransformationFilter(e,
			new StringSink(ciphertext)
		) // StreamTransformationFilter
	); // StringSource

	string encoded;

	StringSource(ciphertext, true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource

	return encoded;
}

std::string CAESManager::Decrypt(std::string ciphertext)
{
	//Key and IV setup
	//AES encryption uses a secret key of a variable length (128-bit, 196-bit or 256-   
	//bit). This key is secretly exchanged between two parties before communication   
	//begins. DEFAULT_KEYLENGTH= 16 bytes
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
	memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

	CBC_Mode< AES >::Decryption d;
	d.SetKeyWithIV(key, sizeof(key), iv);

	string recovered;

	// The StreamTransformationFilter removes
	//  padding as required.
	StringSource s(ciphertext, true,
		new StreamTransformationFilter(d,
			new HexEncoder(
				new StringSink(recovered)
			)
		) // StreamTransformationFilter
	); // StringSource


	return recovered;
}