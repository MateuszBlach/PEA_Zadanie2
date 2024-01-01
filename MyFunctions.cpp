#include <cstdlib>
#include <ctime>

using namespace std;

int generateRandomInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}

