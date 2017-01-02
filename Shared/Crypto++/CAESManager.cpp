#include "CAESManager.h"
#include "../Shared/Crypto++/modes.h"
#include "../Shared/Crypto++/aes.h"
#include "../Shared/Crypto++/filters.h"
#include <iostream>
#include <string>

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
	CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length() + 1);
	stfEncryptor.MessageEnd();

	return ciphertext;
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

	std::string plaintext;

	CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(plaintext));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
	stfDecryptor.MessageEnd();

	return plaintext;
}