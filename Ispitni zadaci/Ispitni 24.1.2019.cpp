#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <regex>

using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum GodinaStudija { PRVA = 1, DRUGA, TRECA };

char* Alociraj(const char* sadrzaj)
{
    if (sadrzaj == nullptr)
        return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    return temp;
}

template<class T1, class T2>
class Dictionary {
    T1* _elementi1;
    T2* _elementi2;
    int* _trenutno;
public:
    Dictionary() {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _trenutno = new int(0);
    }
	Dictionary(const Dictionary& obj)
    {
        _trenutno = new int(obj.getTrenutno());
        _elementi1 = new T1[*_trenutno];
        _elementi2 = new T2[*_trenutno];

    	for (int i = 0; i < *_trenutno; i++)
    	{
            _elementi1[i] = obj.getElement1(i);
            _elementi2[i] = obj.getElement2(i);
    	}
    }
	Dictionary(Dictionary&& obj) noexcept
    {
        _trenutno = obj._trenutno;
        obj._trenutno = nullptr;
        _elementi1 = obj._elementi1;
        obj._elementi1 = nullptr;
        _elementi2 = obj._elementi2;
        obj._elementi2 = nullptr;
    }
    ~Dictionary() {
        delete[]_elementi1;
    	_elementi1 = nullptr;
        delete[]_elementi2;
    	_elementi2 = nullptr;
        delete _trenutno;
    	_trenutno = nullptr;
    }
	
    T1& getElement1(int lokacija) const
    {
        assert(_elementi1 != nullptr && "Dictionary::getElement1 - _element1 was nullptr");
	    return _elementi1[lokacija];
    }
    T2& getElement2(int lokacija) const
    {
        assert(_elementi2 != nullptr && "Dictionary::getElement2 - _element2 was nullptr");
	    return _elementi2[lokacija];
    }
    int getTrenutno() const
    {
        assert(_trenutno != nullptr && "Dictionary::getTrenutno - _trenutno was nullptr");
	    return *_trenutno;
    }

    void AddElement(const T1 obj1,const T2 obj2)
    {
        if (_trenutno == nullptr)
            _trenutno = new int(0);
    	
        T1* temp1 = _elementi1;
        T2* temp2 = _elementi2;
        _elementi1 = new T1[*_trenutno + 1];
        _elementi2 = new T2[*_trenutno + 1];

    	for (int i = 0; i < *_trenutno; i++)
    	{
            _elementi1[i] = temp1[i];
            _elementi2[i] = temp2[i];
    	}

        delete[] temp1;
        delete[] temp2;

        _elementi1[*_trenutno] = obj1;
        _elementi2[*_trenutno] = obj2;
        *_trenutno += 1;
    }
	Dictionary getRange(int start, int end)
    {
        if (end > getTrenutno() || start < 0 || start > end)
            throw exception("Granice su van opsega");

        Dictionary temp;

    	for (int i = start; i <= end; i++)
    	{
            temp.AddElement(getElement1(i), getElement2(i));
    	}

        return temp;
    }
	
	Dictionary& operator= (const Dictionary& obj)
    {
	    if (this != &obj)
	    {
            delete _trenutno;
            _trenutno = new int(obj.getTrenutno());

            delete[] _elementi1;
            delete[] _elementi2;

            _elementi1 = new T1[obj.getTrenutno()];
            _elementi2 = new T2[obj.getTrenutno()];

	    	for (int i = 0; i < *_trenutno; i++)
	    	{
                _elementi1[i] = obj.getElement1(i);
                _elementi2[i] = obj.getElement2(i);
	    	}
	    }

        return *this;
    }
    friend ostream& operator<< (ostream& COUT, const Dictionary& obj)
	{
        for (size_t i = 0; i < obj.getTrenutno(); i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
};
class DatumVrijeme {
    int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
    DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0)
	{
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
        _sati = new int(sati);
        _minuti = new int(minuti);
    }
	DatumVrijeme(const DatumVrijeme& obj)
    {
        _dan = new int(obj.getDan());
        _mjesec = new int(obj.getMjesec());
        _godina = new int(obj.getGodina());
        _sati = new int(obj.getSati());
        _minuti = new int(obj.getMinuti());
    }
	DatumVrijeme(DatumVrijeme&& obj) noexcept
    {
        _dan = obj._dan;
        obj._dan = nullptr;
        _mjesec = obj._mjesec;
        obj._mjesec = nullptr;
        _godina = obj._godina;
        obj._godina = nullptr;
        _sati = obj._sati;
        obj._sati = nullptr;
        _minuti = obj._minuti;
        obj._minuti = nullptr;
    }
    ~DatumVrijeme() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
        delete _sati; _sati = nullptr;
        delete _minuti; _minuti = nullptr;
    }

    int getDan() const
    {
        assert(_dan != nullptr && "DatumVrijeme::getDan - _dan was nullptr");
        return *_dan;
    }
    int getMjesec() const
    {
        assert(_mjesec != nullptr && "DatumVrijeme::getMjesec - _mjesec was nullptr");
        return *_mjesec;
    }
    int getGodina() const
    {
        assert(_godina != nullptr && "DatumVrijeme::getGodina - _godina was nullptr");
        return *_godina;
    }
    int getSati() const
    {
        assert(_sati != nullptr && "DatumVrijeme::getSati - _sati was nullptr");
        return *_sati;
    }
    int getMinuti() const
    {
        assert(_minuti != nullptr && "DatumVrijeme::getMinuti - _minuti was nullptr");
        return *_minuti;
    }

	char* ToCharArray() const
    {
        //treba ispisati: 19/06/2019 10:15
    	
        stringstream temp;
    	
        if (this->getDan() < 10)
            temp << 0 << this->getDan() << "/";
        else
            temp << this->getDan() << "/";
        if (this->getMjesec() < 10)
            temp << 0 << this->getMjesec();
        else
            temp << this->getMjesec();

        temp << ' ' << this->getSati() << ":" << this->getMinuti();

        static char t_temp[20] = "";
        strcpy_s(t_temp, 20, temp.str().c_str());

        return t_temp;
    }

	DatumVrijeme& operator= (const DatumVrijeme& obj)
    {
	    if (this != &obj)
	    {
            delete _dan;
            _dan = new int(obj.getDan());
            delete _mjesec;
            _mjesec = new int(obj.getMjesec());
            delete _godina;
            _godina = new int(obj.getGodina());
            delete _sati;
            _sati = new int(obj.getSati());
            delete _minuti;
            _minuti = new int(obj.getMinuti());
	    }

        return *this;
    }
    friend ostream& operator<< (ostream& COUT, const DatumVrijeme& obj)
	{
        COUT << obj.getDan() << "." << obj.getMjesec() << "." << obj.getGodina() << " " << obj.getSati() << ":" << obj.getMinuti() << endl;
        return COUT;
    }
	friend int operator- (const DatumVrijeme& obj1, const DatumVrijeme& obj2)
    {
        int vrijeme1 = obj1.getGodina() * 365 + obj1.getMjesec() * 30 + obj1.getDan();
        int vrijeme2 = obj2.getGodina() * 365 + obj2.getMjesec() * 30 + obj2.getDan();

        vrijeme1 *= 1440;
        vrijeme2 *= 1440;

        vrijeme1 += obj1.getSati() * 60 + obj1.getMinuti();
        vrijeme2 += obj2.getSati() * 60 + obj2.getMinuti();

        return abs(vrijeme1 - vrijeme2);
    }
};

bool operator== (const DatumVrijeme& obj1, const DatumVrijeme& obj2)
{
    int vrijeme1 = obj1.getGodina() * 365 + obj1.getMjesec() * 30 + obj1.getDan();
    int vrijeme2 = obj2.getGodina() * 365 + obj2.getMjesec() * 30 + obj2.getDan();

    vrijeme1 *= 1440;
    vrijeme2 *= 1440;

    vrijeme1 += obj1.getSati() * 60 + obj1.getMinuti();
    vrijeme2 += obj2.getSati() * 60 + obj2.getMinuti();

    return vrijeme1 == vrijeme2;
}

bool operator!= (const DatumVrijeme& obj1, const DatumVrijeme& obj2)
{
    return !(obj1 == obj2);
}

bool operator> (const DatumVrijeme& obj1, const DatumVrijeme& obj2)
{
    int vrijeme1 = obj1.getGodina() * 365 + obj1.getMjesec() * 30 + obj1.getDan();
    int vrijeme2 = obj2.getGodina() * 365 + obj2.getMjesec() * 30 + obj2.getDan();

    vrijeme1 *= 1440;
    vrijeme2 *= 1440;

    vrijeme1 += obj1.getSati() * 60 + obj1.getMinuti();
    vrijeme2 += obj2.getSati() * 60 + obj2.getMinuti();

    return vrijeme1 > vrijeme2;
}

bool operator< (const DatumVrijeme& obj1, const DatumVrijeme& obj2)
{
    int vrijeme1 = obj1.getGodina() * 365 + obj1.getMjesec() * 30 + obj1.getDan();
    int vrijeme2 = obj2.getGodina() * 365 + obj2.getMjesec() * 30 + obj2.getDan();

    vrijeme1 *= 1440;
    vrijeme2 *= 1440;

    vrijeme1 += obj1.getSati() * 60 + obj1.getMinuti();
    vrijeme2 += obj2.getSati() * 60 + obj2.getMinuti();

    return vrijeme1 < vrijeme2;
}

class Predmet {
    char* _naziv;
    int _ocjena;
    string _napomena;
public:
    Predmet(const char* naziv = "", int ocjena = 0, string napomena = "")
	{
        _naziv = Alociraj(naziv);
        _ocjena = ocjena;
        _napomena = napomena;
    }
	Predmet(const Predmet& obj)
    {
        _naziv = Alociraj(obj.GetNaziv());
        _ocjena = obj.GetOcjena();
        _napomena = obj.GetNapomena();
    }
	Predmet(Predmet&& obj) noexcept
        : _napomena(move(obj._napomena))
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
        _ocjena = obj.GetOcjena();
        obj._ocjena = 0;
    }
    ~Predmet() {
        delete[] _naziv;
    	_naziv = nullptr;
    }
	
    string GetNapomena() const { return _napomena; }
    char* GetNaziv() const { return _naziv; }
    int GetOcjena() const { return _ocjena; }
	
    void DodajNapomenu(string napomena)
	{
        _napomena += " " + napomena;
    }

	Predmet& operator= (const Predmet& obj)
    {
	    if (this != &obj)
	    {
            delete[] _naziv;
            _naziv = Alociraj(obj.GetNaziv());
            _ocjena = obj.GetOcjena();
            _napomena = obj.GetNapomena();
	    }

        return *this;
    }
    friend ostream& operator<< (ostream& COUT, const Predmet& obj)
	{
        COUT << obj.GetNaziv() << " (" << obj.GetOcjena() << ") " << obj.GetNapomena() << endl;
        return COUT;
    }
};

bool operator==(const Predmet& obj1, const Predmet& obj2)
{
    if (strcmp(obj1.GetNaziv(), obj2.GetNaziv()) != 0)
        return false;
    if (obj1.GetOcjena() != obj2.GetOcjena())
        return false;
    if (obj1.GetNapomena() != obj2.GetNapomena())
        return false;

    return true;
}

bool operator!=(const Predmet& obj1, const Predmet& obj2)
{
    return !(obj1 == obj2);
}

class Uspjeh
{
    GodinaStudija* _godina;
    //datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
    Dictionary<Predmet, DatumVrijeme> _predmeti;
public:
	Uspjeh()
	{
        _godina = nullptr;
	}
    Uspjeh(GodinaStudija godina)
	{
        _godina = new GodinaStudija(godina);
    }
	Uspjeh(Predmet& predmet, DatumVrijeme datum, GodinaStudija godina)
	{
        _godina = new GodinaStudija(godina);
        _predmeti.AddElement(predmet, godina);
	}
	Uspjeh(const Uspjeh& obj)
		: _predmeti(obj._predmeti)
    {
	    _godina = new GodinaStudija(*obj.GetGodinaStudija());
    }
	Uspjeh(Uspjeh&& obj) noexcept
		: _predmeti(move(obj._predmeti))
    {
        _godina = obj._godina;
        obj._godina = nullptr;
    }
    ~Uspjeh() { delete _godina; _godina = nullptr; }

    Dictionary<Predmet, DatumVrijeme>* GetPredmeti() { return &_predmeti; }
    GodinaStudija* GetGodinaStudija() const { return _godina; }

	void AddPredmet(Predmet predmet, DatumVrijeme datum)
	{
        _predmeti.AddElement(predmet, datum);
	}
	float GetProsjek() const
	{
        float prosjek = 0.0f;

        for (int i = 0; i < _predmeti.getTrenutno(); i++)
            prosjek += _predmeti.getElement1(i).GetOcjena();

        if (prosjek == 0.0f)
            return prosjek;

        return prosjek / _predmeti.getTrenutno();
	}

	Uspjeh& operator= (const Uspjeh& obj)
	{
		if (this != &obj)
		{
            delete _godina;
            _godina = new GodinaStudija(*obj.GetGodinaStudija());
            _predmeti = *const_cast<Uspjeh&>(obj).GetPredmeti();
		}

        return *this;
	}
    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj)
	{
        COUT << *obj.GetGodinaStudija() << " " << *const_cast<Uspjeh&>(obj).GetPredmeti() << endl;
        return COUT;
    }
    friend bool operator==(const Uspjeh& obj1, const Uspjeh& obj2);
};

bool operator== (const Uspjeh& obj1, const Uspjeh& obj2)
{
	if (*obj1.GetGodinaStudija() != *obj2.GetGodinaStudija())
        return false;
    if (obj1._predmeti.getTrenutno() != obj2._predmeti.getTrenutno())
        return false;

	for (int i = 0; i < obj1._predmeti.getTrenutno(); i++)
	{
        if (obj1._predmeti.getElement1(i) != obj2._predmeti.getElement1(i))
            return false;
        if (obj1._predmeti.getElement2(i) != obj2._predmeti.getElement2(i))
            return false;
	}

    return true;
}

bool operator!= (const Uspjeh& obj1, const Uspjeh& obj2)
{
    return !(obj1 == obj2);
}

mutex mtx;

class Student
{
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh> _uspjeh;

	void SaljiMail(GodinaStudija godina) const
	{
        ///*nakon evidentiranja uspjeha na bilo kojem predmetu tj. prilikom uspjesno izvrsene funkcije AddPredmet, Studentu se salje email sa sadrzajem:
		//FROM:info@fit.ba
		//TO: emailStudenta
		//Postovani ime i prezime, evidentirali ste uspjeh za X godinu studija.
		//Pozdrav.
		//FIT Team.
		//ukoliko je, nakon dodavanja predmeta, prosjek na nivou te godine veci od 8.0 Studentu se, pored email-a, salje i SMS sa sadrzajem: "Svaka cast za uspjeh X.X       ostvaren u X godini studija".
		//slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
		//*/

        mtx.lock();
        cout << "FROM: info@fit.ba\nTO: " << _emailAdresa << endl;
        cout << "Postovani " << _imePrezime << ", evidentirali ste uspjeh za " << godina << " godinu studija.\nPozdrav.\nFIT Team." << endl;
        mtx.unlock();
	}

	void SaljiSMS(GodinaStudija godina, float uspjeh) const
	{
        mtx.lock();
        cout << "Svaka cast za uspjeh " << uspjeh << "ostvaren u " << godina << " godini studija." << endl;
        mtx.unlock();
	}

	static bool ValidirajEmail(string email)
	{
        ///*
		//email adresa mora biti u formatu: text-text@ nakon cega slijedi neka od sljedecih domena: hotmail.com ili
		////outlook.com ili fit.ba. Pod text se podrazumijeva bilo koje slovo, malo ili veliko.
		//u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@fit.ba
		//za provjeru koristiti regex
		//*/

        return regex_match(email, regex("[A-z]+-[A-z]+@(hotmail\\.com|outlook\\.com|fit\\.ba)"));
	}
public:
	Student()
	{
        _imePrezime = nullptr;
	}
    Student(const char* imePrezime, string emailAdresa, string brojTelefona)
	{
        _imePrezime = Alociraj(imePrezime);
        ValidirajEmail(emailAdresa) ? _emailAdresa = emailAdresa : _emailAdresa = "notSet@fit.ba";
        _brojTelefona = brojTelefona;
    }
	Student(const Student& obj)
		: _uspjeh(obj._uspjeh)
	{
        _imePrezime = Alociraj(obj.GetImePrezime());
        _emailAdresa = obj.GetEmail();
        _brojTelefona = obj.GetBrojTelefona();
	}
	Student(Student&& obj) noexcept
        : _emailAdresa(move(obj._emailAdresa)), _brojTelefona(move(obj._brojTelefona)), _uspjeh(move(obj._uspjeh))
	{
        _imePrezime = obj._imePrezime;
        obj._imePrezime = nullptr;
	}
    ~Student() {
        delete[] _imePrezime;
		_imePrezime = nullptr;
    }
	
    char* GetImePrezime() const { return _imePrezime; }
    string GetEmail() const { return _emailAdresa; }
    string GetBrojTelefona() const { return _brojTelefona; }
    vector<Uspjeh>* GetUspjeh() { return &_uspjeh; }

	bool AddPredmet(Predmet predmet, GodinaStudija godina_studija, DatumVrijeme datum)
	{
        ///*
		 //uspjeh se dodaje za svaki predmet na nivou godine studija.
		 //tom prilikom onemoguciti:
		 //- dodavanje istoimenih predmeta na nivou jedne godine,
		 //- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jedne godine, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 1 sat).
		 //godine (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za drugu godinu, pa onda za prvu godinu i sl.).
		 //Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
		 //*/

        auto duplikat = [this, &godina_studija, &predmet]()
        {
            for (auto& uspjeh : _uspjeh)
            {
	            if (*uspjeh.GetGodinaStudija() == godina_studija)
	            {
		            for (int i = 0; i < uspjeh.GetPredmeti()->getTrenutno(); i++)
		            {
                        if (uspjeh.GetPredmeti()->getElement1(i) == predmet)
                            return false;
		            }
	            }
            }

            return true;
        };

        auto razlika = [this, &godina_studija, &datum]()
        {
            for (auto& uspjeh : _uspjeh)
            {
	            if (*uspjeh.GetGodinaStudija() == godina_studija)
	            {
                    for (int i = 0; i < uspjeh.GetPredmeti()->getTrenutno(); i++)
                    {
                        if (uspjeh.GetPredmeti()->getElement2(i) - datum < 60)
                        {
                            return false;
                        }
                    }
	            }
            }

            return true;
        };

        if (!duplikat() || !razlika())
            return false;

		for (auto& uspjeh : _uspjeh)
		{
            if (godina_studija == *uspjeh.GetGodinaStudija())
            {
                uspjeh.AddPredmet(predmet, datum);
            	
				thread worker1(&Student::SaljiMail, this, godina_studija);
				worker1.join();
            	
            	if (uspjeh.GetProsjek() > 8.0f)
            	{
                    thread worker2(&Student::SaljiSMS, this, godina_studija, uspjeh.GetProsjek());
                    worker2.join();
            	}
            	
                return true;
            }
		}

        Uspjeh temp(predmet, datum, godina_studija);
        _uspjeh.push_back(temp);
		
		thread worker1(&Student::SaljiMail, this, godina_studija);
		worker1.join();
		
		if (temp.GetProsjek() > 8.0f)
		{
            thread worker2(&Student::SaljiSMS, this, godina_studija, temp.GetProsjek());
            worker2.join();
		}
		
        return true;
	}
	int BrojPonavljanjaRijeci(const char* rijec) 
	{
        int counter = 0;
		
		for (auto& uspjeh : _uspjeh)
            for (int i = 0; i < uspjeh.GetPredmeti()->getTrenutno(); i++)
            {
                const string s = uspjeh.GetPredmeti()->getElement1(i).GetNapomena();
                regex potraga(rijec);
                auto begin = sregex_iterator(s.begin(), s.end(), potraga);
                auto end = sregex_iterator();

                counter += distance(begin, end);
            }

        return counter;
	}
	
	Uspjeh* operator[] (const char* godina)
	{
        GodinaStudija t_godina = PRVA;

        if (strcmp(godina, "DRUGA") == 0)
            t_godina = DRUGA;
        if (strcmp(godina, "TRECA") == 0)
            t_godina = TRECA;
		
		for (auto& uspjeh : _uspjeh)
		{
            if (t_godina == *uspjeh.GetGodinaStudija())
                return new Uspjeh(uspjeh);
		}

        return nullptr;
	}
	vector<Predmet> operator() (DatumVrijeme* start, DatumVrijeme* end)
	{
        vector<Predmet> temp;
		
        for (auto& uspjeh : _uspjeh)
        {
	        for (int i = 0; i < uspjeh.GetPredmeti()->getTrenutno(); i++)
	        {
                if (uspjeh.GetPredmeti()->getElement2(i) > *start && uspjeh.GetPredmeti()->getElement2(i) < *end)
                    temp.push_back(uspjeh.GetPredmeti()->getElement1(i));
	        }
        }

        delete start;
        delete end;
		
        return temp;
	}
	Student& operator= (const Student& obj)
	{
		if (this != &obj)
		{
            delete[] _imePrezime;
            _imePrezime = Alociraj(obj.GetImePrezime());
            _emailAdresa = obj.GetEmail();
            _brojTelefona = obj.GetBrojTelefona();
            _uspjeh = *const_cast<Student&>(obj).GetUspjeh();
		}

        return *this;
	}
    friend ostream& operator<< (ostream& COUT, const Student& obj)
	{
        COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " << obj.GetBrojTelefona() << endl;
        COUT << "Uspjeh: " << endl;

        ostream_iterator<Uspjeh> it(std::cout, "\n");
        copy(obj._uspjeh.begin(), obj._uspjeh.end(), it);
		
        return COUT;
    }
};

void main() {
    ///****************************************************************************
    //1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
    //2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
    //3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
    //4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
    //5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU
    //DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
    //6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
    //****************************************************************************/
    
    
    cout << "NA KRAJU ISPITA, RAD PREDAJTE U ODGOVARAJUCI FOLDER NA FTP SERVERU (INTEGRALNI)!" << endl;
    DatumVrijeme temp,
        datum19062019_1015(19, 6, 2019, 10, 15),
        datum20062019_1115(20, 6, 2019, 11, 15),
        datum30062019_1215(30, 6, 2019, 12, 15),
        datum05072019_1231(5, 7, 2019, 12, 31);
	
    //funkcija ToCharArray vraca datum i vrijeme kao char *. konverziju izvrsiti koristeci stringstream objekat.
    //voditi racuna o tome da se izmedju datuma i vremena nalazi samo jedan razmak, te da su vrijednosti dana i mjeseca 
    //iskazane kao dvije cifre
    cout << datum19062019_1015.ToCharArray() << endl;//treba ispisati: 19/06/2019 10:15
    temp = datum05072019_1231;
    cout << temp.ToCharArray() << endl;//treba ispisati: 05/07/2019 12:31

    const int DictionaryTestSize = 9;
    Dictionary<int, int> Dictionary1;
    for (size_t i = 0; i < DictionaryTestSize; i++)
        Dictionary1.AddElement(i + 1, i * i);

    try {
        //vraca elemente kolekcije koji se nalaze na lokacijama definisanim vrijednostima parametara (npr. 2 - 7). 
        //funkcija baca izuzetak u slucaju da se zahtijeva lokacija koja ne postoji ili je vrijednost posljednje lokacije manja od pocetne
        Dictionary<int, int> opseg = Dictionary1.getRange(2, 7);
        cout << opseg << endl;
        Dictionary1.getRange(7, 11);
    }
    catch (exception& err) {
        cout << err.what() << endl;
    }
    cout << Dictionary1 << endl;

    Dictionary<int, int> Dictionary2 = Dictionary1;
    cout << Dictionary2 << crt;

    Dictionary<int, int> Dictionary3;
    Dictionary3 = Dictionary1;
    cout << Dictionary3 << crt;

    //napomena se moze dodati i prilikom kreiranja objekta
    Predmet MAT("Matematika", 7, "Ucesce na takmicenju"),
        UIT("Uvod u informacijske tehnologije", 9),
        RM("Racunarske mreze", 8),
        EN("Engleski jezik", 6);
    UIT.DodajNapomenu("Pohvala za ostvareni uspjeh");
    cout << MAT << endl;
    cout << UIT << endl;
    cout << crt << endl;

    Dictionary<DatumVrijeme, DatumVrijeme> test;

    test.AddElement(DatumVrijeme(2, 3, 400), DatumVrijeme(2, 3, 400));
    test.AddElement(DatumVrijeme(3, 4, 500), DatumVrijeme(2, 6, 400));

	

    ///*
    //email adresa mora biti u formatu: text-text@ nakon cega slijedi neka od sljedecih domena: hotmail.com ili
    ////outlook.com ili fit.ba. Pod text se podrazumijeva bilo koje slovo, malo ili veliko.
    //u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@fit.ba
    //za provjeru koristiti regex
    //*/
    Student jasmin("Jasmin Azemovic", "jasmin.azemovic@hotmail.com", "033 281 172");
    Student adel("Adel Handzic", "adel.handzic@fit.ba", "033 281 170");
    Student emailNotValid("Ime Prezime", "korisnik@lazna.ba", "033 281 170");

    ///*
    //uspjeh se dodaje za svaki predmet na nivou godine studija.
    //tom prilikom onemoguciti:
    //- dodavanje istoimenih predmeta na nivou jedne godine,
    //- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jedne godine, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 1 sat).
    //godine (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za drugu godinu, pa onda za prvu godinu i sl.).
    //Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    //*/
    if (jasmin.AddPredmet(UIT, DRUGA, datum20062019_1115))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(RM, DRUGA, datum30062019_1215))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(EN, PRVA, datum19062019_1015))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(MAT, PRVA, datum20062019_1115))
        cout << "Predmet uspjesno dodan!" << crt;
    //ne treba dodati MAT jer je vec dodana u prvoj godini
    if (!jasmin.AddPredmet(MAT, PRVA, datum05072019_1231))
        cout << "Predmet nije uspjesno dodan!" << crt;
    //ne treba dodati UIT jer nije prosao 1 sat od dodavanja posljednjeg predmeta
    if (!jasmin.AddPredmet(UIT, PRVA, datum20062019_1115))
        cout << "Predmet nije uspjesno dodan!" << crt;

    cout << jasmin << endl;
	
    ///*nakon evidentiranja uspjeha na bilo kojem predmetu tj. prilikom uspjesno izvrsene funkcije AddPredmet, Studentu se salje email sa sadrzajem:
    //FROM:info@fit.ba
    //TO: emailStudenta
    //Postovani ime i prezime, evidentirali ste uspjeh za X godinu studija.
    //Pozdrav.
    //FIT Team.
    //ukoliko je, nakon dodavanja predmeta, prosjek na nivou te godine veci od 8.0 Studentu se, pored email-a, salje i SMS sa sadrzajem: "Svaka cast za uspjeh X.X       ostvaren u X godini studija".
    //slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    //*/
    cout << "USPJEH ISPISATI KORISTECI OSTREAM_ITERATOR" << endl;
    cout << jasmin << endl;
    ////vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
    cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;


    cout << crt << crt;
    ////vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
    vector<Predmet> jasminUspjeh = jasmin(new DatumVrijeme(18, 06, 2019, 10, 15), new DatumVrijeme(21, 06, 2019, 10, 10));
    for (Predmet u : jasminUspjeh)
        cout << u << endl;

    cout << crt;
	
    Uspjeh* uspjeh_I_godina = jasmin["PRVA"];//vraca uspjeh Studenta ostvaren u prvoj godini studija
    if (uspjeh_I_godina != nullptr)
        cout << *uspjeh_I_godina << endl;

    cin.get();
}