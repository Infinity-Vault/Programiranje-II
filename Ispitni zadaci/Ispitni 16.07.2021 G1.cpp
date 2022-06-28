#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <cassert>
#include <regex>

using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum Predmet { UIT, PRI, PRII, PRIII, RSI, RSII };
const int brojRjesenja = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";

char* GetNizKaraktera(const char* sadrzaj) {
    if (sadrzaj == nullptr)
        return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    return temp;
}

bool ValidirajLozinku(string lozinka)
{
    //    za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
    //    -   najmanje 7 znakova
    //    -   velika i mala slova
    //    -   najmanje jedan broj
    //    -   najmanje jedan specijalni znak

    if (!regex_search(lozinka, regex(".{7}")))
        return false;
    if (!regex_search(lozinka, regex("([A-Z][a-z])|([a-z][A-Z])")))
        return false;
    if (!regex_search(lozinka, regex("[*!@#$%&]+")))
        return false;

    return true;
}

template<class T1, class T2, int max = 10>
class Kolekcija {
    T1 _elementi1[max];
    T2 _elementi2[max];
    int* _trenutno;
public:
    Kolekcija()
    {
        _trenutno = new int(0);
    }
    Kolekcija(const Kolekcija& obj)
    {
        _trenutno = new int(obj.getTrenutno());

        for (int i = 0; i < getTrenutno(); i++)
        {
            _elementi1[i] = obj.getElement1(i);
            _elementi2[i] = obj.getElement2(i);
        }
    }
    Kolekcija(Kolekcija&& obj) noexcept
    {
        _trenutno = obj._trenutno;

        for (int i = 0; i < getTrenutno(); i++)
        {
            _elementi1[i] = obj.getElement1(i);
            _elementi1[i] = obj.getElement2(i);

            // Idealno ja bih movao na ovaj nacin, ali to bi znacilo da svaki T1 i T2 mora imati move assignment operator
            _elementi1 = move(obj._elementi1[i]);
        }
    }
    ~Kolekcija()
    {
        delete _trenutno;
        _trenutno = nullptr;
    }

    T1 getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2 getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() const
    {
        assert(_trenutno != nullptr);
        return *_trenutno;
    }

    void AddElement(const T1 obj1, const T2 obj2)
    {
        if (_trenutno == nullptr)
            _trenutno = new int(0);
        if (getTrenutno() == max)
            throw exception("Maksimalan broj elemenata je dostignut");

        _elementi1[getTrenutno()] = obj1;
        _elementi2[getTrenutno()] = obj2;
        *_trenutno += 1;
    }
    void AddElement(const T1 obj1, const T2 obj2, int index)
    {
        Kolekcija* t_Kolekcija = new Kolekcija(*this);

        for (int i = index + 1; i < getTrenutno() + 1; i++)
        {
            _elementi1[i] = t_Kolekcija->getElement1(i - 1);
            _elementi2[i] = t_Kolekcija->getElement2(i - 1);
        }

        _elementi1[index] = obj1;
        _elementi2[index] = obj2;

        delete t_Kolekcija;

        *_trenutno += 1;
    }
    void RemoveAt(int index)
    {
        if (getTrenutno() == 0)
            return;
        // Ovdje bih idealno imao exception ili bih uradio custom error message
        if (index < 0 || index >= getTrenutno())
            return;

        for (int i = index; i < getTrenutno() - 1; i++)
        {
            _elementi1[i] = getElement1(i + 1);
            _elementi2[i] = getElement2(i + 1);
        }

        *_trenutno -= 1;
    }

    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj)
    {
        for (size_t i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    Kolekcija& operator= (const Kolekcija& obj)
    {
        if (this != &obj)
        {
            if (_trenutno == nullptr)
                _trenutno = new int();
            *_trenutno = obj.getTrenutno();

            for (int i = 0; i < getTrenutno(); i++)
            {
                _elementi1[i] = obj.getElement1(i);
                _elementi2[i] = obj.getElement2(i);
            }
        }

        return *this;
    }
    T2& operator[] (const T1 obj)
    {
        for (int i = 0; i < getTrenutno(); i++)
        {
            if (getElement1(i) == obj)
                return _elementi2[i];
        }
    }
};
class Datum
{
    int* _dan, * _mjesec, * _godina;

    int toDan() const
    {
        return *_dan + *_mjesec * 30 + *_godina * 365;
    }
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000)
    {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& obj)
    {
        // Na ispitu ja bih vjerovatno napravio gettere za dane, mjesece i godine kako bih provjerio da li su nullptr i sprijecio ikakav exception ovdje zbog
        // dereferenciranja
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);

    }
    Datum(Datum&& obj) noexcept
    {
        _dan = obj._dan;
        _mjesec = obj._mjesec;
        _godina = obj._godina;
    }
    ~Datum()
    {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }


    friend ostream& operator<< (ostream& COUT, const Datum& obj)
    {
        // Isto i ovdje, getteri sa assert da sprijece exception zbog nullptr
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
    friend int operator- (const Datum& d1, const Datum& d2)
    {
        return abs(d1.toDan() - d2.toDan());
    }
    friend bool operator< (const Datum& d1, const Datum& d2)
    {
        return d1.toDan() < d2.toDan();
    }
    friend bool operator== (const Datum& d1, const Datum& d2)
    {
        return *d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina;
    }
    friend bool operator!= (const Datum& d1, const Datum& d2)
    {
        return !(d1 == d2);
    }
    Datum& operator=(const Datum& obj)
    {
        if (this != &obj)
        {
            // Ista stvar sa getterima ovdje
            if (_dan == nullptr)
                _dan = new int();
            *_dan = *obj._dan;
            if (_mjesec == nullptr)
                _mjesec = new int();
            *_mjesec = *obj._mjesec;
            if (_godina == nullptr)
                _godina = new int();
            *_godina = *obj._godina;
        }

        return *this;
    }
};
/*na odredjeno pitanje kandidat je duzan postaviti vise rjesenja od kojih ce
svako biti ocijenjeno*/

class Pitanje
{
    char* _sadrzaj;
    //int se odnosi na ocjenu u opsegu  1 – 5, a Datum na datum kada je 
    Kolekcija<int, Datum, brojRjesenja>* _ocjeneRjesenja;
public:
    Pitanje(const char* sadrzaj)
    {
        _sadrzaj = GetNizKaraktera(sadrzaj);
        _ocjeneRjesenja = new Kolekcija<int, Datum, 6>();
    }
    Pitanje(const Pitanje& obj)
    {
        _sadrzaj = GetNizKaraktera(obj.GetSadrzaj());
        _ocjeneRjesenja = new Kolekcija<int, Datum, 6>(obj.GetOcjene());
    }
    Pitanje(Pitanje&& obj) noexcept
    {
        _sadrzaj = obj._sadrzaj;
        _ocjeneRjesenja = obj._ocjeneRjesenja;
    }
    ~Pitanje()
    {
        delete[] _sadrzaj; _sadrzaj = nullptr;
        delete _ocjeneRjesenja; _ocjeneRjesenja = nullptr;
    }

    char* GetSadrzaj() const { return _sadrzaj; }
    Kolekcija<int, Datum, brojRjesenja>& GetOcjene() const
    {
        assert(_ocjeneRjesenja != nullptr);
        return *_ocjeneRjesenja;
    }

    bool AddOcjena(int ocjena, Datum& datum)
    {
        //    /*svako pitanje moze imati vise ocjena tj. razlicita rjesenja/odgovori se
        //mogu postaviti u vise navrata.Drugim rijecima, ocjena, rjesenje i odgovor se
        //mogu posmatrati kao sinonimi.
        //        -   razmak izmedju postavljanja dva rjesenja mora biti najmanje 3
        //dana
        //        -   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec
        //evidentirane (bez obzira sto je stariji od 3 dana)
        //    */

        if (GetOcjene().getTrenutno() == 0)
        {
            GetOcjene().AddElement(ocjena, datum);
            return true;
        }

        if (GetOcjene().getElement2(GetOcjene().getTrenutno() - 1) - datum < 3)
            return false;
        for (int i = 0; i < GetOcjene().getTrenutno(); i++)
        {
            if (datum < GetOcjene().getElement2(i))
                return false;
        }

        GetOcjene().AddElement(ocjena, datum);
        return true;
    }
    float prosjekOcjena() const
    {
        float prosjek = 0.0f;

        for (int i = 0; i < GetOcjene().getTrenutno(); i++)
            prosjek += GetOcjene().getElement1(i);

        if (prosjek == 0.0f)
            return prosjek;

        return prosjek / GetOcjene().getTrenutno();
    }

    friend ostream& operator<< (ostream& COUT, const Pitanje& obj)
    {
        COUT << "Pitanje: " << obj.GetSadrzaj() << endl;
        COUT << obj.GetOcjene() << endl;
        COUT << "Prosjek: " << obj.prosjekOcjena() << endl;

        return COUT;
    }
    friend bool operator== (const Pitanje& obj1, const Pitanje& obj2)
    {
        if (strcmp(obj1.GetSadrzaj(), obj2.GetSadrzaj()) != 0)
            return false;

        if (obj1.GetOcjene().getTrenutno() != obj2.GetOcjene().getTrenutno())
            return false;

        for (int i = 0; i < obj1.GetOcjene().getTrenutno(); i++)
        {
            if (obj1.GetOcjene().getElement1(i) != obj2.GetOcjene().getElement1(i))
                return false;
            if (obj1.GetOcjene().getElement2(i) != obj2.GetOcjene().getElement2(i))
                return false;
        }
        return true;
    }
    Pitanje& operator= (const Pitanje& obj)
    {
        if (this != &obj)
        {
            delete[] _sadrzaj;
            _sadrzaj = GetNizKaraktera(obj.GetSadrzaj());

            if (_ocjeneRjesenja == nullptr)
                _ocjeneRjesenja = new Kolekcija<int, Datum, 6>();
            *_ocjeneRjesenja = obj.GetOcjene();
        }

        return *this;
    }
};
class Ispit {
    Predmet _predmet;
    vector<Pitanje*> _pitanjaOdgovori;
public:
    Ispit()
        : _predmet(UIT)
    {

    }
    Ispit(Predmet predmet = UIT)
    {
        _predmet = predmet;
    }
    Ispit(Predmet predmet, const Pitanje& pitanje)
    {
        _predmet = predmet;
        _pitanjaOdgovori.push_back(new Pitanje(pitanje));
    }
    Ispit(const Ispit& obj)
    {
        _predmet = obj.GetPredmet();

        // Getter+Assert/Throw za _pitanjaOdgovori bi bila dobra ideja radi nullptr
        for (int i = 0; i < obj._pitanjaOdgovori.size(); i++)
            _pitanjaOdgovori.push_back(new Pitanje(*obj._pitanjaOdgovori[i]));
    }
    Ispit(Ispit&& obj) noexcept
    {
        _predmet = obj._predmet;

        for (int i = 0; i < _pitanjaOdgovori.size(); i++)
        {
            _pitanjaOdgovori[i] = obj._pitanjaOdgovori[i];
            obj._pitanjaOdgovori[i] = nullptr;
        }
    }
    ~Ispit()
    {
        for (size_t i = 0; i < _pitanjaOdgovori.size(); i++)
        {
            delete _pitanjaOdgovori[i];
            _pitanjaOdgovori[i] = nullptr;
        }
    }

    vector<Pitanje*>& GetPitanjaOdgovore() { return _pitanjaOdgovori; }
    Predmet GetPredmet() const { return _predmet; }

    float prosjekPitanja() const
    {
        float prosjek = 0.0f;

        for (auto& pitanje : _pitanjaOdgovori)
            prosjek += pitanje->prosjekOcjena();

        if (prosjek == 0.0f)
            return prosjek;

        return prosjek / _pitanjaOdgovori.size();
    }

    friend ostream& operator<< (ostream& COUT, const Ispit& obj) {
        COUT << obj._predmet << endl;
        for (size_t i = 0; i < obj._pitanjaOdgovori.size(); i++)
            COUT << *obj._pitanjaOdgovori[i];
        return COUT;
    }
    Ispit& operator= (const Ispit& obj)
    {
        if (this != &obj)
        {
            _predmet = obj.GetPredmet();

            for (int i = 0; i < _pitanjaOdgovori.size(); i++)
            {
                delete _pitanjaOdgovori[i];
                // I ovdje bi dobro dosao bio jedan getter koji ce provjeriti da li je nullptr uzeto Pitanje od obj
                // ili jednostavno assertati ovdje ovako:
                assert(obj._pitanjaOdgovori[i] != nullptr);
                _pitanjaOdgovori[i] = new Pitanje(*obj._pitanjaOdgovori[i]);
            }
        }

        return *this;
    }
};
class Korisnik {
    char* _imePrezime;
    string _emailAdresa;
    string _lozinka;
public:
    Korisnik()
    {
        _imePrezime = nullptr;
    }
    Korisnik(const char* imePrezime, string emailAdresa, string lozinka)
    {
        _imePrezime = GetNizKaraktera(imePrezime);
        _emailAdresa = emailAdresa;
        if (!ValidirajLozinku(lozinka))
            _lozinka = "<VRIJEDNOST_NIJE_VALIDNA>";
        else
            _lozinka = lozinka;
    }
    Korisnik(const Korisnik& obj)
    {
        _imePrezime = GetNizKaraktera(obj.GetImePrezime());
        _emailAdresa = obj.GetEmail();
        _lozinka = obj.GetLozinka();
    }
    Korisnik(Korisnik&& obj) noexcept
    {
        _imePrezime = obj._imePrezime;
        _emailAdresa = move(obj._emailAdresa);
        _lozinka = move(obj._lozinka);
    }

    virtual ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }

    string GetEmail() const { return _emailAdresa; }
    string GetLozinka() const { return _lozinka; }
    char* GetImePrezime() const { return _imePrezime; }

    virtual void Info() {}

    Korisnik& operator= (const Korisnik& obj)
    {
        if (this != &obj)
        {
            delete[] _imePrezime;
            _imePrezime = GetNizKaraktera(obj.GetImePrezime());

            _emailAdresa = obj.GetEmail();
            _lozinka = obj.GetLozinka();
        }

        return *this;
    }
};

mutex mtx;

class Kandidat : public Korisnik
{
    vector<Ispit> _polozeniPredmeti;

    void SendMail(Pitanje& pitanje) const
    {
        //    /*nakon evidentiranja ocjene na bilo kojem odgovoru, kandidatu se salje
        //email sa porukom:
        //
        //    FROM:info@kursevi.ba
        //    TO: emailKorisnika
        //
        //    Postovani ime i prezime, evidentirana vam je ocjena X za odgovor na
        //pitanje Y. Dosadasnji uspjeh (prosjek ocjena)
        //    za pitanje Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim
        //predmetima iznosi Z.
        //    Pozdrav.
        //
        //    EDUTeam.
        mtx.lock();
        cout << "FROM: info@kursevi.ba" << endl;
        cout << "TO: " << GetEmail() << "\n" << endl;
        cout << "Postovani " << GetImePrezime() << ", evidentirana vam je ocjena X za odgovor na pitanje " << pitanje.GetSadrzaj() << ".Dosadasnji uspjeh za pitanje " << pitanje.GetSadrzaj() << " iznosi " << pitanje.prosjekOcjena() << ", a ukupni uspjeh na svim predmetima iznosi " << prosjekUspjeha() << "." << endl;
        cout << "Pozdrav.\n\nEDUTeam." << endl;
        mtx.unlock();
    }
public:
    Kandidat()
    {

    }
    Kandidat(const char* imePrezime, string emailAdresa, string lozinka)
        : Korisnik(imePrezime, emailAdresa, lozinka)
    {

    }
    Kandidat(const Kandidat& obj)
        : Korisnik(obj), _polozeniPredmeti(obj._polozeniPredmeti)
    {

    }
    Kandidat(Kandidat&& obj) noexcept
        : Korisnik(move(obj)), _polozeniPredmeti(move(obj._polozeniPredmeti))
    {

    }
    ~Kandidat() override
    {
        cout << crt << "DESTRUKTOR -> Kandidat" << crt;
    }

    vector<Ispit>& GetPolozeniPredmeti()
    {
        return _polozeniPredmeti;
    }

    bool AddPitanje(Predmet predmet, Pitanje& pitanje)
    {
        //    /*
        //    svi odgovori na nivou jednog predmeta (PRI, PRII... ) se evidentiraju
        //unutar istog objekta tipa Ispit tj. pripadajuceg objekta tipa Pitanje,
        //    tom prilikom onemoguciti:
        //    - dodavanje istih (moraju biti identicne vrijednosti svih atributa)
        //odgovora na nivou jednog predmeta,
        //    - dodavanje odgovora za viši predmet ako prethodni predmet nema
        //evidentirana najmanje 3 pitanja ili nema prosjecnu ocjenu svih pitanja vecu
        //od 3.5
        //    (onemoguciti dodavanje pitanja za PRII ako ne postoje najmanje 3 pitanja
        //za predmet PRI ili njihov prosjek nije veci od 3.5)
        //    funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
        //    */

        auto duplikat = [this, &predmet, &pitanje]()
        {
            for (auto& polozeni : _polozeniPredmeti)
                if (polozeni.GetPredmet() == predmet)
                {
                    for (auto& pitanje_odgovor : polozeni.GetPitanjaOdgovore())
                        if (*pitanje_odgovor == pitanje)
                            return false;
                }
            return true;
        };

        auto visi_predmet = [this, &predmet, &pitanje]()
        {
            const auto prethodni_predmet = (Predmet)(predmet - 1);

            for (auto& polozeni : _polozeniPredmeti)
                if (polozeni.GetPredmet() == prethodni_predmet)
                {
                    if (polozeni.GetPitanjaOdgovore().size() < 3)
                        return false;
                    if (polozeni.prosjekPitanja() <= 3.5f)
                        return false;
                }
            return true;
        };

        if (!duplikat() || !visi_predmet())
            return false;

        for (auto& polozeni : _polozeniPredmeti)
            if (predmet == polozeni.GetPredmet())
            {
                polozeni.GetPitanjaOdgovore().push_back(new Pitanje(pitanje));
                thread worker(&Kandidat::SendMail, this, ref(pitanje));
                worker.join();
                return true;
            }

        Ispit temp(predmet, pitanje);
        _polozeniPredmeti.push_back(temp);
        thread worker(&Kandidat::SendMail, this, ref(pitanje));
        worker.join();
        return true;
    }
    float prosjekUspjeha() const
    {
        float prosjek = 0.0f;

        for (auto& polozeni : _polozeniPredmeti)
            prosjek += polozeni.prosjekPitanja();

        if (prosjek == 0.0f)
            return prosjek;

        return prosjek / _polozeniPredmeti.size();
    }

    friend ostream& operator<< (ostream& COUT, Kandidat& obj)
    {
        COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " <<
            obj.GetLozinka() << endl;
        for (size_t i = 0; i < obj._polozeniPredmeti.size(); i++)
            COUT << obj._polozeniPredmeti[i];
        return COUT;
    }
    Kandidat& operator= (const Kandidat& obj)
    {
        if (this != &obj)
        {
            Korisnik(*this) = obj;
            _polozeniPredmeti = obj._polozeniPredmeti;
        }

        return *this;
    }
};

void main()
{
    Datum   datum19062020(19, 6, 2020),
        datum20062020(20, 6, 2020),
        datum30062020(30, 6, 2020),
        datum05072020(5, 7, 2020);

    int kolekcijaTestSize = 10;

    Kolekcija<int, int> kolekcija1;
    for (int i = 0; i < kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    cout << kolekcija1 << endl;

    //
    try {
        /*metoda AddElement baca izuzetak u slucaju da se pokusa
        dodati vise od maksimalnog broja elemenata*/
        kolekcija1.AddElement(11, 11);
    }
    catch (exception& err) {
        cout << crt << "Greska -> " << err.what() << crt;
    }
    cout << kolekcija1 << crt;
    //
    //
    kolekcija1.RemoveAt(2);
    //    /*uklanja par (T1 i T2) iz kolekcije koji se nalazi na lokaciji sa
    //proslijedjenim indeksom.
    //    nakon uklanjanja vrijednosti onemoguciti pojavu praznog prostora unutar
    //kolekcije tj.
    //    pomjeriti sve elemente koji se nalaze nakon proslijedjene lokacije za
    //jedno mjesto unazad
    //    npr. ako unutar kolekcije postoje elementi
    //    0 0
    //    1 1
    //    2 2
    //    3 3
    //    nakon uklanjanja vrijednosti na lokaciji 1, sadrzaj kolekcije ce biti
    //sljedeci
    //    0 0
    //    2 2
    //    3 3
    //    */
    //
    cout << kolekcija1 << crt;
    //
    kolekcija1.AddElement(9, 9, 2);
    //    /*funkciji AddElement se, kao treci parametar, moze proslijediti i
    //lokacija na koju se dodaju
    //    nove vrijednosti pri cemu treba zadrzati postojece vrijednosti pomjerene
    //za jedno mjesto unaprijed
    //    u odnosu na definisanu lokaciju npr. ako unutar kolekcije postoje
    //elementi
    //    0 0
    //    1 1
    //    2 2
    //    3 3
    //    nakon dodavanja vrijednosti 9 i 9 na lokaciju 1, sadrzaj kolekcije ce
    //biti sljedeci
    //    0 0
    //    9 9
    //    1 1
    //    2 2
    //    3 3
    //    */
    //
    cout << kolekcija1 << crt;
    //
    Kolekcija<int, int> kolekcija2 = kolekcija1;

    cout << kolekcija1 << crt;
    //
    //    //na osnovu vrijednosti T1 mijenja vrijednost T2.  
    kolekcija1[9] = 2;
    cout << kolekcija1 << crt;
    //    /* npr.ako unutar kolekcije postoje elementi:
    //    0 0
    //    9 9
    //    1 1
    //    2 2
    //    3 3
    //    nakon promjene vrijednosti sadrzaj kolekcije ce biti sljedeci
    //    0 0
    //    9 2
    //    1 1
    //    2 2
    //    3 3
    //    */
    //
    Pitanje sortiranjeNiza("Navedite algoritme za sortiranje clanova niza."),
        dinamickaMemorija("Navedite pristupe za upravljanje dinamickom memorijom."),
        visenitnoProgramiranje("Na koji se sve nacin moze koristiti veci broj niti tokom izvrsenja programa."),
        regex("Navedite par primjera regex validacije podataka.");
    //
    //    /*svako pitanje moze imati vise ocjena tj. razlicita rjesenja/odgovori se
    //mogu postaviti u vise navrata.Drugim rijecima, ocjena, rjesenje i odgovor se
    //mogu posmatrati kao sinonimi.
    //        -   razmak izmedju postavljanja dva rjesenja mora biti najmanje 3
    //dana
    //        -   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec
    //evidentirane (bez obzira sto je stariji od 3 dana)
    //    */
    if (sortiranjeNiza.AddOcjena(1, datum19062020))
        cout << "Ocjena evidentirana!" << endl;
    if (!sortiranjeNiza.AddOcjena(5, datum20062020))
        cout << "Ocjena NIJE evidentirana!" << endl;
    if (sortiranjeNiza.AddOcjena(5, datum30062020))
        cout << "Ocjena evidentirana!" << endl;
    //
    //    // ispisuje sadrzaj/tekst pitanja, ocjene (zajedno sa datumom) i 
    //    prosjecnu ocjenu za sve odgovore / rjesenja
    //        // ukoliko pitanje nema niti jednu ocjenu prosjecna treba biti 0 
    cout << sortiranjeNiza << endl;
    //
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
    //
    //    /*
    //    za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
    //    -   najmanje 7 znakova
    //    -   velika i mala slova
    //    -   najmanje jedan broj
    //    -   najmanje jedan specijalni znak
    //
    //    za provjeru validnosti lozinke koristiti globalnu funkciju
    //ValidirajLozinku, a unutar nje regex metode.
    //    validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u
    //slucaju da nije validna
    //    postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
    //    */
    //
    Korisnik* jasmin = new Kandidat("Jasmin Azemovic", "jasmin@kursevi.ba", "j@sm1N*");
    Korisnik* adel = new Kandidat("Adel Handzic", "adel@edu.kursevi.ba", "4Ade1*H");
    Korisnik* lozinkaNijeValidna = new Kandidat("John Doe", "john.doe@google.com", "johndoe");
    //
    //    /*
    //    svi odgovori na nivou jednog predmeta (PRI, PRII... ) se evidentiraju
    //unutar istog objekta tipa Ispit tj. pripadajuceg objekta tipa Pitanje,
    //    tom prilikom onemoguciti:
    //    - dodavanje istih (moraju biti identicne vrijednosti svih atributa)
    //odgovora na nivou jednog predmeta,
    //    - dodavanje odgovora za viši predmet ako prethodni predmet nema
    //evidentirana najmanje 3 pitanja ili nema prosjecnu ocjenu svih pitanja vecu
    //od 3.5
    //    (onemoguciti dodavanje pitanja za PRII ako ne postoje najmanje 3 pitanja
    //za predmet PRI ili njihov prosjek nije veci od 3.5)
    //    funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    //    */
    //
    //    //doraditi klase da nacin da omoguce izvrsenje naredne linije koda 
    Kandidat* jasminPolaznik = dynamic_cast<Kandidat*>(jasmin);
    //
    if (jasminPolaznik != nullptr) {
        if (jasminPolaznik->AddPitanje(PRI, dinamickaMemorija))
            cout << "Pitanje uspjesno dodano!" << crt;
        //ne treba dodati visenitnoProgramiranje jer ne postoje evidentirana 
        // 3 pitanja za predmet PRI
        if (!jasminPolaznik->AddPitanje(PRII, visenitnoProgramiranje))
            cout << "Pitanje NIJE uspjesno dodano!" << crt;
        if (jasminPolaznik->AddPitanje(PRI, visenitnoProgramiranje))
            cout << "Pitanje uspjesno dodano!" << crt;
        if (jasminPolaznik->AddPitanje(PRI, regex))
            cout << "Pitanje uspjesno dodano!" << crt;
        if (jasminPolaznik->AddPitanje(PRI, sortiranjeNiza))
            cout << "Pitanje uspjesno dodano!" << crt;
        //ne treba dodati sortiranjeNiza jer je vec dodana za predmet PRI 
        if (!jasminPolaznik->AddPitanje(PRI, sortiranjeNiza))
            cout << "Pitanje NIJE uspjesno dodano!" << crt;
        //
                //ispisuje sve dostupne podatke o kandidatu 
        cout << *jasminPolaznik << crt;
    }
    //    /*nakon evidentiranja ocjene na bilo kojem odgovoru, kandidatu se salje
    //email sa porukom:
    //
    //    FROM:info@kursevi.ba
    //    TO: emailKorisnika
    //
    //    Postovani ime i prezime, evidentirana vam je ocjena X za odgovor na
    //pitanje Y. Dosadasnji uspjeh (prosjek ocjena)
    //    za pitanje Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim
    //predmetima iznosi Z.
    //    Pozdrav.
    //
    //    EDUTeam.
    //    slanje email poruka implemenitrati koristeci zasebne thread-ove.
    //    */
    //
    //    //osigurati da se u narednim linijama poziva i destruktor klase Kandidat 
    delete jasmin;
    delete adel;
    delete lozinkaNijeValidna;
    //
    cin.get();
}