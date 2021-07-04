#include<iostream>
#include <vector>
#include <mutex>
#include <regex>
#include <string>
#include <thread>
#include <cassert>
#include <fstream>
using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";

enum Sortiranje { ASC, DESC };
enum Predmet { MATEMATIKA, FIZIKA, HEMIJA, GEOGRAFIJA, NOT_SET };
enum Razred { I = 1, II, III, IV };

char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

bool ValidirajTelefon(string telefon)
{
	// +387(6X)XXX-XXX ili +387 6X XXX-XXX

	regex prvi("\\+387\\(6\\d\\)\\d{3}-\\d{3}");
	regex drugi("\\+387\\s6\\d\\s\\d{3}-\\d{3}");
	if (!regex_match(telefon, prvi) && !regex_match(telefon, drugi))
		return false;
	return true;
}
template <class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int _trenutnoElemenata;
	bool _dozvoliDupliranje;
	Sortiranje _sortiranje;
	void Sortiraj()
	{
		auto sort = [this](T1 prvi, T1 drugi)
		{
			if (_sortiranje == ASC)
				return prvi > drugi;
			return prvi < drugi;
		};
		auto sort2 = [this](T2 prvi, T2 drugi)
		{
			if (_sortiranje == ASC)
				return prvi > drugi;
			return prvi < drugi;
		};
		for (int i = 0; i < _trenutnoElemenata - 1; i++)
		{
			for (int j = 0; j < _trenutnoElemenata - i - 1; j++)
			{
				if (sort(*_elementi1[j], *_elementi1[j + 1]))
				{
					T1 temp = *_elementi1[j];
					*_elementi1[j] = *_elementi1[j + 1];
					*_elementi1[j + 1] = temp;
				}
				if (sort2(*_elementi2[j], *_elementi2[j + 1]))
				{
					T2 temp = *_elementi2[j];
					*_elementi2[j] = *_elementi2[j + 1];
					*_elementi2[j + 1] = temp;
				}
			}
		}
	}
public:
	Kolekcija(Sortiranje sortiranje = ASC, bool dozvoliDupliranje = true)
	{
		_trenutnoElemenata = 0;
		_sortiranje = sortiranje;
		_dozvoliDupliranje = dozvoliDupliranje;
	}

	Kolekcija(const Kolekcija& obj)
	{
		_trenutnoElemenata = obj.GetTrenutno();
		_sortiranje = obj._sortiranje;
		_dozvoliDupliranje = obj._dozvoliDupliranje;
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			_elementi1[i] = new T1(obj.GetElement1(i));
			_elementi2[i] = new T2(obj.GetElement2(i));
		}
	}
	Kolekcija(Kolekcija&& obj)noexcept
	{
		_trenutnoElemenata = obj._trenutnoElemenata;
		obj._trenutnoElemenata = 0;
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
			obj._elementi1[i] = nullptr;
			obj._elementi2[i] = nullptr;
		}
		_sortiranje = obj._sortiranje;
		_dozvoliDupliranje = obj._dozvoliDupliranje;
	}
	Kolekcija& operator =(const Kolekcija& obj)
	{
		if (this == &obj)
			return *this;
		_sortiranje = obj._sortiranje;
		_dozvoliDupliranje = obj._dozvoliDupliranje;
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			delete _elementi1[i];
			_elementi1[i] = nullptr;
			delete _elementi2[i];
			_elementi2[i] = nullptr;
		}
		_trenutnoElemenata = obj.GetTrenutno();
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			_elementi1[i] = new T1(obj.GetElement1(i));
			_elementi2[i] = new T2(obj.GetElement2(i));
		}
		return *this;
	}
	~Kolekcija()
	{
		for (size_t i = 0; i < _trenutnoElemenata; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1& GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi1[lokacija];
	}
	T2& GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi2[lokacija];
	}
	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
	void AddElement(T1 prvi, T2 drugi)
	{
		if (_trenutnoElemenata == max)
			throw exception("Nizovi su puni !!!");
		if (_dozvoliDupliranje == false)
		{
			for (int i = 0; i < _trenutnoElemenata; i++)
			{
				if (*_elementi1[i] == prvi && *_elementi2[i] == drugi)
					throw exception("Nije dozvoljeno dupliranje elemenata !!!");
			}
		}
		_elementi1[_trenutnoElemenata] = new T1(prvi);
		_elementi2[_trenutnoElemenata] = new T2(drugi);
		_trenutnoElemenata++;
		Sortiraj();
	}
	void addElementwoSort(T1 prvi, T2 drugi)
	{
		if (_trenutnoElemenata == max)
			throw exception("Nizovi su puni !!!");
		if (_dozvoliDupliranje == false)
		{
			for (int i = 0; i < _trenutnoElemenata; i++)
			{
				if (*_elementi1[i] == prvi && *_elementi2[i] == drugi)
					throw exception("Nije dozvoljeno dupliranje elemenata !!!");
			}
		}
		_elementi1[_trenutnoElemenata] = new T1(prvi);
		_elementi2[_trenutnoElemenata] = new T2(drugi);
		_trenutnoElemenata++;
	}
};

class Aktivnost {
	shared_ptr<Razred> _razred;
	string _opis;
	int _ocjena;//1-5
public:
	Aktivnost(Razred razred = I, int ocjena = 0, string opis = not_set)
	{
		_ocjena = ocjena;
		_opis = opis;
		_razred = make_shared<Razred>(razred);
	}
	Aktivnost(const Aktivnost& obj)
		:_opis(obj.GetOpis())
	{
		_razred = make_shared<Razred>(obj.GetRazred());
		_ocjena = obj.GetOcjenu();
	}
	Aktivnost(Aktivnost&& obj)noexcept
		:_opis(move(obj._opis))
	{
		_razred = obj._razred;
		obj._razred.reset();
		_ocjena = obj._ocjena;
		obj._ocjena = 0;
	}
	~Aktivnost()
	{

	}
	int  GetOcjenu() const { return _ocjena; }
	string GetOpis() const { return _opis; }
	Razred GetRazred() const { return *_razred; }

	friend ostream& operator<<(ostream& COUT, const Aktivnost& obj) {
		COUT << *obj._razred << " " << obj._ocjena << " " << obj._opis << endl;
		return COUT;
	}
};
bool operator ==(const Aktivnost& a1, const Aktivnost& a2)
{
	if (a1.GetRazred() != a2.GetRazred())
		return false;
	if (a1.GetOpis() != a2.GetOpis())
		return false;
	if (a1.GetOcjenu() != a2.GetOcjenu())
		return false;
	return true;
}
bool operator !=(const Aktivnost& a1, const Aktivnost& a2)
{
	return !(a1 == a2);
}
class Polaznik {
protected:
	char* _imePrezime;
	string _brojTelefona;
public:
	Polaznik()
	{
		_imePrezime = nullptr;
	}
	Polaznik(string imePrezime, string brojTelefona)
		: _imePrezime(AlocirajNizKaraktera(imePrezime.c_str()))
	{
		_brojTelefona = ValidirajTelefon(brojTelefona) ? brojTelefona : not_set;
	}
	Polaznik(const Polaznik& obj)
		:_brojTelefona(obj.GetTelefon())
	{
		_imePrezime = AlocirajNizKaraktera(obj.GetImePrezime());
	}
	Polaznik(Polaznik&& obj)noexcept
		:_brojTelefona(move(obj._brojTelefona))
	{
		_imePrezime = obj._imePrezime;
		obj._imePrezime = nullptr;
	}
	Polaznik& operator = (const Polaznik& obj)
	{
		if (this == &obj)
			return *this;
		delete[]_imePrezime;
		_imePrezime = AlocirajNizKaraktera(obj.GetImePrezime());
		_brojTelefona = obj.GetTelefon();
		return *this;
	}
	virtual ~Polaznik()
	{
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}
	char* GetImePrezime() const { return _imePrezime; }
	string GetTelefon() const { return _brojTelefona; }
	virtual void PredstaviSe() = 0;
	friend ostream& operator <<(ostream& COUT, const  Polaznik& obj)
	{
		COUT << "Ime i prezime : " << obj.GetImePrezime() << endl;
		COUT << "Broj telefona: " << obj.GetTelefon() << endl;
		return COUT;
	}
};
bool operator == (const Polaznik& p1, const Polaznik& p2)
{
	if (strcmp(p1.GetImePrezime(), p2.GetImePrezime()) != 0)
		return false;
	if (p1.GetTelefon() != p2.GetTelefon())
		return false;
	return true;
}
bool operator != (const Polaznik& p1, const Polaznik& p2)
{
	return !(p1 == p2);
}
class Ucenik :public Polaznik
{
	Kolekcija<Predmet, Aktivnost, 16>* _aktivnosti;
public:
	virtual void PredstaviSe()override
	{

	}

	Ucenik()
		:Polaznik()
	{
		_aktivnosti = nullptr;
	}
	Ucenik(const Ucenik& obj)
		:Polaznik(obj), _aktivnosti(new Kolekcija<Predmet, Aktivnost, 16>(*obj._aktivnosti))
	{

	}
	Ucenik(const char* imePrezime, string telefon)
		:Polaznik(imePrezime, telefon)
	{
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>();
	}
	Ucenik(Ucenik&& obj)noexcept
		:Polaznik(move(obj)), _aktivnosti(new Kolekcija<Predmet, Aktivnost, 16>(move(*obj._aktivnosti)))
	{

	}
	Ucenik& operator = (const Ucenik& obj)
	{
		if (this == &obj)
			return *this;
		static_cast<Polaznik&>(*this) = obj;
		if (_aktivnosti == nullptr)
			_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>();
		*_aktivnosti = *obj._aktivnosti;
		return *this;
	}
	virtual ~Ucenik() override
	{
		delete _aktivnosti;
		_aktivnosti = nullptr;
	}
	Kolekcija<Predmet, Aktivnost, 16>& GetAktivnosti() const { return *_aktivnosti; }
	friend ostream& operator<<(ostream& COUT, const Ucenik& obj)
	{
		COUT << "Ime prezime: " << obj.GetImePrezime() << endl;
		COUT << "Broj telefona: " << obj.GetTelefon() << endl;
		COUT << *obj._aktivnosti << endl;
		return COUT;
	}
	float prosjekUcenika()const
	{
		float prosjek = 0.0f;
		for (int i = 0; i < _aktivnosti->GetTrenutno(); i++)
		{
			prosjek += _aktivnosti->GetElement2(i).GetOcjenu();
		}
		if (prosjek > 0)
			return prosjek /= _aktivnosti->GetTrenutno();
		return prosjek;
	}
};
bool operator== (const Ucenik& u1, const Ucenik& u2)
{
	if (strcmp(u1.GetImePrezime(), u2.GetImePrezime()) != 0)
		return false;
	if (u1.GetTelefon() != u2.GetTelefon())
		return false;
	if (u1.GetAktivnosti().GetTrenutno() != u2.GetAktivnosti().GetTrenutno())
		return false;
	for (int i = 0; i < u1.GetAktivnosti().GetTrenutno(); i++)
	{
		if (u1.GetAktivnosti().GetElement1(i) != u2.GetAktivnosti().GetElement1(i) || u1.GetAktivnosti().GetElement2(i) != u2.GetAktivnosti().GetElement2(i))
			return false;
	}
	return true;
}
bool operator!= (const Ucenik& u1, const Ucenik& u2)
{
	return !(u1 == u2);
}
mutex m;
class Skola {
	char* _naziv;
	vector<Ucenik> _ucenici;
	void posaljiSMS(Razred razred, float prosjecnaOcjena)
	{
		m.lock();
		/*u slucaju da je ucenik uspjesno (ocjenom vecom od 1) realizovao aktivnosti na nivou odredjenog razreda,
		 *te posjeduje validan broj telefona,
		u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti u okviru X razreda sa
		prosjecnom ocjenom X.X"*/
		cout << "Uspjesno ste okoncali aktivnosti u okviru " << razred << " razreda sa prosjecnom ocjenom " << prosjecnaOcjena << endl;
		m.unlock();
	}
public:
	Skola(const char* naziv = nullptr)
	{
		_naziv = AlocirajNizKaraktera(naziv);
	}
	Skola(const Skola& obj)
		:_ucenici(obj._ucenici)
	{
		_naziv = AlocirajNizKaraktera(obj.GetNaziv());
	}
	Skola(Skola&& obj)noexcept
		:_ucenici(move(obj._ucenici))
	{
		_naziv = obj._naziv;
		obj._naziv = nullptr;
	}
	Skola& operator = (const Skola& obj)
	{
		if (this == &obj)
			return *this;
		delete[]_naziv;
		_naziv = AlocirajNizKaraktera(obj.GetNaziv());
		_ucenici = obj._ucenici;
		return *this;
	}
	~Skola()
	{
		delete[] _naziv; _naziv = nullptr;
	}
	char* GetNaziv()const { return _naziv; }
	vector<Ucenik>& GetUcenici() { return _ucenici; }

	friend ostream& operator<<(ostream& COUT, const Skola& obj) {
		COUT << "Skola: " << obj._naziv << endl;
		COUT << "Ucenici: " << endl;
		for (size_t i = 0; i < obj._ucenici.size(); i++)
			COUT << obj._ucenici[i] << endl;
		return COUT;
	}
	void operator () (const char* imePrezime, string telefon)
	{
		if (_ucenici.size() == 0)
		{
			_ucenici.push_back(Ucenik(imePrezime, telefon));
			return;
		}
		for (int i = 0; i < _ucenici.size(); i++)
		{
			if (strcmp(_ucenici[i].GetImePrezime(), imePrezime) == 0)
				throw exception("Ucenik sa ovim imenom vec postoji !!!");
			else if (_ucenici[i].GetTelefon() == telefon)
				throw exception("Ucenik sa ovim telefonom vec postoji !!!");
		}
		_ucenici.push_back(Ucenik(imePrezime, telefon));
	}
	bool DodajAktivnost(const char* imePrezime, Predmet predmet, Aktivnost aktivnost)
	{
		/*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti,
		*a takodjer, na nivou razreda se ne smiju ponavljati aktivnosti iz istog predmeta*/
		auto checkForIste = [this, &imePrezime, &predmet, &aktivnost]()
		{
			for (auto& ucenik : _ucenici)
			{
				for (int i = 0; i < ucenik.GetAktivnosti().GetTrenutno(); i++)
				{
					if (ucenik.GetAktivnosti().GetElement2(i).GetRazred() == aktivnost.GetRazred()) {
						if (ucenik.GetAktivnosti().GetTrenutno() > 4)
							return false;//Mogu se dodati samo max 4;
						if (ucenik.GetAktivnosti().GetElement1(i) == predmet)
							if (strcmp(ucenik.GetImePrezime(), imePrezime) == 0)
								return false;//Ponavlja se isti predmet;
					}
				}
			}
			return true;
		};
		if (checkForIste())
		{
			for (int i = 0; i < _ucenici.size(); i++)
			{
				_ucenici[i].GetAktivnosti().addElementwoSort(predmet, aktivnost);
				if (ValidirajTelefon(_ucenici[i].GetTelefon()))
				{
					for (int j = 0; j < _ucenici[i].GetAktivnosti().GetTrenutno(); j++)
					{
						if (_ucenici[i].GetAktivnosti().GetElement2(j).GetOcjenu() > 1)
						{
							thread t(&Skola::posaljiSMS, this, _ucenici[i].GetAktivnosti().GetElement2(j).GetRazred(), _ucenici[i].prosjekUcenika());
							t.join();
						}
					}
				}
				return true;
			}
		}
		return false;
	}
	pair <Polaznik*, float> GetNajboljegUcenika()
	{
		Polaznik* najbolji = nullptr;
		float najboljiProsjek = 0.0f;
		pair <Polaznik*, float> povratni;
		for (int i = 0; i < _ucenici.size(); i++)
		{
			if (_ucenici[i].prosjekUcenika() > najboljiProsjek)
			{
				najboljiProsjek = _ucenici[i].prosjekUcenika();
				najbolji = &_ucenici[i];
			}
		}
		povratni.first = najbolji;
		povratni.second = najboljiProsjek;
		return povratni;
	}
	bool SpasiUFajl(const char* file, bool pobrisi = false)const
	{
		ofstream upis;
		upis.open(file);
		if (pobrisi == true)
		{
			upis.open(file, ios::trunc);
		}

		if (upis.is_open())
		{
			upis << *this;
		}
		upis.close();
		ifstream ispis;
		ispis.open(file);
		if (ispis.is_open())
		{
			cout << ispis.rdbuf();
			return true;
		}
		return false;
	}
};

int main() {

	Kolekcija<int, int, 10> kolekcija1(DESC, false);
	try {
		kolekcija1.AddElement(1, 2);
		//dupliranje elemenata nije dozvoljeno
		kolekcija1.AddElement(1, 2);
	}
	catch (exception& ex) {
		cout << ex.what();
	}

	/*nakon svakog dodavanja, elemente sortirati prema T1 i vrijednosti atributa _sortiranje*/
	for (size_t i = 1; i < kolekcija1.GetMax() - 1; i++)
		kolekcija1.AddElement(rand(), rand());

	cout << kolekcija1 << endl;

	try {
		//prekoracen maksimalan broj elemenata
		kolekcija1.AddElement(rand(), rand());
	}
	catch (exception& ex) {
		cout << ex.what();
	}
	cout << kolekcija1 << endl;

	Kolekcija<int, int, 10> kolekcija2(kolekcija1);
	cout << kolekcija2 << endl;
	Kolekcija<int, int, 10> kolekcija3(move(kolekcija1));

	Skola gimnazijaMostar("GIMNAZIJA MOSTAR");
	//dodaje novog ucenika u skolu
	gimnazijaMostar("Jasmin Azemovic", "+387(61)111-222");
	gimnazijaMostar("Adel Handzic", "+387(61)333-444");

	/*
	koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
	onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set
	*/
	gimnazijaMostar("Telefon NotValidFormat", "387 61)333-444");

	try
	{
		/*onemoguciti dodavanje ucenika sa istim imenom i prezimenom ili brojem telefona*/
		gimnazijaMostar("Adel Handzic", "+387(61)333-444");
	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
	}

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Priprema za takmicenje iz Matematije koje se odrzava u Konjicu 07.02.2019")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti,
	 *a takodjer, na nivou razreda se ne smiju ponavljati aktivnosti iz istog predmeta*/
	if (!gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Aktivnosti iz matematike")))
		cout << "Aktivnost nije uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", HEMIJA, Aktivnost(I, 5, "Priprema otopina za vjezbe iz predmeta Hemija")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", FIZIKA, Aktivnost(I, 4, "Analiza stepena apsorpcije materijala ")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*u slucaju da je ucenik uspjesno (ocjenom vecom od 1) realizovao aktivnosti na nivou odredjenog razreda,
	 *te posjeduje validan broj telefona,
	u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti u okviru X razreda sa
	prosjecnom ocjenom X.X"*/
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", GEOGRAFIJA, Aktivnost(I, 4, "Izrada 5 reljefa Mostara")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Adel Handzic", MATEMATIKA, Aktivnost(I, 5, "Izrada skripte na temu integralni racun")))
		cout << "Aktivnost uspjesno dodana" << endl;

	//ispisuje sve ucenike i njihove aktivnosti
	cout << gimnazijaMostar << endl;


	pair<Polaznik*, float> par = gimnazijaMostar.GetNajboljegUcenika();
	cout << "Najbolji ucenik je " << par.first->GetImePrezime() << " sa prosjekom " << par.second << endl;

	/*U fajl (npr. Gimnazija.txt) upisati podatke (podatke upisati kao obicni tekst) o skoli i svim ucenicima.
	Nakon upisa, potrebno je ispisati sadrzaj fajla. Parametar tipa bool oznacava da li ce ranije dodani sadrzaj fajla
	prethodno biti pobrisan*/

	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt"))
		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;
	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt", true))
		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;

	cin.get();
	system("pause>0");
	return 0;
}