#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <regex>
#include <thread>
using namespace std;
const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. ATRIBUTI, NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"-------------------------------------------------------------------------------\n";
const char* crt = "\n-------------------------------------------\n";
enum Predmet { UIT, PRI, PRII, PRIII, RSI, RSII };
enum Dupliranje { SA_DUPLIKATIMA, BEZ_DUPLIKATA };
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
    /*
    za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi 6 ili 7 znakova postujuci sljedeca pravila:
    -   pocinje sa znak * (zvjezdica) ili _ (donja crtica), nakon cega slijedi
    -   broj, nakon cega slijedi
    -   malo slovo, nakon cega slijedi
    -   razmak koji NIJE OBAVEZAN, nakon cega slijedi
    -   veliko slovo, nakon cega slijedi
    -   broj, nakon cega slijedi
    -   malo slovo.*/
    return regex_match(lozinka, regex("(\\*|\\_)[0-9][a-z]\\s?[A-Z][0-9][a-z]"));
}
template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int* _trenutno;
    Dupliranje _dupliranje;
public:
    Kolekcija(Dupliranje dupliranje = SA_DUPLIKATIMA)
    {
        _trenutno = new int(0);
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _dupliranje = dupliranje;
    }
    Kolekcija(const Kolekcija& obj)
    {
        _trenutno = new int(obj.getTrenutno());
        _elementi1 = new T1[*_trenutno];
        _elementi2 = new T2[*_trenutno];
        for (int i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = obj.getElement1(i);
            _elementi2[i] = obj.getElement2(i);
        }
        _dupliranje = obj._dupliranje;
    }
    Kolekcija(Kolekcija&& obj)noexcept
    {
        _trenutno = obj._trenutno;
        obj._trenutno = nullptr;
        _elementi1 = obj._elementi1;
        _elementi2 = obj._elementi2;
        obj._elementi1 = nullptr;
        obj._elementi2 = nullptr;
        _dupliranje = obj._dupliranje;
    }
    Kolekcija& operator = (const Kolekcija& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_elementi1;
        delete[]_elementi2;
        if (_trenutno == nullptr)
            _trenutno = new int();
        *_trenutno = obj.getTrenutno();
        _elementi1 = new T1[*_trenutno];
        _elementi2 = new T2[*_trenutno];
        for (int i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = obj.getElement1(i);
            _elementi2[i] = obj.getElement2(i);
        }
        _dupliranje = obj._dupliranje;
        return *this;
    }
    ~Kolekcija()
    {
        delete _trenutno; _trenutno = nullptr;
        delete[] _elementi1; _elementi1 = nullptr;
        delete[] _elementi2; _elementi2 = nullptr;
    }
    T1 getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2 getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno()const
    {
        assert(_trenutno != nullptr);
        return *_trenutno;
    }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    void prosiriNiz()
    {
        T1* temp1 = _elementi1;
        T2* temp2 = _elementi2;
        _elementi1 = new T1[*_trenutno + 1];
        _elementi2 = new T2[*_trenutno + 1];
        for (int i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = temp1[i];
            _elementi2[i] = temp2[i];
        }
        delete[]temp1;
        delete[]temp2;
        temp1 = nullptr;
        temp2 = nullptr;
    }
    void AddElement(T1 prvi, T2 drugi)
    {
        if (_dupliranje == BEZ_DUPLIKATA)
        {
            for (int i = 0; i < *_trenutno; i++)
            {
                if (_elementi1[i] == prvi && _elementi2[i] == drugi)
                    throw exception("Pokusaj dodavanja istih, vec dodanih elemenata !!!");
            }
        }
        prosiriNiz();
        _elementi1[*_trenutno] = prvi;
        _elementi2[*_trenutno] = drugi;
        (*_trenutno)++;
    }
    Kolekcija operator [](T1 key)
    {
        Kolekcija<T1, T2> povratni;
        for (int i = 0; i < *_trenutno; i++)
        {
            if (_elementi1[i] == key)
                povratni.AddElement(_elementi1[i], _elementi2[i]);
        }
        return povratni;
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
        obj._dan = nullptr;
        _mjesec = obj._mjesec;
        obj._mjesec = nullptr;
        _godina = obj._godina;
        obj._godina = nullptr;
    }
    Datum& operator =(const Datum& obj)
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
int operator -(const Datum& d1, const Datum& d2)
{
    return abs(d1.toDani() - d2.toDani());
}
class Pitanje {
    char* _sadrzaj;
    //int se odnosi na ocjenu u opsegu  1 – 5, a Datum na datum kada je odgovor/rjesenje ocijenjeno
    Kolekcija<int, Datum*> _ocjeneRjesenja;
public:
    Pitanje(const char* sadrzaj = "")
    {
        _sadrzaj = GetNizKaraktera(sadrzaj);
    }
    Pitanje(const Pitanje& obj)
        :_ocjeneRjesenja(obj._ocjeneRjesenja)
    {
        _sadrzaj = GetNizKaraktera(obj.GetSadrzaj());
    }
    Pitanje(Pitanje&& obj)noexcept
        :_ocjeneRjesenja(move(obj._ocjeneRjesenja))
    {
        _sadrzaj = obj._sadrzaj;
        obj._sadrzaj = nullptr;
    }
    Pitanje& operator = (const Pitanje& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_sadrzaj;
        _sadrzaj = GetNizKaraktera(obj.GetSadrzaj());
        _ocjeneRjesenja = obj._ocjeneRjesenja;
        return *this;
    }
    ~Pitanje() {
        delete[] _sadrzaj; _sadrzaj = nullptr;
    }
    char* GetSadrzaj() const { return _sadrzaj; }
    Kolekcija<int, Datum*>& GetOcjene() { return _ocjeneRjesenja; }
    float getProsjecnaOcjenaPitanja()const
    {
        float prosjek = 0.0f;
        for (int i = 0; i < _ocjeneRjesenja.getTrenutno(); i++)
        {
            prosjek += _ocjeneRjesenja.getElement1(i);
        }
        if (prosjek > 0)
            return prosjek /= _ocjeneRjesenja.getTrenutno();
        return prosjek;
    }
    friend ostream& operator << (ostream& COUT, const Pitanje& obj)
    {
        COUT << "Sadrzaj pitanja: " << obj.GetSadrzaj();
        COUT << "Ocjene: " << endl;
        for (int i = 0; i < obj._ocjeneRjesenja.getTrenutno(); i++)
        {
            COUT << "Ocjena: " << obj._ocjeneRjesenja.getElement1(i) << " Datum: " << *obj._ocjeneRjesenja.getElement2(i) << endl;;
        }

        COUT << "Prosjecna ocjena pitanja: " << obj.getProsjecnaOcjenaPitanja() << endl;
        return COUT;
    }
    bool AddOcjena(int ocjena, Datum& date)
    {
        /*svako pitanje moze imati vise ocjena tj. razlicita rjesenja/odgovori se mogu postaviti u vise navrata.
         - razmak izmedju postavljanja dva rjesenja mora biti najmanje 3 dana
         - nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)*/
        if (_ocjeneRjesenja.getTrenutno() == 0)
        {
            _ocjeneRjesenja.AddElement(ocjena, &date);
            return true;
        }
        auto checkDatum = [this, &date]()
        {
            for (int i = 0; i < _ocjeneRjesenja.getTrenutno(); i++)
            {
                if (*_ocjeneRjesenja.getElement2(i) - date < 3)
                    return false;
            }
            return true;
        };
        if (checkDatum())
        {
            _ocjeneRjesenja.AddElement(ocjena, &date);
            return true;
        }
        return false;
    }
};
bool operator == (const Pitanje& p1, const Pitanje& p2)
{
    if (strcmp(p1.GetSadrzaj(), p2.GetSadrzaj()) != 0)
        return false;
    if (const_cast<Pitanje&>(p1).GetOcjene().getTrenutno() != const_cast<Pitanje&>(p2).GetOcjene().getTrenutno())
        return false;
    for (int i = 0; i < const_cast<Pitanje&>(p1).GetOcjene().getTrenutno(); i++)
    {
        if (const_cast<Pitanje&>(p1).GetOcjene().getElement1(i) != const_cast<Pitanje&>(p2).GetOcjene().getElement1(i) || *const_cast<Pitanje&>(p1).GetOcjene().getElement2(i) != *const_cast<Pitanje&>(p2).GetOcjene().getElement2(i))
            return false;
    }
    return true;
}
bool operator != (const Pitanje& p1, const Pitanje& p2)
{
    return !(p1 == p2);
}
class Ispit {
    Predmet _predmet;
    //string se odnosi na napomenu/zapazanje nastavnika
    Kolekcija<Pitanje, string> _pitanjaOdgovori;
public:
    Ispit(Predmet predmet = UIT)
    {
        _predmet = predmet;
    }
    Ispit(Predmet predmet, Pitanje pitanje, string napomena)
    {
        _predmet = predmet;
        _pitanjaOdgovori.AddElement(pitanje, napomena);
    }
    Ispit(const Ispit& obj)
        :_pitanjaOdgovori(obj._pitanjaOdgovori)
    {
        _predmet = obj.GetPredmet();
    }
    Ispit(Ispit&& obj)noexcept
        :_predmet(move(obj._predmet)), _pitanjaOdgovori(move(obj._pitanjaOdgovori))
    {

    }
    Ispit& operator= (const Ispit& obj)
    {
        if (this == &obj)
            return *this;
        _predmet = obj.GetPredmet();
        _pitanjaOdgovori = obj._pitanjaOdgovori;
        return *this;
    }
    Kolekcija<Pitanje, string>& GetPitanjaOdgovore() { return _pitanjaOdgovori; }
    Predmet GetPredmet() const { return _predmet; }

    friend ostream& operator<< (ostream& COUT, const Ispit& obj) {
        COUT << obj._predmet << endl;
        for (size_t i = 0; i < obj._pitanjaOdgovori.getTrenutno(); i++)
            COUT << obj._pitanjaOdgovori;
        return COUT;
    }
    float prosjekSvihPitanja()const
    {
        float prosjek = 0.0f;
        for (int i = 0; i < _pitanjaOdgovori.getTrenutno(); i++)
        {
            prosjek += _pitanjaOdgovori.getElement1(i).getProsjecnaOcjenaPitanja();
        }
        if (prosjek > 0)
            return prosjek /= _pitanjaOdgovori.getTrenutno();
        return prosjek;
    }
};
bool operator == (const Ispit& i1, const Ispit& i2)
{
    if (i1.GetPredmet() != i2.GetPredmet())
        return false;
    if (const_cast<Ispit&>(i1).GetPitanjaOdgovore().getTrenutno() != const_cast<Ispit&>(i2).GetPitanjaOdgovore().getTrenutno())
        return false;
    for (int i = 0; i < const_cast<Ispit&>(i1).GetPitanjaOdgovore().getTrenutno(); i++)
    {
        if (const_cast<Ispit&>(i1).GetPitanjaOdgovore().getElement1(i) != const_cast<Ispit&>(i2).GetPitanjaOdgovore().getElement1(i) || const_cast<Ispit&>(i1).GetPitanjaOdgovore().getElement2(i) != const_cast<Ispit&>(i2).GetPitanjaOdgovore().getElement2(i))
            return false;
    }
    return true;
}
bool operator != (const Ispit& i1, const Ispit& i2)
{
    return !(i1 == i2);
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
        : _emailAdresa(move(obj._emailAdresa)), _lozinka(move(obj._lozinka))
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
    virtual ~Korisnik()
    {
        delete[] _imePrezime;
        _imePrezime = nullptr;
    }
    string GetEmail() const { return _emailAdresa; }
    string GetLozinka() const { return _lozinka; }
    char* GetImePrezime() const { return _imePrezime; }
    virtual void Info() = 0;
    friend ostream& operator <<(ostream& COUT, const Korisnik& obj)
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
class Kandidat : public Korisnik {
    vector<Ispit*> _polozeniPredmeti;
    void posaljiEmail(Pitanje pitanje, int ocjena, float prosjekOcjena, float ukupniProsjek)const
    {
        /*nakon evidentiranja ocjene na bilo kojem odgovoru, kandidatu se salje email sa porukom:
          FROM:info@kursevi.ba
          TO: emailKorisnika
          Postovani ime i prezime, evidentirana vam je ocjena X za odgovor na pitanje Y. Dosadasnji uspjeh (prosjek ocjena)
          za pitanje Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim predmetima iznosi Z.
          Pozdrav.
          EDUTeam.
          slanje email poruka implemenitrati koristeci zasebne thread-ove.
          */
        m.lock();
        cout << "FROM:info@kursevi.ba" << endl;
        cout << "TO: " << this->GetEmail() << endl;
        cout << "Postovani" << this->GetImePrezime() << " , evidentirana vam je ocjena " << ocjena << " za odgovor na pitanje " << pitanje.GetSadrzaj() << ".  Dosadasnji uspjeh (prosjek ocjena) za " << pitanje.GetSadrzaj() << " iznosi " << prosjekOcjena << " , a ukupni uspjeh (prosjek ocjena) na svim predmetima iznosi " << ukupniProsjek << " ." << endl;
        cout << "Pozdrav." << endl;
        cout << "EDUTeam." << endl;
        m.unlock();
    }
public:

    virtual void Info()override
    {

    }
    Kandidat()
        :Korisnik()
    {

    }
    Kandidat(const char* imePrezime, string emailAdresa, string lozinka)
        : Korisnik(imePrezime, emailAdresa, lozinka)
    {

    }
    Kandidat(const Kandidat& obj)
        :Korisnik(obj), _polozeniPredmeti(obj._polozeniPredmeti)
    {

    }
    Kandidat(Kandidat&& obj)noexcept
        :Korisnik(move(obj)), _polozeniPredmeti(move(obj._polozeniPredmeti))
    {

    }
    Kandidat& operator = (const Kandidat& obj)
    {
        if (this == &obj)
            return *this;
        static_cast<Korisnik&>(*this) = obj;
        _polozeniPredmeti = obj._polozeniPredmeti;
        return *this;
    }
    virtual ~Kandidat() override
    {
        for (size_t i = 0; i < _polozeniPredmeti.size(); i++)
            delete _polozeniPredmeti[i];
    }
    friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
        COUT << static_cast<Korisnik&>(obj) << endl;
        for (size_t i = 0; i < obj._polozeniPredmeti.size(); i++)
            COUT << *obj._polozeniPredmeti[i];
        return COUT;
    }
    vector<Ispit*>& GetPolozeniPredmeti() { return _polozeniPredmeti; }
    bool AddPitanje(Predmet predmet, Pitanje& pitanje, string napomena = " ")
    {
        /*
    svi odgovori na nivou jednog predmeta (PRI, PRII... ) se evidentiraju unutar istog objekta tipa Ispit tj. pripadajuceg objekta
    tipa Pitanje, tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih atributa) odgovora na nivou jednog predmeta,
    - dodavanje odgovora za visi predmet ako prethodni predmet nema evidentirana najmanje 3 pitanja ili nema prosjecnu ocjenu
    svih pitanja vecu od 3.5
    */
        if (_polozeniPredmeti.size() == 0)
        {
            _polozeniPredmeti.push_back(new Ispit(predmet, pitanje, napomena));
            float prosjek = pitanje.getProsjecnaOcjenaPitanja();
            float ukupniProsjek = 0.0f;
            int ocjena = 0;
            for (auto ispit : _polozeniPredmeti)
            {
                ukupniProsjek = ispit->prosjekSvihPitanja();
                for (int j = 0; j < ispit->GetPitanjaOdgovore().getTrenutno(); j++)
                {
                    for (int k = 0; k < ispit->GetPitanjaOdgovore().getElement1(j).GetOcjene().getTrenutno(); k++)
                    {
                        ocjena = ispit->GetPitanjaOdgovore().getElement1(j).GetOcjene().getElement1(k);
                    }
                }
            }
            thread t1(&Kandidat::posaljiEmail, this, pitanje, ocjena, prosjek, ukupniProsjek);
            t1.join();
            return true;

        }
        auto checkDuplikat = [this, &pitanje, &napomena]()
        {
            for (auto ispit : _polozeniPredmeti)
            {
                for (int i = 0; i < ispit->GetPitanjaOdgovore().getTrenutno(); i++)
                {
                    if (ispit->GetPitanjaOdgovore().getElement1(i) == pitanje && ispit->GetPitanjaOdgovore().getElement2(i) == napomena)
                        return false;
                }
            }
            return true;
        };
        auto checkPrethodni = [this, &predmet]()
        {
            Predmet prethodni = static_cast<Predmet>(predmet - 1);
            for (auto ispit : _polozeniPredmeti)
            {
                if (ispit->GetPredmet() == prethodni)
                {
                    if (ispit->GetPitanjaOdgovore().getTrenutno() < 3)
                        return false;
                    if (ispit->prosjekSvihPitanja() < 3.5f)
                        return false;
                }
            }
            return true;
        };
        if (checkDuplikat() && checkPrethodni())
        {
            int trenutno = _polozeniPredmeti.size();
            _polozeniPredmeti.push_back(new Ispit(predmet, pitanje, napomena));
            float prosjek = pitanje.getProsjecnaOcjenaPitanja();
            float ukupniProsjek = 0.0f;
            int ocjena = 0;
            for (auto ispit : _polozeniPredmeti)
            {
                ukupniProsjek = ispit->prosjekSvihPitanja();
                for (int j = 0; j < ispit->GetPitanjaOdgovore().getTrenutno(); j++)
                {
                    for (int k = 0; k < ispit->GetPitanjaOdgovore().getElement1(j).GetOcjene().getTrenutno(); k++)
                    {
                        ocjena = ispit->GetPitanjaOdgovore().getElement1(j).GetOcjene().getElement1(k);
                    }
                }
            }
            thread t1(&Kandidat::posaljiEmail, this, pitanje, ocjena, prosjek, ukupniProsjek);
            t1.join();
            return true;
        }
        return false;
    }
    int operator () (string trazenaRijec)const
    {
        regex pretrazi(trazenaRijec);
        auto pocetak = sregex_iterator(trazenaRijec.begin(), trazenaRijec.end(), pretrazi);
        auto kraj = sregex_iterator();
        return distance(pocetak, kraj);
    }
};

const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Pojasnite na koji nacin tip nasljedjivanja (javno, zasticeno, privatno) utjece na dostupnost atributa bazne klase unutar izvedene klase?\n";
    return "Javno nasljedjivanje bazne klase omogucava da su svi njeni atributi metode i funkcije iz javnog dijela dostupne i kod izvedene klase, dok takodjer svi protected clanovi ce takodjer biti dostupni u izvedenoj klasi; Zasticeno nasljedjivanje omogucava da se svi zasticeni atribtui metode i fije mogu koristiti i biti ce zasticeni u izvedenoj klasi, ali ne i izvan nje; Privatno nasljedjivanje cesto nema smisla jer nista nece biti dostupno izvedenoj klasi.";
}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Ukratko pojasnite primjenu funkcija seekg i tellg?\n";
    return "Seekg funkcija kada joj se proslijedi parametar ce da pomjeri nas kursor na odredjenu lokaciju u fajlu. Dok tellg ce nas kursor da pomjeri na krajnju lokaciju u fajlu;";
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

    Kolekcija<int, int> kolekcija1(BEZ_DUPLIKATA);
    for (int i = 0; i < kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    cout << kolekcija1 << endl;

    try {
        /*ukoliko dupliranje vrijednosti nije dozvoljeno (BEZ_DUPLIKATA)
        metoda AddElement baca izuzetak u slucaju da se pokusa dodati par sa vrijednostima
        identicnim postojecem paru unutar kolekcije */
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << crt << "Greska -> " << err.what() << crt;
    }
    cout << kolekcija1 << crt;

    Kolekcija<int, int> kolekcija2;
    kolekcija2 = kolekcija1;
    cout << kolekcija2 << crt;

    /*na osnovu vrijednosti tip T1 (u primjeru vrijednost 1) pronalazi i vraca iz kolekcije sve parove
    koji kao vrijednost T1 imaju proslijedjenu vrijednost*/
    cout << kolekcija1[1] << crt;
    /* npr. ako unutar kolekcije postoje parovi:
    0 0
    1 9
    1 1
    2 2
    3 3
    ispis dobijenih/vracenih vrijednosti ce biti sljedeci:
    1 9
    1 1
    */

    Pitanje sortiranjeNiza("Navedite algoritme za sortiranje clanova niza."),
        dinamickaMemorija("Navedite pristupe za upravljanje dinamickom memorijom."),
        visenitnoProgramiranje("Na koji se sve nacin moze koristiti veci broj niti tokom izvrsenja programa."),
        regex("Navedite par primjera regex validacije podataka.");

    /*svako pitanje moze imati vise ocjena tj. razlicita rjesenja/odgovori se mogu postaviti u vise navrata.
        -   razmak izmedju postavljanja dva rjesenja mora biti najmanje 3 dana
        -   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)
    */
    if (sortiranjeNiza.AddOcjena(1, datum19062020))
        cout << "Ocjena evidentirana!" << endl;
    if (!sortiranjeNiza.AddOcjena(5, datum20062020))
        cout << "Ocjena NIJE evidentirana!" << endl;
    if (sortiranjeNiza.AddOcjena(5, datum30062020))
        cout << "Ocjena evidentirana!" << endl;

    // ispisuje sadrzaj/tekst pitanja, ocjene (zajedno sa datumom) i prosjecnu ocjenu za sve odgovore/rjesenja
     // ukoliko pitanje nema niti jednu ocjenu prosjecna treba biti 0
    cout << sortiranjeNiza << endl;

    if (ValidirajLozinku("*2gT2x"))
        cout << "Lozinka validna" << endl;
    if (ValidirajLozinku("*7aT2x"))
        cout << "Lozinka validna" << endl;
    if (ValidirajLozinku("_6gU9z"))
        cout << "Lozinka validna" << endl;
    if (ValidirajLozinku("*3aB1y"))
        cout << "Lozinka validna" << endl;
    if (ValidirajLozinku("*1a T2l"))
        cout << "Lozinka validna" << endl;
    if (!ValidirajLozinku("-1a T2l"))
        cout << "Lozinka NIJE validna" << endl;

    /*
    za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi 6 ili 7 znakova postujuci sljedeca pravila:
    -   pocinje sa znak * (zvjezdica) ili _ (donja crtica), nakon cega slijedi
    -   broj, nakon cega slijedi
    -   malo slovo, nakon cega slijedi
    -   razmak koji NIJE OBAVEZAN, nakon cega slijedi
    -   veliko slovo, nakon cega slijedi
    -   broj, nakon cega slijedi
    -   malo slovo.
    za provjeru validnosti lozinke koristiti globalnu funkciju ValidirajLozinku, a unutar nje regex metode.
    validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u slucaju da nije validna
    postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
    */

    Korisnik* jasmin = new Kandidat("Jasmin Azemovic", "jasmin@kursevi.ba", "*2gT2x");
    Korisnik* adel = new Kandidat("Adel Handzic", "adel@edu.kursevi.ba", "_6gU9z");
    Korisnik* lozinkaNijeValidna = new Kandidat("John Doe", "john.doe@google.com", "johndoe");

    /*
    svi odgovori na nivou jednog predmeta (PRI, PRII... ) se evidentiraju unutar istog objekta tipa Ispit tj. pripadajuceg objekta tipa Pitanje,
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih atributa) odgovora na nivou jednog predmeta,
    - dodavanje odgovora za visi predmet ako prethodni predmet nema evidentirana najmanje 3 pitanja ili nema prosjecnu ocjenu svih pitanja vecu od 3.5
    (onemoguciti dodavanje pitanja za PRII ako ne postoje najmanje 3 pitanja za predmet PRI ili njihov prosjek nije veci od 3.5)
    funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */

    //ukoliko je potrebno, doraditi klase da nacin da omoguce izvrsenje naredne linije koda
    Kandidat* jasminPolaznik = dynamic_cast<Kandidat*>(jasmin);

    if (jasminPolaznik != nullptr) {
        if (jasminPolaznik->AddPitanje(PRI, dinamickaMemorija, "nedostaje operator delete"))
            cout << "Pitanje uspjesno dodano!" << crt;
        //ne treba dodati visenitnoProgramiranje jer ne postoje evidentirana 3 pitanja za predmet PRI
        if (!jasminPolaznik->AddPitanje(PRII, visenitnoProgramiranje))
            cout << "Pitanje NIJE uspjesno dodano!" << crt;
        if (jasminPolaznik->AddPitanje(PRI, visenitnoProgramiranje))
            cout << "Pitanje uspjesno dodano!" << crt;
        if (jasminPolaznik->AddPitanje(PRI, regex, "unutar posljednjeg dijela nedostaje opis glavnih operatora"))
            cout << "Pitanje uspjesno dodano!" << crt;
        if (jasminPolaznik->AddPitanje(PRI, sortiranjeNiza))
            cout << "Pitanje uspjesno dodano!" << crt;
        //ne treba dodati sortiranjeNiza jer je vec dodana za predmet PRI
        if (!jasminPolaznik->AddPitanje(PRI, sortiranjeNiza))
            cout << "Pitanje NIJE uspjesno dodano!" << crt;

        //ispisuje sve dostupne podatke o kandidatu
        cout << *jasminPolaznik << crt;

        //vraca broj ponavljanja odredjene rijeci unutar napomena nastalih tokom polaganja ispita.
        int brojPonavljanja = (*jasminPolaznik)("nedostaje");
        cout << "Rijec nedostaje se ponavlja " << brojPonavljanja << " puta." << endl;

    }


    /*nakon evidentiranja ocjene na bilo kojem odgovoru, kandidatu se salje email sa porukom:
    FROM:info@kursevi.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirana vam je ocjena X za odgovor na pitanje Y. Dosadasnji uspjeh (prosjek ocjena)
    za pitanje Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim predmetima iznosi Z.
    Pozdrav.
    EDUTeam.
    slanje email poruka implemenitrati koristeci zasebne thread-ove.
    */

    delete jasmin;
    delete adel;
    delete lozinkaNijeValidna;

    cin.get();
    system("pause>0");
}
