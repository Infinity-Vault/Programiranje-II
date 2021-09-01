#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <regex>
#include <mutex>
#include<thread>
using namespace std;
const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE - U, \n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"10.ZA TESTIRANJE BUDITE SLOBODNI DODATI TESTNIH PODATAKA (POZIVA METODA) KOLIKO GOD SMATRATE DA JE POTREBNO!\n"
"------------------------------------------------------------------------------- \n";
const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";
char* GetNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template<class T1, class T2, int max = 10>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int* _trenutno;
public:
	Kolekcija() {
		_trenutno = new int(0);
	}
	Kolekcija(const Kolekcija&obj)
	{
		_trenutno = new int(*obj._trenutno);
		for (int i=0; i<*obj._trenutno; i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
	}
	Kolekcija&operator=(const Kolekcija&obj)
	{
		if (this==&obj)
		{
			return *this;
		}
		for (int i=0; i<*_trenutno; i++)
		{
			delete _elementi1[i];
			_elementi1[i] = nullptr;
			delete _elementi2[i];
			_elementi2[i] = nullptr;
			
		}
		if (_trenutno == nullptr) { _trenutno = new int();}
		*_trenutno = *obj._trenutno;
		for (int i=0; i<*_trenutno; i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
		return *this;
	}
	~Kolekcija() {
		for (size_t i = 0; i < *_trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}

		delete _trenutno; _trenutno = nullptr;
	}
	T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
	int getTrenutno() const{ return *_trenutno; }
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	/*metoda AddElement baca izuzetak u slucaju da se pokusa dodati vise od maksimalnog broja elemenata*/
	void AddElement(T1 el1,T2 el2)
	{
		if (*_trenutno >= max) { throw exception("Prekoracili ste maksimalan broj elemenata"); }
		_elementi1[*_trenutno] = new T1(el1);
		_elementi2[*_trenutno] = new T2(el2);
		(*_trenutno)++;
	}
	void RemoveAt(int lokacija)
	{
		/*uklanja par (T1 i T2) iz kolekcije koji se nalazi na lokaciji sa   proslijedjenim indeksom.
		nakon uklanjanja vrijednosti onemoguciti pojavu praznog prostora unutar    kolekcije tj.
		pomjeriti sve elemente koji se nalaze nakon proslijedjene lokacije za   jedno mjesto unazad
		npr. ako unutar kolekcije postoje elementi
		0 0
		1 1
		2 2
		3 3
		nakon uklanjanja vrijednosti na lokaciji 1, sadrzaj kolekcije ce biti  sljedeci
		0 0
		2 2
		3 3
		*/
		/*
		 * izbaci 7
		 * _trenutno 10
		 *  -----------------------------------------
		 *  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
		 *  -----------------------------------------
		 *  idemo od pomocne do trenutno to jeste od broja kojeg zelimo izbaciti do trenutno i onda to prazno mjesto za jedno mjesto u unaprijed pomjeramo
		 *  remove element od pomocne do trenunto
		 */
		if(lokacija<0 || lokacija>=max)
		{
			throw exception("Prekoracnje opsega");
		}
		for (int i=lokacija ;i<*_trenutno; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		(*_trenutno)--;
	}
	void AddElement(T1 el1, T2 el2, int lokacija)
	{
		/*funkciji AddElement se, kao treci parametar, moze proslijediti i  lokacija na koju se dodaju
		nove vrijednosti pri cemu treba zadrzati postojece vrijednosti pomjerene  za jedno mjesto unaprijed
		u odnosu na definisanu lokaciju npr. ako unutar kolekcije postoje   elementi*/

		if (*_trenutno >= max) { throw exception("Prekoracili ste opseg niza"); }
	
		for(int i=*_trenutno; i>lokacija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		_elementi1[lokacija] =new T1(el1);
		_elementi2[lokacija] = new T2(el2);
		(*_trenutno)++;
		
	}
	
	T2& operator[](T1 el1)
	{
		for (int i=0; i<*_trenutno; i++)
		{
			if (*_elementi1[i]==el1)
			{
				return *_elementi2[i];
			}
		}
	}
	

};
class Datum {
	int* _dan, * _mjesec, * _godina;
	int toDays()const { return *_godina * 365 + *_mjesec * 30 + *_dan; }
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		/* greska 
		 *_dan = &dan;
		_mjesec = &mjesec;
		_godina = &godina;*/
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum&obj)
	{
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum&operator=(const Datum&obj)
	{
		if (this==&obj)
		{
			return *this;
		}
		if (_dan == nullptr) { _dan = new int; }
		*_dan = *obj._dan;
		if (_mjesec == nullptr) { _mjesec = new int; }
		*_mjesec = *obj._mjesec;
		if (_godina == nullptr) { _godina = new int; }
		*_godina = *obj._godina;
		return *this;
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
	friend int operator-(const Datum& d1, const Datum& d2) { return abs(d1.toDays() - d2.toDays()); }
	friend bool operator <(const Datum& d1, const Datum& d2) { return d1.toDays() < d2.toDays(); }
	friend bool operator ==(const Datum&d1, const Datum&d2)
	{
		return *d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina;
	}
	friend bool operator!=(const Datum& d1, const Datum& d2) { return !(d1 == d2); }
	
	
};

class Tehnika {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a Datum na datum kada je ocijenjena odredjena tehnika
	Kolekcija<int, Datum*, brojTehnika> _ocjene;
public:
	Tehnika(const char* naziv) {
		_naziv = GetNizKaraktera(naziv);
		
	}
	Tehnika(const Tehnika&obj)
	{
		_naziv = GetNizKaraktera(obj._naziv);
		_ocjene = obj._ocjene;
	}
	Tehnika&operator=(const Tehnika&obj)
	{
		if (this==&obj)
		{
			return *this;
		}
		delete[]_naziv;
		_naziv = GetNizKaraktera(obj._naziv);
		_ocjene = obj._ocjene;
		return *this;
	}
		
	/*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise   navrata.
		- razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
		- nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec  evidentirane (bez obzira sto je stariji od 3 dana)
	*/
	bool AddOcjena(int ocjena, Datum& datum)
	{
		if (_ocjene.getTrenutno()==0)
		{
			_ocjene.AddElement(ocjena, new Datum(datum));
			return true;
		}
		
		if (*GetOcjene().getElement2(GetOcjene().getTrenutno() - 1) - datum < 3){return false;	}
		
		for (int i=0; i<_ocjene.getTrenutno(); i++)
		{
			if (datum<*_ocjene.getElement2(i))
			{
				return false;
			}
		}
		_ocjene.AddElement(ocjena,new Datum(datum));
		return true;
			
	}
	~Tehnika() {
		delete[] _naziv; _naziv = nullptr;
	}
	char* GetNaziv() { return _naziv; }
	Kolekcija<int, Datum*, brojTehnika>& GetOcjene() { return _ocjene; }
	// ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0
	float prosjekTehnike()const
	{
		float prosjek = 0.0f;

		if (_ocjene.getTrenutno()==0)
		{
			return 0.0f;
		}
		for (int i=0; i<_ocjene.getTrenutno(); i++)
		{
			prosjek += _ocjene.getElement1(i);
		}
		return prosjek / _ocjene.getTrenutno();


	}
	// ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
	friend ostream&operator<<(ostream&COUT, const Tehnika&t1)
	{
		COUT << "Naziv tehnike: " << t1._naziv << endl;
		COUT << "Ocjene: " << endl;
		for (int i=0; i<t1._ocjene.getTrenutno(); i++)
		{
			COUT << t1._ocjene.getElement1(i) << "  " << *t1._ocjene.getElement2(i) << endl;
		}
		COUT << "Prosjek ocjene tehnike: " << t1.prosjekTehnike() << endl;
		return COUT;
	}
	friend bool operator==(const Tehnika&t1,const Tehnika&t2)
	{
		if (strcmp(t1._naziv,t2._naziv)!=0)
		{
			return false;
		}
		return true;
	}
		
};

class Polaganje {
	Pojas _pojas;
	vector<Tehnika*> _polozeneTehnike;
public:
	Polaganje() { _pojas = BIJELI; }
	Polaganje(Pojas pojas = BIJELI) {
		_pojas = pojas;
	}
	Polaganje(const Polaganje&obj)
	{
		_pojas = obj._pojas;
		for(int i=0; i<obj._polozeneTehnike.size(); i++)
		{
			_polozeneTehnike.push_back(new Tehnika(*obj._polozeneTehnike[i]));
		}
	}
	Polaganje(Pojas pojas, Tehnika tehnika)
	{
		_pojas = pojas;
		_polozeneTehnike.push_back(new Tehnika(tehnika));
	}
	Polaganje&operator=(const Polaganje&obj)
	{
		if(this==&obj)
		{
			return *this;
		}
		_pojas = obj._pojas;
		_polozeneTehnike = obj._polozeneTehnike;
		return *this;
	}
	~Polaganje() {
		for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
			delete _polozeneTehnike[i];
			_polozeneTehnike[i] = nullptr;
		}
	}
	vector<Tehnika*>& GetTehnike() { return _polozeneTehnike; }
	Pojas GetPojas() { return _pojas; }
	friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
		COUT << obj._pojas << endl;
		for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
			COUT << *obj._polozeneTehnike[i];
		return COUT;
	}
float ukupanProsjekPojasa()const
{
	float prosjek = 0;
	if (_polozeneTehnike.size()==0)
	{
		return  prosjek;
	}
	for (int i=0; i<_polozeneTehnike.size(); i++)
	{
		prosjek += _polozeneTehnike[i]->prosjekTehnike();
	}
	return prosjek / _polozeneTehnike.size();
}
};
bool ValidirajLozinku(string lozinka)
{
	/*
	za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
	- najmanje 7 znakova
	- velika i mala slova
	- najmanje jedan broj
    - najmanje jedan specijalni znak
	za provjeru validnosti lozinke koristiti globalnu funkciju  ValidirajLozinku, a unutar nje regex metode.
	Validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u  slucaju da nije validna
	postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
	*/
	 regex length{ "^.{7,}" };//Kapica je pocetak;
    regex velika_mala{ "[A-Z]+[a-z]+" };
    regex jedan_broj{ "[0-9]+" };
    regex specijalni_znak{ "[*!@#$%&]+" };

    if (!regex_search(lozinka, length))//Negiramo svaki if;
        return false;
    if (!regex_search(lozinka, velika_mala))
        return false;
    if (!regex_search(lozinka, jedan_broj))
        return false;
    if (!regex_search(lozinka, specijalni_znak))
        return false;

    return true;
	  
}
class Korisnik {
	char* _imePrezime;
	string _emailAdresa;
	string _lozinka;
public:
	Korisnik() { _imePrezime = nullptr; }
	Korisnik(const char* imePrezime, string emailAdresa, string lozinka)
	{
		_imePrezime = GetNizKaraktera(imePrezime);
		_emailAdresa = emailAdresa;
		_lozinka = ValidirajLozinku(lozinka)?lozinka:NIJE_VALIDNA;//inicijalizirati na nacin zahtijevan u zadatku
	}
	Korisnik(const Korisnik&obj)
	{
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_emailAdresa = obj._emailAdresa;
		_lozinka = obj._lozinka;
	}
	Korisnik&operator=(const Korisnik& obj)
	{
		if (this==&obj){	return *this;}
		delete[]_imePrezime;
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_emailAdresa = obj._emailAdresa;
		_lozinka = obj._lozinka;
		return *this;
	}
	virtual ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }
	string GetEmail()const { return _emailAdresa; }
	string GetLozinka()const { return _lozinka; }
	char* GetImePrezime() const{ return _imePrezime; }
	friend ostream&operator<<(ostream&COUT, const Korisnik&k1)
	{
		COUT << "Ime i prezime: " << k1.GetImePrezime() << endl;
		COUT << "Email: " << k1.GetEmail() << endl;
		COUT << "Lozinka: " << k1.GetLozinka() << endl;
		return COUT;
	}
};

mutex m;
class KaratePolaznik:public Korisnik {
	vector<Polaganje*> _polozeniPojasevi;
    void sendMail(Pojas pojas, Tehnika& tehnika, float ukupan)const
    {
		/*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje
	   email sa porukom:
		FROM:info@karate.ba
		TO: emailKorisnika
		Postovani ime i prezime, evidentirana vam je thenika X za Y pojas.  Dosadasnji uspjeh (prosjek ocjena)
		na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
	  	Pozdrav.
		KARATE Team.
		slanje email poruka implemenitrati koristeci zasebne thread-ove.
		*/
		m.lock();
		cout << "FROM:info@karate.ba" << endl;
		cout << "TO: " << GetEmail() << endl;
		cout << "Postovani " << GetImePrezime() << " , evidentirana Vam je ocjena tehnika " << tehnika.GetNaziv() << " za " << pojas << " pojas. Dosadasnji uspjeh (prosjek ocjene) na pojasu " << pojas << " iznosi " << tehnika.prosjekTehnike() << " a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi "<<ukupan <<"."<< endl;
		cout << "Pozdrav." << endl;
		cout << "KARATE Team." << endl;
    	m.unlock();
    	
    }
	
public:
	KaratePolaznik():Korisnik(){}
	KaratePolaznik(const char* imePrezime, string emailAdresa, string	lozinka):Korisnik(imePrezime,emailAdresa,lozinka) {}
	KaratePolaznik(const KaratePolaznik&obj):Korisnik(obj)
	{
		_polozeniPojasevi = obj._polozeniPojasevi;
	}
	KaratePolaznik&operator=(const KaratePolaznik&obj)
	{
		if (this==&obj)
		{
			return *this;
		}
		Korisnik(*this) = obj;
		_polozeniPojasevi = obj._polozeniPojasevi;
		return *this;
	}
	virtual ~KaratePolaznik()override {
		cout << crt << "DESTRUKTOR -> KaratePolaznik" << crt;
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++) {
			delete _polozeniPojasevi[i];
			_polozeniPojasevi[i] = nullptr;
		}
	}
	
	friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
		COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " << obj.GetLozinka() << endl;
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
			COUT << *obj._polozeniPojasevi[i];
		return COUT;
	}
	vector<Polaganje*>& GetPolozeniPojasevi() { return _polozeniPojasevi; }
	bool AddTehniku(Pojas pojas, Tehnika tehnika)
	{
		/*
		svi kandidati podrazumijevano imaju BIJELI pojas (za njega se ne dodaju  tehnike)
		sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
		tom prilikom onemoguciti:
		- dodavanje istih (moraju biti identicne vrijednosti svih atributa)   tehnika na nivou jednog pojasa,
		- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
		(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3   tehnike za ZUTI pojas i njihov prosjek je veci od 3.5)
		funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
		*/

		// dodavanje istih (moraju biti identicne vrijednosti svih atributa)   tehnika na nivou jednog pojasa,

		auto isIsti = [this, pojas, tehnika]
		{
			for (auto&polozeno:_polozeniPojasevi)
			{
				if (polozeno->GetPojas()==pojas)
				{
					for (auto&polozeneTehnike:polozeno->GetTehnike())
					{
						if (*polozeneTehnike==tehnika)
						{
							return false;
						}
					}
				}
			}
			return true;
		};
		// dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
		auto prethodniPojas = [this, pojas, tehnika]
		{
			Pojas prethodni = static_cast<Pojas>(pojas - 1);
			for (auto&polozeno:_polozeniPojasevi)
			{
				if (polozeno->GetPojas()==prethodni)
				{
					if (polozeno->GetTehnike().size()<3)
					{
						return false;
					}
					if (polozeno->ukupanProsjekPojasa()<3.5f)
					{
						return false;
					}
				}
			}
			return true;
		};
		if (!isIsti() || !prethodniPojas())
		{
			return false;
		}
		for (auto&polozeno:_polozeniPojasevi)
		{
			if (polozeno->GetPojas()==pojas)
			{
				polozeno->GetTehnike().push_back(new Tehnika(tehnika));
				thread posalji(&KaratePolaznik::sendMail, this, pojas, ref(tehnika),polozeno->ukupanProsjekPojasa());
				posalji.join();
				return true;
			}
		}
		Polaganje p(pojas);
		p.GetTehnike().push_back(new Tehnika(tehnika));
		_polozeniPojasevi.push_back(new Polaganje(p));
		thread posalji(&KaratePolaznik::sendMail, this, pojas, ref(tehnika),p.ukupanProsjekPojasa());
		posalji.join();
		return true;
		
	}
};
const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Pojasnite ulogu operatora const_cast?\n";
	return "Odgovor -> Operator const_cast dodaje ili uklanja konstantu ili promnjenjivi modifikator tipu ili iz tog tipa";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Ukratko opisite redoslijed kreiranja objekta bazne klase u slucaju visestrukog nasljedjivanja(prilikom instanciranja objekta najizvedenije klase), te koja su moguca rjesenja najznacajnijih problema u tom kontekstu ? \n";
	return "Odgovor ->Kad pozevemo konstruktor te klase prvo se redom pozivaju konstruktori klasa koje ona nasljeduje, bazna klasa bude zadanja.\nA u slucaju visestrukog nasledjivanja zavisi od toga jel klasa virtuelna\n, jer ako nije pozvat ce konstruktor svake klase koju ona nasljedjuje gdje moze doci do problema sa vise vrijednosti nekog istog elementa ili razbacivanje memorije ";
}
void main() {
	cout << PORUKA;
	cin.get();
	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();

	Datum datum19062020(19, 6, 2020),
		datum20062020(20, 6, 2020),
		datum30062020(30, 6, 2020),
		datum05072020(5, 7, 2020);
		int kolekcijaTestSize = 10;
		Kolekcija<int, int> kolekcija1;
		for (int i = 0; i < kolekcijaTestSize; i++)
			kolekcija1.AddElement(i, i);
		cout << kolekcija1 << endl;
		try {
			/*metoda AddElement baca izuzetak u slucaju da se pokusa dodati vise od maksimalnog broja elemenata*/
			kolekcija1.AddElement(11, 11);
		}
		catch (exception& err) {
			cout << crt << "Greska -> " << err.what() << crt;
		}
		cout << kolekcija1 << crt;
		kolekcija1.RemoveAt(2);
		/*uklanja par (T1 i T2) iz kolekcije koji se nalazi na lokaciji sa   proslijedjenim indeksom.
		nakon uklanjanja vrijednosti onemoguciti pojavu praznog prostora unutar    kolekcije tj.
		pomjeriti sve elemente koji se nalaze nakon proslijedjene lokacije za   jedno mjesto unazad
		npr. ako unutar kolekcije postoje elementi
		0 0
		1 1
		2 2
		3 3
		nakon uklanjanja vrijednosti na lokaciji 1, sadrzaj kolekcije ce biti  sljedeci
		0 0
		2 2
		3 3
		*/
		cout << kolekcija1 << crt;
		kolekcija1.AddElement(9, 9, 2);
		/*funkciji AddElement se, kao treci parametar, moze proslijediti i  lokacija na koju se dodaju
		nove vrijednosti pri cemu treba zadrzati postojece vrijednosti pomjerene  za jedno mjesto unaprijed
		u odnosu na definisanu lokaciju npr. ako unutar kolekcije postoje   elementi
		0 0
		1 1
		2 2
		3 3
		nakon dodavanja vrijednosti 9 i 9 na lokaciju 1, sadrzaj kolekcije ce  biti sljedeci
		0 0
		9 9
		1 1
		2 2
		3 3
		*/
		cout << kolekcija1 << crt;
		Kolekcija<int, int> kolekcija2 = kolekcija1;
		cout << kolekcija1 << crt;
		//na osnovu vrijednosti T2 mijenja vrijednost T1.
		kolekcija1[9] = 2;
		/* npr.ako unutar kolekcije postoje elementi:
		0 0
		9 9
		1 1
		2 2
		3 3
		nakon promjene vrijednosti sadrzaj kolekcije ce biti sljedeci
		0 0
		2 9
		1 1
		2 2
		3 3
		*/
	
		Tehnika choku_zuki("choku_zuki"),
			gyaku_zuki("gyaku_zuki"),
			kizami_zuki("kizami_zuki"),
			oi_zuki("oi_zuki");
		/*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise
	   navrata.
		- razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
		- nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec
	   evidentirane (bez obzira sto je stariji od 3 dana)
		*/
		if (choku_zuki.AddOcjena(1, datum19062020))
			cout << "Ocjena evidentirana!" << endl;
		if (!choku_zuki.AddOcjena(5, datum20062020))
			cout << "Ocjena NIJE evidentirana!" << endl;
		if (choku_zuki.AddOcjena(5, datum30062020))
			cout << "Ocjena evidentirana!" << endl;
		// ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
			// ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0
		cout << choku_zuki << endl;

		if (ValidirajLozinku("john4Do*e"))
			cout << "OK" << crt;
		if (!ValidirajLozinku("john4Doe"))
			cout << "Specijalni znak?" << crt;
		if (!ValidirajLozinku("jo*4Da"))
			cout << "7 znakova?" << crt;
		if (!ValidirajLozinku("4jo-hnoe"))
			cout << "Veliko slovo?" << crt;
		if (ValidirajLozinku("@john2Doe"))
			cout << "OK" << crt;
		/*
//		za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
//		- najmanje 7 znakova
//		- velika i mala slova
//		- najmanje jedan broj
//	   Fakultet informacijskih tehnologija
//	   Pismeni dio ispita iz predmeta Programiranje II – 16.07.2021
//	   6
//		- najmanje jedan specijalni znak
//		za provjeru validnosti lozinke koristiti globalnu funkciju
//	   ValidirajLozinku, a unutar nje regex metode.
//		validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u
//	   slucaju da nije validna
//		postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
//		*/
		Korisnik* jasmin = new KaratePolaznik("Jasmin Azemovic", "jasmin@karate.ba", "j@sm1N*");
		Korisnik* adel = new KaratePolaznik("Adel Handzic", "adel@edu.karate.ba", "4Ade1*H");
		Korisnik* lozinkaNijeValidna = new KaratePolaznik("John Doe", "john.doe@google.com", "johndoe");
		/*
		svi kandidati podrazumijevano imaju BIJELI pojas (za njega se ne dodaju  tehnike)
		sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
		tom prilikom onemoguciti:
		- dodavanje istih (moraju biti identicne vrijednosti svih atributa)   tehnika na nivou jednog pojasa,
		- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
		(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3   tehnike za ZUTI pojas i njihov prosjek je veci od 3.5)
		funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
		*/
		//doraditi klase da nacin da omoguce izvrsenje naredne linije koda
		KaratePolaznik* jasminPolaznik = dynamic_cast<KaratePolaznik*>(jasmin);
		if (jasminPolaznik != nullptr) {
			if (jasminPolaznik->AddTehniku(ZUTI, gyaku_zuki))
				cout << "Tehnika uspjesno dodan!" << crt;
			//ne treba dodati kizami_zuki jer ne postoje evidentirane 3 tehnike za ZUTI pojas
			if (!jasminPolaznik->AddTehniku(NARANDZASTI, kizami_zuki))
				cout << "Tehnika NIJE uspjesno dodana!" << crt;
			if (jasminPolaznik->AddTehniku(ZUTI, kizami_zuki))
				cout << "Tehnika uspjesno dodan!" << crt;
			if (jasminPolaznik->AddTehniku(ZUTI, oi_zuki))
				cout << "Tehnika uspjesno dodan!" << crt;
			if (jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
				cout << "Tehnika uspjesno dodan!" << crt;
			//ne treba dodati choku_zuki jer je vec dodana za zuti pojas
			if (!jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
				cout << "Tehnika NIJE uspjesno dodana!" << crt;
			//ispisuje sve dostupne podatke o karate polazniku
			cout << *jasminPolaznik << crt;
		}
		/*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje
	   email sa porukom:
		FROM:info@karate.ba
		TO: emailKorisnika
		Postovani ime i prezime, evidentirana vam je thenika X za Y pojas.
	   Dosadasnji uspjeh (prosjek ocjena) na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima  iznosi Z.
	   	Pozdrav.
		KARATE Team.
		slanje email poruka implemenitrati koristeci zasebne thread-ove.
		*/
		//osigurati da se u narednim linijama poziva i destruktor klase KaratePolaznik
	
		delete jasmin;
		delete adel;
		delete lozinkaNijeValidna;
		cin.get();
		system("pause>0");
}