#include <iostream>
#include "Menu.h"
#include "Matrix.h"


using namespace std;

int mainMenuChoice;

//Zmienie i obiekty potrzebne do wykonania algorytmu
Matrix matrix = Matrix();
int* minPath;
int minPathLength;

void manageMainMenu();

int main()
{
    Menu menu = Menu();
    do {
        menu.displayMainMenu();
        manageMainMenu();
    } while (mainMenuChoice != 0);
    menu.~Menu();
}

void manageMainMenu() {
	cin >> mainMenuChoice;
	switch (mainMenuChoice) {
	case 1:
		//Wczytanie z pliku
		break;
	case 2:
		//Wczytanie z pliku XML
		break;
	case 3:
		//Podanie kryterium stopu
		break;
	case 4:
		//Podanie wspolczynnika zmiany temperatury
		break;
	case 5:
		//Uruchomienie algorytmu 
		break;
	case 6:
		//Zapis sciezki do pliku txt
		break;
	case 7:
		//Wczytanie sciezki z pliku txt i obliczenie drogi
		break;
	default:
		break;
	}
}
