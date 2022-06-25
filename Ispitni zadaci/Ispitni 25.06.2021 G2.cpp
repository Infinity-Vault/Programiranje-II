#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <mutex>
#include <thread>

using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. ATRIBUTI, NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U, "
"OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE "
"POMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
enum Dupliranje { BEZ_DUPLIKATA, SA_DUPLIKATIMA };

char* GetNizKaraktera(const char* sadrzaj)
{
    if (sadrzaj == nullptr)
        return nullptr;

    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);

    return temp;
}

template<class T1, class T2, int max = 15>
class Kolekcija {
    T1 _elementi1[max];
    T2 _elementi2[max];
    int* _trenutno;
    Dupliranje _dupliranje;
public:
    Kolekcija(Dupliranje dupliranje = SA_DUPLIKATIMA)
        : _trenutno(new int(0)), _dupliranje(dupliranje)
    {

    }

    Kolekcija(const Kolekcija& obj)
        : _trenutno(new int(*obj._trenutno)), _dupliranje(obj._dupliranje)
    {
        for (int i = 0; i < *obj._trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];
            _elementi2[i] = obj._elementi2[i];
        }
    }

    ~Kolekcija()
    {
        delete _trenutno; _trenutno = nullptr;
    }

    Kolekcija& operator= (const Kolekcija& obj)
    {
        if (this == &obj)
            return *this;

        _trenutno = new int(*obj._trenutno);
        _dupliranje = obj._dupliranje;

        for (int i = 0; i < *obj._trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];
            _elementi2[i] = obj._elementi2[i];
        }

        return *this;
    }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj)
    {
        for (size_t i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;

        return COUT;
    }
    Kolekcija<T1, T2> operator() (int from, int to)
    {
        if (from < 0 || to < 0 || from >= *_trenutno || to >= *_trenutno)
            throw exception("Uneseni opseg nije validan!");

        Kolekcija<T1, T2> pom;

        for (int i = from; i <= to; i++)
        {
            pom.AddElement(_elementi1[i], _elementi2[i]);
        }

        return pom;
    }

    T1 getElement1(int lokacija) const { return _elementi1[lokacija]; }
    T2 getElement2(int lokacija) const { return _elementi2[lokacija]; }
    int getTrenutno() const { return *_trenutno; }

    void AddElement(T1 el1, T2 el2)
    {
        if (*_trenutno == max)
            throw exception("Prekoracen opseg kolekcije!");

        if (_dupliranje == Dupliranje::BEZ_DUPLIKATA)
        {
            for (int i = 0; i < *_trenutno; i++)
            {
                if (_elementi1[i] == el1 && _elementi2[i] == el2)
                {
                    throw exception("Nije moguce dodavanje duplih elemenata");
                }
            }
        }

        _elementi1[*_trenutno] = el1;
        _elementi2[*_trenutno] = el2;
        (*_trenutno)++;
    }
};

class Datum
{
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000)
        : _dan(new int(dan)), _mjesec(new int(mjesec)), _godina(new int(godina))
    {

    }

    Datum(const Datum& obj)
        : _dan(new int(*obj._dan)), _mjesec(new int(*obj._mjesec)), _godina(new int(*obj._godina))
    {

    }

    Datum& operator= (const Datum& obj)
    {
        if (this == &obj)
            return *this;

        if (_dan == nullptr)
            _dan = new int;
        *_dan = *obj._dan;

        if (_mjesec == nullptr)
            _mjesec = new int;
        *_mjesec = *obj._mjesec;

        if (_godina == nullptr)
            _godina = new int;
        *_godina = *obj._godina;

        return *this;
    }

    ~Datum()
    {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }

    friend ostream& operator<< (ostream& COUT, const Datum& obj)
    {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
    friend bool operator==(const Datum& d1, const Datum& d2)
    {
        return *d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina;
    }
    friend bool operator!=(const Datum& d1, const Datum& d2) { return !(d1 == d2); }
    friend bool operator>(const Datum& d1, const Datum& d2) { return (d1.Dani() > d2.Dani()); }
    friend bool operator<(const Datum& d1, const Datum& d2) { return (d1.Dani() < d2.Dani()); }

    int Dani() const { return *_godina * 365 + *_mjesec * 30 + *_dan; }
};

class Tehnika
{
    char* _naziv;
    //int se odnosi na ocjenu u opsegu od 1 – 5, a datum na momenat postizanja ocjene
    Kolekcija<Datum, int>* _ocjene;
public:
    Tehnika(const char* naziv = "", Datum datum = Datum(), int ocjena = 0)
        : _naziv(GetNizKaraktera(naziv)), _ocjene(new Kolekcija<Datum, int>())
    {
        if (ocjena > 0)
            AddOcjena(&datum, ocjena);
    }

    Tehnika(const Tehnika& obj)
        : _naziv(GetNizKaraktera(obj._naziv)), _ocjene(new Kolekcija<Datum, int>(*obj._ocjene))
    {

    }

    ~Tehnika()
    {
        delete[] _naziv; _naziv = nullptr;
        delete _ocjene; _ocjene = nullptr;
    }

    char* GetNaziv() const { return _naziv; }
    Kolekcija<Datum, int> GetOcjene() const { return *_ocjene; }

    void AddOcjena(const Datum* datum, int ocjena)
    {
        _ocjene->AddElement(*datum, ocjena);
    }
    float ProsjecnaOcjena() const
    {
        int prosjek = 0.0f;

        if (_ocjene->getTrenutno() == 0)
        {
            return prosjek;
        }

        for (int i = 0; i < _ocjene->getTrenutno(); i++)
        {
            prosjek += _ocjene->getElement2(i);
        }

        return prosjek / _ocjene->getTrenutno();
    }

    friend ostream& operator<<(ostream& COUT, const Tehnika& t1)
    {
        COUT << "Naziv Tehnike: " << t1.GetNaziv() << endl;
        COUT << "Ocjene za tehniku i datum: " << endl;

        for (int i = 0; i < t1._ocjene->getTrenutno(); i++)
        {
            COUT << t1._ocjene->getElement1(i) << " " << t1._ocjene->getElement2(i) << endl;
        }

        COUT << "Prosjecna ocjena za tehniku iznosi: " << t1.ProsjecnaOcjena() << endl;
        return COUT;
    }
    friend bool operator==(const Tehnika& tehnika1, const Tehnika& tehnika2)
    {
        if (strcmp(tehnika1.GetNaziv(), tehnika2.GetNaziv()) != 0)
            return false;
        if (tehnika1.GetOcjene().getTrenutno() != tehnika2.GetOcjene().getTrenutno())
            return false;

        for (int i = 0; i < tehnika1.GetOcjene().getTrenutno(); i++)
        {
            if (tehnika1.GetOcjene().getElement1(i) != tehnika2.GetOcjene().getElement1(i))
                return false;
            if (tehnika1.GetOcjene().getElement2(i) != tehnika2.GetOcjene().getElement2(i))
                return false;
        }

        return true;
    }
};

class Polaganje
{
    Pojas _pojas;
    //string se odnosi na napomenu o polozenoj tehnici
    Kolekcija<Tehnika*, string> _polozeneTehnike;
public:
    Polaganje(Pojas pojas = BIJELI)
        : _pojas(pojas)
    {

    }

    Polaganje(const Polaganje& obj)
        : _pojas(obj._pojas), _polozeneTehnike(obj._polozeneTehnike)
    {

    }

    Polaganje(const Pojas pojas, const Tehnika& tehnika, const char* napomena)
        : _pojas(pojas)
    {
        _polozeneTehnike.AddElement(new Tehnika(tehnika), GetNizKaraktera(napomena));
    }

    friend ostream& operator<< (ostream& COUT, const Polaganje& obj)
    {
        COUT << "Ispis pojasa " << obj._pojas << " :" << endl;
        for (int i = 0; i < obj._polozeneTehnike.getTrenutno(); i++)
        {
            COUT << *obj._polozeneTehnike.getElement1(i) << "  " << obj._polozeneTehnike.getElement2(i) << endl;
        }

        return COUT;
    }

    Kolekcija<Tehnika*, string>& GetTehnike() { return _polozeneTehnike; }
    Pojas GetPojas() const { return _pojas; }

    float Prosjek() const
    {
        float prosjek = 0.0f;

        for (int i = 0; i < _polozeneTehnike.getTrenutno(); i++)
        {
            prosjek += _polozeneTehnike.getElement1(i)->ProsjecnaOcjena();
        }

        if (prosjek == 0.0f)
            return prosjek;

        return prosjek / _polozeneTehnike.getTrenutno();
    }
    void DodajTehniku(const Tehnika& nazivTehnike, const char* napomena)
    {
        _polozeneTehnike.AddElement(new Tehnika(nazivTehnike), GetNizKaraktera(napomena));
    }
};

bool ValidirajEmail(string email)
{/*
    OD 6 MAILOVA KOJE IMAJU U MAIN 4 TREBAJU DA SU VALIDNA A 2 DA NISU VALIDNA
    KOD MN 3 PRODU DA SU VALIDNA  A 4 NECE, A KOD NE VALIDNIH OBA PROLAZE I KAZU DA NISU VALIDNI
    */
    /*
      email adresa treba biti u sljedecem formatu 3_ime.prezime@karate.ba tj. zadovoljavati sljedeća pravila:
      - poceti sa jednim brojem nakon cega slijedi donja crtica
      - u imenu posjedovati najmanje 3 karaktera
      - izmedju imena i prezimena moze biti tacka ili donja crtica ili nista od navedenog
      - u prezimenu posjedovati najmanje 3 karaktera
      - znak @
      - domenu karate.ba ili edu.karate.ba. Pored drzavne(.ba), dozvoljene su oznake .com i .org.
      za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
      validacija email adrese ce se vrsiti unutar konstruktora klase KaratePolaznik, a u slucaju da nije validna
      postaviti je na defaultnu adresu: notSet@edu.karate.ba
      */
      //string formatMail = "\\d{1}\\_[\\w][\\._]?[\\w]+@(karate.ba|karate.com|karate.org|edu.karate.ba|edu.karate.com|edu.karate.org)";
      //string mail = "\\d{1}[_][\\w][._]?[\\w]+@(karate.ba|karate.com|karate.org|edu.karate.ba|edu.karate.com|edu.karate.org)";
      //.svaki karakter 
    string mail = "\\d_\\w{3,}[._]?\\w{3,}@(karate.ba|karate.com|karate.org|edu.karate.ba|edu.karate.com|edu.karate.org)";
    regex provjera(mail);
    return regex_match(email, provjera);
}

mutex mtx;

class KaratePolaznik
{
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Polaganje> _polozeniPojasevi;

    void SaljiMail(Pojas pojas, const Tehnika& nazivTehnike, float prosjekOcjena, float ukupniUspjeh)
    {
        /*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje email sa porukom:
        FROM:info@karate.ba
        TO: emailKorisnika
        Postovani ime i prezime, evidentirana vam je thenika X za Y pojas. Dosadasnji uspjeh (prosjek ocjena)
        na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
        Pozdrav.
        KARATE Team.
        ukoliko je prosjek na nivou tog pojasa veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.D na X pojasu".
        slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
        */
        mtx.lock();
        cout << "FROM:info@karate.ba" << endl;
        cout << "TO: " << this->GetEmail() << endl;
        cout << "Postovani " << this->GetImePrezime() << " evidentirana Vam je tehnika " << nazivTehnike.GetNaziv() << " za " << pojas << ". Dosadasnji uspjeh (prosjek ocjena) na pojasu " << pojas << " iznosi " << prosjekOcjena << " a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi " << ukupniUspjeh << "." << endl;
        cout << "Pozdrav" << endl;
        cout << "KARATE Team" << endl;
        mtx.unlock();
    }
public:
    KaratePolaznik(const char* imePrezime, string emailAdresa, string brojTelefona)
        : _imePrezime(GetNizKaraktera(imePrezime)), _emailAdresa(emailAdresa), _brojTelefona(brojTelefona)
    {

    }

    KaratePolaznik(const KaratePolaznik& obj)
        : _imePrezime(GetNizKaraktera(obj._imePrezime)), _emailAdresa(obj._emailAdresa), _brojTelefona(obj._brojTelefona)
    {

    }

    ~KaratePolaznik()
    {
        delete[] _imePrezime; _imePrezime = nullptr;
    }

    friend ostream& operator<< (ostream& COUT, const KaratePolaznik& obj)
    {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;

        for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
            COUT << obj._polozeniPojasevi[i];

        return COUT;
    }

    Kolekcija<Tehnika*, float>operator()(Datum d1, Datum d2)
    {
        //vraca kolekciju tehnika koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
       //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na tehnici

        Kolekcija<Tehnika*, float>temp;

        for (auto& polozeno : _polozeniPojasevi)
        {
            for (int i = 0; i < polozeno.GetTehnike().getTrenutno(); i++)
            {
                for (int j = 0; j < polozeno.GetTehnike().getElement1(i)->GetOcjene().getTrenutno(); j++)
                {
                    if (polozeno.GetTehnike().getElement1(i)->GetOcjene().getElement1(j) > d1 && d2 > polozeno.GetTehnike().getElement1(i)->GetOcjene().getElement1(j))
                    {
                        temp.AddElement(polozeno.GetTehnike().getElement1(i), 1.0f);
                    }
                }
            }
        }

        return temp;
    }

    vector<Polaganje>& GetPolozeniPojasevi() { return _polozeniPojasevi; }
    string GetEmail() const { return _emailAdresa; }
    string GetBrojTelefona() const { return _brojTelefona; }
    char* GetImePrezime() const { return _imePrezime; }

    bool AddTehniku(Pojas pojas, const Tehnika& novaTehnika, const string& napomena)
    {
        /*
        svi kandidati podrazumijevano imaju BIJELI pojas (za njega se ne dodaju tehnike)
        uspjeh se dodaje za svaku tehniku na nivou pojasa (ZUTI, ZELENI ... ).
        tom prilikom onemoguciti:
        - dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
        - dodavanje tehnika za vise pojaseve ako ne postoji najmanje jedna tehnika za nizi pojas (ne mozemo dodati tehniku za NARANDZASTI ako ne postoji niti jedna tehnika za ZUTI pojas)
        - dodavanje tehnika kod kojih je prosjecna ocjena manja od 3.5
        funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
        */

        // - dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
        auto isIsti = [this, pojas, novaTehnika]
        {
            for (auto& polaganje : _polozeniPojasevi)
            {
                if (polaganje.GetPojas() == pojas)
                {
                    for (int i = 0; i < polaganje.GetTehnike().getTrenutno(); i++)
                    {
                        if (*polaganje.GetTehnike().getElement1(i) == novaTehnika)
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        };

        // - dodavanje tehnika kod kojih je prosjecna ocjena manja od 3.5
        auto provjeriProsjek = [this, novaTehnika]
        {
            if (novaTehnika.ProsjecnaOcjena() < 3.5f)
                return false;
            return true;
        };

        //dodavanje tehnika za vise pojaseve ako ne postoji najmanje jedna tehnika za nizi pojas (ne mozemo dodati tehniku za NARANDZASTI ako ne postoji niti jedna tehnika za ZUTI pojas)
        auto prethodniPojas = [this, pojas, novaTehnika]
        {
            if (pojas <= 1)
                return true;

            auto prethodni = static_cast<Pojas>(pojas - 1);

            for (auto& polozeni : _polozeniPojasevi)
            {
                if (polozeni.GetPojas() == prethodni)
                    return true;
            }

            return false;
        };

        // Pozivamo svaku lambdu kako bi testirali sve trazeno
        if (!isIsti() || !provjeriProsjek() || !prethodniPojas())
            return false;

        for (auto& polozeno : _polozeniPojasevi)
        {
            if (polozeno.GetPojas() == pojas)
            {
                polozeno.DodajTehniku(novaTehnika, napomena.c_str());

                thread worker(&KaratePolaznik::SaljiMail, this, pojas, novaTehnika, novaTehnika.ProsjecnaOcjena(), polozeno.Prosjek());
                worker.join();

                return true;
            }
        }

        Polaganje p(pojas);
        p.DodajTehniku(novaTehnika, napomena.c_str());
        _polozeniPojasevi.push_back(p);

        thread worker2(&KaratePolaznik::SaljiMail, this, pojas, novaTehnika, novaTehnika.ProsjecnaOcjena(), p.Prosjek());
        worker2.join();

        return true;
    }
};
const char* GetOdgovorNaPrvoPitanje()
{
    cout << "Pitanje -> Za sta se koriste modovi ios::ate i ios::trunc ?\n";
    return "Odgovor -> ios::trunc komandom brisemo stare podatke iz fajla kad se otvori fajl, a ios::ate(at end) postavlja stream poziciju na kraj fajla kada se otvori fajl";
}

const char* GetOdgovorNaDrugoPitanje()
{
    cout << "Pitanje -> Pojasniti ulogu i način koristenja iteratora?\n";
    return "Odgovor -> Iteratori su objekti koji nam omogucavaju da prodjemo kroz elemente nekog niza ili kolekcije i da im preko njega pristupimo pravilno. Poznajemo ostram operator preko kojeg mozemo ispitati neke podatke sregex iterator preko kojeg mozemo prebrojati koliko rijeci ima u nekom tekstu";
}

int main() {

    cout << PORUKA;
    cin.get();

    cout << GetOdgovorNaPrvoPitanje() << endl;
    cin.get();
    cout << GetOdgovorNaDrugoPitanje() << endl;
    cin.get();

    Datum temp,
        datum19062021(19, 6, 2021),
        datum20062021(20, 6, 2021),
        datum30062021(30, 6, 2021),
        datum05072021(5, 7, 2021);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(BEZ_DUPLIKATA);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    try {
        //ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
        //takodjer, izuzetak se baca i u slucaju da se prekoraci maksimalan broj elemenata
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }
    cout << kolekcija1 << crt;

    /*objekat kolekcija2 ce biti inicijalizovan elementima koji se u objektu kolekcija1 nalaze na lokacijama 1 - 4
    ukljucujuci i te lokacije. u konkretnom primjeru to ce biti parovi sa vrijednostima: 1 1 2 2 3 3 4 4*/
    Kolekcija<int, int> kolekcija2 = kolekcija1(1, 4);
    cout << kolekcija2 << crt;
    try {
        //primjeri u kojima opseg nije validan, te bi funkcija trebala baciti izuzetak
        Kolekcija<int, int> temp1 = kolekcija1(1, 14);//imamo 10 elemenata
        Kolekcija<int, int> temp2 = kolekcija1(-1, 8);//lokacija -1 ne postoji
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }
    //svaka tehnika moze imati vise ocjena i polaze se u vise navrata (istog ili drugog dana)
    //parametri: nazivTehnike, prva ocjena, datum polaganja
    Tehnika choku_zuki("choku_zuki", datum19062021, 5),
        gyaku_zuki("gyaku_zuki", datum20062021, 5),
        kizami_zuki("kizami_zuki", datum30062021, 5),
        oi_zuki("oi_zuki", datum05072021, 5);

    choku_zuki.AddOcjena(&datum05072021, 3);
    choku_zuki.AddOcjena(&datum05072021, 5);

    // ispisuje: naziv tehnike, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu za tu tehniku
    // ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0
    cout << choku_zuki << endl;

    /*
 //  email adresa treba biti u sljedecem formatu 3_ime.prezime@karate.ba tj. zadovoljavati sljedeća pravila:
 //  - poceti sa jednim brojem nakon cega slijedi donja crtica
 //  - u imenu posjedovati najmanje 3 karaktera
 //  - izmedju imena i prezimena moze biti tacka ili donja crtica ili nista od navedenog
 //  - u prezimenu posjedovati najmanje 3 karaktera
 //  - znak @
 //  - domenu karate.ba ili edu.karate.ba. Pored drzavne(.ba), dozvoljene su oznake .com i .org.
 //
 //  za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
 //  validacija email adrese ce se vrsiti unutar konstruktora klase KaratePolaznik, a u slucaju da nije validna
 //  postaviti je na defaultnu adresu: notSet@edu.karate.ba
 //  */
 //
    if (ValidirajEmail("2_ime.prezime@edu.karate.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("3_ime_prezime@karate.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("4_imeprezime@karate.com"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("8_imeprezime@karate.org"))
        cout << "Email validan" << crt;
    if (!ValidirajEmail("2ime.prezime@edu.karate.org"))
        cout << "Email NIJE validan" << crt;
    if (!ValidirajEmail("5_ime prezime@edu.karate.org"))
        cout << "Email NIJE validan" << crt;


    KaratePolaznik* jasmin = new KaratePolaznik("Jasmin Azemovic", "1_jasmin.azemovic@karate.ba", "033 281 172");
    KaratePolaznik* adel = new KaratePolaznik("Adel Handzic", "2_adel_handzic@edu.karate.ba", "033 281 170");
    KaratePolaznik* emailNotValid = new KaratePolaznik("Ime Prezime", "korisnik@karate.ru", "033 281 170");

    /*
    svi kandidati podrazumijevano imaju BIJELI pojas (za njega se ne dodaju tehnike)
    uspjeh se dodaje za svaku tehniku na nivou pojasa (ZUTI, ZELENI ... ).
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
    - dodavanje tehnika za vise pojaseve ako ne postoji najmanje jedna tehnika za nizi pojas (ne mozemo dodati tehniku za NARANDZASTI ako ne postoji niti jedna tehnika za ZUTI pojas)
    - dodavanje tehnika kod kojih je prosjecna ocjena manja od 3.5
    funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */

    //ne treba dodati kizami_zuki jer ne postoji niti jedna tehnika za ZUTI pojas
    if (jasmin->AddTehniku(NARANDZASTI, kizami_zuki, "Napomena 0"))
        cout << "Tehnika uspjesno dodan!" << crt;
    if (jasmin->AddTehniku(ZUTI, gyaku_zuki, "Napomena 1"))
        cout << "Tehnika uspjesno dodan!" << crt;
    if (jasmin->AddTehniku(ZUTI, kizami_zuki, "Napomena 2"))
        cout << "Tehnika uspjesno dodan!" << crt;
    if (jasmin->AddTehniku(ZUTI, oi_zuki, "Napomena 3"))
        cout << "Tehnika uspjesno dodan!" << crt;
    if (jasmin->AddTehniku(ZUTI, choku_zuki, "Napomena 4"))
        cout << "Tehnika uspjesno dodan!" << crt;
    //ne treba dodati choku_zuki jer je vec dodana za zuti pojas
    if (!jasmin->AddTehniku(ZUTI, choku_zuki, "Napomena 5"))
        cout << "Tehnika NIJE uspjesno dodana!" << crt;


    /*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje email sa porukom:
    FROM:info@karate.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirana vam je thenika X za Y pojas. Dosadasnji uspjeh (prosjek ocjena)
    na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
    Pozdrav.
    KARATE Team.
    ukoliko je prosjek na nivou tog pojasa veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.D na X pojasu".
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    cout << *jasmin << crt;

    //vraca kolekciju tehnika koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
    //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na tehnici
    Kolekcija<Tehnika*, float> jasminUspjeh = (*jasmin)(Datum(18, 06, 2021), Datum(21, 06, 2021));
    for (size_t i = 0; i < jasminUspjeh.getTrenutno(); i++)
        cout << *jasminUspjeh.getElement1(i) << " " << jasminUspjeh.getElement2(i) << crt;

    delete jasmin;
    delete adel;
    delete emailNotValid;

    cin.get();
    system("pause>0");
}