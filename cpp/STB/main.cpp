#include "stbalgo.h"
#include <string>

int main() {
	std::string synchro = "verygoodsynchro.";
	std::string key = "Lorem ipsum dolor amet, consect."; //"verygoodkeyverygoodkeyverygoodke";

	STB stb = STB(key, synchro);

	stb.encrypt_gamming();
	stb.decrypt_gamming();

	return 0;
}