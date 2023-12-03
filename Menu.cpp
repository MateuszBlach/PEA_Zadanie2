#include "Menu.h"
#include <iostream>

using namespace std;

Menu::Menu() {}

Menu::~Menu() {}

void Menu::displayMainMenu() {
	cout << "Glowne menu" << endl;
	cout << "1. Wczytaj dane z pliku" << endl;
	cout << "2. Wczytaj dane z pliku XML" << endl;
	cout << "3. Podaj kryterium stopu" << endl;
	cout << "4. Podaj wspolczynnik zmiany temperatury" << endl;
	cout << "5. Uruchom algorytm - Symulowane Wyzarzanie" << endl;
	cout << "6. Zapisz sciezke do pliku txt" << endl;
	cout << "7. Wczytaj sciezke z pliku txt i oblicz droge" << endl;
	cout << "0. Zakoncz program" << endl;
	cout << "Wybor: ";

}