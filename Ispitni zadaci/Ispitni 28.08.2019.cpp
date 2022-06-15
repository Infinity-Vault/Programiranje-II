#include <iostream>
#include <cassert>
#include <vector>
#include <mutex>
#include <thread>
#include <regex>

using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
enum SortirajPo { T1, T2 };

char* Alociraj(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;

	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);

	return temp;
}

bool ValidirajEmail(const string& email)
{
	/*
	email adresa mora biti u formatu: text@outlook.com ili text@edu.fit.ba
	u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@edu.fit.ba
	za provjeru koristiti regex
	*/
	return regex_match(email, regex("[A-Za-z]+@(outlook.com|edu.fit.ba)"));
}

template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;

	void ProsiriNizove()
	{
		T1* temp1 = _elementi1;
		T2* temp2 = _elementi2;
		_elementi1 = new T1[_trenutno + 1];
		_elementi2 = new T2[_trenutno + 1];

		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = temp1[i];
			_elementi2[i] = temp2[i];
		}

		delete[] temp1;
		delete[] temp2;
	}
public:
	Kolekcija(bool omoguciDupliranje = true)
		: _elementi1(nullptr), _elementi2(nullptr), _trenutno(0), _omoguciDupliranje(omoguciDupliranje)
	{

	}

	Kolekcija(const Kolekcija& obj)
		: _trenutno(obj._trenutno), _omoguciDupliranje(obj._omoguciDupliranje)
	{
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];

		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj.GetElement1(i);
			_elementi2[i] = obj.GetElement2(i);
		}
	}

	Kolekcija(Kolekcija&& obj) noexcept
	{
		_trenutno = obj._trenutno;
		obj._trenutno = 0;
		_elementi1 = obj._elementi1;
		_elementi2 = obj._elementi2;
		obj._elementi1 = nullptr;
		obj._elementi2 = nullptr;
		_omoguciDupliranje = obj._omoguciDupliranje;
	}

	Kolekcija& operator =(const Kolekcija& obj)
	{
		if (this == &obj)
			return*this;

		delete[]_elementi1;
		delete[]_elementi2;
		_trenutno = obj.GetTrenutno();
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];

		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj.GetElement1(i);
			_elementi2[i] = obj.GetElement2(i);
		}

		_omoguciDupliranje = obj._omoguciDupliranje;

		return *this;
	}
	~Kolekcija()
	{
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
	}

	T1& GetElement1(int lokacija) const { return _elementi1[lokacija]; }
	T2& GetElement2(int lokacija) const { return _elementi2[lokacija]; }
	int GetTrenutno() const { return _trenutno; }

	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj)
	{
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}

	void AddElement(T1 prvi, T2 drugi)
	{
		if (_omoguciDupliranje == false)
		{
			for (int i = 0; i < _trenutno; i++)
			{
				if (_elementi1[i] == prvi && _elementi2[i] == drugi)
					throw exception("Nije dozvoljeno dodavanje duplih elemenata !!!");
			}
		}

		ProsiriNizove();

		_elementi1[_trenutno] = prvi;
		_elementi2[_trenutno] = drugi;
		_trenutno++;
	}

	void SortirajRastuci(SortirajPo algoritam)
	{
		auto sort = [&algoritam](T1 prvi, T1 drugi)
		{
			if (algoritam)
				return prvi > drugi;
			return prvi < drugi;
		};

		for (int i = 0; i < _trenutno - 1; i++)
		{
			for (int j = 0; j < _trenutno - i - 1; j++)
			{
				if (sort(_elementi1[j], _elementi1[j + 1]))
				{
					T1 temp = _elementi1[j];
					_elementi1[j] = _elementi1[j + 1];
					_elementi1[j + 1] = temp;
				}
				if (sort(_elementi2[j], _elementi2[j + 1]))
				{
					T2 temp = _elementi2[j];
					_elementi2[j] = _elementi2[j + 1];
					_elementi2[j + 1] = temp;
				}
			}
		}
	}
};

class DatumVrijeme
{
	int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0)
		: _dan(new int(dan)), _mjesec(new int(mjesec)), _godina(new int(godina)), _sati(new int(sati)), _minuti(new int(minuti))
	{

	}

	DatumVrijeme(const DatumVrijeme& obj)
		: _dan(new int(obj.GetDan())), _mjesec(new int(obj.GetMjesec())), _godina(new int(obj.GetGodina())), _sati(new int(obj.GetSati())), _minuti(new int(obj.GetMinuti()))
	{

	}

	DatumVrijeme(DatumVrijeme&& obj) noexcept
	{
		_dan = obj._dan;
		_mjesec = obj._mjesec;
		_godina = obj._godina;
		_sati = obj._sati;
		_minuti = obj._minuti;

		obj._dan = obj._mjesec = obj._godina = obj._sati = obj._minuti = nullptr;
	}
	DatumVrijeme& operator =(const DatumVrijeme& obj)
	{
		if (this == &obj)
			return *this;

		delete _dan;
		_dan = new int(obj.GetDan());

		delete _mjesec;
		_mjesec = new int(obj.GetMjesec());

		delete _godina;
		_godina = new int(obj.GetMjesec());

		delete _sati;
		_sati = new int(obj.GetSati());

		delete _minuti;
		_minuti = new int(obj.GetMinuti());

		return *this;
	}

	~DatumVrijeme()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}

	int GetDan() const
	{
		assert(_dan != nullptr);
		return *_dan;
	}

	int GetMjesec() const
	{
		assert(_mjesec != nullptr);
		return *_mjesec;
	}

	int GetGodina() const
	{
		assert(_godina != nullptr);
		return *_godina;
	}

	int GetSati()const
	{
		assert(_sati != nullptr);
		return *_sati;
	}

	int GetMinuti()const
	{
		assert(_minuti != nullptr);
		return *_minuti;
	}

	friend ostream& operator<< (ostream& COUT, const DatumVrijeme& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}

	int ToMinuti()const
	{
		int minuti = *_dan * 1440 + *_mjesec * (30 * 1440) + *_godina * (12 * 30 * 1440);
		int minuti2 = *_sati * 60 + *_minuti;

		return minuti + minuti2;
	}

};

bool operator == (const DatumVrijeme& d1, const DatumVrijeme& d2)
{
	return d1.GetDan() == d2.GetDan() && d1.GetMjesec() == d2.GetMjesec() && d1.GetGodina() == d2.GetGodina() && d1.GetSati() == d2.GetSati() && d1.GetMinuti() == d2.GetMinuti();
}

bool operator != (const DatumVrijeme& d1, const DatumVrijeme& d2)
{
	return !(d1 == d2);
}

int operator -(const DatumVrijeme& d1, const DatumVrijeme& d2)
{
	return abs(d1.ToMinuti() - d2.ToMinuti());
}

bool operator > (const DatumVrijeme& d1, const DatumVrijeme& d2)
{
	return d1.ToMinuti() > d2.ToMinuti();
}

class Predmet
{
	char* _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char* naziv = "", int ocjena = 0, string napomena = "")
		: _naziv(Alociraj(naziv)), _ocjena(ocjena), _napomena(napomena)
	{

	}

	Predmet(const Predmet& obj)
		: _naziv(Alociraj(obj.GetNaziv())), _ocjena(obj.GetOcjena()), _napomena(obj.GetNapomena())
	{

	}

	Predmet(Predmet&& obj) noexcept
	{
		_naziv = obj._naziv;
		obj._naziv = nullptr;
		_ocjena = obj._ocjena;
		obj._ocjena = 0;
		_napomena = obj._napomena;
		obj._napomena = "";
	}

	Predmet& operator =(const Predmet& obj)
	{
		if (this == &obj)
			return *this;

		delete[]_naziv;
		_naziv = Alociraj(obj.GetNaziv());
		_ocjena = obj.GetOcjena();
		_napomena = obj.GetNapomena();

		return *this;
	}

	~Predmet()
	{
		delete[] _naziv; _naziv = nullptr;
	}

	friend ostream& operator<< (ostream& COUT, const Predmet& obj)
	{
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}

	string GetNapomena() const { return _napomena; }
	char* GetNaziv() const { return _naziv; }
	int GetOcjena() const { return _ocjena; }

	void DodajNapomenu(const string& napomena)
	{
		_napomena += " " + napomena;
	}
};

bool operator == (const Predmet& p1, const Predmet& p2)
{
	if (strcmp(p1.GetNaziv(), p2.GetNaziv()) != 0)
		return false;
	if (p1.GetOcjena() != p2.GetOcjena())
		return false;
	if (p1.GetNapomena() != p2.GetNapomena())
		return false;

	return true;
}

bool operator != (const Predmet& p1, const Predmet& p2)
{
	return !(p1 == p2);
}

class Uspjeh
{
	eRazred* _razred;
	//formalni argument DatumVrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Kolekcija<Predmet, DatumVrijeme> _predmeti;
public:
	Uspjeh()
		: _razred(nullptr)
	{

	}

	Uspjeh(eRazred razred)
		: _razred(new eRazred(razred))
	{

	}

	Uspjeh(eRazred razred, const Predmet& predmet, DatumVrijeme& date)
	{
		_razred = new eRazred(razred);
		_predmeti.AddElement(predmet, date);
	}

	Uspjeh(const Uspjeh& obj)
		:_razred(new eRazred(*obj.GetERazred())), _predmeti(obj._predmeti)
	{

	}

	Uspjeh(Uspjeh&& obj) noexcept
		:_predmeti(move(obj._predmeti))
	{
		_razred = obj._razred;
		obj._razred = nullptr;
	}

	Uspjeh& operator= (const Uspjeh& obj)
	{
		if (this == &obj)
			return *this;

		if (_razred == nullptr)
			_razred = new eRazred();

		*_razred = *obj.GetERazred();
		_predmeti = obj._predmeti;

		return *this;
	}

	~Uspjeh()
	{
		delete _razred;
		_razred = nullptr;
	}

	Kolekcija<Predmet, DatumVrijeme>* GetPredmeti() { return &_predmeti; }
	eRazred* GetERazred() const { return _razred; }
	int GetPredmetiCount() const { return _predmeti.GetTrenutno(); }

	friend ostream& operator<< (ostream& COUT, const Uspjeh& obj)
	{
		COUT << *obj._razred << " " << obj._predmeti << endl;
		COUT << "Prosjecna ocjena: " << obj.ProsjekOcjena() << endl;
		return COUT;
	}

	bool operator== (const Uspjeh& obj) const
	{
		if (*GetERazred() != *obj.GetERazred())
			return false;

		if (GetPredmetiCount() != obj.GetPredmetiCount())
			return false;

		for (int i = 0; i < GetPredmetiCount(); i++)
		{
			if (const_cast<Uspjeh&>(*this).GetPredmeti()->GetElement1(i) != const_cast<Uspjeh&>(obj).GetPredmeti()->GetElement1(i) || const_cast<Uspjeh&>(*this).GetPredmeti()->GetElement2(i) != const_cast<Uspjeh&>(obj).GetPredmeti()->GetElement2(i))
				return false;
		}

		return true;
	}

	bool operator!= (const Uspjeh& obj) const
	{
		return !(*this == obj);
	}

	float ProsjekOcjena() const
	{
		float prosjek = 0.0f;

		for (int i = 0; i < _predmeti.GetTrenutno(); i++)
		{
			prosjek += _predmeti.GetElement1(i).GetOcjena();
		}

		if (prosjek > 0.0f)
			return  prosjek /= _predmeti.GetTrenutno();

		return prosjek;
	}
};

mutex m;

class Kandidat {
	char* _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh> _uspjeh;

	void PosaljiEmail(eRazred raz) const
	{
		/*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
	   FROM:info@edu.fit.ba
	   TO: emailKorisnika
	   Postovani ime i prezime, evidentirali ste uspjeh za X razred.
	   Pozdrav.
	   FIT Team.

	   ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh X.X u X
	   razredu"
	   */
		m.lock();
		cout << " FROM:info@edu.fit.ba" << endl;
		cout << "TO: " << this->GetEmail() << endl;
		cout << "Postovani " << this->GetImePrezime() << " , evidentirali ste uspjeh za " << raz << " razred." << endl;
		m.unlock();
	}

	void PosaljiSMS(eRazred raz, float prosjek) const
	{
		m.lock();
		cout << "Svaka cast za uspjeh " << prosjek << "  u " << raz << " razredu" << endl;
		m.unlock();
	}
public:
	Kandidat()
		: _imePrezime(nullptr)
	{

	}

	Kandidat(const char* imePrezime, const string& emailAdresa, const string& brojTelefona)
		: _imePrezime(Alociraj(imePrezime)), _brojTelefona(brojTelefona)
	{
		_emailAdresa = ValidirajEmail(emailAdresa) ? emailAdresa : "notSet@edu.fit.ba";
	}

	Kandidat(const Kandidat& obj)
		: _emailAdresa(obj.GetEmail()), _brojTelefona(obj.GetBrojTelefona()),
		_uspjeh(obj._uspjeh)
	{
		_imePrezime = Alociraj(obj.GetImePrezime());
	}

	Kandidat(Kandidat&& obj) noexcept
		:_emailAdresa(move(obj._emailAdresa)), _brojTelefona(move(obj._brojTelefona)),
		_uspjeh(move(obj._uspjeh))
	{
		_imePrezime = obj._imePrezime;
		obj._imePrezime = nullptr;
	}

	Kandidat& operator =(const Kandidat& obj)
	{
		if (this == &obj)
			return *this;

		delete[]_imePrezime;
		_imePrezime = Alociraj(obj.GetImePrezime());
		_emailAdresa = obj.GetEmail();
		_brojTelefona = obj.GetBrojTelefona();
		_uspjeh = obj._uspjeh;

		return *this;
	}

	~Kandidat()
	{
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}

	friend ostream& operator<< (ostream& COUT, const Kandidat& obj)
	{
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		ostream_iterator<Uspjeh> ispis(cout, " ");
		copy(obj._uspjeh.begin(), obj._uspjeh.end(), ispis);

		return COUT;
	}

	vector<Uspjeh>* GetUspjeh() { return &_uspjeh; }
	const string& GetEmail() const { return _emailAdresa; }
	const string& GetBrojTelefona() const { return _brojTelefona; }
	char* GetImePrezime() const { return _imePrezime; }

	bool AddPredmet(eRazred razred, Predmet& predmet, DatumVrijeme& date)
	{
		/*
		uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
		tom prilikom onemoguciti:
		- dodavanje istoimenih predmeta na nivou jednog razreda,
		- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja
		 pojedinih predmeta mora biti najmanje 5 minuta).

		*/
		if (_uspjeh.size() == 0)
		{
			_uspjeh.push_back(Uspjeh(razred, predmet, date));
			thread t(&Kandidat::PosaljiEmail, this, razred);
			t.join();
			for (auto& uspjeh : _uspjeh)
			{
				if (*uspjeh.GetERazred() == razred)
					if (uspjeh.ProsjekOcjena() > 4.5)
					{
						thread t1(&Kandidat::PosaljiSMS, this, razred, uspjeh.ProsjekOcjena());
						t1.join();
					}
			}
			return true;
		}

		auto checkIste = [this, &razred, &predmet]()
		{
			for (auto uspjeh : _uspjeh)
			{
				if (*uspjeh.GetERazred() == razred)
					for (int i = 0; i < uspjeh.GetPredmeti()->GetTrenutno(); i++)
					{
						if (strcmp(uspjeh.GetPredmeti()->GetElement1(i).GetNaziv(), predmet.GetNaziv()) == 0)
							return false;//Nema dodavanja istoimenih predmeta na nivou jednog razreda;
					}
			}
			return true;
		};
		auto checkTime = [this, &razred, &date]()
		{
			for (auto uspjeh : _uspjeh)
			{
				if (*uspjeh.GetERazred() == razred)
					for (int i = 0; i < uspjeh.GetPredmeti()->GetTrenutno(); i++)
					{
						if (uspjeh.GetPredmeti()->GetElement2(i) - date < 5)
							return false;//Nema dodavanja ako je razmak manji od 5 minuta
					}
			}
			return true;
		};

		if (checkIste() && checkTime())
		{
			_uspjeh.push_back(Uspjeh(razred, predmet, date));
			thread t(&Kandidat::PosaljiEmail, this, razred);
			t.join();
			for (auto uspjeh : _uspjeh)
			{
				if (*uspjeh.GetERazred() == razred)
					if (uspjeh.ProsjekOcjena() > 4.5)
					{
						thread t1(&Kandidat::PosaljiSMS, this, razred, uspjeh.ProsjekOcjena());
						t1.join();
					}
			}
			return true;
		}
		return false;
	}

	int BrojPonavljanjaRijeci(const string& key) const
	{
		regex pattern(key);
		auto pocetak = sregex_iterator(key.begin(), key.end(), pattern);
		auto kraj = sregex_iterator();

		return distance(pocetak, kraj);
	}

	vector<Predmet> operator () (const DatumVrijeme& from, const DatumVrijeme& to)
	{
		vector<Predmet> povratni;

		for (auto uspjeh : _uspjeh)
		{
			for (int i = 0; i < uspjeh.GetPredmeti()->GetTrenutno(); i++)
			{
				if (uspjeh.GetPredmeti()->GetElement2(i) > from && to > uspjeh.GetPredmeti()->GetElement2(i))
					povratni.push_back(uspjeh.GetPredmeti()->GetElement1(i));
			}
		}

		return povratni;
	}

	Uspjeh* operator [](eRazred razred)
	{
		for (int i = 0; i < _uspjeh.size(); i++)
		{
			if (*_uspjeh[i].GetERazred() == razred)
				return &_uspjeh[i];
		}

		return nullptr;
	}
};

void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR AE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEEILO GUBLJENJE URA?ENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKO?ER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIENI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUEAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOANE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAEENO.
	7. IMATE POTPUNU SLOBODU DA U MAIN FUNKCIJI, ZA POTREBE TESTIRANJA, DODAJETE VISE POZIVA ILI SALJETE DRUGE VRIJEDNOSTI PARAMETARA
	****************************************************************************/
	//cout << ValidirajEmail("haris@edu.fist.ba");
	DatumVrijeme temp,
		datum19062019_1015(19, 6, 2019, 10, 15),
		datum20062019_1115(20, 6, 2019, 11, 15),
		datum30062019_1215(30, 6, 2019, 12, 15),
		datum05072019_1231(5, 7, 2019, 12, 31);

	const int kolekcijaTestSize = 9;
	Kolekcija<int, int> kolekcija1(false);
	for (size_t i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i + 1, 20 - i);

	try {
		//ukoliko nije dozvoljeno dupliranje elemenata, metoda AddElement baca izuzetak
		kolekcija1.AddElement(6, 15);
	}
	catch (exception& err) {
		cout << err.what() << endl;
	}
	cout << kolekcija1 << endl;

	//na osnovu vrijednosti parametra sortira clanove kolekcije u rastucem redoslijedu 
	kolekcija1.SortirajRastuci(SortirajPo::T2);
	cout << kolekcija1 << endl;


	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;

	Kolekcija<int, int> kolekcija3;
	kolekcija3 = kolekcija1;
	cout << kolekcija3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet Matematika("Matematika", 5, "Ucesce na takmicenju"),
		Fizika("Fizika", 5),
		Hemija("Hemija", 2),
		Engleski("Engleski", 5);
	Fizika.DodajNapomenu("Pohvala za ostvareni uspjeh");
	cout << Matematika << endl;

	/*
	email adresa mora biti u formatu: text@outlook.com ili text@edu.fit.ba
	u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@edu.fit.ba
	za provjeru koristiti regex
	*/


	Kandidat jasmin("Jasmin Azemovic", "jasmin@outlook.com", "033 281 172");
	Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
	Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti:
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 5 minuta).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
	Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (jasmin.AddPredmet(DRUGI, Fizika, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Hemija, datum30062019_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Engleski, datum19062019_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Matematika, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Matematiku jer je vec dodana u prvom razredu
	if (jasmin.AddPredmet(PRVI, Matematika, datum05072019_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Fiziku jer nije proslo 5 minuta od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(PRVI, Fizika, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;


	/*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
	FROM:info@edu.fit.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirali ste uspjeh za X razred.
	Pozdrav.
	FIT Team.

	ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh X.X u X razredu".
	slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	*/
	cout << "USPJEH ISPISATI KORISTECI OSTREAM_ITERATOR" << endl;
	cout << jasmin << endl;


	//vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
	cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;

	//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	vector<Predmet> jasminUspjeh = jasmin(DatumVrijeme(18, 06, 2019, 10, 15), DatumVrijeme(21, 06, 2019, 10, 10));
	for (size_t i = 0; i < jasminUspjeh.size(); i++)
		cout << jasminUspjeh[i] << endl;

	Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
	if (uspjeh_Irazred != nullptr)
		cout << *uspjeh_Irazred << endl;

	cin.get();
	system("pause>0");
}