#include <iostream>
#include <vector>
#include <regex>
#include <mutex>
#include <string>
#include <thread>
#include <exception>

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
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };

char* GetNizKaraktera(const char* sadrzaj) {
    if (sadrzaj == nullptr)return nullptr;
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
    bool _omoguciDupliranje;
public:
    Kolekcija(bool omoguciDupliranje = true) {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _omoguciDupliranje = omoguciDupliranje;
        _trenutno = 0;
    }
    Kolekcija(const Kolekcija& obj)
    {
        _trenutno = obj._trenutno;
        _omoguciDupliranje = obj._omoguciDupliranje;
        _elementi1 = new T1[_trenutno];
        _elementi2 = new T2[_trenutno];
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];
            _elementi2[i] = obj._elementi2[i];
        }
    }
    Kolekcija(Kolekcija&& obj)
    {
        _trenutno = obj._trenutno;
        obj._trenutno = 0;
        _elementi1 = obj._elementi1;
        obj._elementi1 = nullptr;
        _elementi2 = obj._elementi2;
        obj._elementi2 = nullptr;
        _omoguciDupliranje = obj._omoguciDupliranje;

    }
    Kolekcija& operator=(const Kolekcija& obj)
    {
        if (this == &obj)
        {
            return *this;
        }
        delete[]_elementi1;
        delete[]_elementi2;
        _trenutno = obj._trenutno;
        _omoguciDupliranje = obj._omoguciDupliranje;
        _elementi1 = new T1[_trenutno];
        _elementi2 = new T2[_trenutno];
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];
            _elementi2[i] = obj._elementi2[i];
        }

    }
    ~Kolekcija() {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
    }
    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno()const { return _trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    void AddElement(T1 el1, T2 el2)
    {
        if (_omoguciDupliranje == false)
        {
            for (int i = 0; i < _trenutno; i++)
            {
                if (_elementi1[i] == el1 && _elementi2[i] == el2)
                {
                    throw exception("Nije moguce dodavanje duplih elemenata");
                }
            }
        }

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
        temp1 = nullptr;
        delete[]temp2;
        temp2 = nullptr;
        _elementi1[_trenutno] = el1;
        _elementi2[_trenutno] = el2;
        _trenutno++;

    }
    Kolekcija<T1, T2>operator()(int from, int to)
    {
        Kolekcija<T1, T2>temp;
        for (int i = from; i <= to; i++)
        {
            temp.AddElement(_elementi1[i], _elementi2[i]);
        }
        return temp;
    }

};
class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& obj)
    {
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }
    Datum(Datum&& obj) noexcept
    {
        _dan = obj._dan;
        obj._dan = nullptr;
        _mjesec = obj._mjesec;
        obj._mjesec = nullptr;
        _godina = obj._godina;
        obj._godina = nullptr;
    }
    Datum& operator=(const Datum& obj)
    {
        if (this == &obj)
        {
            return *this;
        }
        if (_dan == nullptr)
        {
            _dan = new int;
        }
        *_dan = *obj._dan;
        if (_mjesec == nullptr)
        {
            *_mjesec = *obj._mjesec;
        }
        *_mjesec = *obj._mjesec;
        if (_godina == nullptr)
        {
            _godina = new int;
        }
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
    int Dani()const
    {
        return *_godina * 365 + *_mjesec * 30 + *_dan;
    }
    friend bool operator==(const Datum& d1, const Datum& d2)
    {
        return *d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina;
    }
    friend bool operator!=(const Datum& d1, const Datum& d2) { return !(d1 == d2); }
    friend bool operator>(const Datum& d1, const Datum& d2) { return d1.Dani() > d2.Dani(); }
    friend bool operator<(const Datum& d1, const Datum& d2) { return d1.Dani() < d2.Dani(); }

};

class Predmet {
    char* _naziv;
    Kolekcija<Datum*, int>* _ocjene;
public:
    Predmet(const char* naziv = "", Datum datum = Datum(), int ocjena = 0) {
        _naziv = GetNizKaraktera(naziv);

        _ocjene = new Kolekcija<Datum*, int>();
        if (ocjena > 0)
            AddOcjena(datum, ocjena);
    }
    Predmet(const Predmet& obj)
    {
        _naziv = GetNizKaraktera(obj._naziv);
        _ocjene = new Kolekcija<Datum*, int>(*obj._ocjene);

    }
    Predmet& operator=(const Predmet& obj)
    {
        if (this == &obj)
        {
            return *this;
        }
        delete[]_naziv;
        _naziv = GetNizKaraktera(obj._naziv);
        _ocjene = obj._ocjene;
        return *this;
    }
    ~Predmet() {
        delete[] _naziv; _naziv = nullptr;
    }
    void AddOcjena(Datum& datum, int ocjena)
    {
        _ocjene->AddElement(new Datum(datum), ocjena);
    }
    char* GetNaziv() { return _naziv; }
    Kolekcija<Datum*, int> GetOcjene() { return _ocjene; }
    float prosjecnaOcjena()const
    {
        float prosjek = 0.0f;
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

    // ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu ukoliko predmet nema niti jednu ocjenu prosjecna treba biti jednaka 0
    friend  ostream& operator<<(ostream& COUT, const Predmet& p1)
    {
        COUT << "Naziv predmeta: " << p1._naziv << endl;
        COUT << "Ocjene: " << endl;
        for (int i = 0; i < p1._ocjene->getTrenutno(); i++)
        {
            COUT << *p1._ocjene->getElement1(i) << " " << p1._ocjene->getElement2(i) << endl;
        }
        COUT << "Prosjecna ocjena: " << p1.prosjecnaOcjena() << endl;
        return  COUT;
    }
    friend  bool operator ==(const Predmet& p1, const Predmet& p2)
    {
        if (strcmp(p1._naziv, p2._naziv) != 0)
        {
            return false;
        }
        return true;
    }
};

class Uspjeh {
    eRazred _razred;
    //char* se odnosi na napomenu o polozenom predmetu
    Kolekcija<Predmet*, const char*> _polozeniPredmeti;
public:
    Uspjeh(eRazred razred = PRVI) {
        _razred = razred;

    }
    Uspjeh(Predmet predmet, string napomena, eRazred razred)
    {
        _razred = razred;
        _polozeniPredmeti.AddElement(new Predmet(predmet), GetNizKaraktera(napomena.c_str()));
    }

    Uspjeh(const Uspjeh& obj)
    {
        _razred = obj._razred;
        _polozeniPredmeti = obj._polozeniPredmeti;
    }
    Uspjeh& operator=(const Uspjeh& obj)
    {
        if (this == &obj)
        {
            return *this;
        }
        _razred = obj._razred;
        _polozeniPredmeti = obj._polozeniPredmeti;
    }
    Kolekcija<Predmet*, const char*> GetPredmeti() { return _polozeniPredmeti; }
    eRazred GetERazred() { return _razred; }
    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
        COUT << obj._razred << endl;
        for (int i = 0; i < obj._polozeniPredmeti.getTrenutno(); i++)
        {
            COUT << *obj._polozeniPredmeti.getElement1(i) << "  " << obj._polozeniPredmeti.getElement2(i) << endl;
        }
        return COUT;
    }
    float prosjekUspjeh()const
    {
        float prosjek = 0;
        for (int i = 0; i < _polozeniPredmeti.getTrenutno(); i++)
        {
            prosjek += _polozeniPredmeti.getElement1(i)->prosjecnaOcjena();
        }
        return prosjek / _polozeniPredmeti.getTrenutno();
    }
    void DodajPredmet(Predmet predmet, const char* napomena)
    {
        _polozeniPredmeti.AddElement(new Predmet(predmet), GetNizKaraktera(napomena));
    }

};

bool ValidirajBrojTelefona(string telefon)
{
    /*
       broj telefona mora biti u formatu
       - znak +
       - pozivni broj drzave (2 ili 3 cifre)
       - pozivni broj operatera (2 cifre) npr. 063, 061, 065 pri cemu je broj 0 opcionalan
       - prvi dio broja (2 ili 3 cifre)
       - razmak ili crtica, oboje opcionalno tj. broj telefona ne mora sadrzavati niti jedno niti drugo
        - drugi dio broja (2 ili 3 cifre)
       ukoliko broj telefona nije u validnom formatu, njegova vrijednost se postavlja na NOT VALID
       */
    string format = "\\+\\d{2,3}0?\\d{2}\\d{2,3}[ -]?\\d{2,3}";
    regex uzorak(format);
    return regex_match(telefon, uzorak);
}
mutex staIma;
class Kandidat {
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh> _uspjeh;
    void saljiMail(eRazred razred, float prosjecnaOcjena, float ukupanProsjek)
    {
        /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
        FROM:info@fit.ba
        TO: emailKorisnika
        Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
        na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
        Pozdrav.
        FIT Team.*/
        staIma.lock();
        cout << " FROM:info@fit.ba" << endl;
        cout << "TO: " << this->GetEmail() << endl;
        cout << "Postovani " << this->GetImePrezime() << ", evidentirali ste uspjeh za " << razred << " Dosadasnji uspjeh (prosjek) na nivou " << razred << " iznosi " << prosjecnaOcjena << " a ukupni uspjeh u toku skolovanja iznosi " << ukupanProsjek << "." << endl;
        cout << "Pozdrav" << endl;
        cout << "FIT Team" << endl;
        staIma.unlock();
    }
    void saljiPoruku(eRazred razred, float prosjecnaOcjena)
    {
        //  ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
        //Slanje SMS-a izvrsiti samo u slucaju da je broj telefona validna, u protivnom ispisati poruku BROJ TELEFONA NIJE VALIDAN
        staIma.lock();
        cout << "Svaka cast za uspjeh " << prosjecnaOcjena << " u " << razred << " razred " << endl;
        staIma.unlock();
    }
public:
    Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
        _imePrezime = GetNizKaraktera(imePrezime);
        _emailAdresa = emailAdresa;
        _brojTelefona = brojTelefona;//izvrsiti validaciju broja telefona
    }
    ~Kandidat() {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
        for (size_t i = 0; i < obj._uspjeh.size(); i++)
            COUT << obj._uspjeh[i];
        return COUT;
    }
    bool AddPredmet(eRazred razred, Predmet predmet, string napomena)
    {
        /*
        uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
        tom prilikom onemoguciti:
        - dodavanje istih (moraju biti identicne vrijednosti svih clanova tipa Predmet) predmeta na nivou jednog razreda,
        - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
        - dodavanje vise od 5 predmeta na nivou jednog razreda
        razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
        Metoda vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
        */
        //-dodavanje istih(moraju biti identicne vrijednosti svih clanova tipa Predmet) predmeta na nivou jednog razreda,
        auto isIsti = [this, razred, predmet]
        {
            //ne smijemo dodati novi uspjeh ako vec imamo postoji za taj predmet
            //ili za novi predmet ako ima
            for (auto& uspjeh : _uspjeh)
            {
                if (uspjeh.GetERazred() == razred)
                {
                    for (int i = 0; i < uspjeh.GetPredmeti().getTrenutno(); i++)
                    {
                        if (*uspjeh.GetPredmeti().getElement1(i) == predmet)
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        };
        //- dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
        auto prosjekPredmeta = [this, predmet]
        {
            if (predmet.prosjecnaOcjena() < 2.5f)
            {
                return false;
            }

            return true;
        };
        //- dodavanje vise od 5 predmeta na nivou jednog razreda
        auto provjeraBrojaPredmeta = [this, razred]
        {//provjera da li svaki razred ima 5 predmeta
            for (auto& uspjeh : _uspjeh)
            {
                if (uspjeh.GetERazred() == razred)
                {
                    if (uspjeh.GetPredmeti().getTrenutno() == 5)
                    {
                        return false;
                    }
                }
            }
            return true;
        };
        if (!isIsti() || !prosjekPredmeta() || !provjeraBrojaPredmeta())
        {
            return false;
        }
        for (auto& uspjeh : _uspjeh)
        {//provjeravali dali imamo uspjeh u razreeduy
            if (uspjeh.GetERazred() == razred)
            {
                uspjeh.DodajPredmet(predmet, napomena.c_str());
                thread work(&Kandidat::saljiMail, this, razred, predmet.prosjecnaOcjena(), uspjeh.prosjekUspjeh());
                work.join();
                if (uspjeh.prosjekUspjeh() >= 4.5f)
                {
                    thread poruka(&Kandidat::saljiPoruku, this, razred, uspjeh.prosjekUspjeh());
                    poruka.join();
                }
                return true;

            }


        }
        /*   //ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom : "Svaka cast za uspjeh 4.5 u X razredu".
          OVDE JE TREBALO IMPLEMENTIRATI AKO JE PROSJEK VECI OD 4.5 DA SE ISPISE PORUKA JA SAM PROBAO OVAKO
          I POKUSAO SAM ISTO DA JE STAVIM U FUNKCIJU PROSJEK PREDMETA KAO ELSE DA ISPISUJE ALI NIJE RADILO
           for (auto uspjeh : _uspjeh)
           {
               if (uspjeh.GetERazred() == razred)
               {
                   for (int j = 0; j < uspjeh.GetPredmeti().getTrenutno(); j++)
                   {
                       if (uspjeh.GetPredmeti().getElement1(j)->prosjecnaOcjena() > 4.5f)
                       {
                           thread t1(&Kandidat::saljiPoruku, this, razred, uspjeh.prosjekUspjeh());
                           t1.join();
                       }
                   }
               }
           }*/
           //a sada ako nemamo uspjeh u razredu dodaj ga
        Uspjeh temp(predmet, napomena, razred);
        _uspjeh.push_back(temp);
        thread work(&Kandidat::saljiMail, this, razred, predmet.prosjecnaOcjena(), temp.prosjekUspjeh());
        work.join();
        if (temp.prosjekUspjeh() >= 4.5f)
        {
            thread poruka(&Kandidat::saljiPoruku, this, razred, temp.prosjekUspjeh());
            poruka.join();
        }
        return true;
    }

    vector<Uspjeh>& GetUspjeh() { return _uspjeh; }
    string GetEmail() { return _emailAdresa; }
    string GetBrojTelefona() { return _brojTelefona; }
    char* GetImePrezime() { return _imePrezime; }
    Kolekcija<Predmet, float>operator()(Datum* datum1, Datum* datum2)
    {
        //vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
       //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
        Kolekcija<Predmet, float>temp;
        for (auto& uspjeh : _uspjeh)
        {
            for (int i = 0; i < uspjeh.GetPredmeti().getTrenutno(); i++)
            {

                for (int j = 0; j < uspjeh.GetPredmeti().getElement1(i)->GetOcjene().getTrenutno(); j++)
                {
                    if (uspjeh.GetPredmeti().getElement1(i)->GetOcjene().getElement1(j) > datum1 && datum2 > uspjeh.GetPredmeti().getElement1(i)->GetOcjene().getElement1(j))
                    {
                        temp.AddElement(*uspjeh.GetPredmeti().getElement1(i), 1.0f);
                    }
                }
            }
        }
        return temp;
    }
};

const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Pojasnite ulogu i nacin koristenja generickog tipa future<>?\n";
    return "Odgovor -> Future je objekat koj moze vratiti vrijednost od nekogprovider objekta ili funkcije, pravilno sihnorizirajuci taj pristup ako je u razlicitim thredovima. Validni futuri su konstruisani pozivajuci npr funkciju async. prmise::get_future";
}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Ukratko opisite na koji nacin se moze izbjeci pojavljivanje vise podobjekata bazne klase u slucaju visestrukog nasljedjivanja?\n";
    return "Odgovor -> Virtualno nasljedivanje omogucuva da objekti najizvedenijih klasa posjeduju samo 1 podobjekt inderektnih baznih klasa";
}

void main() {

    cout << PORUKA;
    cin.get();

    cout << GetOdgovorNaPrvoPitanje() << endl;
    cin.get();
    cout << GetOdgovorNaDrugoPitanje() << endl;
    cin.get();

    Datum
        datum19062021(19, 6, 2021),
        datum20062021(20, 6, 2021),
        datum30062021(30, 6, 2021),
        datum05072021(5, 7, 2021);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    try {
        //ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
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

    //parametri: nazivPredmeta, datum, prva ocjena
    Predmet Matematika("Matematika", datum19062021, 5),
        Fizika("Fizika", datum20062021, 5),
        Hemija("Hemija", datum30062021, 2),
        Engleski("Engleski", datum05072021, 5);

    Matematika.AddOcjena(datum05072021, 3);
    Matematika.AddOcjena(datum05072021, 3);

    // ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
    // ukoliko predmet nema niti jednu ocjenu prosjecna treba biti jednaka 0
    cout << Matematika << endl;


    /*
       broj telefona mora biti u formatu
       - znak +
       - pozivni broj drzave (2 ili 3 cifre)
       - pozivni broj operatera (2 cifre) npr. 063, 061, 065 pri cemu je broj 0 opcionalan
       - prvi dio broja (2 ili 3 cifre)
       - razmak ili crtica, oboje opcionalno tj. broj telefona ne mora sadrzavati niti jedno niti drugo
        - drugi dio broja (2 ili 3 cifre)
       ukoliko broj telefona nije u validnom formatu, njegova vrijednost se postavlja na NOT VALID
       */

    if (ValidirajBrojTelefona("+38761222333"))
        cout << "Broj telefona validan" << crt;
    if (ValidirajBrojTelefona("+38761222-333"))
        cout << "Broj telefona validan" << crt;
    if (ValidirajBrojTelefona("+38761222 333"))
        cout << "Broj telefona validan" << crt;
    if (ValidirajBrojTelefona("+387061222 333"))
        cout << "Broj telefona validan" << crt;
    if (!ValidirajBrojTelefona("+38761 222 333"))
        cout << "Broj NIJE telefona validan" << crt;
    if (!ValidirajBrojTelefona("+387 61222 333"))
        cout << "Broj NIJE telefona validan" << crt;


    Kandidat jasmin("Jasmin Azemovic", "jasmin@fit.ba", "+38761222333");
    Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "+387061222 333");
    Kandidat brojTelefonaNotValid("Ime Prezime", "korisnik@klix.ba", "+387 61222 333");

    /*
    uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih clanova tipa Predmet) predmeta na nivou jednog razreda,
    - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
    - dodavanje vise od 5 predmeta na nivou jednog razreda
    razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
    Metoda vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
    if (jasmin.AddPredmet(DRUGI, Fizika, "Napomena 1"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(DRUGI, Hemija, "Napomena 2"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Engleski, "Napomena 3"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 4"))
        cout << "Predmet uspjesno dodan!" << crt;
    //Matematiku je vec dodana u prvom razredu
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 5"))
        cout << "Predmet uspjesno dodan!" << crt;

    /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
    FROM:info@fit.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
    na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
    Pozdrav.
    FIT Team.
    ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom:
    "Svaka cast za uspjeh 4.X u X razredu". Slanje SMS-a izvrsiti samo u slucaju da je broj telefona validna, u protivnom ispisati poruku BROJ TELEFONA NIJE VALIDAN
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    cout << jasmin << crt;

    //vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
    //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
    Kolekcija<Predmet, float> jasminUspjeh = jasmin(new Datum(18, 06, 2021), new  Datum(21, 06, 2021));
    cout << jasminUspjeh << crt;

    cin.get();
    system("pause>0");
  
}