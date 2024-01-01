#include <iostream>
#include "Menu.h"
#include "Matrix.h"
#include <chrono>
#include <math.h>
#include "MyFunctions.h"
#include <random>
#include <fstream>
#include <vector>


using namespace std;
using namespace std::chrono;

int mainMenuChoice;
Menu menu = Menu();
//Zmienie i obiekty potrzebne do wykonania algorytmu
Matrix matrix = Matrix();

int* bestPath;
int bestPathLength;
long long timeToFindBest;

string savePath;

//kryterium stopu
int stop;

//wspolczynnik zmiany temperatury
double alpha;

//zmienne potrzebne do wykresow
vector<long long> timeVector;
vector<double> bladVector;

void manageMainMenu();

int* generateStartingPath();
double calculateStartingTemperature();
double simulatedAnnealingAlgorithm(double temperature,int* startingPath, int startingPathLength);
void savePathToFile(int* path);
void readPathFromFile(string filePath);

void test();

void testBladWzgledny();

int opt;

int main()
{
	srand(time(NULL));

    do {
        menu.displayMainMenu();
        manageMainMenu();
    } while (mainMenuChoice != 0);
    menu.~Menu();

	delete[] bestPath;
	return 0;
}

void manageMainMenu() {
	string path;
	string readPath;
	int fileChoice;

	int* startingPath = nullptr;
	int startingPathLength;


	double temperature;
	double bladWzgledny;
	double wyrazenieTemperatury;

	int numberOfCities;

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
		cout << "Podaj kryterium stopu w sekundach " << endl;
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
		temperature = simulatedAnnealingAlgorithm(temperature, startingPath,startingPathLength);
		cout << "Dlugosc sciezki wyliczonej: " << bestPathLength << endl;

		for (int i = 0; i < matrix.numberOfCities; i++) {
			cout << bestPath[i] << "->";
		}
		cout << bestPath[matrix.numberOfCities] << endl;
		cout << "Czas potrzebny na znalezienie rozwiazania: " << timeToFindBest/1000000 << "s" << endl;

		bladWzgledny = abs(bestPathLength - opt) / (double)opt;
		cout << "Blad wzgledny " << bladWzgledny << "%" << endl;
		cout << "Temperatura koncowa " << temperature << endl;
		wyrazenieTemperatury = exp(-1 / temperature);
		cout << "exp(-1/Tk) " << wyrazenieTemperatury << endl;
 
		break;
	case 5:
		//Zapis sciezki do pliku txt
		savePathToFile(bestPath);
		break;
	case 6:
		//Wczytanie sciezki z pliku txt i obliczenie drogi
		cout << "Podaj sciezke do pliku: ";
		cin >> readPath;
		readPathFromFile(readPath);
		
		bestPathLength = 0;
		for (int i = 0; i < matrix.numberOfCities; i++) {
			cout << bestPath[i] <<"->";
			bestPathLength += matrix.matrix[bestPath[i]][bestPath[i + 1]];
		}
		cout << bestPath[matrix.numberOfCities] << endl;
		cout << "Dlugosc wczytanej sciezki: " << bestPathLength << endl;
		break;
	case 7:
		//Testy do sprawka
		test();
	case 8:
		//Testy pod wykresy
		testBladWzgledny();
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
	// Zamknięcie cyklu
	startingBest[matrix.numberOfCities] = startingBest[0]; 
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

double simulatedAnnealingAlgorithm(double temperature, int* startingPath, int startingPathLength) {
	steady_clock::time_point start = steady_clock::now();
	steady_clock::duration duration;

	//na potrzeby stworzenia wykresow
	timeVector.clear();
	bladVector.clear();

	//na poczatek najlepsza sciezka jest sciezka startowa - wyliczona metoda zachlanna
	bestPath = startingPath;
	bestPathLength = startingPathLength;

	//sciezka najlepsza lokalnie
	int* currentBestPath = new int[matrix.numberOfCities + 1];
	for (int i = 0; i <= matrix.numberOfCities; i++) {
		currentBestPath[i] = startingPath[i];
	}
	int currentBestPathLength = startingPathLength;

	//zmienna pomocna do losowej zmiany w tabeli
	int temporary;

	do {
		//sprawdzenie czy lokalnie najlepsza sciezka jest lepsza od najlepszej globalnie
		if (currentBestPathLength < bestPathLength) {
			duration = steady_clock::now() - start;
			timeToFindBest = duration_cast<microseconds>(duration).count();
			delete[] bestPath;
			bestPath = new int[matrix.numberOfCities + 1];
			for (int i = 0; i <= matrix.numberOfCities; i++) {
				bestPath[i] = currentBestPath[i];
			}
			bestPathLength = currentBestPathLength;

			timeVector.push_back(timeToFindBest);
			bladVector.push_back((bestPathLength - opt) / (double)opt);
		}

		//losowanie indeksow
		int v1, v2;
		do {
			v1 = generateRandomInt(1, matrix.numberOfCities - 1);
			v2 = generateRandomInt(1, matrix.numberOfCities - 1);
		} while (
			v1 == v2
			);


		int* newPath = new int[matrix.numberOfCities + 1];
		int newPathLength;
		//kopia lokalnej najlepszej do nowej sciezki losowej
		for (int i = 0; i <= matrix.numberOfCities; i++) {
			newPath[i] = currentBestPath[i];
		}

		//zamiana indeksow wylosowanych
		temporary = newPath[v2];
		newPath[v2] = newPath[v1];
		newPath[v1] = temporary;


		//obliczenie nowo powstałej trasy
		newPathLength = 0;
		for (int i = 0; i < matrix.numberOfCities; i++) {
			newPathLength += matrix.matrix[newPath[i]][newPath[i + 1]];
		}

		//decyzja czy zmiana nastąpi
		if (newPathLength < currentBestPathLength) {
			delete[] currentBestPath;
			currentBestPath = new int[matrix.numberOfCities + 1];
			for (int i = 0; i <= matrix.numberOfCities; i++) {
				currentBestPath[i] = newPath[i];
			}
			currentBestPathLength = newPathLength;
		}
		else {
			double probability = exp(-abs(newPathLength - currentBestPathLength) / temperature);
			random_device rd;
			mt19937 gen(rd());
			uniform_real_distribution<double> dis(0.0, 1.0);
			double randomDouble = dis(gen);

			if (randomDouble < probability) {
				delete[] currentBestPath;
				currentBestPath = new int[matrix.numberOfCities + 1];
				for (int i = 0; i <= matrix.numberOfCities; i++) {
					currentBestPath[i] = newPath[i];
				}
				currentBestPathLength = newPathLength;
			}
		}

		delete[] newPath;

		//schładzanie
		temperature *= alpha;

		//sprawdzenie czasu
		duration = steady_clock::now() - start;
	} while (duration_cast<seconds>(duration).count() < static_cast<long long>(stop) * 1);
	return temperature;
}

void savePathToFile(int* path) {
	//ofstream outfile(savePath, std::ios::out | std::ios::app);
	ofstream outfile(savePath);
	if (outfile.is_open()) {
		outfile << matrix.numberOfCities << endl;
		for (int i = 0; i <= matrix.numberOfCities; i++) {
			outfile << path[i] << endl;;
		}
		outfile.close();
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
		bestPathLength = 0;

		int len;
		myFile >> len;
		delete[] bestPath;
		bestPath = new int[len+1];

		for (int i = 0; i <= len; i++) {
			myFile >> bestPath[i];
		}


		myFile.close();
	}
}

void test() {

	double testingAlpha[] = { 0.99999975,0.999999925};
	int stopArr[] = { 60,120,240 };
	int optArr[] = { 1608,2755,1163 };
	string filePaths[] = { "testing_files/ftv55.atsp","testing_files/ftv170.atsp","testing_files/rbg358.atsp" };
	string savePaths[] = { "testing_files/sa_ftv55.txt","testing_files/sa_ftv170.txt","testing_files/sa_rbg358.txt" };



	int* startingPath = nullptr;
	int startingPathLength;


	double temperature;
	double bladWzgledny;
	double wyrazenieTemperatury;


	for (int fileNumber = 0; fileNumber < 3; fileNumber++) {
		matrix.loadFromFile(filePaths[fileNumber]);
		savePath = savePaths[fileNumber];
		stop = stopArr[fileNumber];
		opt = optArr[fileNumber];

		int* bestForFile = new int[matrix.numberOfCities + 1];
		int bestForFileLen = INT_MAX;

		for (int alphaNumber = 0; alphaNumber < 2; alphaNumber++) {
			cout << "Testowanie alpha:" << testingAlpha[alphaNumber] << " plik " << filePaths[fileNumber] << endl;
			alpha = testingAlpha[alphaNumber];
			cout << "alpha " << alpha << endl;
			for (int k = 0; k < 10; k++) {
				
				startingPath = generateStartingPath();
				startingPathLength = 0;
				for (int i = 0; i < matrix.numberOfCities; i++) {
					startingPathLength += matrix.matrix[startingPath[i]][startingPath[i + 1]];
				}

				temperature = calculateStartingTemperature();
				temperature = simulatedAnnealingAlgorithm(temperature, startingPath, startingPathLength);
				bladWzgledny = abs(bestPathLength - opt) / (double)opt;
				wyrazenieTemperatury = exp(-1 / temperature);

				ofstream outfile("testing_files/testResults.txt", std::ios::out | std::ios::app);
				if (outfile.is_open()) {
					outfile << bestPathLength;
					outfile << ";";
					outfile << bladWzgledny;
					outfile << ";";
					outfile << temperature;
					outfile << ";";
					outfile << wyrazenieTemperatury;
					outfile << ";";
					outfile << alpha;
					outfile << ";";
					outfile << timeToFindBest;
					outfile << ";" << endl;
					outfile.close();
				}
				else {
					cout << "Nie mozna otworzyc pliku." << endl;
				}

				if (bestPathLength < bestForFileLen) {
					bestForFileLen = bestPathLength;
					delete[] bestForFile;
					bestForFile = new int[matrix.numberOfCities + 1];
					for (int i = 0; i <= matrix.numberOfCities; i++) {
						bestForFile[i] = bestPath[i];
					}
				}

			}
		}
		savePathToFile(bestForFile);
		delete[] bestForFile;
		//zapis najlepszego rozwiazania
	}

}

void testBladWzgledny() {
	double testingAlpha[] = {
		0.99999975,
		0.9999999,
		0.999999925 };
	int stopArr[] = { 60,120,240 };
	int optArr[] = { 1608,2755,1163 };
	string filePaths[] = { "testing_files/ftv55.atsp","testing_files/ftv170.atsp","testing_files/rbg358.atsp" };


	int* startingPath = nullptr;
	int startingPathLength;


	double temperature;
	double bladWzgledny;
	double wyrazenieTemperatury;


	for (int fileNumber = 0; fileNumber < 3; fileNumber++) {
		matrix.loadFromFile(filePaths[fileNumber]);
		stop = stopArr[fileNumber];
		opt = optArr[fileNumber];


		for (int alphaNumber = 0; alphaNumber < 3; alphaNumber++) {
			alpha = testingAlpha[alphaNumber];
			startingPath = generateStartingPath();
			startingPathLength = 0;
			for (int i = 0; i < matrix.numberOfCities; i++) {
				startingPathLength += matrix.matrix[startingPath[i]][startingPath[i + 1]];
			}

			temperature = calculateStartingTemperature();
			temperature = simulatedAnnealingAlgorithm(temperature, startingPath, startingPathLength);

			ofstream outfile("testing_files/testBlad.txt", std::ios::out | std::ios::app);
			if (outfile.is_open()) {
				outfile << filePaths[fileNumber] << ";" << testingAlpha[alphaNumber] << ";" << endl;
				for (int i = 0; i < bladVector.size(); i++) {
					outfile << timeVector[i] << ";";
				}
				outfile << endl << endl;
				for (int i = 0; i < bladVector.size(); i++) {
					outfile << bladVector[i] << ";";
				}
				outfile << endl;
				outfile.close();
			}
			else {
				cout << "Nie mozna otworzyc pliku." << endl;
			}
			
		}
	}
}