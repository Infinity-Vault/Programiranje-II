#include <iostream>
using namespace std;

//Z0.1

//Zadatak donje funckije jeste da se alocira pokazivac na novi niz karaktera velicine tekst:
char* AlocirajIKopiraj(const char* tekst)
{
	if (tekst == nullptr)
		return nullptr;
	int velicina = strlen(tekst) + 1;//+1 se stavi radi nullterminirajuceg znaka;
	char* novi_Tekst = new char[velicina];
	strcpy_s(novi_Tekst, velicina, tekst);
	return novi_Tekst;
}

struct Datum
{
	int* _dan = nullptr;
	int* _mjesec = nullptr;
	int* _godina = nullptr;
	//Z1.1
	//Zadatak ove funkcije jeste da  unesene parametre dodijeli propertijima dan mjesec i godina:
	void Unos(int d, int m, int g)
	{
		_dan = new int;
		_mjesec = new int;
		_godina = new int;
		*_dan = d;
		*_mjesec = m;
		*_godina = g;
		//Posto je funckija tipa void ne vraca nista!
	}
	//Z1.2
	//Zadatak ove funkcije jeste da omoguci unos propertia dan mjesec i godina ali ovaj put se to radi preko objekta tipa datum koji je poslan po referenci:
	void Unos(Datum& datum)
	{
		_dan = new int;
		_mjesec = new int;
		_godina = new int;
		*_dan = *datum._dan;//Zvjezdica na pocetku jer je _dan pointer koji se mora dereferencirati!
		*_mjesec = *datum._mjesec;
		*_godina = *datum._mjesec;
		//Funkcija je tipa void te ne vraca nista!
	}
	//Z1.3
	//Zadatak sljedece funckije jeste da omoguci unos propertia dan mjesec i godina ali ovaj put se radi preko pointera tipa datum:
	void Unos(Datum* datum)
	{
		//*(*(niz+i)+j)->ptr;
		_dan = new int;
		_mjesec = new int;
		_godina = new int;
		*_dan = *datum->_dan;//Datum se dereferencira sa strelicom a pointer _dan sa zvjezdicom na pocetku;
		*_mjesec = *(*datum)._mjesec;//Moze i ovako, prva zvjezdica derefer. _mjesec a druga pointer datum;
		*_godina = *datum->_godina;
		//Funckija je tipa void tako da ne vraca nista!
	}
	//Z1.4
	//Zadatak ove  funkcije jeste da vrati dan:
	int GetDan()
	{
		return *_dan;
	}
	//Zadatak ove funkcije jeste da vrati mjesec:
	int GetMjesec()
	{
		return *_mjesec;
	}
	//Zadatak ove funkcije jeste da vrati godinu:
	int GetGodina()
	{
		return *_godina;
	}
	//Z1.5
	//Zadatak ove funckije jeste da proslijedjeni parametar postavi za dan:
	void SetDan(int dan)
	{
		if (_dan == nullptr)//Moramo pitati da li je pointer _dan u upotrebi tj. da li pokazuje i na sta, ako ne pokazuje onda napraviti novi int i pokazati na njega
			_dan = new int;
		*_dan = dan;
	}
	//Z1.6
	//Zadatak ove funkcije jeste da proslijedjeni parametar postavi za mjesec:
	void SetMjesec(int mjesec)
	{
		if (_mjesec == nullptr)//Moramo provjeriti da li je pointer " prazan ";
			_mjesec = new int;
		*_mjesec = mjesec;
	}
	//Z1.7
	//Zadatak ove funkcije  jeste da proslijedjeni parametar postavi za godinu:
	void SetGodina(int godina)
	{
		if (_godina == nullptr)//Moramo provjeriti da li je pointer _godina prazan;
			_godina = new int;
		*_godina = godina;
	}
	//Z1.8
	//Zadatak ove funkcije jeste da ispis datum u formatu dan, mjesec, godina:
	void Ispis()
	{

		cout << *_dan << "." << *_mjesec << "." << *_godina << ".";
	}
	//Z1.9
	//Zadatak ove funkcije jeste da dealocira svu dinamicki alociranu memoriju:
	void Dealokacija()
	{
		delete _dan, _mjesec, _godina;//Sve pointere izbrise;
		_dan = _mjesec = _godina = nullptr;//Sve pointere nullpointa;
	}
};
//Z1.10
//Zadatak ove funkcije jeste da provjeri da li su dva datuma ista (0 vrati ako nisu a bilo sta drugo vraceno znaci da jesu):
bool JeLiIsti(Datum& datum_Jedan, Datum& datum_Dva)
{
	//Oba datuma su primljenja po referenci kako bi se poslali sa svojim adresama!!!
	return *datum_Jedan._dan == *datum_Dva._dan && *datum_Jedan._mjesec == *datum_Dva._mjesec && *datum_Jedan._godina == *datum_Dva._godina;
	//U sustini ista je logika da smo koritili if i pitali da li su dan mjesec i godina isti i vratili tada true a u suprtonom false.Ovo je samo krace.
}
//Z1.11
//Zadatak ove funkcije jeste da vrati referencu na objekat (datum) koji je najstariji:
Datum& GetStarijiDatum(Datum& datum_Jedan, Datum& datum_Dva)
{
	//Oba datuma (objekta) su primljena po refernenci kako bi se imala njihova adresa!!!
	if (*datum_Jedan._godina < *datum_Dva._godina)
		return datum_Jedan;
	else if (*datum_Jedan._godina == *datum_Dva._godina)
	{
		if (*datum_Jedan._mjesec < *datum_Dva._mjesec)
			return datum_Jedan;
		else if (*datum_Jedan._mjesec == *datum_Dva._mjesec)
		{
			if (*datum_Jedan._dan <= *datum_Dva._dan)
				return datum_Jedan;
			return datum_Dva;
		}
		return datum_Dva;
	}
	return datum_Dva;
}
//Z1.12
//Zadatak ove funkcije jeste da vrati referencu na objekat (datum) koji je najmladji:
Datum& GetNovijiDatum(Datum& datum_Jedan, Datum& datum_Dva)
{
	//Oba datuma se prime po referenci sa svojim adresama!!!
	if (JeLiIsti(GetStarijiDatum(datum_Jedan, datum_Dva), datum_Jedan))
		return datum_Dva;
	return datum_Jedan;
}
//Z1.13
//Zadatak ove funkcije jeste da vrati pokazivac tipa Datum na  najstariji datum u nizu datuma:
Datum* GetNajstarijiDatum(Datum datum[], int size)
{
	Datum* najstariji = datum;//Inicijaliziramo novi pokazivac na prvi element u nizu datum;
	for (int i = 0; i < size; i++)
	{
		najstariji = &GetStarijiDatum(*najstariji, datum[i]);//Size broj puta poredi datum na pokazivacu najstariji i datum tj i-ti element niza datum, te to smjesta u najstariji;
	}
	return najstariji;
}
//Z1.14
//Zadatak ove funkcije jeste da vrati pokazivac na datum koji je najnoviji u nizu:
Datum* GetNajnovijiDatum(Datum datum[], int size)
{
	Datum* najnoviji = datum;//Inicijaliziramo  pokazivac najnoviji da pokazuje na prvi element niza datum;
	for (int i = 0; i < size; i++)
	{
		najnoviji = &GetNovijiDatum(*najnoviji, datum[i]);//Size broj puta se uporedi datum na pokazivacu najnoviji i  i-ti element niza datum, te vidi koji je noviji i spremi se adresa tog objekta na pointer najnoviji;
	}
	return najnoviji;
}

struct Glumac {
	char _jmbg[14] = "";
	char* _ime = nullptr;
	char* _prezime = nullptr;
	Datum* _datumRodjenja = nullptr;
	char* _mjestoRodjenja = nullptr;
	char _drzava[100];
	bool _spol; //1:Muski, 0:Zenski
	//Z2.1
	//Zadatak ove funkcije jeste da se izvrsi unos propertija koji su poslani preko parametara:
	void Unos(const char* jmbg, const char* ime, const char* prezime, Datum& datumRodjenja, const char* mjestoRodjenja, const char* drzava, bool spol)
	{
		strcpy_s(_jmbg, 14, jmbg);//Zato sto je niz?
		_ime = AlocirajIKopiraj(ime);
		_prezime = AlocirajIKopiraj(prezime);
		_datumRodjenja = new Datum;
		_datumRodjenja->Unos(datumRodjenja);
		_mjestoRodjenja = AlocirajIKopiraj(mjestoRodjenja);
		strcpy_s(_drzava, 100, drzava);
		_spol = spol;
	}
	//Z2.2
	//Zadatak ove funkcije jeste da omoguci unos obiljezja ali ovaj put preko reference na objekat tipa Glumac:
	void Unos(Glumac& glumac)
	{
		strcpy_s(_jmbg, 14, glumac._jmbg);
		_ime = AlocirajIKopiraj(glumac._ime);
		_prezime = AlocirajIKopiraj(glumac._prezime);
		_datumRodjenja = new Datum;
		_datumRodjenja->Unos(glumac._datumRodjenja);
		_mjestoRodjenja = AlocirajIKopiraj(glumac._mjestoRodjenja);
		strcpy_s(_drzava, 100, glumac._drzava);
		_spol = glumac._spol;
	}
	//Z2.3
	//Zadatak ove funkcije jeste da unese jmbg iz datog parametra:
	void SetJmbg(const char* jmbg)
	{
		strcpy_s(_jmbg, 14, jmbg);
	}
	//Z2.4
	//Zadatak ove funkcije jeste da unese ime iz datog parametra:
	void SetIme(const char* ime)
	{
		delete[]_ime;//Zasto ovo??
		_ime = AlocirajIKopiraj(ime);
	}
	//Z2.5
	//Zadatak ove funkcije jeste da unese prezime iz datog parametra:
	void SetPrezime(const char* prezime)
	{
		delete[] _prezime;
		_prezime = AlocirajIKopiraj(prezime);
	}
	//Z2.6
	//Zadatak ove funkcij jeste da unese datum iz datog parametra:
	void SetDatumRodjenja(Datum& datumRodjenja)
	{
		if (_datumRodjenja == nullptr)
			_datumRodjenja = new Datum;
		/*_datumRodjenja->Unos(datumRodjenja);*/
		//Ili:
		_datumRodjenja->SetDan(datumRodjenja.GetDan());
		_datumRodjenja->SetMjesec(datumRodjenja.GetMjesec());
		_datumRodjenja->SetGodina(datumRodjenja.GetGodina());
	}
	//Z2.7
	//Zadatak ove funkcije jeste da unese mjesto rodjenja iz datog parametra:
	void SetMjestoRodjenja(const char* mjestoRodjenja)
	{
		delete[] _mjestoRodjenja;
		_mjestoRodjenja = AlocirajIKopiraj(mjestoRodjenja);
	}
	//Z2.8
	//Zadatak ove funkcije jeste da unese drzave iz datog parametra:
	void SetDrzava(const char* drzava)
	{
		strcpy_s(_drzava, 100, drzava);
	}
	//Z2.9
	//Zadatak ove funkcije jeste da unese spol iz datog parametra:
	void SetSpol(bool spol)
	{
		_spol = spol;
	}
	//Z2.10
	//Zadatak funkcije dole jeste da se ispisu sva obiljezja od strukture Glumac:
	void Ispis()
	{
		cout << endl;
		cout << "Ime je: " << _ime << "\t";
		cout << "Prezime je: " << _prezime << "\t";
		cout << endl;
		cout << "Rodjen je: " << endl;
		_datumRodjenja->Ispis();
		cout << "\t";
		cout << "Mjesto rodjenja je: " << _mjestoRodjenja << " a drzava je: " << _drzava << " \t";
		//cout << "Spol je (1-Musko/0-Zensko): " << _spol << endl;
		cout << "Spol je: " << ((_spol == true) ? "Musko" : "Zensko") << endl;
	}
	//Z2.11
	//Zadatak  ove funkcije je da dealocira svu dinamicki kreiranu memoriju:
	void Dealokacija()
	{
		_datumRodjenja->Dealokacija();//Posto je ovaj ptr tipa Datum strukture, prvo se njegovi properties brisu pa onda sam on!;
		delete _datumRodjenja;
		_datumRodjenja = nullptr;
		delete[] _ime, _prezime, _mjestoRodjenja;
		_ime = _prezime = _mjestoRodjenja = nullptr;
	}
};

struct Film {
	char _filmID[50] = ""; //prema ISAN standardu
	char* _naziv = nullptr;
	char _zanr[40];
	int _godinaIzlaska;
	int _trenutnoGlumaca = 0;
	Glumac _glumackaPostava[20];
	int _trenutnoOcjena = 0;
	int* _ocjene = nullptr; //Dinamicki prosirivati ovaj niz svakim novim dodavanjem
	//Z3.1
	//Zadatak ove funkcije jeste da kroz parametre izvrsi unos:
	void Unos(const char* filmID, const char* naziv, const char* zanr, int godinaIzlaska)
	{
		strcpy_s(_filmID, 50, filmID);
		_naziv = AlocirajIKopiraj(naziv);
		strcpy_s(_zanr, 40, zanr);
		_godinaIzlaska = godinaIzlaska;
	}
	//Z3.2
	//Zadatak ove funkcije jeste da izvrsi unos obiljezja ali ovaj put parametar je referenca na objekat "film":
	void Unos(Film& film)
	{
		strcpy_s(_filmID, 50, film._filmID);
		_naziv = AlocirajIKopiraj(film._naziv);
		strcpy_s(_zanr, 40, film._zanr);
		_godinaIzlaska = film._godinaIzlaska;
		_trenutnoGlumaca = film._trenutnoGlumaca;
		for (int i = 0; i < _trenutnoGlumaca; i++)
		{
			_glumackaPostava[i].Unos(film._glumackaPostava[i]);//Za svakog glumca se unesu obiljezja od unesenog parametra;
		}
		_trenutnoOcjena = film._trenutnoOcjena;
		_ocjene = new int[_trenutnoOcjena];//Dinamicki se prosiruje vazda za trenutnu velicinu;
		for (int i = 0; i < _trenutnoOcjena; i++)
		{
			_ocjene[i] = film._ocjene[i];//Stara ocjena od objekta film se prekopira (dodijeli u novu);Moze i ovako *(film._ocjene+i) aritmetikom ptr;
		}
	}
	//Z3.3
	//Zadatak ove funkcije jeste da provjeri da li je glumac dodan i ako nije da ga doda:
	bool DodajGlumca(Glumac& g)
	{
		if (_trenutnoGlumaca == 20)
			return false;
		_glumackaPostava[_trenutnoGlumaca].Unos(g);//Za trenutnog glumca u nizu od 20 glumaca unesi obiljezja iz glumca "g";
		_trenutnoGlumaca++;
		return true;
	}
	//Z3.4
	//Zadatak ove funkcije jeste da provjeri da li je ocjena dodana ili ne i ako nije da parametar dodijeli trenutnoj ocjeni:
	bool DodajOcjenu(int ocjena)
	{
		int* temp = _ocjene;//Napravimo pomocni pokazivac na niz ocjena;
		_ocjene = new int[_trenutnoOcjena + 1];//Stari niz ocjena prosirimo za jednu, odatle +1;Brojac se ovdje ne POVECAVA!
		for (int i = 0; i < _trenutnoOcjena; i++)
		{
			_ocjene[i] = temp[i];//Za svako i prekopiramo ocjene ali posto se niz prosirio za jednu, jedna (zadnja) ocjena je ne popunjena;
		}
		delete[]temp;//Obrisemo dinam niz;
		temp = nullptr;//Nullpointamo pointer temp;
		_ocjene[_trenutnoOcjena] = ocjena;//Zadnjoj ocjeni dodijelimo  ocjenu iz parametra;
		_trenutnoOcjena++;//Povecamo brojac;
		return true;//Vratimo true;
	}
	//Z3.5
	//Zadatak ove funkcije jeste da ispise obiljezja:
	void Ispis()
	{
		cout << "Naziv filma je: " << _naziv << "\t";
		cout << "Zanr filma je: " << _zanr << "\t";
		cout << "Godina izlaska filma je: " << _godinaIzlaska << "\t";
		cout << "Trenutno ima: " << _trenutnoGlumaca << " glumaca." << "\t";
		cout << endl;
		for (int i = 0; i < _trenutnoGlumaca; i++)
		{
			_glumackaPostava[i].Ispis();//Izvrsi ispis svakog glumca i njegovih obiljezja iz niza;
		}
		cout << "Ukupno ima " << _trenutnoOcjena << " ocjena." << "\t";
		cout << "Prosjecna ocjena je: " << GetProsjecnaOcjena() << endl;
	}
	//Z3.6
	//Zadatak ove funkcije jeste da pronadje prosjecnu ocjenu u nizu:
	float GetProsjecnaOcjena()
	{
		float prosjek = 0.0;
		for (int i = 0; i < _trenutnoOcjena; i++)
		{
			prosjek += _ocjene[i];
		}
		prosjek /= _trenutnoOcjena;
		return prosjek;
	}
	//Z3.7
	//Zadatak ove funkcije jeste da dealocira svu dinamicki alociranu memoriju:
	void Dealokacija()
	{
		for (int i = 0; i < _trenutnoGlumaca; i++)
		{
			_glumackaPostava[i].Dealokacija();
		}
		delete[] _naziv, _ocjene;
		_naziv = nullptr;
		_ocjene = nullptr;
	}
};

void Zadatak1() {
	cout << ":Zadatak 1:" << endl;
	Datum prviMart, danNezavisnosti, danDrzavnosti;
	prviMart.Unos(1, 3, 2021);
	danNezavisnosti.Unos(prviMart);
	danDrzavnosti.Unos(&prviMart);
	danDrzavnosti.SetDan(25);
	danDrzavnosti.SetMjesec(11);
	danDrzavnosti.SetGodina(2021);

	cout << "Dan nezavisnosti: " << endl;
	danNezavisnosti.Ispis();
	cout << endl << "Dan drzavnosti: " << endl;
	danDrzavnosti.Ispis();
	prviMart.Dealokacija();
	danNezavisnosti.Dealokacija();
	danDrzavnosti.Dealokacija();

	Datum danD, operacijaBarbarossa, sarajevskiAtentat, apolloSlijetanje, blackThursday;
	danD.Unos(6, 6, 1944);
	operacijaBarbarossa.Unos(22, 6, 1941);
	sarajevskiAtentat.Unos(28, 6, 1914);
	apolloSlijetanje.Unos(24, 7, 1969);
	blackThursday.Unos(24, 10, 1929);

	Datum historijskiDogadjaji[5];
	historijskiDogadjaji[0].Unos(danD);
	historijskiDogadjaji[1].Unos(operacijaBarbarossa);
	historijskiDogadjaji[2].Unos(sarajevskiAtentat);
	historijskiDogadjaji[3].Unos(apolloSlijetanje);
	historijskiDogadjaji[4].Unos(blackThursday);

	cout << endl << "Najstariji datum: " << endl;
	GetNajstarijiDatum(historijskiDogadjaji, 5)->Ispis();
	cout << endl << "Najnoviji datum: " << endl;
	GetNajnovijiDatum(historijskiDogadjaji, 5)->Ispis();

	//DEALOKACIJE
	danD.Dealokacija();
	operacijaBarbarossa.Dealokacija();
	sarajevskiAtentat.Dealokacija();
	apolloSlijetanje.Dealokacija();
	blackThursday.Dealokacija();
	for (size_t i = 0; i < 5; i++)
		historijskiDogadjaji[i].Dealokacija();
	cout << endl << "Dealokacija uspjesna!" << endl;
}

void Zadatak2() {
	cout << ":Zadatak 2:" << endl;

	Glumac alPacino, deNiro;
	Datum temp1, temp2;
	temp1.Unos(25, 4, 1940);
	temp2.Unos(17, 8, 1943);
	alPacino.Unos("1304996055555", "Alfredo", "Pacino", temp1, "Manhattan, NYC", "SAD", 1);
	deNiro.Unos(alPacino);
	deNiro.SetIme("Robert");
	deNiro.SetPrezime("De Niro");
	deNiro.SetDatumRodjenja(temp2);
	deNiro.SetMjestoRodjenja("Greenwich Village");
	deNiro.SetDrzava("SAD");
	deNiro.SetSpol(1);

	Datum datumi[5];
	datumi[0].Unos(7, 6, 1995);
	datumi[1].Unos(7, 11, 1991);
	datumi[2].Unos(3, 6, 1995);
	datumi[3].Unos(5, 6, 1992);
	datumi[4].Unos(1, 6, 1993);
	Glumac glumciNarodnogPozorista[5];
	glumciNarodnogPozorista[0].Unos("0706995088888", "Barb", "Dwyer", datumi[0], "Mostar", "BiH", 1);
	glumciNarodnogPozorista[1].Unos("0711991144488", "Anna", "Graham", datumi[1], "Ostrozac", "BiH", 0);
	glumciNarodnogPozorista[2].Unos("0306995099999", "Benjamin", "Button", datumi[2], "Sarajevo", "BiH", 1);
	glumciNarodnogPozorista[3].Unos("0506992011993", "Jack", "Pott", datumi[3], "Vogosca", "BiH", 1);
	glumciNarodnogPozorista[4].Unos("0106993094444", "Justin", "Case", datumi[4], "Kotor-Varos", "BiH", 1);

	cout << "Ispis svih glumaca:: " << endl;
	for (size_t i = 0; i < 5; i++) {
		cout << "-----------------------------------------------" << endl;
		glumciNarodnogPozorista[i].Ispis();
	}
	cout << "-----------------------------------------------" << endl;
	cout << endl;

	//DEALOKACIJE
	temp1.Dealokacija();
	temp2.Dealokacija();
	alPacino.Dealokacija();
	deNiro.Dealokacija();
	for (size_t i = 0; i < 5; i++) {
		datumi[i].Dealokacija();
		glumciNarodnogPozorista[i].Dealokacija();
	}
	cout << "Dealokacija uspjesna!" << endl;
}

void Zadatak3() {
	Datum temp1, temp2;
	temp1.Unos(25, 4, 1940);
	temp2.Unos(17, 8, 1943);
	Glumac alPacino, deNiro;
	alPacino.Unos("2504940555551", "Alfredo", "Pacino", temp1, "Manhattan, NYC", "SAD", 1);
	deNiro.Unos("1708943055555", "Robert", "De Niro", temp2, "NYC", "SAD", 1);

	Film irishman;
	irishman.Unos("XXXX-AAAA-BBBB-CCCC", "The Irishman", "Biografija, krimi, drama", 2019);
	irishman.DodajGlumca(alPacino);
	irishman.DodajGlumca(deNiro);
	irishman.DodajOcjenu(10);
	irishman.DodajOcjenu(8);
	irishman.DodajOcjenu(9);
	irishman.DodajOcjenu(8);
	irishman.DodajOcjenu(8);
	Film kopija;
	kopija.Unos(irishman);
	kopija.Ispis();
	//DEALOKACIJE
	irishman.Dealokacija();
	kopija.Dealokacija();
	alPacino.Dealokacija();
	deNiro.Dealokacija();
	temp1.Dealokacija();
	temp2.Dealokacija();
}

void main() {
	int nastaviDalje = 1;
	while (nastaviDalje == 1) {
		int izbor = 0;
		do {
			system("cls");//Ocisti ekran
			cout << "::Zadaci::" << endl;
			cout << "(1) Zadatak 1" << endl;
			cout << "(2) Zadatak 2" << endl;
			cout << "(3) Zadatak 3" << endl;
			cout << "Unesite odgovarajuci broj zadatka za testiranje: -->: ";
			cin >> izbor;
			cout << endl;
		} while (izbor < 1 || izbor > 3);
		switch (izbor) {
		case 1: Zadatak1(); cout << "Done." << endl; break;
		case 2: Zadatak2(); cout << "Done." << endl; break;
		case 3: Zadatak3(); cout << "Done." << endl; break;
		default:break;
		}
		do {
			cout << "DA LI ZELITE NASTAVITI DALJE? (1/0): ";
			cin >> nastaviDalje;
		} while (nastaviDalje != 0 && nastaviDalje != 1);
	}
}
