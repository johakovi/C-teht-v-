// C++Harjoitus.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Johanna Koivistoinen

#include <iostream>
#include <vector>
#include <math.h>
#include <chrono> 
#include <iomanip>
#include <locale>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

class Huone 
{
public:
	Huone(int numero, bool onkoVapaa, string tyyppi) : numero(numero), onkoVapaa(onkoVapaa), tyyppi(tyyppi) {}

	string getStatus() const 
	{
		return "Huone \t" + to_string(numero) + "\t" + tyyppi + "\t" + (onkoVapaa ? "Vapaana" : "Varattu");
	}

	void saveToFile() const 
	{
		string filename = to_string(numero) + ".txt";
		ofstream file(filename);
		if (file.is_open()) 
		{
			file << "Huone numero: " << numero << endl;
			file << "Tyyppi: " << tyyppi << endl;
			file << "Varaus: " << (onkoVapaa ? "Vapaa" : "Varattu") << endl;
			file.close();
		}
		else {
			cerr << "Tiedostoa ei pysty avaamaan " << filename << endl;
		}
	}

	void loadFromFile() 
	{
		string filename = to_string(numero) + ".txt";
		ifstream file(filename);
		if (file.is_open()) 
		{
			string line;
			while (getline(file, line)) 
			{
				if (line.find("Varaus:") != string::npos) 
				{
					onkoVapaa = (line.find("Vapaa") != string::npos);
				}
			}
			file.close();
		}
		else 
		{
			saveToFile(); //Luo tiedosto, jos ei ole olemassa
		}
	}

	void varaaHuone() 
	{
		onkoVapaa = false;
		saveToFile();
	}

	void vapautaHuone()  
	{
		onkoVapaa = true;
		saveToFile();
	}

	bool isVapaa() const 
	{
		return onkoVapaa;
	}

	int getNumero() const 
	{
		return numero;
	}
	string getTyyppi() const
	{
		return tyyppi;
	}


private:
	int numero;
	bool onkoVapaa;
	string tyyppi;
};

struct Vierailija 
{
	string nimi;
	string tarkoitus;
	string tyyppi;
	double yotmaara;
	int huoneNumero;
};

void alustaHuoneet(vector<Huone>& huoneet) 
{
	for (int i = 1; i <= 300; ++i) 
	{
		string tyyppi;
		if (i >= 1 && i <= 150) 
		{
			tyyppi = "Yhden hengen";
		}
		else if (i >= 151 && i <= 300) 
		{
			tyyppi = "Kahden hengen";
		}
		huoneet.emplace_back(i, true, tyyppi);
	}

	// Load or save each room's information
	for (auto& huone : huoneet) 
	{
		huone.loadFromFile();
	}
}


double laskeHinta(const Vierailija& vieras)
{
	if (vieras.huoneNumero >= 1 && vieras.huoneNumero <= 150)
	{
		double hinta = vieras.yotmaara * 100;
		return hinta;
	}
	else if (vieras.huoneNumero >= 151 && vieras.huoneNumero <= 300)
	{
		double hinta = vieras.yotmaara * 150;
		return hinta;
	}
	else
	{
		// Jos huoneNumero ei ole välillä 1-300, palauta 0 tai jokin muu oletusarvo
		return 0.0;
	}
}

int arvoAlennus()
{
	srand(time(0));
	int alennukset[] = { 0, 10, 20 };
	int index = rand() % 3; // Arpoo luvun 0, 1 tai 2
	return alennukset[index];
}


void tulostaVierailijanTiedot( vector<Huone>& huoneet, Vierailija& vieras)
{
	cout << "Vierailijan nimi: " << vieras.nimi << endl;
	cout << "Lomalla/Työ/muu: " << vieras.tarkoitus << endl;
	cout << "Monta yötä?:  " << vieras.yotmaara << endl;
	cout << "Mikä huone?: " << vieras.huoneNumero << endl;

	string huoneTyyppi;
	bool huoneVarattu = false;

	for (auto& huone : huoneet)
	{
		if (huone.getNumero() == vieras.huoneNumero)
		{
			huoneTyyppi = huone.getTyyppi();
			if (huone.isVapaa())
			{
				huone.varaaHuone(); // Merkitään huone varatuksi ja tallennetaan tiedot tiedostoon
				huone.saveToFile(); // Tallennetaan huoneen tiedot tiedostoon
				cout << "Huone " << vieras.huoneNumero << " varattu onnistuneesti." << endl;
				huoneVarattu = true;
			}
			else
			{
				cout << "Huone " << vieras.huoneNumero << " ei ole vapaana." << endl;
			}
			break;
		}
	}

	if (!huoneVarattu)
	{
		cout << "Huoneen varaaminen epäonnistui." << endl;
		return;
	}
	
	// Laske hinta
	double hinta = laskeHinta(vieras);

	// Arvo alennusprosentti
	int alennusProsentti = arvoAlennus();
	double alennus = hinta * alennusProsentti / 100;
	double loppuhinta = hinta - alennus;

	// Luo varausnumero
	srand(time(0));
	int varausnumero = rand() % 90000 + 10000;

	// Luo tiedostonimi
	string tiedostonimi = vieras.nimi + ".txt";
	// Muunna huoneNumero stringiksi
	ostringstream oss;
	oss << "varattu huonenro" << vieras.huoneNumero << ".txt";
	string tiedostonimi2 = oss.str();

	// Tallenna tiedot tiedostoon
	ofstream tiedosto(tiedostonimi);
	if (tiedosto.is_open()) 
	{
		tiedosto << "Varausnumero: " << varausnumero << endl;
		tiedosto << "Vierailijan nimi: " << vieras.nimi << endl;
		tiedosto << "Lomalla/Työ/muu: " << vieras.tarkoitus << endl;
		tiedosto << "Monta yötä?: " << vieras.yotmaara << endl;
		tiedosto << "Huone tyyppi: " << huoneTyyppi << endl;
		tiedosto << "Huone numero: " << vieras.huoneNumero << endl;
		tiedosto << "Hinta: " << hinta << endl;
		tiedosto << "Alennus: " << alennusProsentti << "%" << endl;
		tiedosto << "Loppuhinta: " << loppuhinta << endl;


		tiedosto.close();
		cout << endl;
		cout << "Tiedot tallennettu tiedostoon " << tiedostonimi << endl;
	}
	else 
	{
		cout << "Tiedoston avaaminen epäonnistui." << endl;
	}

	ofstream tiedosto2(tiedostonimi2);
	if (tiedosto2.is_open())
	{
		tiedosto2 << "Varausnumero: " << varausnumero << endl;
		tiedosto2 << "Vierailijan nimi: " << vieras.nimi << endl;
		tiedosto2 << "Lomalla/Työ/muu: " << vieras.tarkoitus << endl;
		tiedosto2 << "Monta yötä?: " << vieras.yotmaara << endl;
		tiedosto2 << "Huone tyyppi: " << huoneTyyppi << endl;
		tiedosto2 << "Huone numero: " << vieras.huoneNumero << endl;
		tiedosto2 << "Hinta: " << hinta << endl;
		tiedosto2 << "Alennus: " << alennusProsentti << "%" << endl;
		tiedosto2 << "Loppuhinta: " << loppuhinta << endl;

		tiedosto2.close();
		cout << endl;
		cout << "Tiedot tallennettu tiedostoon " << tiedostonimi2 << endl;
	}
	else
	{
		cout << "Tiedoston avaaminen epäonnistui." << endl;
	}
	// Tulosta varausnumero komentonäytölle
	cout << "Varausnumero: " << varausnumero << endl;
	cout << "Hinta: " << hinta << endl;
	cout << "Alennus: " << alennusProsentti << "%" << endl;
	cout << "Loppuhinta: " << loppuhinta << endl;
}

void naytaVieraanTiedot(vector<Huone>& huoneet) 
{
	int valinta1;
	cout << "Haku nimellä (1) vai huoneen numerolla (2)?" << endl;
	cin >> valinta1;
	if (valinta1 == 1)
	{
		string nimi;
		cout << "Anna vierailijan nimi: ";
		cin >> nimi;

		string tiedostonimi = nimi + ".txt";
		ifstream tiedosto(tiedostonimi);
		if (tiedosto.is_open()) {
			string rivi;
			while (getline(tiedosto, rivi))
			{
				cout << rivi << endl;
			}
			tiedosto.close();

			char vastaus;
			cout << "Vapautetaanko huone, Kyllä (k), ei (kirjoita muuta)?" << endl;
			cin >> vastaus;

			if (vastaus == 'k')
			{
				// Poista tiedosto
				if (remove(tiedostonimi.c_str()) == 0)
				{
					cout << "Vierailijan tiedosto poistettu onnistuneesti." << endl;
				}
				else {
					cout << "Tiedoston poistaminen epäonnistui." << endl;
				}

				// Vapauta huone
				int huoneNumero;
				cout << "Anna vierailijan huoneen numero vapautusta varten: ";
				cin >> huoneNumero;
				for (auto& huone : huoneet)
				{
					if (huone.getNumero() == huoneNumero)
					{
						huone.vapautaHuone();
						cout << "Huone " << huoneNumero << " vapautettu onnistuneesti." << endl;
						break;
					}
				}
			}


		}
		else
		{
			cout << "Vierailijan tiedostoa ei löytynyt." << endl;
		}
	}

	else if (valinta1 == 2)
	{
		string numero;
		cout << "Anna huoneen numero: ";
		cin >> numero;

		string tiedostonimi2 = "varattu huonenro" + numero + ".txt";
		ifstream tiedosto2(tiedostonimi2);
		if (tiedosto2.is_open()) {
			string rivi;
			while (getline(tiedosto2, rivi))
			{
				cout << rivi << endl;
			}
			tiedosto2.close();

			char vastaus;
			cout << "Vapautetaanko huone, Kyllä (k), ei (kirjoita muuta)?" << endl;
			cin >> vastaus;

			if (vastaus == 'k')
			{
				// Poista tiedosto
				if (remove(tiedostonimi2.c_str()) == 0)
				{
					cout << "Vierailijan tiedosto poistettu onnistuneesti." << endl;
				}
				else {
					cout << "Tiedoston poistaminen epäonnistui." << endl;
				}

				// Vapauta huone
				int huoneNumero;
				cout << "Anna vierailijan huoneen numero vapautusta varten: ";
				cin >> huoneNumero;
				for (auto& huone : huoneet)
				{
					if (huone.getNumero() == huoneNumero)
					{
						huone.vapautaHuone();
						cout << "Huone " << huoneNumero << " vapautettu onnistuneesti." << endl;
						break;
					}
				}
			}


		}
		else
		{
			cout << "Vierailijan tiedostoa ei löytynyt." << endl;
		}
	}
	else
	{

	}
}

int main()
{
	setlocale(LC_ALL, "fi_FI"); //ääkköset

	vector<Huone> huoneet;
	alustaHuoneet(huoneet);

home:
	int valinta;
	cout << "Hotelli SysSoft" << endl;
	cout << "--------------------------------------------" << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "(1)\tHuoneet" << endl;
	cout << "(2)\tVierailijan kirjaus" << endl;
	cout << "(3)\tVieraan tiedot" << endl;
	cout << "(4)\tPoistu ohjelmasta " << endl;
	cout << "" << endl;
	cout << "Valintasi: ";
	cin >> valinta;

	if (valinta == 1)
	{

		system("cls");
		
		cout << "Huoneet" << endl;
		cout << "--------------------------------------------" << endl;
		cout << "" << endl;

		for (const auto& huone : huoneet) 
		{
			cout << huone.getStatus() << endl;
		}
		cout << "" << endl;
		char vastaus;
		cout << "Palataanko takaisin?" << endl;
		cout << "Kyllä (k), ei (kirjoita muuta)" << endl;
		cin >> vastaus;

		if (vastaus == 'k')
		{
			system("cls");
			goto home;

		}

	}
	else if (valinta == 2)
	{
		system("cls");

		cout << "Vierailijan kirjaus" << endl;
		cout << "--------------------------------------------" << endl;
		cout << "" << endl;

		Vierailija vieras;
		cout << "Anna vierailijan nimi: ";
		cin >> vieras.nimi;
		cout << "Lomalla/Työ/muu: ";
		cin >> vieras.tarkoitus;
		cout << "Monta yötä?: ";
		cin >> vieras.yotmaara;
		cout << "Anna huoneen numero: ";
		cin >> vieras.huoneNumero;

		//varaaHuone(huoneet, vieras);
		tulostaVierailijanTiedot(huoneet, vieras);


		char vastaus;
		cout << "Palataanko takaisin?" << endl;
		cout << "Kyllä (k), ei (kirjoita muuta)" << endl;
		cin >> vastaus;

		if (vastaus == 'k')
		{
			system("cls");
			goto home;
		}
	}
	else if (valinta == 3)
	{
		system("cls");


		cout << "Vieraan tiedot" << endl;
		cout << "--------------------------------------------" << endl;
		cout << "" << endl;

		naytaVieraanTiedot(huoneet);

		cout << "" << endl;
		char vastaus;
		cout << "Palataanko takaisin?" << endl;
		cout << "Kyllä (k), ei (kirjoita muuta)" << endl;
		cin >> vastaus;

		if (vastaus == 'k')
		{
			system("cls");
			goto home;

		}

	}
	else if (valinta == 4)
	{

	}
	else
	{
		system("cls");
		goto home;
	}
	cout << "Kiitos kun kävit!" << endl;
	system("pause");
	return(0);
}