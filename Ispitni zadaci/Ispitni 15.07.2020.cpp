#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cassert>
#include <regex>
using namespace std;
const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";


char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    if (dealociraj)
        delete[]sadrzaj;
    return temp;
}

bool ValidirajLozinku(string lozinka)
{
    //za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi :
    //    -   najmanje 7 znakova
    //    -   velika i mala slova
    //    -   najmanje jedan broj
    //    -   najmanje jedan specijalni znak
    regex length{ "^.{7,}" };//Kapica je pocetak;
    regex velika_mala{ "[A-Z]+[a-z]+" };
    regex jedan_broj{ "[0-9]+" };
    regex specijalni_znak{ "[^A-Za-z0-9]+" };//Kapica negira te se dobiju specijalni karakteri;

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

template<class T1, class T2, int max = 10>
class Kolekcija {
    T1* _elementi1[max] = { nullptr };
    T2* _elementi2[max] = { nullptr };
    int  _trenutno;
public:
    Kolekcija()
    {
        _trenutno = 0;
    }
    Kolekcija(const Kolekcija& obj)
    {
        _trenutno = obj.getTrenutno();
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = new T1(obj.getElement1(i));
            _elementi2[i] = new T2(obj.getElement2(i));
        }
    }
    Kolekcija(Kolekcija&& obj)noexcept
    {
        _trenutno = obj._trenutno;
        obj._trenutno = 0;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];
            obj._elementi1[i] = nullptr;
            _elementi2[i] = obj._elementi2[i];
            obj._elementi2[i] = nullptr;
        }
    }
    Kolekcija& operator = (const Kolekcija& obj)
    {
        if (this == &obj)
            return *this;
        for (int i = 0; i < _trenutno; i++)
        {
            delete _elementi1[i];
            delete _elementi2[i];
            _elementi1[i] = nullptr;
            _elementi2[i] = nullptr;
        }
        _trenutno = obj.getTrenutno();
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = new T1(obj.getElement1(i));
            _elementi2[i] = new T2(obj.getElement2(i));
        }
        return *this;
    }
    ~Kolekcija()
    {
        for (size_t i = 0; i < _trenutno; i++)
        {
            delete _elementi1[i]; _elementi1[i] = nullptr;
            delete _elementi2[i]; _elementi2[i] = nullptr;
        }
    }

    T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
    int getTrenutno() const { return _trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    void AddElement(T1 prvi, T2 drugi)
    {
        if (_trenutno == max)
            throw exception("Prekoracenje opsega niza !!!");
        _elementi1[_trenutno] = new T1(prvi);
        _elementi2[_trenutno] = new T2(drugi);
        _trenutno++;
    }
    void RemoveAt(int lokacija)
    {
        if (lokacija < 0 || lokacija >= _trenutno)
            return;
        for (int i = lokacija; i < _trenutno - 1; i++)
        {
            _elementi1[i] = _elementi1[i + 1];
            _elementi2[i] = _elementi2[i + 1];
        }
        _trenutno--;
    }
    void AddElement(T1 prvi, T2 drugi, int location)
    {
        if (location < 0 || location >= _trenutno)//Ako nije validna lokacija;
            return;
        T1* tmp1 = new T1[_trenutno]();//Kreiranje pok na stari niz;
        T2* tmp2 = new T2[_trenutno]();//Kreiranje pok na stari niz;

        for (int i = 0; i < _trenutno; i++)//Prekopiranje vrijednosti;
        {
            tmp1[i] = *_elementi1[i];
            tmp2[i] = *_elementi2[i];
        }

        _elementi1[_trenutno] = new T1();//Alociranje prostora na zadnjem elementu;
        _elementi2[_trenutno] = new T2();

        _trenutno++;//Uvecavanje brojaca;

        *_elementi1[location] = prvi;//Postavljanje elemenata na lokaciji;
        *_elementi2[location] = drugi;

        for (int i = location + 1; i < _trenutno; i++)//Shiftanje elemenata u nizu;
        {
            //krecemo od lolacija+1 jer el. koji je tu ne zelimo dirati;
            *_elementi1[i] = tmp1[i - 1];//Pomjeranje unaprijed;
            *_elementi2[i] = tmp2[i - 1];
        }

        delete[] tmp1;//Obrisemo stare nizove;
        delete[] tmp2;
    }
    T2& operator [] (T1 key)
    {
        for (int i = 0; i < _trenutno; i++)
        {
            if (*_elementi1[i] == key)
                return *_elementi2[i];
        }
    }
};
class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000)
    {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& obj)
    {
        _dan = new int(obj.getDan());
        _mjesec = new int(obj.getMjesec());
        _godina = new int(obj.getGodina());
    }
    Datum(Datum&& obj)noexcept
    {
        _dan = obj._dan;
        _mjesec = obj._mjesec;
        _godina = obj._godina;
        obj._dan = obj._mjesec = obj._godina = nullptr;
    }
    Datum& operator = (const Datum& obj)
    {
        if (this == &obj)
            return *this;
        if (_dan == nullptr)
            _dan = new int();
        if (_mjesec == nullptr)
            _mjesec = new int();
        if (_godina == nullptr)
            _godina = new int();
        *_dan = obj.getDan();
        *_mjesec = obj.getMjesec();
        *_godina = obj.getGodina();
        return *this;
    }
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    int getDan()const
    {
        assert(_dan != nullptr);
        return *_dan;
    }
    int getMjesec()const
    {
        assert(_mjesec != nullptr);
        return *_mjesec;
    }
    int getGodina()const
    {
        assert(_godina != nullptr);
        return *_godina;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
    int toDani()const
    {
        return *_godina * 365 + *_mjesec * 30 + *_dan;
    }
};
bool operator == (const Datum& d1, const Datum& d2)
{
    return d1.getDan() == d2.getDan() && d1.getMjesec() == d2.getMjesec() && d1.getGodina() == d2.getGodina();
}
bool operator != (const Datum& d1, const Datum& d2)
{
    return !(d1 == d2);
}
int operator - (const Datum& d1, const Datum& d2)
{
    return abs(d1.toDani() - d2.toDani());
}
class Tehnika {
    char* _naziv;
    //int se odnosi na ocjenu u opsegu od 1 – 5, a Datum na datum kada je ocijenjena odredjena tehnika 
    Kolekcija<int, Datum, brojTehnika>* _ocjene;
public:
    Tehnika()
    {
        _naziv = nullptr;
        _ocjene = nullptr;
    }
    Tehnika(const char* naziv)
    {
        _naziv = GetNizKaraktera(naziv);
        _ocjene = new Kolekcija<int, Datum, brojTehnika>();
    }
    Tehnika(const Tehnika& obj)
        :_ocjene(new Kolekcija<int, Datum, brojTehnika>(*obj._ocjene))
    {
        _naziv = GetNizKaraktera(obj.GetNaziv());
    }
    Tehnika(Tehnika&& obj)noexcept
        :_ocjene(new Kolekcija<int, Datum, brojTehnika>(move(*obj._ocjene)))
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
    }
    Tehnika& operator = (const Tehnika& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_naziv;
        _naziv = GetNizKaraktera(obj.GetNaziv());
        if (_ocjene == nullptr)
            _ocjene = new Kolekcija<int, Datum, brojTehnika>();
        *_ocjene = obj.GetOcjene();
        return *this;
    }
    ~Tehnika()
    {
        delete[] _naziv; _naziv = nullptr;
        delete _ocjene; _ocjene = nullptr;
    }
    char* GetNaziv() const { return _naziv; }
    Kolekcija<int, Datum, brojTehnika>& GetOcjene() const { return *_ocjene; }
    float prosjekOcjenaTehnike()const
    {
        float prosjek = 0.0f;
        for (int i = 0; i < _ocjene->getTrenutno(); i++)
        {
            prosjek += _ocjene->getElement1(i);
        }
        if (prosjek > 0)
            return prosjek /= _ocjene->getTrenutno();
        return prosjek;
    }
    friend ostream& operator <<(ostream& COUT, const Tehnika& obj)
    {
        COUT << "Naziv: " << obj.GetNaziv() << endl;
        for (int i = 0; i < obj._ocjene->getTrenutno(); i++)
        {
            cout << "Ocjena: " << obj._ocjene->getElement1(i) << "  Datum: " << obj._ocjene->getElement2(i) << endl;
        }
        COUT << "Prosjecna ocjena tehnike: " << obj.prosjekOcjenaTehnike() << endl;
        return COUT;
    }
    bool AddOcjena(int ocjena, Datum& date)
    {
        /*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise navrata.
        -   razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
        -   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)
        */
        if (_ocjene->getTrenutno() == 0)
        {
            _ocjene->AddElement(ocjena, date);
            return true;
        }
        auto checkTime = [this, &date]()
        {
            for (int i = 0; i < _ocjene->getTrenutno(); i++)
            {
                if (_ocjene->getElement2(i) - date < 3)
                    return false;
            }
            return true;
        };
        if (checkTime())
        {
            _ocjene->AddElement(ocjena, date);
            return true;
        }
        return false;
    }
};
bool operator == (const Tehnika& t1, const Tehnika& t2)
{
    if (strcmp(t1.GetNaziv(), t2.GetNaziv()) != 0)
        return false;
    if (t1.GetOcjene().getTrenutno() != t2.GetOcjene().getTrenutno())
        return false;
    for (int i = 0; i < t1.GetOcjene().getTrenutno(); i++)
    {
        if (t1.GetOcjene().getElement1(i) != t2.GetOcjene().getElement1(i) || t1.GetOcjene().getElement2(i) != t2.GetOcjene().getElement2(i))
            return false;
    }
    return true;
}
bool operator != (const Tehnika& t1, const Tehnika& t2)
{
    return !(t1 == t2);
}
class Polaganje {
    Pojas _pojas;
    vector<Tehnika*> _polozeneTehnike;
public:
    Polaganje()
    {
        _pojas = BIJELI;
    }
    Polaganje(Pojas pojas = BIJELI)
    {
        _pojas = pojas;
    }
    Polaganje(const Pojas pojas, const Tehnika& tehnika)
    {
        _pojas = pojas;
        _polozeneTehnike.push_back(new Tehnika(tehnika));
    }
    Polaganje(const Polaganje& obj)
        :_pojas(obj.GetPojas())//,_polozeneTehnike(obj._polozeneTehnike)//Znaci do copy ctora je jer ne moze kopirati *;
    {
        for (int i = 0; i < _polozeneTehnike.size(); i++)
        {
            _polozeneTehnike[i] = new Tehnika(*obj._polozeneTehnike[i]);
        }
    }
    Polaganje(Polaganje&& obj)noexcept
        :_pojas(move(obj._pojas)), _polozeneTehnike(move(obj._polozeneTehnike))
    {

    }
    Polaganje& operator = (const Polaganje& obj)
    {
        if (this == &obj)
            return *this;
        _pojas = obj.GetPojas();
        _polozeneTehnike = obj._polozeneTehnike;
        return *this;
    }
    ~Polaganje()
    {
        for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
            delete _polozeneTehnike[i];
            _polozeneTehnike[i] = nullptr;
        }
    }
    vector<Tehnika*>& GetTehnike() { return _polozeneTehnike; }
    Pojas GetPojas() const { return _pojas; }
    friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
        COUT << obj._pojas << endl;
        for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
            COUT << *obj._polozeneTehnike[i];
        return COUT;
    }
    float prosjekSvihTehnika()const
    {
        float prosjek = 0.0f;
        for (int i = 0; i < _polozeneTehnike.size(); i++)
        {
            prosjek += _polozeneTehnike[i]->prosjekOcjenaTehnike();
        }
        if (prosjek > 0)
            return prosjek /= _polozeneTehnike.size();
        return prosjek;
    }
};
bool operator == (const Polaganje& p1, const Polaganje& p2)
{
    if (p1.GetPojas() != p2.GetPojas())
        return false;
    if (const_cast<Polaganje&>(p1).GetTehnike().size() != const_cast<Polaganje&>(p2).GetTehnike().size())
        return false;
    for (int i = 0; i < const_cast<Polaganje&>(p1).GetTehnike().size(); i++)
    {
        if (*const_cast<Polaganje&>(p1).GetTehnike()[i] != *const_cast<Polaganje&>(p2).GetTehnike()[i])
            return false;
    }
    return true;
}
bool operator != (const Polaganje& p1, const Polaganje& p2)
{
    return !(p1 == p2);
}
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
        _lozinka = ValidirajLozinku(lozinka) ? lozinka : NIJE_VALIDNA;
    }
    Korisnik(const Korisnik& obj)
        :_emailAdresa(obj.GetEmail()), _lozinka(obj.GetLozinka())
    {
        _imePrezime = GetNizKaraktera(obj.GetImePrezime());
    }
    Korisnik(Korisnik&& obj)noexcept
        :_emailAdresa(move(obj._emailAdresa)), _lozinka(move(obj._lozinka))
    {
        _imePrezime = obj._imePrezime;
        obj._imePrezime = nullptr;
    }
    Korisnik& operator = (const Korisnik& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_imePrezime;
        _imePrezime = GetNizKaraktera(obj.GetImePrezime());
        _emailAdresa = obj.GetEmail();
        _lozinka = obj.GetLozinka();
        return *this;
    }
    virtual  ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }
    string GetEmail()const { return _emailAdresa; }
    string GetLozinka() const { return _lozinka; }
    char* GetImePrezime() const { return _imePrezime; }
    friend ostream& operator<<(ostream& COUT, const Korisnik& obj)
    {
        COUT << "Ime i prezime: " << obj.GetImePrezime() << endl;
        COUT << "Email: " << obj.GetEmail() << endl;
        COUT << "Lozinka: " << obj.GetLozinka() << endl;
        return COUT;
    }
};
bool operator == (const Korisnik& k1, const Korisnik& k2)
{
    if (strcmp(k1.GetImePrezime(), k2.GetImePrezime()) != 0)
        return false;
    if (k1.GetEmail() != k2.GetEmail())
        return false;
    if (k1.GetLozinka() != k2.GetLozinka())
        return false;
    return true;
}
bool operator != (const Korisnik& k1, const Korisnik& k2)
{
    return !(k1 == k2);
}
mutex m;
class KaratePolaznik :public Korisnik
{
    vector<Polaganje> _polozeniPojasevi;
    void posaljiEmail(Pojas pojas, Tehnika tehnika, float prosjek, float ukupanProsjek)const
    {
        /*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje email sa porukom:
         FROM:info@karate.ba
         TO: emailKorisnika
         Postovani ime i prezime, evidentirana vam je thenika X za Y pojas. Dosadasnji uspjeh (prosjek ocjena)
         na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
         Pozdrav.
         KARATE Team.
         slanje email poruka implemenitrati koristeci zasebne thread-ove.
         */
        m.lock();
        cout << "FROM:info@karate.ba" << endl;
        cout << "TO: " << this->GetEmail() << endl;
        cout << "Postovani " << this->GetImePrezime() << " , evidentirana vam je tehnika " << tehnika.GetNaziv() << "za " << pojas << " pojas. Dosadasnji uspjeh (prosjek ocjena) na pojasu " << pojas << " iznosi " << prosjek << ", a ukupni uspjeh(prosjek ocjena) na svim pojasevima iznosi " << ukupanProsjek << "." << endl;
        cout << " Pozdrav." << endl;
        cout << " KARATE Team." << endl;
        m.unlock();
    }
public:
    KaratePolaznik()
        :Korisnik()
    {

    }
    KaratePolaznik(const char* imePrezime, string emailAdresa, string lozinka)
        :Korisnik(imePrezime, emailAdresa, lozinka)
    {

    }
    KaratePolaznik(const KaratePolaznik& obj)
        :Korisnik(obj), _polozeniPojasevi(obj._polozeniPojasevi)
    {

    }
    KaratePolaznik(KaratePolaznik&& obj) noexcept
        :Korisnik(move(obj)), _polozeniPojasevi(move(obj._polozeniPojasevi))
    {

    }
    KaratePolaznik& operator = (const KaratePolaznik& obj)
    {
        if (this == &obj)
            return *this;
        static_cast<Korisnik&>(*this) = static_cast<Korisnik>(obj);
        _polozeniPojasevi = obj._polozeniPojasevi;
        return *this;
    }
    virtual ~KaratePolaznik() override
    {
        cout << crt << "DESTRUKTOR -> KaratePolaznik" << crt;
    }
    friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
        COUT << static_cast<Korisnik&>(obj) << endl;
        for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
            COUT << obj._polozeniPojasevi[i];
        return COUT;
    }
    vector<Polaganje>& GetPolozeniPojasevi() { return _polozeniPojasevi; }

    bool AddTehniku(Pojas pojas, Tehnika tehnika)
    {
        /*
      sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
      tom prilikom onemoguciti:
      - dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
      - dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu
      svih tehnika vecu od 3.5
      */
        if (_polozeniPojasevi.size() == 0)
        {
            _polozeniPojasevi.push_back(Polaganje(pojas, tehnika));
            float prosjek = tehnika.prosjekOcjenaTehnike();
            float ukupanProsjek = 0.0f;
            for (auto& tehnikaa : _polozeniPojasevi)
            {
                ukupanProsjek = tehnikaa.prosjekSvihTehnika();
            }
            thread t(&KaratePolaznik::posaljiEmail, this, pojas, tehnika, prosjek, ukupanProsjek);
            t.join();
            return true;
        }
        auto checkIfIsti = [this, &pojas, &tehnika]()
        {
            for (auto& polozeni : _polozeniPojasevi)
            {
                if (polozeni.GetPojas() == pojas)
                {
                    for (auto& tehnikaa : polozeni.GetTehnike())
                    {
                        if (*tehnikaa == tehnika)
                            return false;
                    }
                }
            }
            return true;
        };
        auto checkPrethodniPojas = [this, &pojas]()
        {
            Pojas prethodni = static_cast<Pojas>(pojas - 1);
            for (auto& polozeni : _polozeniPojasevi)
            {
                if (polozeni.GetPojas() == prethodni)
                {
                    if (polozeni.GetTehnike().size() < 3)
                        return false;
                    if (polozeni.prosjekSvihTehnika() < 3.5f)
                        return false;
                }
            }
            return true;
        };
        if (checkIfIsti() && checkPrethodniPojas())
        {
            _polozeniPojasevi.push_back(Polaganje(pojas, tehnika));
            float prosjek = tehnika.prosjekOcjenaTehnike();
            float ukupanProsjek = 0.0f;
            for (auto& tehnikaa : _polozeniPojasevi)
            {
                ukupanProsjek = tehnikaa.prosjekSvihTehnika();
            }
            thread t(&KaratePolaznik::posaljiEmail, this, pojas, tehnika, prosjek, ukupanProsjek);
            t.join();
            return true;
        }
        return false;
    }
};


const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Pojasnite ulogu operatora reinterpret_cast.\n";
    return "Odgovor -> Reinterpret cast se koristi kada nemamo baznu klasu za dva objekta, te iz njih mozemo izvuci bukvalno samo bitove bez nekog znacaja sta ce oni predstavljati kod novokreiranog objekta.";
}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Ukratko opisite znacaj i vrste pametnih pokazivaca.\n";
    return "Odgovor -> Imamo tri razlicita pametna pokazivaca: unique shared i weak. Sva tri su korisna jer su prije svega to genericki tipovi objekata sto znaci da mogu drzati i primitivne tipove podataka kao i slozene (klase i strukture). Druga dobra stvar jeste sto shared pointeri mogu pokazivati na medjusobno djeljenu memorijsku lokaciju a usput ce uvijek pratiti koliko pokazivaca pokazuje na isti objekat pomocu metode use_count.Weak pointeri se nesto manje koriste jer nemaju neku funckionalnost, takodjer najbitnija stvar jeste sto se kod pametnih pokazivaca ne mora korisnik tj programer brinuti o rucnoj dealokaciji iako na raspolaganju ima metode reset() koja brise pok.";
}
void main() {

    cout << PORUKA;
    cin.get();

    cout << GetOdgovorNaPrvoPitanje() << endl;
    cin.get();
    cout << GetOdgovorNaDrugoPitanje() << endl;
    cin.get();

    Datum   datum19062020(19, 6, 2020),
        datum20062020(20, 6, 2020),
        datum30062020(30, 6, 2020),
        datum05072020(5, 7, 2020);

    int kolekcijaTestSize = 10;

    Kolekcija<int, int> kolekcija1;
    for (int i = 0; i < kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    cout << kolekcija1 << endl;

    try {
        /*metoda AddElement baca izuzetak u slucaju da se pokusa
        dodati vise od maksimalnog broja elemenata*/
        kolekcija1.AddElement(11, 11);
    }
    catch (exception& err) {
        cout << crt << "Greska -> " << err.what() << crt;
    }
    cout << kolekcija1 << crt;
    //Kolekcija<int, int> kolekcija3;
    //kolekcija3 = kolekcija1;
    //Kolekcija<int, int> kolekcija5(move(kolekcija3));

    kolekcija1.RemoveAt(2);
    /*uklanja par (T1 i T2) iz kolekcije koji se nalazi na lokaciji sa proslijedjenim indeksom.
    nakon uklanjanja vrijednosti onemoguciti pojavu praznog prostora unutar kolekcije tj.
    pomjeriti sve elemente koji se nalaze nakon proslijedjene lokacije za jedno mjesto unazad
    npr. ako unutar kolekcije postoje elementi
    0 0
    1 1
    2 2
    3 3
    nakon uklanjanja vrijednosti na lokaciji 1, sadrzaj kolekcije ce biti sljedeci
    0 0
    2 2
    3 3
    */

    cout << kolekcija1 << crt;

    kolekcija1.AddElement(9, 9, 2);
    /*funkciji AddElement se, kao treci parametar, moze proslijediti i lokacija na koju se dodaju
    nove vrijednosti pri cemu treba zadrzati postojece vrijednosti pomjerene za jedno mjesto unaprijed
    u odnosu na definisanu lokaciju npr. ako unutar kolekcije postoje elementi
    0 0
    1 1
    2 2
    3 3
    nakon dodavanja vrijednosti 9 i 9 na lokaciju 1, sadrzaj kolekcije ce biti sljedeci
    0 0
    9 9
    1 1
    2 2
    3 3
    */

    cout << kolekcija1 << crt;

    Kolekcija<int, int> kolekcija2 = kolekcija1;
    cout << kolekcija1 << crt;

    //na osnovu vrijednosti T1 mijenja vrijednost T2. 
    kolekcija1[9] = 2;
    /* npr.ako unutar kolekcije postoje elementi:
    0 0
    9 9
    1 1
    2 2
    3 3
    nakon promjene vrijednosti sadrzaj kolekcije ce biti sljedeci
    0 0
    9 2
    1 1
    2 2
    3 3
    */
    cout << kolekcija1 << crt;

    Tehnika choku_zuki("choku_zuki"),
        gyaku_zuki("gyaku_zuki"),
        kizami_zuki("kizami_zuki"),
        oi_zuki("oi_zuki");

    /*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise navrata.
        -   razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
        -   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)
    */
    if (choku_zuki.AddOcjena(1, datum19062020))
        cout << "Ocjena evidentirana!" << endl;
    if (!choku_zuki.AddOcjena(5, datum20062020))
        cout << "Ocjena NIJE evidentirana!" << endl;
    if (choku_zuki.AddOcjena(5, datum30062020))
        cout << "Ocjena evidentirana!" << endl;

    /* ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
       ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0*/
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
    za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
    -   najmanje 7 znakova
    -   velika i mala slova
    -   najmanje jedan broj
    -   najmanje jedan specijalni znak
    za provjeru validnosti lozinke koristiti globalnu funkciju ValidirajLozinku, a unutar nje regex metode.
    validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u slucaju da nije validna
    postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
    */

    Korisnik* jasmin = new KaratePolaznik("Jasmin Azemovic", "jasmin@karate.ba", "j@sm1N*");
    Korisnik* adel = new KaratePolaznik("Adel Handzic", "adel@edu.karate.ba", "4Ade1*H");
    Korisnik* emailNijeValidan = new KaratePolaznik("John Doe", "john.doe@google.com", "johndoe");

    /*
    sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
    - dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
    (onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3 tehnike za ZUTI pojas ili njihov prosjek nije veci od 3.5)
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
        if (jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
            cout << "Tehnika uspjesno dodan!" << crt;
        //ne treba dodati choku_zuki jer je vec dodana za zuti pojas
        if (!jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
            cout << "Tehnika NIJE uspjesno dodana!" << crt;

        //ispisuje sve dostupne podatke o karate polazniku
        cout << *jasminPolaznik << crt;
    }

    /*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje email sa porukom:
    FROM:info@karate.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirana vam je thenika X za Y pojas. Dosadasnji uspjeh (prosjek ocjena)
    na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
    Pozdrav.
    KARATE Team.
    slanje email poruka implemenitrati koristeci zasebne thread-ove.
    */

    //osigurati da se u narednim linijama poziva i destruktor klase KaratePolaznik
    delete jasmin;
    delete adel;
    delete emailNijeValidan;

    cin.get();
    system("pause>0");
}