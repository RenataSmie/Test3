#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <istream>
#include <iostream>
#include<ctime>
#include <cstdlib>
#include <sstream>
#include <conio.h>
using namespace std;
// 20.01.2017 Renata Smietanka s16139
/*----------------------------------------------------------------*/
/*Program super - odpytywacz. - projekt koncowy
Cel - s³u¿y do nauki s³ówek przed egzaminami z jêzyków obcych.

Funkcjonalnoœci:
1. Wczytywanie pliku CSV ze s³ówkami i t³umaczeniami
2. Wybór preferencji u¿ytkownika odnoœnie t³umaczenia - czy odpytywanie z j.obcego na polski czy z polskiego na j.obcy
3. Generowanie quizu(losowanie podzbioru s³ówek do t³umaczenia)
4. Automatyczna ocena quizu
5. Pokazanie pytañ z b³ednymi odpowiedziami oraz wskazanie poprawnych
6. Mo¿liwoœæ ponownego wykonania quizu po jego zakoñczeniu
/*----------------------------------------------------------------*/

//struktura tlumaczenie zawierajaca slowko z j.polskiego i z j.obcego
struct tlumaczenie
{
	string slowo1, slowo2;
};

tlumaczenie tworz_tlumaczenie(string wejscie){ 
	int pozycja_przecinka = wejscie.find(',');
	tlumaczenie wynik;
	wynik.slowo1 = wejscie.substr(0, pozycja_przecinka);
	wynik.slowo2 = wejscie.substr(pozycja_przecinka + 1, wejscie.length());
	return wynik;
}

bool wczytaj_plik(string nazwaPliku, vector<tlumaczenie> *wsk_na_wektor);

// funkcja, ktora stwarza zestaw testowy z poszeczegolnymi tlumaczeniami i je losuje 
vector<tlumaczenie> stworz_zestaw_testowy(vector<tlumaczenie>* dane, int ile_pytan){ 
	vector<tlumaczenie> wynik;
	for (int i = 0; i < ile_pytan; ++i){
		wynik.push_back(
			(*dane)[ // losowanie z wektora dane
				rand() % ((*dane).size())
			]);
		//cout << wynik[i].slowo1 <<" "<< wynik[i].slowo2 << "\n"; // aby uzytkownik nie widzial wczytanych slow
	}
	return wynik;
}

// wykorzystywane do wypisywania poprawnej odpowiedzi
string formatuj_tlumaczenie(tlumaczenie tl, int kierunek){
	stringstream ss; // strumien do napisow (strumieñ ³añcuchów)
	if (kierunek == 0)
		ss << tl.slowo1 << "->" << tl.slowo2; // wysylamy to do strumienia - doklejam na sam koniec ³añcucha znaków
	else // zmiany rsm
		ss << tl.slowo2 << "->" << tl.slowo1;
	return ss.str(); //zwracamy zawartosc strumienia (zwraca stringa, którego on zawiera)
}

int zadaj_pytanie(tlumaczenie tl, vector<pair<tlumaczenie, string> > *bledne_tlumaczenia, int kierunek){
	string odp;
	if (kierunek == 0) {
		cout << tl.slowo1 << " -> ";
		cin >> odp;
		if (odp == tl.slowo2) return 1;
		else {
			bledne_tlumaczenia->push_back(make_pair(tl, odp));
		}
	}
	else {
		cout << tl.slowo2 << " -> ";
		cin >> odp;
		if (odp == tl.slowo1) return 1;
		else {
			bledne_tlumaczenia->push_back(make_pair(tl, odp));
		}
	}
	return 0;
}

int wykonaj_quiz(
	vector<tlumaczenie> dane, // dane wszystkie, ktore mamy zamiar sprawdzic w tym quizie
	vector<pair<tlumaczenie, string> > *bledne_tlumaczenia, 
	// trzyma pare - 1. jakie byly tlumaczenia 2. jaka byla odpowiedz uzytkownika [w tym wektorze beda powkladane bledne odpowiedzi uzytkownika]
	int kierunek) {
	int poprawne_odpowiedzi = 0;
	for (int i = 0; i < dane.size(); ++i){
		poprawne_odpowiedzi += zadaj_pytanie(dane[i], bledne_tlumaczenia, kierunek);
	}
	return poprawne_odpowiedzi; 
}

void quiz(vector<tlumaczenie> *dane){
	int kierunek, ile_pytan;
	cout << "Jaki kierunek quizu wybierasz? \n";
	cout << "0 - z j.polskiego na j.obcy;\n";
	cout << "1 - z j.obcego na j.polski;\n";
	cin >> kierunek; 
	// walidacja kierunku
	while (cin.fail() || (kierunek != 0 && kierunek != 1)){
		//clear() wyczysci flage fail
		cin.clear();
		//ignore wyczysci pozostale dane znajdujace sie w buforze strumienia wejsciowego (cin) az do nowego wiersza 
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Niepoprawnie wczytany kierunek.\n";
		cout << "Wpisz 0, jesli tlumaczenie z j.polskiego na j.obcy \n";
		cout << "Wpisz 1, jesli tlumaczenie z j.obcego na j.polski \n";
		cin >> ile_pytan;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "ile pytan ma liczyc quiz? \n";
	cin >> ile_pytan;
	//walidacja danych
	while (cin.fail() || ile_pytan <= 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Niepoprawnie wczytana liczba. Podaj liczbe calkowita wieksza od zera." << endl;
		cin >> ile_pytan;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	vector<pair<tlumaczenie, string> > bledne_rozwiazania; // spamietujemy bledne odp i na sam koniec wyswietlimy je uzytkownikowi
	wykonaj_quiz(stworz_zestaw_testowy(dane, ile_pytan), &bledne_rozwiazania, kierunek);
	for (int i = 0; i < bledne_rozwiazania.size(); ++i){
		cout << "\nUdzieliles blednej odpowiedzi na tlumaczenie: "
			<< formatuj_tlumaczenie(bledne_rozwiazania[i].first, kierunek) << "\n";
		cout << "Twoja bledna odpowiedz to: "
			<< bledne_rozwiazania[i].second << "\n\n";
	}
	cout << "Liczba poprawnych odpowiedzi to: " << ile_pytan - bledne_rozwiazania.size() << " z " << ile_pytan << " pytan. \n\n";
}

int main(int argc, char* argv[]) { // argumanty 1. ile jest parametrow 2. konkretne parametry
	srand(time(NULL));
	vector<tlumaczenie> dane;
	cout << "Uzywam pliku ze slowkami: " << argv[1] << "\n";
	wczytaj_plik(argv[1], &dane); // pytanie czy w miejscu argv moge wstawic po prostu nazwe wczytywanego pliku?
	//for (int i = 0; i<dane.size(); ++i) cout << dane[i].slowo1 << dane[i].slowo2 << "\n"; // zakomentowane, zaby uzytkownik nie mial podgladu do listy wczytanych slow
	cout << "Dane zostaly wczytane \n\n";
	char zakonczenie;
	do
	{
		quiz(&dane);
		cout << "\nCzy chcesz zakonczyc quizy: t/n? ";
		cin >> zakonczenie;
		system("cls");
	} while (zakonczenie != 't');
	return 0;
}

bool wczytaj_plik(string nazwaPliku, vector<tlumaczenie> *wsk_na_wektor)
{
	ifstream plik; // plik - nazwa zmiennej 
	plik.open(nazwaPliku.c_str()); // metoda open na klasie ifstream, parametr - nazwa pliku, w strumieniu pojawia sie zawartosc pliku
	if (!plik.good()) // sprawdzenie czy plik sie dobrze otworzyl
		return false;
	while (true) 
	{
		tlumaczenie tmp;
		string wejscie;
		getline(plik, wejscie);
		if (!plik.good())
			break; //zakoñcz wczytywanie danych - wyst¹pi³ jakiœ b³¹d (np. nie ma wiêcej danych w pliku)
		wsk_na_wektor->push_back(tworz_tlumaczenie(wejscie));
	} 
	cout << "Plik liczy " << wsk_na_wektor->size() << " slowek \n\n";
	return true;

}



