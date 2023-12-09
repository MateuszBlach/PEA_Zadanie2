#include <iostream>
#include "Menu.h"
#include "Matrix.h"
#include <chrono>
#include <math.h>
#include "MyFunctions.h"
#include <random>
#include <fstream>


using namespace std;
using namespace std::chrono;

int mainMenuChoice;
Menu menu = Menu();
//Zmienie i obiekty potrzebne do wykonania algorytmu
Matrix matrix = Matrix();

int* bestPath;
int bestPathLength;


string savePath;

//kryterium stopu
int stop;

//wspolczynnik zmiany temperatury
double alpha;

void manageMainMenu();

int* generateStartingPath();
double calculateStartingTemperature();
void simulatedAnnealingAlgorithm(double temperature,int* startingPath, int startingPathLength);
void savePathToFile();
void readPathFromFile(string filePath);

int opt;

int main()
{
	srand(time(NULL));

    do {
        menu.displayMainMenu();
        manageMainMenu();
    } while (mainMenuChoice != 0);
    menu.~Menu();
}

void manageMainMenu() {
	string path;
	string readPath;
	int fileChoice;

	int* startingPath;
	int startingPathLength;


	double temperature;
	double bladWzgledny;

	cin >> mainMenuChoice;
	switch (mainMenuChoice) {
	case 1:
		//Wczytanie z pliku
		menu.displayFileMenu();
		cin >> fileChoice;

		switch (fileChoice) {
			case 1:
				matrix.loadFromFile("testing_files/br17.atsp");
				savePath = "testing_files/sa_br17.txt";
				break;
			case 2:
				matrix.loadFromFile("testing_files/ftv55.atsp");
				savePath = "testing_files/sa_ftv55.txt";
				opt = 1608;
				break;
			case 3:
				matrix.loadFromFile("testing_files/ftv170.atsp");
				savePath = "testing_files/sa_ftv170.txt";
				opt = 2755;
				break;
			case 4:
				matrix.loadFromFile("testing_files/rbg358.atsp");
				savePath = "testing_files/sa_rbg358.txt";
				opt = 1163;
				break;
			case 5:
				cout << "Podaj sciezke do pliku" << endl;
				cin >> path;
				savePath = "sa_" + path;
				matrix.loadFromFile(path);
				break;
			default:
				break;
		}
		//if(fileChoice) matrix.display();
		break;
	case 2:
		//Podanie kryterium stopu
		cout << "Podaj kryterium stopu w minutach " << endl;
		cin >> stop;
		break;
	case 3:
		//Podanie wspolczynnika zmiany temperatury
		cout << "Podaj wspolczynnik zmiany temperatury" << endl;
		cin >> alpha;
		break;
	case 4:
		//Uruchomienie algorytmu 

		//Wygenerowanie sciezki startowej za pomoca metody zachlannej
		startingPath = generateStartingPath();
		startingPathLength = 0;
		for (int i = 0; i < matrix.numberOfCities; i++) {
			startingPathLength += matrix.matrix[startingPath[i]][startingPath[i+1]];
		}
		cout << "Dlugosc sciezki startowej: " << startingPathLength << endl;
		for (int i = 0; i < matrix.numberOfCities; i++) {
			cout << startingPath[i] << "->";
		}
		cout << startingPath[matrix.numberOfCities] << endl;

		//Obliczenie temperatury startowej
		temperature = calculateStartingTemperature();

		//Algorytm
		simulatedAnnealingAlgorithm(temperature, startingPath,startingPathLength);
		cout << "Dlugosc sciezki wyliczonej: " << bestPathLength << endl;

		for (int i = 0; i < matrix.numberOfCities; i++) {
			cout << bestPath[i] << "->";
		}
		cout << bestPath[matrix.numberOfCities] << endl;

		bladWzgledny = abs(bestPathLength - opt) / (double)opt;
		cout << "Blad wzgledny " << bladWzgledny << endl;
		break;
	case 5:
		//Zapis sciezki do pliku txt
		savePathToFile();
		break;
	case 6:
		//Wczytanie sciezki z pliku txt i obliczenie drogi
		cout << "Podaj sciezke do pliku: ";
		cin >> readPath;
		readPathFromFile(readPath);


		break;
	default:
		break;
	}
}


int* generateStartingPath() {
	bool* visited = new bool[matrix.numberOfCities];
	int* startingBest = new int[matrix.numberOfCities + 1];
	int currentCity = 0;

	for (int i = 0; i < matrix.numberOfCities; i++) {
		visited[i] = false;
	}
	visited[currentCity] = true;

	startingBest[0] = currentCity;

	for (int i = 0; i < matrix.numberOfCities - 1; i++) {
		int best = INT_MAX;
		int nextCity = -1;

		for (int j = 0; j < matrix.numberOfCities; j++) {
			if (!visited[j]) {
				if (matrix.matrix[startingBest[i]][j] < best) {
					best = matrix.matrix[startingBest[i]][j];
					nextCity = j;
				}
			}
		}

		if (nextCity != -1) {
			visited[nextCity] = true;
			startingBest[i + 1] = nextCity;
		}
	}

	startingBest[matrix.numberOfCities] = startingBest[0]; // Zamknięcie cyklu
	delete[] visited;

	return startingBest;
}

double calculateStartingTemperature() {
	int max = -1;
	int min = INT_MAX;

	for (int i = 0; i < matrix.numberOfCities; i++) {
		for (int j = 0; j < matrix.numberOfCities; j++) {
			if (i != j) {
				int current = matrix.matrix[i][j];
				if (current < min) {
					min = current;
				}
				if (current > max) {
					max = current;
				}
			}
		}
	}
	double minD = (double)min;
	double maxD = (double)max;

	return (maxD - minD) / (minD+1.0);
}

void simulatedAnnealingAlgorithm(double temperature, int* startingPath, int startingPathLength) {
	steady_clock::time_point start = steady_clock::now();
	steady_clock::duration duration;

	bestPath = startingPath;
	bestPathLength = startingPathLength;


	int temp = 0;
	do {
		//zamiana losowych miast
		int v1, v2;
		do {
			v1 = generateRandomInt(1, matrix.numberOfCities-1);
			v2 = generateRandomInt(1, matrix.numberOfCities-1);
		} while (
			v1 == v2 
			);

		 
		int* newPath = new int[matrix.numberOfCities+1];

		for (int i = 0; i <= matrix.numberOfCities; i++) {
			newPath[i] = bestPath[i];
			if (newPath[i] != bestPath[i]) cout << "NIE " << endl;
		}

		int temporary = newPath[v2];
		newPath[v2] = newPath[v1];
		newPath[v1] = temporary;

		//obliczenie nowo powstałej trasy
		int newPathLength = 0;
		for (int i = 0; i < matrix.numberOfCities; i++) {
			newPathLength += matrix.matrix[newPath[i]][newPath[i + 1]];
		}
		//cout << v1 << " " << v2 << " " << newPathLength << endl;

		//decyzja czy zmiana nastąpi
		if (newPathLength < bestPathLength) {
			for (int i = 0; i <= matrix.numberOfCities; i++) {
				bestPath[i] = newPath[i];
			}
			bestPathLength = newPathLength;
		}
		else {
			double probability = exp(-abs(newPathLength - bestPathLength) / temperature);

			random_device rd;
			mt19937 gen(rd());
			uniform_real_distribution<double> dis(0.0, 1.0);
			double randomDouble = dis(gen);

			//cout << "randD: " << randomDouble << " prob: " << probability << endl;
			if (randomDouble < probability) {
				for (int i = 0; i <= matrix.numberOfCities; i++) {
					bestPath[i] = newPath[i];
				}
				bestPathLength = newPathLength;
			}
		}

		//schładzanie
		temperature *= alpha;
		//cout << "temp " << temperature << endl;
		duration = steady_clock::now() - start;
	} while (duration_cast<seconds>(duration).count() < static_cast<long long>(stop) * 1);

	
}

void savePathToFile() {
	ofstream outfile(savePath, std::ios::out | std::ios::app);
	if (outfile.is_open()) {
		outfile << matrix.numberOfCities;
		for (int i = 0; i <= matrix.numberOfCities; i++) {
			outfile << " "+bestPath[i];
		}
	}
	else {
		std::cout << "Nie mozna otworzyc pliku." << endl;
	}
}

void readPathFromFile(string filePath) {
	ifstream  myFile(filePath);
	if (myFile.fail()) {
		cout << "Podano bledna sciezke do pliku!" << endl;
	}
	else {
		bestPath = NULL;
		bestPathLength = 0;

		int len;
		myFile >> len;
		bestPath = new int[len+1];

		for (int i = 0; i <= len; i++) {
			myFile >> bestPath[i];

		}


		myFile.close();
	}
}


