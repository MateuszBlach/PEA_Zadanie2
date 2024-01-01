#include "Menu.h"
#include <iostream>

using namespace std;

Menu::Menu() {}

Menu::~Menu() {}

void Menu::displayMainMenu() {
	cout << "Glowne menu" << endl;
	cout << "1. Wczytaj dane z pliku" << endl;
	cout << "2. Podaj kryterium stopu" << endl;
	cout << "3. Podaj wspolczynnik zmiany temperatury" << endl;
	cout << "4. Uruchom algorytm - Symulowane Wyzarzanie" << endl;
	cout << "5. Zapisz sciezke do pliku txt" << endl;
	cout << "6. Wczytaj sciezke z pliku txt i oblicz droge" << endl;
	cout << "7. Testy" << endl;
	cout << "8. Testy pod wykres" << endl;
	cout << "0. Zakoncz program" << endl;
	cout << "Wybor: ";
}

void Menu::displayFileMenu() {
	cout << "1. br17.atsp" << endl;
	cout << "2. ftv55.atsp" << endl;
	cout << "3. ftv170.atsp" << endl;
	cout << "4. rbg358.atsp" << endl;
	cout << "5. Inny plik" << endl;
	cout << "Wybor: ";
}