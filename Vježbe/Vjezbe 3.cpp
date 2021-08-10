#include <iostream>
using namespace std;

//Koristene skracenice u komentarima
// dflt. = default
// user-def. = user-defined (korisnicki-definirano)
// ctor = constructor (konstruktor)
// copy ctor = copy constructor (konstruktor kopije)
// move ctor = move constructor (konstruktor premjestanja)

//Z1.0
//Kreiranje  funckije koja alocira novi char array i prekopira stari u njega:
char* AlocirajIKopiraj(const char* tekst)
{
	if (tekst == nullptr)
		return nullptr;//Ako je postojeci  char array prazan vrati prazni array;
	int size = strlen(tekst) + 1;
	char* noviTxt = new char[size];
	strcpy_s(noviTxt, size, tekst);
	return noviTxt;
}

//Kreiranje funkcije koja vraca min el od dva unesena:
int Min(int a, int b)
{
	if (a <= b)
		return a;
	return b;
}
//Kreiranje funkcije koja vraca max element od dva unesena:
int Max(int a, int b)
{
	if (a >= b)
		return a;
	return b;
}

//Kreiranje klase Datum:
class Datum
{
private:
	int* _dan;
	int* _mjesec;
	int* _godina;
public:
	//Z1.1 Dflt. ctor [Postaviti na dflt. vrijednosti]
	Datum()
	{
		_dan = nullptr;
		_mjesec = nullptr;
		_godina = nullptr;
	}
	//Z1.2 User-def. ctor
	Datum(int d, int m, int g)
	{
		_dan = new int;//Dflt.ctor za int;
		*_dan = d;
		//Drugi nacin:
		_mjesec = new int(m);// Copy.ctor za int;
		_godina = new int(g);
	}
	//Z1.3 Copy ctor
	Datum(const Datum& obj)//Parametar moze biti const jer se na njemu nista ne radi od modifikacija;
	{
		_dan = new int;//Dflt.ctor za int;
		*_dan = *obj._dan;
		//Drugi nacin:
		_mjesec = new int(*obj._mjesec);//Copy.ctor za int;
		_godina = new int(*obj._godina);
	}
	//Z1.4 Move ctor
	Datum(Datum&& obj) noexcept //Ne moze da bude parametar  const  jer se vrse na njemu promjene
	{
		_dan = obj._dan;//Pointer _dan poprima lok i sadrzaj pointera _dan od objekta;
		obj._dan = nullptr;//Brisemo sadrzaj i lok ovog pointera
		_mjesec = obj._mjesec;
		obj._mjesec = nullptr;
		_godina = obj._godina;
		obj._godina = nullptr;
	}
	//Z1.5
//Geteri:
	int GetDan() const { return *_dan; }
	int GetMjesec() const { return *_mjesec; }
	int GetGodina() const { return *_godina; }
	//Z1.6
//Seteri:
	void SetDan(int dan)
	{
		if (_dan == nullptr)//Moramo provjeriti da li je alocirana varijabla prije nego sto joj dodjelimo nesta;
			_dan = new int;
		*_dan = dan;
	}
	void SetMjesec(int mjesec)
	{
		if (_mjesec == nullptr)
			_mjesec = new int;
		*_mjesec = mjesec;
	}
	void SetGodina(int godina)
	{
		if (_godina == nullptr)
			_godina = new int;
		*_godina = godina;
	}
	//Z1.7
	void Ispis()
	{
		cout << *_dan << "." << *_mjesec << "." << *_godina << ".";
	}
	//Z1.8
	~Datum()
	{
		delete _dan, _mjesec, _godina;
		_dan = _mjesec = _godina = nullptr;
	}
};

//Kreiranje klase Glumac:
class Glumac
{
private:
	char* _ime;
	char* _prezime;
	char* _zemljaPorijekla;
	Datum* _datumRodjenja;
	bool* _spol; //1-Muski, 0-Zenski
public:
	//Z2.0 Dflt. ctor
	Glumac()
	{
		_ime = nullptr;
		_prezime = nullptr;
		_zemljaPorijekla = nullptr;
		_datumRodjenja = nullptr;
		_spol = nullptr;
	}
	//Z2.1 User-def. ctor
	Glumac(const char* ime, const char* prezime, const char* zemlja, int d, int m, int g, bool spol)
		:_datumRodjenja(new Datum(d,m,g))//user-def .ctor
	{
		_ime = AlocirajIKopiraj(ime);
		_prezime = AlocirajIKopiraj(prezime);
		_zemljaPorijekla = AlocirajIKopiraj(zemlja);
		_spol = new bool(spol);
	}
	//Z2.2 Copy ctor
	Glumac(const Glumac& obj)
		//:_datumRodjenja(obj._datumRodjenja)//Vidi da li moze ovako ???
		:_datumRodjenja(new Datum(*obj._datumRodjenja))//Copy .ctor;
	{
		_ime = AlocirajIKopiraj(obj._ime);
		_prezime = AlocirajIKopiraj(obj._prezime);
		_zemljaPorijekla = AlocirajIKopiraj(obj._zemljaPorijekla);
		_spol = new bool(*obj._spol);//Cpy .ctor; * JER MORA VRATIT INT A NE ADRESU NEKOG INT !!!
	}
	//Z2.3 Move ctor
	Glumac(Glumac&& obj) noexcept
		:_datumRodjenja(move(obj._datumRodjenja))
	{
		_ime = obj._ime;
		obj._ime = nullptr;
		_prezime = obj._prezime;
		obj._prezime = nullptr;
		_zemljaPorijekla = obj._zemljaPorijekla;
		obj._zemljaPorijekla = nullptr;
		//_datumRodjenja = obj._datumRodjenja;
		obj._datumRodjenja = nullptr;
		_spol = obj._spol;
		obj._spol = nullptr;
	}
	//Z2.4
//Geteri:
	char* GetIme() const { return _ime; }
	char* GetPrezime() const { return _prezime; }
	char* GetZemljaPorijekla() const { return _zemljaPorijekla; }
	Datum GetDatumRodjenja() const { return *_datumRodjenja; }//Posto vracamo Datum a ne Datum& ondje gdje se pozvala ova fija ce biti kreiran i pozvan dflt.ctor
	bool GetSpol() const { return *_spol; }

	//Z2.5
//Setteri:
	void SetIme(const char* ime)
	{
		delete[] _ime;//Nema potrebe da se pita jel nullptr jer su oni vazda junk ili puni medjutim mora im se sadrzaj izbrisati;
		_ime = AlocirajIKopiraj(ime);
	}
	void SetPrezime(const char* prezime)
	{
		delete[]_prezime;
		_prezime = AlocirajIKopiraj(prezime);
	}
	void SetZemljaPorijekla(const char* zemlja)
	{
		delete[]_zemljaPorijekla;
		_zemljaPorijekla = AlocirajIKopiraj(zemlja);
	}
	void SetDatumRodjenja(Datum datumRodjenja)
	{
		if (_datumRodjenja == nullptr)
			_datumRodjenja = new Datum;//Dflt .ctor
		//Posto je vec kreiran moramo iskoristit setere i getere tog objekta:
		//_datumRodjenja(datumRodjenja.GetDan(), datumRodjenja.GetMjesec(), datumRodjenja.GetGodina()) OVO NE MOZE JER JE OVO POZIVANJE CTOR-A ZA VEC KREIRAN OBJEKAT
		_datumRodjenja->SetDan(datumRodjenja.GetDan());//Dereferenciranje objekta _datumRodjenja sa ->
		_datumRodjenja->SetMjesec(datumRodjenja.GetMjesec());
		_datumRodjenja->SetGodina(datumRodjenja.GetGodina());
	}
	void SetSpol(bool spol)
	{
		if (_spol == nullptr)
			_spol = new bool;//Dflt .ctor
		*_spol = spol;
	}

	//Z2.6
	void Ispis()
	{
		cout << "Ime i prezime : " << _ime << " " << _prezime << endl;
		cout << "Zemlja porijekla : " << _zemljaPorijekla << endl;
		cout << "Datum rodjenja : ";//Nema endl; jer necemo u novi red;
		_datumRodjenja->Ispis();
		cout << "Spol je : " << ((*_spol==true) ? "Musko" : "Zensko") << endl;//Ne zab dereferencirat _spol jer je pointer !!!
	}
	//Z2.7
	~Glumac()
	{
		delete[] _ime, _prezime, _zemljaPorijekla;
		_ime= _prezime= _zemljaPorijekla = nullptr;
		delete _datumRodjenja, _datumRodjenja = nullptr;
		delete _spol, _spol = nullptr;
	}
};

//Kreiranje klase Epizoda:
class Epizoda {
private:
	char* _naziv;
	int* _trajanje; //u minutama
	char _kratakSadrzaj[100];
	Datum _datumPremijere;

	int _maxBrojOcjena; // velicina niza
	int _trenutnoOcjena; //brojac
	int* _ocjene;
public:
	//Z3.0
	//Settovati vrijednosti na defaultne
	Epizoda()
		:_datumPremijere()//Iako nema potrebe za ovim jer je to uradjeno vec gore (pozvan dflt.ctor);
	{
		_naziv = nullptr;
		_trajanje = nullptr;
		strcpy_s(_kratakSadrzaj, 100, "");//Postavljanje na empty string;
		_maxBrojOcjena = 0;
		_trenutnoOcjena = 0;
		_ocjene = nullptr;
	}
	//Z3.1
	Epizoda(const char* naziv, int trajanje, const char* kratakOpis, Datum datum, int ukupnoOcjena)
		:_datumPremijere(datum.GetDan(),datum.GetMjesec(),datum.GetGodina())//Jer je ovo user-def .ctor;
	{
		_naziv = AlocirajIKopiraj(naziv);
		_trajanje = new int(trajanje);//Copy .ctor za int;
		strcpy_s(_kratakSadrzaj, 100, kratakOpis);
		//Header ili:
		/*_datumPremijere.SetDan(datum.GetDan());
		_datumPremijere.SetMjesec(datum.GetMjesec());
		_datumPremijere.SetGodina(datum.GetGodina());*/
		_maxBrojOcjena = ukupnoOcjena;
		_ocjene = new int[_maxBrojOcjena];
		_trenutnoOcjena = 0;
	}
	//Z3.2
	Epizoda(const Epizoda& obj)
		:_datumPremijere((obj._datumPremijere))//Cpy .ctor 
	{
		_naziv = AlocirajIKopiraj(obj._naziv);
		_trajanje = new int(*obj._trajanje);
		strcpy_s(_kratakSadrzaj, 100, obj._kratakSadrzaj);
		_maxBrojOcjena = obj._maxBrojOcjena;
		_ocjene = new int[_maxBrojOcjena];
		for (int i = 0; i < _maxBrojOcjena; i++)
		{
			_ocjene[i] = obj._ocjene[i];
		}
		_trenutnoOcjena = obj._trenutnoOcjena;
	}
	//Z3.3
	Epizoda(Epizoda&& obj) noexcept
		:_datumPremijere(move(obj._datumPremijere))//Move .ctor
	{
		_naziv = obj._naziv;
		obj._naziv = nullptr;
		_trajanje = obj._trajanje;;
		obj._trajanje = nullptr;
		strcpy_s(_kratakSadrzaj, 100, obj._kratakSadrzaj);
		strcpy_s(obj._kratakSadrzaj, 100, "");//Kako bi se vratilo u prazni string;
		_maxBrojOcjena = obj._maxBrojOcjena;
		obj._maxBrojOcjena = 0;
		_ocjene = obj._ocjene;
		obj._ocjene = nullptr;
		_trenutnoOcjena = obj._trenutnoOcjena;
		obj._trenutnoOcjena = 0;
	}

	//Z3.4
//Geteri:
	char* GetNaziv() const { return _naziv; }
	int GetTrajanje() const { return *_trajanje; }
	const char* GetKratakSadrzaj() const { return _kratakSadrzaj; }
	Datum GetDatumPremijere() const { return _datumPremijere; }//Ondje gdje se pozove tu se okine Datum dflt.ctor jer ne vraca Datum& vec Datum;
	int GetTrenutnoOcjena() const { return _trenutnoOcjena; }
	int GetMaxBrojOcjena() const { return _maxBrojOcjena; }

	//Z3.5
	//Vratiti vrijednost na lokaciji specificiranoj ulaznim parametrom 'index'
	//Ukoliko je proslijedjena nevalidna vrijednost, potrebno je vratiti element niza sa najblizim validnim indeksom
	//Npr. Ako je index = -5, vratiti vrijednost _ocjene na indexu 0; Isto vazi i za indekse vece od vrijednosti brojaca '_trenutnoOcjena'.
	int GetOcjena(int index) const
	{
		index = Max(index, 0);
		index = Min(index, _trenutnoOcjena - 1);//-1 jer se krece od 0 ???
		return _ocjene[index];//Zar se sa ove dvije linije nece mijenjati indeks ???
	}
	//Z3.6
//
	void SetNaziv(const char* naziv)
	{
		delete[]_naziv;
		_naziv = AlocirajIKopiraj(naziv);
	}
	void SetTrajanje(int trajanje)
	{
		if (_trajanje == nullptr)
			_trajanje = new int;//Dflt .ctor za int;
		*_trajanje = trajanje;
	}
	void SetKratakSadrzaj(const char* kratakSadrzaj)
	{
		strcpy_s(_kratakSadrzaj, 100, kratakSadrzaj);
	}
	void SetDatumPremijere(Datum datum)
	{
		_datumPremijere.SetDan(datum.GetDan());
		_datumPremijere.SetMjesec(datum.GetMjesec());
		_datumPremijere.SetGodina(datum.GetGodina());
	}

	//Z3.7
	//Prosiriti niza _ocjene na sljedeci nacin:
	//	*Konstruisati novi niz velicine [_maxBrojOcjena + prosiriZa]
	//  *Kopirati vrijednosti iz starog niza u novi niz
	//  *Dealocirati stari niz
	void ProsiriNizOcjena(int prosiriZa)
	{
		//Prvo moramo provjeriti da li je stari niz uopce alociran:
		if(_ocjene==nullptr)
		{
			_ocjene = new int[prosiriZa];//Jer ce to biti trenutna velicina niza posto nije nista prije bilo;
			return;//Kako bi izasli iz fije;
		}
		_maxBrojOcjena = _maxBrojOcjena + prosiriZa;
		_ocjene = new int[_maxBrojOcjena];//Prosirimo niz za novu velicinu;
		int* temp = _ocjene;//Napravimo pokazivac koji pokazuje na stari niz ocjena;
		for (int i = 0; i < _trenutnoOcjena; i++)
		{
			_ocjene[i] = temp[i];//Prekopiramo sve iz starog niza stim da ce novi niz biti sa jednim viska mjestom (koje smo sada kreirali);
		}
		delete[]temp;//Obrisemo niz;
		temp = nullptr;//Nullpointamo niz;
	}

	//Z3.8
	//Ukoliko je brojac dosao do kraja (jednak velicini niza), uraditi prosirivanje niza za 10 elemenata;
	void DodajOcjenu(int ocjena)
	{
		if (_trenutnoOcjena == _maxBrojOcjena)
			ProsiriNizOcjena(10);//Prosirimo niz za 10;
		else {
			_ocjene[_trenutnoOcjena] = ocjena;//Dodamo ocjenu;
			_trenutnoOcjena++;//Povecamo brojac ocjena;
		}
	}
	//Z3.9
	bool UkloniZadnjuOcjenu()
	{
		if (_trenutnoOcjena == 0)
			return false;//Ne mozemo ukloniti ako nista nema;
		_trenutnoOcjena--;//Dekrementujemo brojac ocjena;
		return true;
	}
	//Z3.10
	float GetProsjecnaOcjena()
	{
		float prosjek = 0.0;
		for (int i = 0; i < _trenutnoOcjena; i++)
		{
			prosjek += _ocjene[i];
		}
		if (_trenutnoOcjena >= 1)
			return prosjek /= _trenutnoOcjena;
		else
			return 0.0;//Ako je 0 elemenata logicno da nema prosjeka;
	}
	//Z3.11
	void Ispis()
	{
		cout << "Naziv : " << _naziv << endl;
		cout << "Trajanje (u minutama) : " << _trajanje << endl;
		cout << "Kratak sadrzaj : " << _kratakSadrzaj << endl;
		cout << "Premijerno prikazivanje : ";//Ne zelimo endl;
		_datumPremijere.Ispis();
		cout << endl;
		cout << "Prosjecna ocjena : " << GetProsjecnaOcjena() << endl;
	}
	//Z3.12
	~Epizoda()
	{
		delete[]_naziv,_ocjene;
		_ocjene = nullptr, _naziv = nullptr;
		delete _trajanje;
		_trajanje = nullptr;
	}
};

//Kreiranje klase Uloga:
class Uloga {
private:
	Glumac* _glumac;
	char* _opis;
	char* _tipUloge; //Glavna, sporedna, epizodna, statista, gostujuca zvijezda, cameo ...
public:
	//Z4.0
	Uloga()
	{
		_glumac = nullptr;
		_opis = nullptr;
		_tipUloge = nullptr;
	}
	//Z4.1
	Uloga(Glumac& glumac, const char* opis, const char* tip)
		:_glumac(new Glumac(glumac))//User-def .ctor
	{
		_opis = AlocirajIKopiraj(opis);
		_tipUloge = AlocirajIKopiraj(tip);
	}
	//Z4.2
	Uloga(const Uloga& obj)
		:_glumac(new Glumac(*obj._glumac))//Copy .ctor; * jer je _glumac pok na objekat tipa Glumac a ocekuje se samo objekat  tipa Glumac;
	{
		_opis = AlocirajIKopiraj(obj._opis);
		_tipUloge = AlocirajIKopiraj(obj._tipUloge);
	}
	//Z4.3
	Uloga(Uloga&& obj) noexcept
		:_glumac(move(obj._glumac))//Move .ctor
	{
		_opis = obj._opis;
		obj._opis = nullptr;
		_tipUloge = obj._tipUloge;
		obj._tipUloge = nullptr;
		obj._glumac = nullptr;
	}
	//Z4.4
//Geteri:
	Glumac GetGlumac() const { return *_glumac; }
	char* GetOpis() const { return _opis; }
	char* GetTipUloge() const { return _tipUloge; }
	//Z4.5
//Seteri:
	void SetGlumac(Glumac glumac)
	{
		if (_glumac == nullptr)
			_glumac = new Glumac;
		_glumac->SetIme(glumac.GetIme());
		_glumac->SetPrezime(glumac.GetPrezime());
		_glumac->SetDatumRodjenja(glumac.GetDatumRodjenja());
		_glumac->SetSpol(glumac.GetSpol());
		_glumac->SetZemljaPorijekla(glumac.GetZemljaPorijekla());
	}
	void SetOpis(const char* opis)
	{
		delete[] _opis;
		_opis = AlocirajIKopiraj(opis);
	}
	void SetTipUloge(const char* tipUloge)
	{
		delete[]_tipUloge;
		_tipUloge = AlocirajIKopiraj(tipUloge);
	}
	//Z4.6
	void Ispis()
	{
		cout << "Glumac : "<<_glumac << endl;
		_glumac->Ispis();
		cout << "Opis uloge : " << _opis << endl;
		cout << "Tip uloge : " << _tipUloge << endl;
	}
	//Z4.7
	~Uloga()
	{
		delete[] _tipUloge, _opis;
		_tipUloge= _opis = nullptr;
		delete _glumac, _glumac = nullptr;
	}
};

//Kreiranje klase Serija:
class Serija {
private:
	char* _naziv;
	int _trenutnoUloga;
	Uloga* _uloge[50] = { nullptr }; //Svi elementi (pokazivaci) se postavljaju na NULL

	int _maxBrojEpizoda;
	int _trenutnoEpizoda;
	Epizoda* _epizode;
public:
	//Z5.0
	Serija()
	{
		_naziv = nullptr;
		_trenutnoEpizoda = 0;
		_trenutnoUloga = 0;
		_maxBrojEpizoda = 0;
		_epizode = nullptr;
	}
	//Z5.1
	Serija(const char* naziv, int maxBrojEpizoda)
	{
		_naziv = AlocirajIKopiraj(naziv);
		_maxBrojEpizoda = maxBrojEpizoda;
		_epizode = new Epizoda[_maxBrojEpizoda];
		_trenutnoEpizoda = 0;
		_trenutnoUloga = 0;
	}
	//Z5.2
	Serija(const Serija& obj)
	{
		_naziv = AlocirajIKopiraj(obj._naziv);
		_trenutnoUloga = obj._trenutnoUloga;
		for (int i = 0;i  < _trenutnoUloga; i++)
		{
			_uloge[i] = new Uloga(*obj._uloge[i]);//Nove pokazivace stavi da pokazuju na stare elemente niza (uloge) pomocu copy .ctor;
		}
		_maxBrojEpizoda = obj._maxBrojEpizoda;
		_epizode = new Epizoda[_maxBrojEpizoda];
		for (int i = 0; i < _maxBrojEpizoda; i++)
		{
			_epizode[i].SetNaziv(obj._epizode[i].GetNaziv());
			_epizode[i].SetTrajanje(obj._epizode[i].GetTrajanje());
			_epizode[i].SetDatumPremijere(obj._epizode[i].GetDatumPremijere());
			_epizode[i].SetKratakSadrzaj(obj._epizode[i].GetKratakSadrzaj());
			for (int j = 0; j < _epizode[i].GetTrenutnoOcjena(); j++)
			{
				_epizode[i].DodajOcjenu(obj._epizode[i].GetOcjena(j));
			}
		}
	}
	//Z5.3
	bool DodajUlogu(Uloga& uloga)
	{
		if (_trenutnoUloga == 50)
			return false;
		/*_uloge[_trenutnoUloga]->SetGlumac(uloga.GetGlumac());
		_uloge[_trenutnoUloga]->SetOpis(uloga.GetOpis());
		_uloge[_trenutnoUloga]->SetTipUloge(uloga.GetTipUloge());*/
		//Ili:
		_uloge[_trenutnoUloga] = new Uloga(uloga);//Cpy .ctor;
		_trenutnoUloga++;
		return true;
	}
	//Z5.4
	bool DodajEpizodu(Epizoda& ep)
	{
		if (_trenutnoEpizoda == _maxBrojEpizoda)
			return false;
		_epizode[_trenutnoEpizoda].SetNaziv(ep.GetNaziv());
		_epizode[_trenutnoEpizoda].SetTrajanje(ep.GetTrajanje());
		_epizode[_trenutnoEpizoda].SetKratakSadrzaj(ep.GetKratakSadrzaj());
		_epizode[_trenutnoEpizoda].SetDatumPremijere(ep.GetDatumPremijere());
		for (int i = 0; i < ep.GetTrenutnoOcjena(); i++)
		{
			int ocjena = ep.GetOcjena(i);
			_epizode[_trenutnoEpizoda].DodajOcjenu(ocjena);
		}
		_trenutnoEpizoda++;
		return true;
	}
	//Z5.5
	Epizoda* GetNajboljeOcijenjenaEpizoda()
	{
		float najboljaProsjecna = 0.0;
		Epizoda* najbolja = &_epizode[0];
		for (int i = 0; i < _trenutnoEpizoda; i++)
		{
			if(_epizode[i].GetProsjecnaOcjena()>=najboljaProsjecna)
			{
				najbolja = _epizode + i;
				najboljaProsjecna = _epizode[i].GetProsjecnaOcjena();
			}
		}
		return najbolja;
	}
	//Z5.6 :: Pored ostalih atributa ispisati i sve uloge i sve epizode
	void Ispis()
	{
		cout << "Naziv serije : " << _naziv << endl;
		cout << "Broj uloga : " << _trenutnoUloga << endl;
		for (int i = 0; i < _trenutnoUloga; i++)
		{
			cout << "**************************************************\n";
			cout << "Uloga " << "[" << i + 1 << "]" << endl;//Broj  trenutne uloge;
			cout << "---------------------------------------------\n";
			_uloge[i]->Ispis();
			cout << "---------------------------------------------\n";
		}
		cout << "Broj episoda : " << _trenutnoEpizoda << endl;
		for (int i = 0; i < _trenutnoEpizoda; i++)
		{
			cout << "**************************************************\n";
			cout << "Epizoda " << "[" << i + 1 << "]" << endl;//Broj  trenutne epizode;
			cout << "---------------------------------------------\n";
			_epizode[i].Ispis();
			cout << "---------------------------------------------\n";
		}
	}
	//Z5.7
	~Serija()
	{
		delete[] _naziv, _naziv = nullptr;
		delete[] _epizode, _epizode = nullptr;
		for (int i = 0; i < _trenutnoUloga; i++)
		{
			delete _uloge[i];
			_uloge[i] = nullptr;
		}
	}
};


void Zadatak1() {
	cout << "Testiranje klase 'Datum'\n\n";
	Datum novaGodina; //Def. ctor
	novaGodina.SetDan(1);
	novaGodina.SetMjesec(1);
	novaGodina.SetGodina(2021);
	novaGodina.Ispis();
	cout << endl;
	//
	Datum prviFebruar(novaGodina.GetDan(), novaGodina.GetMjesec() + 1, novaGodina.GetGodina());
	prviFebruar.Ispis();
	cout << endl;

	Datum prviMart(1, 3, 2021); //User-def. ctor
	prviMart.Ispis();
	cout << endl;

	Datum danSale(prviMart); //Copy ctor
	danSale.SetMjesec(4);
	danSale.Ispis();
	cout << endl;

	Datum praznikRada(move(danSale)); //Move ctor
	praznikRada.SetMjesec(5);
	praznikRada.Ispis();
	cout << endl;
	cout << "Dealokacija ..." << endl;
}
void Zadatak2() {
	cout << "Testiranje klase 'Glumac'\n\n";
	Glumac ryanGosling; //Def. ctor
	ryanGosling.SetIme("Ryan");
	ryanGosling.SetPrezime("Gosling");
	ryanGosling.SetSpol(1);
	ryanGosling.SetDatumRodjenja(Datum(1, 1, 1980));
	ryanGosling.SetZemljaPorijekla("Kanada");
	ryanGosling.Ispis();
	cout << endl;
	//
	Glumac harrisonFord("Harrison", "Ford", "SAD", 2, 2, 1955, 1); //User-def. ctor
	Glumac michellePfeifer("Michelle", "Pfeiffer", "SAD", 3, 3, 1966, 0); //User-def. ctor
	harrisonFord.Ispis();
	cout << endl;
	michellePfeifer.Ispis();
	cout << endl;

	Glumac jackNicholson(harrisonFord); // copy ctor
	jackNicholson.SetIme("Jack");
	jackNicholson.SetPrezime("Nicholson");
	jackNicholson.SetDatumRodjenja(Datum(1, 4, 1945));
	jackNicholson.Ispis();
	cout << endl;

	Glumac heathLedger(move(jackNicholson)); //move ctor
	heathLedger.SetIme("Heath");
	heathLedger.SetPrezime("Ledger");
	heathLedger.SetDatumRodjenja(Datum(5, 3, 1983));
	heathLedger.SetZemljaPorijekla("Australija");
	heathLedger.Ispis();
	cout << endl;
	cout << "Dealokacija ..." << endl;
}
void Zadatak3() {
	cout << "Testiranje klase 'Epizoda'\n\n";
	Epizoda e1;
	e1.SetNaziv("What's Cooking?");
	e1.SetTrajanje(21);
	e1.SetKratakSadrzaj("Bender decides to become a chef so ...");
	e1.SetDatumPremijere(Datum(5, 5, 2021));
	e1.Ispis();
	cout << endl;

	Epizoda e2("This Mission is Trash", 22, "Fry, Leela, and Bender travel to the garbage meteor and discover loads of discarded junk.", Datum(13, 5, 2021), 10);
	e2.Ispis();
	cout << endl;

	Epizoda e3(e2);
	e3.SetNaziv("Smell-o-Scope");
	e3.SetTrajanje(20);
	e3.SetKratakSadrzaj("Using Professor Farnsworth's Smell-o-Scope, Fry locates the stinkiest object in the universe.");
	e3.SetDatumPremijere(Datum(21, 5, 2021));
	e3.Ispis();
	cout << endl;

	Epizoda e4(move(e3));
	e4.SetNaziv("Electric Drug");
	e4.SetTrajanje(24);
	e4.SetKratakSadrzaj("Bender's electricity addiction puts the Planet Express crew in danger");
	e4.SetDatumPremijere(Datum(29, 5, 2021));
	for (size_t i = 0; i < 15; i++)
		e4.DodajOcjenu(rand() % 10 + 1);
	e4.UkloniZadnjuOcjenu();
	e4.UkloniZadnjuOcjenu(); //Brisemo zadnje dvije ocjene
	cout << endl;
	e4.Ispis();
	cout << "Dealokacija ..." << endl;
}

void Zadatak4() {
	cout << "Testiranje klase 'Uloga'\n\n";
	Glumac seanConnery("Sean", "Connery", "Velika Britanija", 25, 8, 1930, 1);
	Glumac danielCraig("Daniel", "Craig", "Velika Britanija", 2, 3, 1968, 1);
	Uloga jamesBond(seanConnery, "MI6 Detective James Bond ....", "Main role");
	jamesBond.SetGlumac(danielCraig);
	jamesBond.SetOpis("After earning 00 status and a licence to kill, Secret Agent James Bond sets out on his first mission as 007.");
	jamesBond.SetTipUloge("Main role");

	Uloga bond25(jamesBond);
	Uloga bond26(move(bond25));
	bond26.Ispis();
	cout << "Dealokacija ..." << endl;
}

void Zadatak5() {
	cout << "Testiranje klase 'Serija'\n\n";
	Serija teorijaVelikogPraska("The Big Bang Theory", 200);

	Glumac jimParsons("Jim", "Parsons", "SAD", 17, 7, 1967, 1);
	Glumac johnnyGalecki("Johnny", "Galecki", "SAD", 15, 3, 1975, 1);
	Glumac kaleyCuoco("Kaley", "Cuoco", "SAD", 13, 4, 1985, 0);
	Uloga sheldonCooper(jimParsons, "Dr. Sheldon Cooper, a theoretical physicist at Caltech", "Series regular");
	Uloga leonardHofstadter(johnnyGalecki, "Dr. Leonard Hofstadter, a experimental physicist at Caltech", "Series regular");
	Uloga penny(kaleyCuoco, "Penny, a waitress at Cheesecake factory", "Series regular");
	//Serija::Dodavanje uloga
	teorijaVelikogPraska.DodajUlogu(sheldonCooper);
	teorijaVelikogPraska.DodajUlogu(leonardHofstadter);
	teorijaVelikogPraska.DodajUlogu(penny);

	Epizoda E1("The Big Bran Hypothesis", 22, "When Sheldon and Leonard drop off a box of flat pack furniture...", Datum(1, 6, 2021), 100);
	Epizoda E2("The Luminous Fish Effect", 21, "Sheldon is fired from his job as a physicist after insulting his new boss...", Datum(8, 6, 2021), 100);
	Epizoda E3("The Bat Jar Conjecture", 22, "The guys decide to compete in a university quiz called physics bowl...", Datum(15, 6, 2021), 100);
	Epizoda E4("The Nerdvana Annihilation", 21, "In an online auction, Leonard buys a full-sized replica of the time machine...", Datum(22, 6, 2021), 100);

	//Epizoda::DodajOcjenu
	int ocjene1[] = { 5,7,8 }, ocjene2[] = { 10,5,7,10,9 }, ocjene3[] = { 9,8,9,9 }, ocjene4[] = { 10,5,3,7,6,6 };
	for (size_t i = 0; i < size(ocjene1); i++)
		E1.DodajOcjenu(ocjene1[i]);
	for (size_t i = 0; i < size(ocjene2); i++)
		E2.DodajOcjenu(ocjene2[i]);
	for (size_t i = 0; i < size(ocjene3); i++)
		E3.DodajOcjenu(ocjene3[i]);
	for (size_t i = 0; i < size(ocjene4); i++)
		E4.DodajOcjenu(ocjene4[i]);

	//Serija::DodajEpizodu
	teorijaVelikogPraska.DodajEpizodu(E1);
	teorijaVelikogPraska.DodajEpizodu(E2);
	teorijaVelikogPraska.DodajEpizodu(E3);
	teorijaVelikogPraska.DodajEpizodu(E4);

	Serija bigbangTheory1(teorijaVelikogPraska);
	bigbangTheory1.Ispis();

	Epizoda* ep = bigbangTheory1.GetNajboljeOcijenjenaEpizoda();
	cout << "Najbolje ocijenjena epizoda: " << ep->GetNaziv() << endl;
	cout << "Ocjena: " << ep->GetProsjecnaOcjena() << endl;
	cout << "Dealokacija ..." << endl;
}

void main() {
	int nastaviDalje = 1;
	while (nastaviDalje == 1) {
		int izbor = 0;
		do {
			system("cls");
			cout << "::Zadaci::" << endl;
			cout << "(1) Zadatak 1" << endl;
			cout << "(2) Zadatak 2" << endl;
			cout << "(3) Zadatak 3" << endl;
			cout << "(4) Zadatak 4" << endl;
			cout << "(5) Zadatak 5" << endl;
			cout << "Unesite odgovarajuci broj zadatka za testiranje: -->: ";
			cin >> izbor;
			cout << endl;
		} while (izbor < 1 || izbor > 5);
		switch (izbor) {
		case 1: Zadatak1(); cout << "Zadatak 1. Done." << endl; break;
		case 2: Zadatak2(); cout << "Zadatak 2. Done." << endl; break;
		case 3: Zadatak3(); cout << "Zadatak 3. Done." << endl; break;
		case 4: Zadatak4(); cout << "Zadatak 4. Done." << endl; break;
		case 5: Zadatak5(); cout << "Zadatak 5. Done." << endl; break;
		default:break;
		}
		do {
			cout << "DA LI ZELITE NASTAVITI DALJE? (1/0): ";
			cin >> nastaviDalje;
		} while (nastaviDalje != 0 && nastaviDalje != 1);
	}
}
