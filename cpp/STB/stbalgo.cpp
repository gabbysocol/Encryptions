#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..//Common/Common.h"
#include "stbalgo.h"

// 4 режима шифрования, описанные
// в стандарте СТБ 34.101.31-2011 
enum mode {
	PLAIN,
	BLOCK,
	GAMMING,
	COUNTER
};

#define U8V(v)      ((unsigned char)(v)  & 0xFFU)
#define U16V(v)     ((unsigned short)(v) & 0xFFFFU)
#define U32V(v)     ((unsigned int)(v) & 0xFFFFFFFFUL)

#define ROTL8(v, n) (U8V((v) << (n)) | ((v) >> (8 - (n))))
#define ROTL16(v, n) (U16V((v) << (n)) | ((v) >> (16 - (n))))
#define ROTL32(v, n) (U32V((v) << (n)) | ((v) >> (32 - (n))))

#define ROTR8(v, n) ROTL8(v, 8 - (n))
#define ROTR16(v, n) ROTL16(v, 16 - (n))
#define ROTR32(v, n) ROTL32(v, 32 - (n))

#define HU1(x,H) ((unsigned int) ((H)[ (((x) >> 24) & 0xff) ]) << 24 )
#define HU2(x,H) ((unsigned int) ((H)[ (((x) >> 16) & 0xff) ]) << 16 )
#define HU3(x,H) ((unsigned int) ((H)[ (((x) >> 8) & 0xff) ]) << 8 )
#define HU4(x,H) ((unsigned int) ((H)[ (((x) >> 0) & 0xff) ]) << 0 )
#define G(x,H,r) ROTL32(HU4((x),(H)) | HU3((x),(H)) | HU2((x),(H)) | HU1((x),(H)),(r))
#define SWAP(x,y,tmp) (tmp) = (x); (x) = (y); (y) = (tmp);

using namespace std;

// Битовые преобразования в Int
static unsigned int load32(const void* in)
{
	const unsigned char* p = (const unsigned char*)in;
	return ((unsigned int)p[0] << 0) |
		((unsigned int)p[1] << 8) |
		((unsigned int)p[2] << 16) |
		((unsigned int)p[3] << 24);
}

// Битовые преобразования из Int
static void store32(void* out, const unsigned int v)
{
	unsigned char* p = (unsigned char*)out;
	p[0] = (unsigned char)(v >> 0);
	p[1] = (unsigned char)(v >> 8);
	p[2] = (unsigned char)(v >> 16);
	p[3] = (unsigned char)(v >> 24);
}

// Перестановка, используемая 
// на каждом раунде шифрования
static const unsigned char H[256] =
{
	0xB1, 0x94, 0xBA, 0xC8, 0x0A, 0x08, 0xF5, 0x3B, 0x36, 0x6D, 0x00, 0x8E, 0x58, 0x4A, 0x5D, 0xE4,
	0x85, 0x04, 0xFA, 0x9D, 0x1B, 0xB6, 0xC7, 0xAC, 0x25, 0x2E, 0x72, 0xC2, 0x02, 0xFD, 0xCE, 0x0D,
	0x5B, 0xE3, 0xD6, 0x12, 0x17, 0xB9, 0x61, 0x81, 0xFE, 0x67, 0x86, 0xAD, 0x71, 0x6B, 0x89, 0x0B,
	0x5C, 0xB0, 0xC0, 0xFF, 0x33, 0xC3, 0x56, 0xB8, 0x35, 0xC4, 0x05, 0xAE, 0xD8, 0xE0, 0x7F, 0x99,
	0xE1, 0x2B, 0xDC, 0x1A, 0xE2, 0x82, 0x57, 0xEC, 0x70, 0x3F, 0xCC, 0xF0, 0x95, 0xEE, 0x8D, 0xF1,
	0xC1, 0xAB, 0x76, 0x38, 0x9F, 0xE6, 0x78, 0xCA, 0xF7, 0xC6, 0xF8, 0x60, 0xD5, 0xBB, 0x9C, 0x4F,
	0xF3, 0x3C, 0x65, 0x7B, 0x63, 0x7C, 0x30, 0x6A, 0xDD, 0x4E, 0xA7, 0x79, 0x9E, 0xB2, 0x3D, 0x31,
	0x3E, 0x98, 0xB5, 0x6E, 0x27, 0xD3, 0xBC, 0xCF, 0x59, 0x1E, 0x18, 0x1F, 0x4C, 0x5A, 0xB7, 0x93,
	0xE9, 0xDE, 0xE7, 0x2C, 0x8F, 0x0C, 0x0F, 0xA6, 0x2D, 0xDB, 0x49, 0xF4, 0x6F, 0x73, 0x96, 0x47,
	0x06, 0x07, 0x53, 0x16, 0xED, 0x24, 0x7A, 0x37, 0x39, 0xCB, 0xA3, 0x83, 0x03, 0xA9, 0x8B, 0xF6,
	0x92, 0xBD, 0x9B, 0x1C, 0xE5, 0xD1, 0x41, 0x01, 0x54, 0x45, 0xFB, 0xC9, 0x5E, 0x4D, 0x0E, 0xF2,
	0x68, 0x20, 0x80, 0xAA, 0x22, 0x7D, 0x64, 0x2F, 0x26, 0x87, 0xF9, 0x34, 0x90, 0x40, 0x55, 0x11,
	0xBE, 0x32, 0x97, 0x13, 0x43, 0xFC, 0x9A, 0x48, 0xA0, 0x2A, 0x88, 0x5F, 0x19, 0x4B, 0x09, 0xA1,
	0x7E, 0xCD, 0xA4, 0xD0, 0x15, 0x44, 0xAF, 0x8C, 0xA5, 0x84, 0x50, 0xBF, 0x66, 0xD2, 0xE8, 0x8A,
	0xA2, 0xD7, 0x46, 0x52, 0x42, 0xA8, 0xDF, 0xB3, 0x69, 0x74, 0xC5, 0x51, 0xEB, 0x23, 0x29, 0x21,
	0xD4, 0xEF, 0xD9, 0xB4, 0x3A, 0x62, 0x28, 0x75, 0x91, 0x14, 0x10, 0xEA, 0x77, 0x6C, 0xDA, 0x1D
};

// Подстановка индексов ключа
static const size_t KeyIndex[8][7] =
{
	{ 0, 1, 2, 3, 4, 5, 6 },
	{ 7, 0, 1, 2, 3, 4, 5 },
	{ 6, 7, 0, 1, 2, 3, 4 },
	{ 5, 6, 7, 0, 1, 2, 3 },
	{ 4, 5, 6, 7, 0, 1, 2 },
	{ 3, 4, 5, 6, 7, 0, 1 },
	{ 2, 3, 4, 5, 6, 7, 0 },
	{ 1, 2, 3, 4, 5, 6, 7 }
};

// Алгоритм расширения ключа, предложенный в стандарте
void belt_init(unsigned char* ks, const unsigned char* k, size_t klen)
{
	size_t i;
	switch (klen)
	{
	case 16:
		for (i = 0; i < 16; ++i)
		{
			ks[i + 0] = k[i];
			ks[i + 16] = k[i];
		}
		break;

	case 24:
		for (i = 0; i < 24; ++i)
		{
			ks[i] = k[i];
		}
		store32(ks + 24, load32(k + 0) ^ load32(k + 4) ^ load32(k + 8));
		store32(ks + 28, load32(k + 12) ^ load32(k + 16) ^ load32(k + 20));
		break;

	case 32:
		for (i = 0; i < 32; ++i)
		{
			ks[i] = k[i];
		}
		break;
	}
}

void belt_encrypt(unsigned char* out, const unsigned char* in, const unsigned char* ks)
{
	unsigned int a = load32(in + 0);
	unsigned int b = load32(in + 4);
	unsigned int c = load32(in + 8);
	unsigned int d = load32(in + 12);
	unsigned int e;
	size_t i;
	unsigned int tmp;
	unsigned int key[8] = { 0 };

	for (i = 0; i < 8; ++i)
	{
		key[i] = load32(ks + (4 * i));
	}

	for (i = 0; i < 8; ++i)
	{
		b ^= G((a + key[KeyIndex[i][0]]), H, 5);
		c ^= G((d + key[KeyIndex[i][1]]), H, 21);
		a -= G((b + key[KeyIndex[i][2]]), H, 13);
		e = (G((b + c + key[KeyIndex[i][3]]), H, 21) ^ (unsigned int)(i + 1));
		b += e;
		c -= e;
		d += G((c + key[KeyIndex[i][4]]), H, 13);
		b ^= G((a + key[KeyIndex[i][5]]), H, 21);
		c ^= G((d + key[KeyIndex[i][6]]), H, 5);
		SWAP(a, b, tmp);
		SWAP(c, d, tmp);
		SWAP(b, c, tmp);
	}
	store32(out + 0, b);
	store32(out + 4, d);
	store32(out + 8, a);
	store32(out + 12, c);
}

void belt_decrypt(unsigned char* out, const unsigned char* in, const unsigned char* ks)
{
	unsigned int a = load32(in + 0);
	unsigned int b = load32(in + 4);
	unsigned int c = load32(in + 8);
	unsigned int d = load32(in + 12);
	unsigned int e;
	size_t i;
	unsigned int tmp;
	unsigned int key[8] = { 0 };

	for (i = 0; i < 8; ++i)
	{
		key[i] = load32(ks + (4 * i));
	}

	for (i = 0; i < 8; ++i)
	{
		b ^= G((a + key[KeyIndex[7 - i][6]]), H, 5);
		c ^= G((d + key[KeyIndex[7 - i][5]]), H, 21);
		a -= G((b + key[KeyIndex[7 - i][4]]), H, 13);
		e = (G((b + c + key[KeyIndex[7 - i][3]]), H, 21) ^ (unsigned int)(7 - i + 1));
		b += e;
		c -= e;
		d += G((c + key[KeyIndex[7 - i][2]]), H, 13);
		b ^= G((a + key[KeyIndex[7 - i][1]]), H, 21);
		c ^= G((d + key[KeyIndex[7 - i][0]]), H, 5);
		SWAP(a, b, tmp);
		SWAP(c, d, tmp);
		SWAP(a, d, tmp);
	}
	store32(out + 0, c);
	store32(out + 4, a);
	store32(out + 8, d);
	store32(out + 12, b);
}

static void store128(void* out, const unsigned char* ins)
{
	unsigned char* p = (unsigned char*)out;
	for (size_t i = 0; i < 16; ++i) {
		p[i] = ins[i];
	}
}

// Режим простой замены
void encrypt_plain(unsigned char* outenc, const unsigned char* inenc, const unsigned char* ks, const int len) {
	unsigned char buffer[16] = { 0 };
	unsigned char outchunk[16] = { 0 };
	for (size_t i = 0; i < (len + 15) / 16; ++i) {
		store128(buffer, inenc + i * 16);
		belt_encrypt(outchunk, buffer, ks);
		store128(outenc + i * 16, outchunk);
	}
}

void decrypt_plain(unsigned char* outdec, const unsigned char* outenc, const unsigned char* ks, const int len) {
	unsigned char buffer[16] = { 0 };
	unsigned char outchunk[16] = { 0 };
	for (size_t i = 0; i < (len + 15) / 16; ++i) {
		store128(buffer, outenc + i * 16);
		belt_decrypt(outchunk, buffer, ks);
		store128(outdec + i * 16, outchunk);
	}
}

// Режим сцепления блоков
void encrypt_block(unsigned char* outenc, const unsigned char* inenc, const unsigned char* ks, const int len, const unsigned char* s) {
	unsigned char buffer[16] = { 0 };
	unsigned char outchunk[16] = { 0 };
	unsigned char synhro[16] = { 0 };
	belt_encrypt(synhro, s, ks);
	for (size_t i = 0; i < (len + 15) / 16; ++i) {
		store128(buffer, inenc + i * 16);

		for (size_t j = 0; j < 16; ++j) {
			buffer[j] = buffer[j] ^ synhro[j];
		}
		belt_encrypt(outchunk, buffer, ks);

		store128(outenc + i * 16, outchunk);
		store128(synhro, outchunk);
	}
}

void decrypt_block(unsigned char* outdec, const unsigned char* outenc, const unsigned char* ks, const int len, const unsigned char* s) {
	unsigned char buffer[16] = { 0 };
	unsigned char outchunk[16] = { 0 };
	unsigned char synhro[16] = { 0 };
	belt_encrypt(synhro, s, ks);
	for (size_t i = 0; i < (len + 15) / 16; ++i) {
		store128(buffer, outenc + i * 16);

		belt_decrypt(outchunk, buffer, ks);
		for (size_t j = 0; j < 16; ++j) {
			outchunk[j] = outchunk[j] ^ synhro[j];
		}

		store128(outdec + i * 16, outchunk);
		store128(synhro, buffer);
	}
}

// Режим гаммирования с обратной связью
void encrypt_gamming(unsigned char* outenc, const unsigned char* inenc, const unsigned char* ks, const int len, const unsigned char* s) {
	unsigned char buffer[16] = { 0 };
	unsigned char outchunk[16] = { 0 };
	unsigned char synhro[16] = { 0 };
	unsigned char ns[16] = { 0 };
	store128(ns, s);
	for (size_t i = 0; i < (len + 15) / 16; ++i) {
		belt_encrypt(synhro, ns, ks);
		store128(buffer, inenc + i * 16);

		for (size_t j = 0; j < 16; ++j) {
			outchunk[j] = buffer[j] ^ synhro[j];
		}

		store128(outenc + i * 16, outchunk);
		store128(ns, outchunk);
	}
}

void decrypt_gamming(unsigned char* outdec, const unsigned char* outenc, const unsigned char* ks, const int len, const unsigned char* s) {
	unsigned char buffer[16] = { 0 };
	unsigned char outchunk[16] = { 0 };
	unsigned char synhro[16] = { 0 };
	unsigned char ns[16] = { 0 };
	store128(ns, s);
	for (size_t i = 0; i < (len + 15) / 16; ++i) {
		belt_encrypt(synhro, ns, ks);
		store128(buffer, outenc + i * 16);

		for (size_t j = 0; j < 16; ++j) {
			outchunk[j] = buffer[j] ^ synhro[j];
		}

		store128(outdec + i * 16, outchunk);
		store128(ns, buffer);
	}
}

// Операция, определенная в стандарте, используемая на каждом
// раунде зашифрования и расшифрования с ключом
static void square_plus(void* out, const unsigned char* ins)
{
	unsigned char* p = (unsigned char*)out;
	for (size_t i = 15; ; i = (i + 15) % 16) {
		if (p[i] == (unsigned char)((1 << 8) - 1)) p[i] = 0;
		else {
			++p[i];
			break;
		}
	}
}

// Режим счетчика
void encrypt_counter(unsigned char* outenc, const unsigned char* inenc, const unsigned char* ks, const int len, const unsigned char* s) {
	unsigned char buffer[16] = { 0 };
	unsigned char outchunk[16] = { 0 };
	unsigned char synhro[16] = { 0 };
	unsigned char ns[16] = { 0 };
	belt_encrypt(ns, s, ks);
	for (size_t i = 0; i < (len + 15) / 16; ++i) {
		belt_encrypt(synhro, ns, ks);
		square_plus(ns, synhro);
		store128(buffer, inenc + i * 16);

		for (size_t j = 0; j < 16; ++j) {
			outchunk[j] = buffer[j] ^ ns[j];
		}

		store128(outenc + i * 16, outchunk);
	}
}

void decrypt_counter(unsigned char* outdec, const unsigned char* outenc, const unsigned char* ks, const int len, const unsigned char* s) {
	encrypt_counter(outdec, outenc, ks, len, s);
}

// Зашифрование файла в выбранном режиме
void encrypt_file(mode m, const unsigned char* keyenc, const unsigned char* synhro,
	const char* init_text_file, const char* encrypted_text_file) {
	
	std::string input = read_secret_file();
	
	unsigned int n = (unsigned int)input.length();
	unsigned char* inenc = (unsigned char *)input.c_str();

	unsigned char* outenc = new unsigned char[n] { 0 };
	unsigned char ks[32] = { 0 };

	belt_init(ks, keyenc, 32);
	switch (m)
	{
	case PLAIN:
		encrypt_plain(outenc, inenc, ks, n);
		break;
	case BLOCK:
		encrypt_block(outenc, inenc, ks, n, synhro);
		break;
	case GAMMING:
		encrypt_gamming(outenc, inenc, ks, n, synhro);
		break;
	case COUNTER:
		encrypt_counter(outenc, inenc, ks, n, synhro);
		break;
	}

	string output(n, 0);

	for (auto i = 0; i < n; ++i) {
		output[i] = outenc[i];
	}

	write_to_encrypted_file(output);
}

// Расшифрование файла в выбранном режиме
void decrypt_file(mode m, const unsigned char* keyenc, const unsigned char* synhro,
	const char* encrypted_text_file, const char* decrypted_text_file) {
	string input = read_encrypted_file();
	
	unsigned int n = (unsigned int)input.length();
	unsigned char* outenc = (unsigned char*)input.c_str();

	unsigned char* outdec = new unsigned char[n] { 0 };
	unsigned char ks[32] = { 0 };

	belt_init(ks, keyenc, 32);
	switch (m)
	{
	case PLAIN:
		decrypt_plain(outdec, outenc, ks, n);
		break;
	case BLOCK:
		decrypt_block(outdec, outenc, ks, n, synhro);
		break;
	case GAMMING:
		decrypt_gamming(outdec, outenc, ks, n, synhro);
		break;
	case COUNTER:
		decrypt_counter(outdec, outenc, ks, n, synhro);
		break;
	}

	string output(n, 0);

	for (auto i = 0; i < n; ++i) {
		output[i] = outdec[i];
	}

	write_to_decrypted_file(output);
}

// ---- Обертка алгоритмов классом STB

void STB::encrypt_plain() {
	encrypt_file(PLAIN, this->keyenc, this->synhro, SECRET_FILE.c_str(), ENCRYPTED_FILE.c_str());
}

void STB::encrypt_block() {
	encrypt_file(BLOCK, this->keyenc, this->synhro, SECRET_FILE.c_str(), ENCRYPTED_FILE.c_str());
}

void STB::encrypt_counter() {
	encrypt_file(COUNTER, this->keyenc, this->synhro, SECRET_FILE.c_str(), ENCRYPTED_FILE.c_str());
}

void STB::encrypt_gamming() {
	encrypt_file(GAMMING, this->keyenc, this->synhro, SECRET_FILE.c_str(), ENCRYPTED_FILE.c_str());
}

void STB::decrypt_plain() {
	decrypt_file(PLAIN, this->keyenc, this->synhro, ENCRYPTED_FILE.c_str(), DECRYPTED_FILE.c_str());
}

void STB::decrypt_block() {
	decrypt_file(BLOCK, this->keyenc, this->synhro, ENCRYPTED_FILE.c_str(), DECRYPTED_FILE.c_str());
}

void STB::decrypt_counter() {
	decrypt_file(COUNTER, this->keyenc, this->synhro, ENCRYPTED_FILE.c_str(), DECRYPTED_FILE.c_str());
}

void STB::decrypt_gamming() {
	decrypt_file(GAMMING, this->keyenc, this->synhro, ENCRYPTED_FILE.c_str(), DECRYPTED_FILE.c_str());
}

STB::STB(const std::string keyenc, const std::string synhro) {
	this->keyenc = (unsigned char*)keyenc.c_str();
	this->synhro = (unsigned char*)synhro.c_str();
}