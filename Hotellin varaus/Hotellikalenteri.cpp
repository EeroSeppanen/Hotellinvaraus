//Hotellin varauskalenteri
// Eero Seppänen

//4-5p
//input check kaikkialla
//1 tai 2 henkilön huoneiden varaus
//satunnainen ja itse päätetty varaus nimellä
//nimellä ja varausnumerolla huoneiden haku
//40-300 huonetta. hinta ja alennus toiminnot
//Ominaisuus, että voi liikkua ajassa eteenpäin
#include<iostream>
#include<vector>
#include<string>
#include<cctype>
#include<ctime>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<limits>

using namespace std;

class Huone //huone luokka
{
public:
	int huoneNro;
	string varaaja;
	bool varattu;
	int koko;
	int varausnumero;
	double hinta;
	int varausAika;

	Huone(int nro, int koko = 1, double hinta = 100.0) : huoneNro(nro), koko(koko), varattu(false), varausnumero(0), hinta(hinta), varaaja("-"), varausAika(0) {}

	float Hinta(int yomaara) 	//Lasekee huoneen hinnan ja alennuksen
	{
		double alePros = (0.1 * (8 + rand() % 3));   //1.0, 0.9, 0.8
		if (koko == 1)
		{
			hinta = 100.0 * alePros * yomaara;
			cout << "\n----------------------------------------------------------------------------";
			cout << "\nAlennusta tuli: " << (1 - alePros) * 100 << "% ja loppuhinnaksi tuli: " << hinta;
			cout << "\n----------------------------------------------------------------------------";
			return hinta;
		}
		else if (koko == 2)
		{
			hinta = 150.0 * alePros * yomaara;
			cout << "\n----------------------------------------------------------------------------";
			cout << "\nAlennusta tuli: " << (1 - alePros) * 100 << "% ja loppuhinnaksi tuli: " << hinta;
			cout << "\n----------------------------------------------------------------------------";
			return hinta;
		}
		else
		{
			cout << "\nHinnan laskennassa tuli virhe!";
			return 150.0;
		}
	}
	bool onkoVarattu() 	//Tarkistaa onko huone varattu
	{
		return varattu;
	}
	void saveData(ofstream& outFile) 	//Tallentaa datan CSV:nä
	{
		outFile << huoneNro << "," << varaaja << "," << varattu << "," << koko << "," << varausnumero << "," << hinta << "," << varausAika << endl;
	}
	static void loadData(ifstream& inFile, vector<Huone>& huoneet) 	//Lataa vanhan datan 
	{
		int nro, koko, varausnumero, varausAika;
		bool varattu;
		float hinta;
		string varaaja;
		string line;

		while (getline(inFile, line))		//CSV lukija
		{
			istringstream s(line);  //stringstream rivist�
			string field;	//kentt�
			//pilkku jakajana lukee rivin tiedot
			getline(s, field, ',');
			nro = stoi(field);
			getline(s, varaaja, ',');
			getline(s, field, ',');
			varattu = (field == "true");
			getline(s, field, ',');
			koko = stoi(field);
			getline(s, field, ',');
			varausnumero = stoi(field);
			getline(s, field, ',');
			hinta = stof(field);
			getline(s, field, ',');
			varausAika = stoi(field);
			//luo huoneen luetuilla arvoilla
			Huone huone(nro, koko, hinta);
			huone.varaaja = varaaja;
			huone.varattu = varattu;
			huone.varausnumero = varausnumero;
			huone.hinta = hinta;
			huone.varausAika;
			huoneet.push_back(huone);
		}
		if (!inFile.eof() && inFile.fail())
		{
			cout << "\nTietojen lukemisessa virhe";
		}
		else cout << "\ntiedot ladattiin onnistuneesti!";
	}
};
void uusiPaiva(vector<Huone>& huoneet) //Vapauttaa varaukset, jos ei ole enää päiviä jäljellä
{
	for (Huone& huone : huoneet)
	{
		if (huone.varausAika > 0) // -1 päiviä varattuihin huoneisiin
		{
			huone.varausAika -= 1;
			if (huone.varausAika == 0) //jo sei päiviä jäljellä niin huone vapautetaan
			{
				huone.varattu = false;
				huone.varaaja = "-";
				huone.varausnumero = 0;
				if (huone.koko == 1)
				{
					huone.hinta = 100.0;
				}
				else
				{
					huone.hinta = 150.0;
				}
			}
		}
	}
	cout << "\nUusi päivä aloitettu!";
}
int nroInputCheck(int eka, int vika, string teksti) //Tarkistaa syötteen halutulla numerovälillä
{
	int vastaus;
	bool virhe;
	do
	{
		virhe = false;
		cin >> vastaus;
		if (cin.fail() || vastaus < eka || vastaus > vika)
		{
			cout << teksti << endl;
			virhe = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

	} while (virhe);
	return vastaus;
}
string stringCheck()
{
	string teksti;
	bool virhe;
	do
	{
		virhe = false;
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //tyhjent�� sy�tt�puskuiri t�ss� vaiheessa, koska muuten koodi antaa virheen ja kysyy uudelleen ennen sy�tett� jostain syyst�
		getline(cin, teksti);

		if (teksti.empty())
		{
			cout << "Nimi ei voi olla tyhjä!\n";
			virhe = true;
			cin.clear();
		}

	} while (virhe);
	return teksti;
}
void varaaHuone(int huoneMaara, vector<Huone>& huoneet) // huoneen varaus funktio
{
	int varaustyyppi, hloMaara, yomaara, jatka, varaa;
	string varaaja;
	int vapaana = 0;
	bool virhe = false, ok = false;
	float hinta;
	cout << "\n---------------------------------------------------------------";
	cout << "\nKuinka monen henkilön huoneen haluat varata? Syötä (1) tai (2): ";
	cout << "\nPääset takaisin alkuvalikkoon syöttämällä (3)";
	cout << "\n---------------------------------------------------------------\n";
	hloMaara = nroInputCheck(1, 3, "Syötä 1, 2 tai 3");
	if (hloMaara == 3)return;

	for (Huone huone : huoneet)	//laskee vapaiden 1 tai 2 henkil�n huoneiden m��r�n
	{
		if (!huone.varattu && huone.koko == hloMaara)
		{
			vapaana += 1;
		}
	}

	if (vapaana == 0) //jatkaa vain jos huoneita on vapaana
	{
		cout << "\nVapaita huoneita ei ole:(";
	}
	else
	{
		cout << "\n------------------------------------------------------------------------------------------------";
		cout << "\nVapaana on " << vapaana << " " << hloMaara << " henkilön huonetta";
		cout << "\nHaluatko listan vapaista huoneista ja varata numerolla? (1) vai haluatko satunnaisen huoneen (2)";
		cout << "\nPääset takaisin alkuvalikkoon syöttämällä (3)";
		cout << "\n------------------------------------------------------------------------------------------------\n";
		varaustyyppi = nroInputCheck(1, 3, "Syötä 1, 2 tai 3");

		if (varaustyyppi == 3) return;

		if (varaustyyppi == 1) //oma valinta varaus----------------------------------------------------------------------
		{

			for (int i = 0; i < huoneMaara; i++)
			{
				if (!huoneet[i].onkoVarattu() && huoneet[i].koko == hloMaara)
				{
					cout << "Huone numero: " << huoneet[i].huoneNro << endl;
				}
			}
			cout << "\n------------------------------------------";
			cout << "\nKirjoita huoneen numero jonka haluat varata: ";
			cout << "\n------------------------------------------\n";
			int huoneNro;
			do
			{
				virhe = false;
				cin >> huoneNro;

				if (huoneNro < 1 || huoneNro > huoneMaara || huoneet[huoneNro - 1].onkoVarattu() || huoneet[huoneNro - 1].koko != hloMaara)
				{
					cout << "Syötä huone jota ei ole varattu ja on halutun kokoinen\n";
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					virhe = true;
				}
			} while (virhe);

			cout << "\n--------------------";
			cout << "\nSyötä varaajan nimi: ";
			cout << "\n--------------------\n";
			varaaja = stringCheck();
	
			cout << "\n-------------------------------------------";
			cout << "\nSyötä moneksiko yöksi huone varataan (1-14)";
			cout << "\n-------------------------------------------\n";
			yomaara = nroInputCheck(1, 14, "Syötä 1-14");
			hinta = huoneet[huoneNro - 1].Hinta(yomaara);

			int varausNro = 10000 + rand() % 100000; 	//arpoo ja tarkistaa varausnumeron
			for (Huone huone : huoneet)
			{
				if (varausNro == huone.varausnumero)
				{
					varausNro = 10000 + rand() % 100000;
				}
			}
			cout << "\n-------------------------------------------------------------------------------------------------";
			cout << "\nOvatko tiedot oikein? Kirjoita (1) jatkaaksesi tai (2) peruuttaksesi varauksen";
			cout << "\nOlet syöttänyt: varaaja " << varaaja << ", huonenumero " << huoneNro << ", yomaara: " << yomaara;
			cout << "\n-------------------------------------------------------------------------------------------------\n";
			varaa = nroInputCheck(1, 2, "Syötä 1 tai 2");
			if (varaa == 1)
			{
				huoneet[huoneNro - 1].varausnumero = varausNro;
				huoneet[huoneNro - 1].varaaja = varaaja;
				huoneet[huoneNro - 1].varattu = true;
				huoneet[huoneNro - 1].varausAika = yomaara;
				huoneet[huoneNro - 1].hinta = hinta;
				//tulostaa huoneen tiedot
				cout << "\n-----------------------------------------------------------------";
				cout << "\nHuone " << huoneet[huoneNro - 1].huoneNro << " varattu nimellä " << huoneet[huoneNro - 1].varaaja << " varausnumerolla " << huoneet[huoneNro - 1].varausnumero << " ja hinnalla " << huoneet[huoneNro - 1].hinta;
				cout << "\n-----------------------------------------------------------------";
			}
			else
			{
				cout << "\nPeruutetaan huoneen varaus";
				return;
			}
		}

		if (varaustyyppi == 2) // random varaus
		{
			//Valitsee randomin huoneen
			int ranHuone = rand() % huoneet.size();

			while (huoneet[ranHuone].onkoVarattu() || huoneet[ranHuone].koko != hloMaara)
			{
				ranHuone = rand() % huoneet.size();
			}

			cout << "\n--------------------";
			cout << "\nSyötä varaajan nimi: ";
			cout << "\n--------------------\n";
			varaaja = stringCheck();


			cout << "\n-------------------------------------------";
			cout << "\nSyötä moneksiko yöksi huone varataan (1-14)";
			cout << "\n-------------------------------------------\n";
			yomaara = nroInputCheck(1, 14, "Syötä 1-14");
			hinta = huoneet[ranHuone].Hinta(yomaara);

			int varausNro = 10000 + rand() % 100000; 			//arpoo ja tarkistaa varausnumeron
			for (Huone huone : huoneet)
			{
				if (varausNro == huone.varausnumero)
				{
					varausNro = 10000 + rand() % 100000;
				}
			}
			cout << "\n------------------------------------------------------------------------------";
			cout << "\nOvatko tiedot oikein? Kirjoita (1) jatkaaksesi tai (2) peruuttaksesi varauksen";
			cout << "\nOlet syöttänyt: varaaja " << varaaja << ", yomaara: " << yomaara;
			cout << "\n------------------------------------------------------------------------------\n";
			varaa = nroInputCheck(1, 2, "Syötä 1 tai 2");
			if (varaa == 1)
			{
				huoneet[ranHuone].varaaja = varaaja;
				huoneet[ranHuone].varattu = true;
				huoneet[ranHuone].varausAika = yomaara;
				huoneet[ranHuone].varausnumero = varausNro;
				huoneet[ranHuone].hinta = hinta;
				cout << "\n-----------------------------------------------------------------";
				cout << "\nHuone " << huoneet[ranHuone].huoneNro << " varattu nimellä " << huoneet[ranHuone].varaaja << " varausnumerolla " << huoneet[ranHuone].varausnumero << " ja hinnalla " << huoneet[ranHuone].hinta;
				cout << "\n-----------------------------------------------------------------";
			}
			else
			{
				cout << "\nPeruutetaan huoneen varaus";
				return;
			}

		}
	}
}

void nimiHaku(vector<Huone>& huoneet)
{
	string nimi;
	cout << "\n----------------------";
	cout << "\nSyötä varaajan nimi: ";
	cout << "\n----------------------\n";
	nimi = stringCheck();

	bool loydetty = false;
	for (Huone huone : huoneet) //k�y l�pi huoneet ja etsii varaajan nimell�
	{
		if (huone.varaaja == nimi)
		{
			loydetty = true;
			cout << "\nHuone löytyi nimellä: " << nimi << " Huoneen numero: " << huone.huoneNro << "  Varausnumero: " << huone.varausnumero;
		}
	}
	if (!loydetty)
	{
		cout << "\nTällä nimellä ei löytynyt varausta";
	}
}

void nroHaku(vector<Huone>& huoneet)
{
	int varausNro;

	cout << "\n---------------------------------";
	cout << "\nSyötä varausnumero (10000-99999): ";
	cout << "\n---------------------------------\n";
	varausNro = nroInputCheck(10000, 99999, "Syötä 10000-99999");

	bool loydetty = false;
	for (Huone huone : huoneet) 	//k�y l�pi huoneet ja etsii varausnumerolla
	{
		if (huone.varausnumero == varausNro)
		{
			loydetty = true;
			cout << "\nHuone löytyi! varausnumerolla " << varausNro << " Huoneen numero: " << huone.huoneNro << "Huoneen varaaja: " << huone.varaaja;
		}
	}
	if (!loydetty)
	{
		cout << "\nTällä varausnumerolla ei löytynyt varausta\n";
	}
}
void tulostaHuoneet(vector<Huone>& huoneet)
{
	for (Huone huone : huoneet)
	{
		cout << "\nHuone: " << huone.huoneNro << " Koko: " << huone.koko << " Varaaja: " << huone.varaaja << " Vaurausnumero: " << huone.varausnumero << " Hinta: " << huone.hinta;
	}
}

int main()
{
	setlocale(LC_ALL, "FI_fi");		//��kk�set
	srand(time(0));		//random seed
	int huoneMaara;
	vector<Huone> huoneet;
	ifstream inFile("C:/Temp/huoneet.txt");
	if (inFile) 	//Jos tiedosto on olemassa niin lataa sen ja muuten luo uuden
	{
		cout << "Vanhat tiedot löytyi!";
		Huone::loadData(inFile, huoneet);
		inFile.close();
	}
	else
	{
		cout << "Vanhoja tietoja ei löytynyt :(";
		huoneMaara = 40 + 2 * (rand() % 131); 		//huoneMaara = random luku 40 + 2*0-130 eli 40-300
		for (int i = 0; i < (huoneMaara / 2); i++) 		//puolet huoneista bool koko 1
		{
			huoneet.push_back(Huone(i + 1, 1, 100.0));

		}
		for (int i = (huoneMaara / 2); i < huoneMaara; i++) 	//puolet koko 2
		{
			huoneet.push_back(Huone(i + 1, 2, 150.0));
		}
	}
	huoneMaara = huoneet.size();
	bool poistu;
	int liike;
	do			//tekee t�t� niin kauan kunnes k�ytt�j� haluaa poistua
	{
		poistu = false;
		cout << "\n-------------------------------------------------------------------------------------------------------------";
		cout << "\nMitä haluat tehdä? (1 varaa huone) (2 nimihaku) (3 nrohaku) (4 seuraava päivä) (5 tulosta huoneet) (6 poistu)";
		cout << "\n-------------------------------------------------------------------------------------------------------------\n";
		liike = nroInputCheck(1, 6, "Syötä luku väliltä 1-6");
		if (liike == 6)
		{
			poistu = true;
		}
		else if (liike == 1)
		{
			varaaHuone(huoneMaara, huoneet);
		}
		else if (liike == 2)
		{
			nimiHaku(huoneet);
		}
		else if (liike == 3)
		{
			nroHaku(huoneet);
		}
		else if (liike == 4)
		{
			uusiPaiva(huoneet);
		}
		else if (liike == 5)
		{
			tulostaHuoneet(huoneet);
		}
		else
		{
			cout << "virhe!";
		}

	} while (!poistu);
	//Poistuessa tallentaa tiedostot
	ofstream outFile("C:/Temp/huoneet.txt");
	if (outFile)
	{
		for (Huone huone : huoneet)
		{
			huone.saveData(outFile);
		}
		outFile.close();
	}
	else cout << "\n!!!!!!!!!!!!!\n!!!\n!!!\n!!!!!!Tiedostojen tallentaminen epäonnistui!!!!!\n!!!\n!!!\n!!!!!!!!!!!!!\n";
	return 0;
}

