#include<iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <sstream>
using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

const char* crt = "\n-------------------------------------------\n";
const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";
const int min_polgavlja = 3;
const int min_karaktera_po_poglavlju = 30;

char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	void prosiriNizove()
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
		delete[]temp1;
		delete[]temp2;
		temp1 = nullptr;
		temp2 = nullptr;
	}
public:
	Kolekcija()
	{
		_trenutno = 0;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	Kolekcija(const Kolekcija& obj)
	{
		_trenutno = obj.getTrenutno();
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj.getElement1(i);
			_elementi2[i] = obj.getElement2(i);
		}
	}
	Kolekcija(Kolekcija&& obj)noexcept
	{
		_trenutno = obj._trenutno;
		obj._trenutno = 0;
		_elementi1 = obj._elementi1;
		_elementi2 = obj._elementi2;
		obj._elementi1 = nullptr;
		obj._elementi2 = nullptr;
	}
	Kolekcija& operator = (const Kolekcija& obj)
	{
		if (this == &obj)
			return *this;
		delete[]_elementi1;
		delete[]_elementi2;
		_trenutno = obj.getTrenutno();
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj.getElement1(i);
			_elementi2[i] = obj.getElement2(i);
		}
		return *this;
	}
	~Kolekcija()
	{
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi1 = nullptr;
	}
	T1* getElementi1Pok() const { return _elementi1; }
	T2* getElementi2Pok()const { return _elementi2; }
	T1& getElement1(int lokacija) const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) const { return _elementi2[lokacija]; }
	int getTrenutno() const { return _trenutno; }

	friend ostream& operator<<(ostream& COUT, const Kolekcija<T1, T2>& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	void addElement(T1 prvi, T2 drugi)
	{
		prosiriNizove();
		_elementi1[_trenutno] = prvi;
		_elementi2[_trenutno] = drugi;
		_trenutno++;
	}
};
class Poglavlje {
	char* _naslov;
	char* _sadrzaj;
	bool _prihvaceno;
	int _ocjena;//da bi se poglavlje smatralo prihvacenim ocjena mora biti u opsegu od 6 - 10
public:
	Poglavlje(const char* naslov = nullptr, const char* sadrzaj = nullptr)
	{
		_naslov = AlocirajNizKaraktera(naslov);
		_sadrzaj = AlocirajNizKaraktera(sadrzaj);
		_prihvaceno = false;
		_ocjena = 0;
	}
	Poglavlje(const Poglavlje& obj)
	{
		_naslov = AlocirajNizKaraktera(obj.GetNaslov());
		_sadrzaj = AlocirajNizKaraktera(obj.GetSadrzaj());
		_prihvaceno = obj.GetPrihvaceno();
		_ocjena = obj.GetOcjena();
	}
	Poglavlje(Poglavlje&& obj)noexcept
	{
		_naslov = obj._naslov;
		obj._naslov = nullptr;
		_sadrzaj = obj._sadrzaj;
		obj._sadrzaj = nullptr;
		_prihvaceno = obj._prihvaceno;
		obj._prihvaceno = false;
		_ocjena = obj._ocjena;
		obj._ocjena = 0;
	}
	Poglavlje& operator = (const Poglavlje& obj)
	{
		if (this == &obj)
			return *this;
		delete[]_naslov;
		delete[]_sadrzaj;
		_naslov = AlocirajNizKaraktera(obj.GetNaslov());
		_sadrzaj = AlocirajNizKaraktera(obj.GetSadrzaj());
		_prihvaceno = obj.GetPrihvaceno();
		_ocjena = obj.GetOcjena();
		return *this;
	}
	~Poglavlje()
	{
		delete[] _naslov; _naslov = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	friend ostream& operator<<(ostream& COUT, const  Poglavlje& obj) {
		if (obj._naslov == nullptr || obj._sadrzaj == nullptr)
			return COUT;
		COUT << endl << obj._naslov << endl << obj._sadrzaj << endl;
		if (obj._prihvaceno)
			COUT << "Ocjena: " << obj._ocjena << endl;;
		return COUT;
	}
	char* GetNaslov() const { return _naslov; }
	char* GetSadrzaj() const { return _sadrzaj; }
	bool GetPrihvaceno() const { return _prihvaceno; }
	int GetOcjena() const { return _ocjena; }
	void setSadrzaj(const char* sadrzaj)
	{
		_sadrzaj = AlocirajNizKaraktera(string(string(_sadrzaj) + string("  ") + string(sadrzaj)).c_str());
	}
	void setNaslov(const char* naslov)
	{
		_naslov = AlocirajNizKaraktera(naslov);
	}
	void setOcjena(int ocj)
	{
		_ocjena = ocj;
	}
	void setPoglavlje(bool set)
	{
		_prihvaceno = set;
	}
};
bool operator == (const Poglavlje& p1, const Poglavlje& p2)
{
	if (strcmp(p1.GetNaslov(), p2.GetNaslov()) != 0)
		return false;
	if (strcmp(p1.GetSadrzaj(), p2.GetSadrzaj()) != 0)
		return false;
	if (p1.GetPrihvaceno() != p2.GetPrihvaceno())
		return false;
	if (p1.GetOcjena() != p2.GetOcjena())
		return false;
	return true;
}
bool operator != (const Poglavlje& p1, const Poglavlje& p2)
{
	return !(p1 == p2);
}
class ZavrsniRad {
	char* _tema;
	vector<Poglavlje> _poglavljaRada;
	string _datumOdbrane;
	float _konacnaOcjena; //prosjek ocjena svih poglavlja u zavrsnom radu koja se izracunava u momentu zakazivanja odbrane
public:
	ZavrsniRad(const char* nazivTeme = nullptr)
	{
		_tema = AlocirajNizKaraktera(nazivTeme);
		_konacnaOcjena = 0.0f;
		_datumOdbrane = not_set;
	}

	ZavrsniRad(const ZavrsniRad& obj)
		: _poglavljaRada(obj._poglavljaRada), _datumOdbrane(obj._datumOdbrane)
	{
		_tema = AlocirajNizKaraktera(obj.GetNazivTeme());
		_konacnaOcjena = obj.GetOcjena();
	}
	ZavrsniRad(ZavrsniRad&& obj)noexcept
		:_poglavljaRada(move(obj._poglavljaRada)), _datumOdbrane(move(obj._datumOdbrane))
	{
		_tema = obj._tema;
		obj._tema = nullptr;
		_konacnaOcjena = obj._konacnaOcjena;
		obj._konacnaOcjena = 0.0f;
	}
	ZavrsniRad& operator = (const ZavrsniRad& obj)
	{
		if (this == &obj)
			return *this;
		delete[]_tema;
		_tema = AlocirajNizKaraktera(obj.GetNazivTeme());
		_poglavljaRada = obj._poglavljaRada;
		_konacnaOcjena = obj.GetOcjena();
		_datumOdbrane = obj.GetDatumOdbrane();
		return *this;
	}
	~ZavrsniRad()
	{
		delete[] _tema; _tema = nullptr;
	}
	char* GetNazivTeme()const { return _tema; }
	vector<Poglavlje>& GetPoglavlja() { return _poglavljaRada; };
	string GetDatumOdbrane()const { return _datumOdbrane; }
	float GetOcjena()const { return _konacnaOcjena; }
	void SetDatumOdbrane(string datumOdbrane) { _datumOdbrane = datumOdbrane; }
	void setZavrsnaOcjena(float ocj)
	{
		_konacnaOcjena = ocj;
	}
	friend ostream& operator<<(ostream& COUT, const ZavrsniRad& obj) {
		COUT << "Tema rada: " << obj._tema << endl;
		COUT << "Sadrzaj: " << endl;
		for (size_t i = 0; i < obj._poglavljaRada.size(); i++)
			COUT << obj._poglavljaRada[i] << endl;
		COUT << "Datum odbrane rada: " << obj._datumOdbrane << endl << " Ocjena: " << obj._konacnaOcjena << endl;
		return COUT;
	}
	void DodajPoglavlje(const char* naziv, const char* sadrzaj)
	{
		/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu,
	 *funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci
	 *(izmedju postojeceg i novog sadrzaja se dodaje prazan prostor). u slucaju da poglavlje ne postoji,
	 *ono se dodaje zajedno sa sadrzaje*/
		if (_poglavljaRada.size() == 0)
		{
			_poglavljaRada.push_back(Poglavlje(naziv, sadrzaj));
			return;
		}

		for (int i = 0; i < _poglavljaRada.size(); i++)
		{
			if (strcmp(_poglavljaRada[i].GetNaslov(), naziv) == 0)
			{
				_poglavljaRada[i].setSadrzaj(sadrzaj);
			}
			else
			{
				_poglavljaRada.push_back(Poglavlje(naziv, sadrzaj));
				return;
			}
		}
	}
	void OcijeniPoglavlje(const char* naziv, int ocjena)
	{
		/*funkcija OcijeniPoglavlje, na osnovu naziva poglavlja, dodjeljuje ocjenu poglavlju te ukoliko
		 *je ocjena pozitivna (6 - 10)*onda poglavlje oznacava prihvacenim.
		 *U slucaju da ocjena nije validna ili poglavlje ne postoji, funkcija baca izuzetak sa odgovarajucom porukom*/
		 //parametri:nazivPoglavlja, ocjena
		bool flag = false;
		for (int i = 0; i < _poglavljaRada.size(); i++)
		{
			if (strcmp(_poglavljaRada[i].GetNaslov(), naziv) == 0)
			{
				_poglavljaRada[i].setOcjena(ocjena);
				if (ocjena > 6 && ocjena < 10)
				{
					_poglavljaRada[i].setPoglavlje(true);
					return;
				}
				else
					throw exception("Ocijena nije pozitivna !!!");
			}
			else
				flag = true;
		}
		if (flag)
			throw exception("Poglavlje je nepostojece !!!");

	}
};
bool operator ==(const ZavrsniRad& z1, const ZavrsniRad& z2)
{
	if (strcmp(z1.GetNazivTeme(), z2.GetNazivTeme()) != 0)
		return false;
	if (z1.GetDatumOdbrane() != z2.GetDatumOdbrane())
		return false;
	if (z1.GetOcjena() != z2.GetOcjena())
		return false;
	if (const_cast<ZavrsniRad&>(z1).GetPoglavlja().size() != const_cast<ZavrsniRad&>(z2).GetPoglavlja().size())
		return false;
	for (int i = 0; i < const_cast<ZavrsniRad&>(z1).GetPoglavlja().size(); i++)
	{
		if (const_cast<ZavrsniRad&>(z1).GetPoglavlja()[i] != const_cast<ZavrsniRad&>(z2).GetPoglavlja()[i])
			return false;
	}
	return true;
}
bool operator !=(const ZavrsniRad& z1, const ZavrsniRad& z2)
{
	return !(z1 == z2);
}
class Osoba {
protected:
	string _imePrezime;
public:
	Osoba()
	{
		_imePrezime = not_set;
	}
	Osoba(string imePrezime)
		: _imePrezime(imePrezime)
	{

	}
	Osoba(const Osoba& obj)
		:_imePrezime(obj.GetImePrezime())
	{

	}
	Osoba(Osoba&& obj)noexcept
		:_imePrezime(move(obj._imePrezime))
	{

	}
	Osoba& operator = (const Osoba& obj)
	{
		if (this == &obj)
			return *this;
		_imePrezime = obj.GetImePrezime();
		return *this;
	}
	virtual ~Osoba()
	{

	}
	string GetImePrezime()const { return _imePrezime; }
	virtual void Info() = 0;
	friend ostream& operator <<(ostream& COUT, const Osoba& obj)
	{
		COUT << "Ime i prezime: " << obj.GetImePrezime() << endl;
		return COUT;
	}
};
bool operator ==(const Osoba& o1, const Osoba& o2)
{
	return o1.GetImePrezime() != o2.GetImePrezime();
}
bool operator !=(const Osoba& o1, const Osoba& o2)
{
	return !(o1 == o2);
}
mutex m;
class Nastavnik :public Osoba
{
	//Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
	Kolekcija<string, ZavrsniRad> _teme;

public:
	virtual void Info()override
	{
		cout << this->GetImePrezime() << endl;
		for (int i = 0; i < _teme.getTrenutno(); i++)
		{
			cout << "IB: " << _teme.getElement1(i) << " " << _teme.getElement2(i) << endl;
		}
		cout << endl;
	}
	Nastavnik()
		:Osoba()
	{

	}
	Nastavnik(const char* imeprezime)
		:Osoba(imeprezime)
	{

	}
	Nastavnik(const Nastavnik& obj)
		:Osoba(obj), _teme(obj._teme)
	{

	}
	Nastavnik(Nastavnik&& obj)noexcept
		:Osoba(move(obj)), _teme(move(obj._teme))
	{

	}
	virtual ~Nastavnik()override
	{

	}
	Nastavnik& operator =(const Nastavnik& obj)
	{
		if (this == &obj)
			return *this;
		static_cast<Osoba&>(*this) = obj;
		_teme = obj._teme;
		return *this;
	}
	Kolekcija<string, ZavrsniRad>& GetTeme() { return _teme; }
	friend ostream& operator <<(ostream& COUT, const Nastavnik& obj)
	{
		COUT << obj.GetImePrezime() << endl;
		COUT << "Teme: " << endl;
		COUT << obj._teme << endl;
		return COUT;
	}
	bool DodajZavrsniRad(string IB, ZavrsniRad rad)
	{
		if (_teme.getTrenutno() == 0)
		{
			_teme.addElement(IB, rad);
			return true;
		}
		auto checkIsteRadove = [this, &rad, &IB]()
		{
			for (int i = 0; i < _teme.getTrenutno(); i++)
			{
				if (strcmp(_teme.getElement2(i).GetNazivTeme(), rad.GetNazivTeme()) == 0)
					return false;//Nema dodavanja istoimenih tema;
				if (_teme.getElement1(i) == IB)
					return false;
			}
			return true;
		};
		if (checkIsteRadove())
		{
			_teme.addElement(IB, rad);
			return true;
		}
		return false;
	}
	ZavrsniRad* ZakaziOdbranuRada(string  IB, string datumOdbrane)
	{
		/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada
		*sto podrazumijeva definisanje datuma odbrane. odbrana rada se moze zakazati samo studentu koji je
		*rad prethodno prijavio i pri tom su zadovoljeni sljedeci uslovi:
		1. zavrsni rad ima broj poglavlja veci od minimalnog
		2. svako poglavlje ima broj karaktera veci od minimalnog
		3. svako poglavlje je prihvaceno/odobreno
		ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja),
		postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazan odbrana.
		u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije
		zadovoljen, funkcija vraca nullptr.
		*/
		float prosjek = 0.0f;
		ZavrsniRad* povratni = nullptr;
		for (int i = 0; i < _teme.getTrenutno(); i++)
		{
			if (_teme.getElement1(i) == IB)//Ovom studentu,
			{
				if (_teme.getElement2(i).GetPoglavlja().size() > min_polgavlja)//Uslov 1 ispunjen,
				{
					for (int j = 0; j < _teme.getElement2(i).GetPoglavlja().size(); j++)
					{
						if (strlen(_teme.getElement2(i).GetPoglavlja()[j].GetSadrzaj()) > min_karaktera_po_poglavlju)//Uslov 2 ispunjen
						{
							if (_teme.getElement2(i).GetPoglavlja()[j].GetPrihvaceno() == true)//Uslov 3 je ispunjen
							{
								prosjek += _teme.getElement2(i).GetPoglavlja()[j].GetOcjena();
								_teme.getElement2(i).SetDatumOdbrane(datumOdbrane);
								povratni = &_teme.getElement2(i);
							}
						}
					}
					prosjek /= _teme.getElement2(i).GetPoglavlja().size();
					_teme.getElement2(i).setZavrsnaOcjena(prosjek);
				}
			}
		}
		return povratni;
	}
	string posaljiEmail(string IB, float ocjena)
	{
		m.lock();
		string mail;
		mail = "Postovani " + IB + " , uzimajuci u obzir cinjenicu da ste kod mentora" + this->GetImePrezime() + " uspjesno odbranili rad sa ocjenom " + to_string(ocjena) + "  cast nam je pozvati vas na dodjelu nagrada za najbolje studente koja ce se odrzati  na	FIT - u 03.07.2019.godine.";
		m.unlock();
		return mail;
	}
};
string PosaljiPozivZaDodjeluNagrada(Nastavnik** nastavnici, int max, float granicaProlaza)
{
	/*Funkcija PosaljiPozivZaDodjeluNagrada ima zadatak da svim studentima koji su uspjesno okoncali svoj zavrsni rad
	kod odredjenog nastavnika/mentora
	i tom prilikom ostvarili ocjenu vecu od proslijedjene, u zasebnom thread.u, posalje email poruku
	(mail adresa: brojIndeksa@edu.fit.ba) sa sadrzajem:
	"Postovani {brojIndeksa}, uzimajuci u obzir cinjenicu da ste kod mentora {imePrezimeMentora} uspjesno odbranili rad sa
	ocjenom {ocjena} cast nam je pozvati vas na dodjelu nagrada za najbolje studente koja ce se odrzatu u na
	FIT-u 03.07.2019. godine."	.
	funkcija treba da vrati sadrzaj svih poslatih email poruka, a ukoliko niti jedan od nastavnika ne posjeduje
	evidentirano mentorstvo na zavrsnom radu, funkcija vraca not_set*/
	string poruka = not_set;
	for (int i = 0; i < max; i++)
	{
		for (int j = 0; j < nastavnici[i]->GetTeme().getTrenutno(); j++)
		{
			if (nastavnici[i]->GetTeme().getElement2(j).GetOcjena() > granicaProlaza)
			{
				stringstream upis;
				auto poziv = [&i, &j, &nastavnici, &upis]()
				{
					upis << nastavnici[i]->posaljiEmail(nastavnici[i]->GetTeme().getElement1(j), nastavnici[i]->GetTeme().getElement2(j).GetOcjena());
					return upis.str();
				};
				thread t(poziv);
				poruka = poziv();
			}
		}
	}
	return poruka;
}
int main() {
	cout << crt << "UPLOAD RADA OBAVEZNO IZVRSITI U ODGOVARAJUCI FOLDER NA FTP SERVERU" << endl;
	cout << "U slucaju da je Upload folder prazan pritisnite tipku F5" << crt;

	//Kolekcija<int, int> brojevi;
	//for (int i = 0; i < 10; i++)
	//{
	//	brojevi.addElement(i, i);
	//}
	//cout << brojevi<<crt;
	//Kolekcija<int, int> brojevi2(brojevi);
	//cout << brojevi2 << crt;
	//Kolekcija<int, int> brojevi3;
	//brojevi3 = brojevi2;
	//cout << brojevi3 << crt;
	//Kolekcija<int, int> brojevi4(move(brojevi));
	//cout << brojevi4 << crt;

	const int max = 4;
	Nastavnik* nastavnici[max];

	nastavnici[0] = new Nastavnik("Denis Music");
	nastavnici[1] = new Nastavnik("Zanin Vejzovic");
	nastavnici[2] = new Nastavnik("Jasmin Azemovic");
	nastavnici[3] = new Nastavnik("Emina Junuz");
	//parametri: naziv zavrsnog rada
	ZavrsniRad multimedijalni("Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
	ZavrsniRad podrsa_operaterima("Sistem za podrsku rada kablovskog operatera");
	ZavrsniRad analiza_sigurnosti("Prakticna analiza sigurnosti bezicnih racunarskih mreza");
	ZavrsniRad kriptografija("Primjena teorije informacija u procesu generisanja kriptografskih kljuceva");

	/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu,
	 *funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci
	 *(izmedju postojeceg i novog sadrzaja se dodaje prazan prostor). u slucaju da poglavlje ne postoji,
	 *ono se dodaje zajedno sa sadrzaje*/
	 //parametri: nazivPoglavlja, sadrzajPoglavlja
	multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
	multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
	multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
	multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
	multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");
	multimedijalni.DodajPoglavlje("Zakljucak", "U ovom radu su predstavljeni osnovni koncepti i prakticna primjena...");

	try {
		/*funkcija OcijeniPoglavlje, na osnovu naziva poglavlja, dodjeljuje ocjenu poglavlju te ukoliko
		 *je ocjena pozitivna (6 - 10)*onda poglavlje oznacava prihvacenim.
		 *U slucaju da ocjena nije validna ili poglavlje ne postoji, funkcija baca izuzetak sa odgovarajucom porukom*/
		 //parametri:nazivPoglavlja, ocjena

		multimedijalni.OcijeniPoglavlje("Uvod", 8);
		multimedijalni.OcijeniPoglavlje("Vrste multimedijalnih sistema", 8);
		multimedijalni.OcijeniPoglavlje("Teorija multimedije", 9);
		multimedijalni.OcijeniPoglavlje("Zakljucak", 7);
		multimedijalni.OcijeniPoglavlje("Naziv poglavlja ne postoji", 8);
	}
	catch (exception& err) {
		cout << "Greska -> " << err.what() << endl;
	}

	/*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu.
	 *zavrsni rad se dodaje studentu sa brojem indeksa proslijedjenim kao prvi parametar.
	sprijeciti dodavanje zavrsnih radova sa istom temom*/
	//parametri: brojIndeksa, zavrsniRad
	if (nastavnici[0]->DodajZavrsniRad("IB130011", multimedijalni))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[0]->DodajZavrsniRad("IB120051", podrsa_operaterima))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad("IB120056", podrsa_operaterima))//dupliranje rada
		cout << "Zavrsni rad nije dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad("IB120051", kriptografija)) //studentu vec dodijeljen rad
		cout << "Zavrsni rad nije dodat!" << endl;
	if (nastavnici[1]->DodajZavrsniRad("IB140102", analiza_sigurnosti))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[2]->DodajZavrsniRad("IB140002", kriptografija))
		cout << "Zavrsni rad uspjesno dodat!" << endl;

	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada
	 *sto podrazumijeva definisanje datuma odbrane. odbrana rada se moze zakazati samo studentu koji je
	 *rad prethodno prijavio i pri tom su zadovoljeni sljedeci uslovi:
		1. zavrsni rad ima broj poglavlja veci od minimalnog
		2. svako poglavlje ima broj karaktera veci od minimalnog
		3. svako poglavlje je prihvaceno/odobreno
	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja),
	postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazan odbrana.
	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije
	zadovoljen, funkcija vraca nullptr.
   */

   //parametri: brojIndeksa, datumOdbrane
	ZavrsniRad* zr1 = nastavnici[0]->ZakaziOdbranuRada("IB130011", "25.09.2018");
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	zr1 = nastavnici[0]->ZakaziOdbranuRada("IB130111", "25.09.2018");//student sa brojem indeksa IB130111 jos uvijek nije prijavio rad
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	//ispisuje sve podatke o nastavniku i njegovim mentorstvima
	nastavnici[0]->Info();

	/*Funkcija PosaljiPozivZaDodjeluNagrada ima zadatak da svim studentima koji su uspjesno okoncali svoj zavrsni rad
	kod odredjenog nastavnika/mentora
	i tom prilikom ostvarili ocjenu vecu od proslijedjene, u zasebnom thread.u, posalje email poruku
	(mail adresa: brojIndeksa@edu.fit.ba) sa sadrzajem:
	"Postovani {brojIndeksa}, uzimajuci u obzir cinjenicu da ste kod mentora {imePrezimeMentora} uspjesno odbranili rad sa
	ocjenom {ocjena} cast nam je pozvati vas na dodjelu nagrada za najbolje studente koja ce se odrzatu u na
	FIT-u 03.07.2019. godine."	.
	funkcija treba da vrati sadrzaj svih poslatih email poruka, a ukoliko niti jedan od nastavnika ne posjeduje
	evidentirano mentorstvo na zavrsnom radu, funkcija vraca not_set*/
	cout << "Studenti za dodjelu nagrada: " << PosaljiPozivZaDodjeluNagrada(nastavnici, max, 9.5) << endl;

	for (int i = 0; i < max; i++) {
		delete nastavnici[i];
		nastavnici[i] = nullptr;
	}
	system("pause>0");
	return 0;
}