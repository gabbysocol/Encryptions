#include "des3.h"

DES3::DES3(ui64 k1, ui64 k2, ui64 k3) :
	des1(k1),
	des2(k2),
	des3(k3)
{
}

// Гениальный 3DES (3 раза DES)
ui64 DES3::encrypt(ui64 block)
{
	return des3.encrypt(des2.decrypt(des1.encrypt(block)));
}

// И расшифровочка тож гениальная
ui64 DES3::decrypt(ui64 block)
{
	return des1.decrypt(des2.encrypt(des3.decrypt(block)));
}
