#include <iostream>
#include <vector>
#include <cassert>
#include <mutex>
#include <thread>
#include <regex>
using namespace std;
const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO RE\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE!\n"
"7. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    if (dealociraj)
        delete[]sadrzaj;
    return temp;
}
bool ValidirajEmail(string email)
{
    /*
    email adresa mora biti u formatu text.text (sa ili bez tacke),
    dok domena moze biti fit.ba ili edu.fit.ba
    nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.
    za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
    validacija email adrese ce se vrsiti unutar konstruktora klase Kandidat, a u slucaju da nije validna
    postaviti je na defaultnu adresu: notSet@edu.fit.ba
    */
    return regex_match(email, regex("[A-Za-z]+\\.?[A-Za-z]+@(fit.ba|edu.fit.ba|fit.com|edu.fit.com|fit.org|edu.fit.org)"));
}
template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int _trenutno;
    bool _omoguciDupliranje;
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
    Kolekcija(bool omoguciDupliranje = true)
    {
        _trenutno = 0;
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _omoguciDupliranje = omoguciDupliranje;
    }
    Kolekcija(const Kolekcija& obj)
    {
        _trenutno = obj.getTrenutno();
        _omoguciDupliranje = obj._omoguciDupliranje;
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
        _omoguciDupliranje = obj._omoguciDupliranje;
    }
    Kolekcija& operator =(const Kolekcija& obj)
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
        _omoguciDupliranje = obj._omoguciDupliranje;
        return *this;
    }
    ~Kolekcija()
    {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
    }
    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() const { return _trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    void AddElement(T1 prvi, T2 drugi)
    {
        if (_omoguciDupliranje == false)
        {
            for (int i = 0; i < _trenutno; i++)
            {
                if (_elementi1[i] == prvi && _elementi2[i] == drugi)
                    throw exception("Nije moguce dodavati duplikate !!!");
            }
        }
        prosiriNizove();
        _elementi1[_trenutno] = prvi;
        _elementi2[_trenutno] = drugi;
        _trenutno++;
    }
    Kolekcija operator () (int from, int to)
    {
        Kolekcija<T1, T2> povratna;
        if (from < 0 || to >= _trenutno)
            throw exception("Ne validan opseg niza !!!");
        for (int i = from; i <= to; i++)
        {
            povratna.AddElement(i, i);
        }
        return povratna;
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
    ~Datum()
    {
        delete _dan;    _dan = nullptr;
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
bool operator ==(const Datum& d1, const Datum& d2)
{
    return d1.getDan() == d2.getDan() && d1.getMjesec() == d2.getMjesec() && d1.getGodina() == d2.getGodina();
}
bool operator !=(const Datum& d1, const Datum& d2)
{
    return !(d1 == d2);
}
bool operator >(const Datum& d1, const Datum& d2)
{
    return d1.toDani() > d2.toDani();
}
class Predmet {
    char* _naziv;
    //int se odnosi na ocjenu u opsegu od 1 – 5, a datum na momenat postizanja ocjene
    Kolekcija<int, Datum> _ocjene;
public:
    Predmet(const char* naziv = "", int ocjena = 0, Datum datum = Datum())
    {
        _naziv = GetNizKaraktera(naziv);
        if (ocjena > 0)
            _ocjene.AddElement(ocjena, datum);
    }
    Predmet(const Predmet& obj)
        :_ocjene(obj._ocjene)
    {
        _naziv = GetNizKaraktera(obj.GetNaziv());
    }
    Predmet(Predmet&& obj)noexcept
        :_ocjene(move(obj._ocjene))
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
    }
    Predmet& operator =(const Predmet& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_naziv;
        _naziv = GetNizKaraktera(obj.GetNaziv());
        _ocjene = obj._ocjene;
        return *this;
    }
    ~Predmet()
    {
        delete[] _naziv; _naziv = nullptr;
    }
    void AddOcjena(int ocjena, Datum datum)
    {
        _ocjene.AddElement(ocjena, datum);
    }
    char* GetNaziv() const { return _naziv; }
    Kolekcija<int, Datum>& GetOcjene() { return _ocjene; }
    float prosjecnaOcjenaPredmeta()const
    {
        float prosjek = 0.0f;
        for (int i = 0; i < _ocjene.getTrenutno(); i++)
        {
            prosjek += _ocjene.getElement1(i);
        }
        if (prosjek > 0)
            return prosjek /= _ocjene.getTrenutno();
        return prosjek;
    }
    friend ostream& operator << (ostream& COUT, const Predmet& obj)
    {
        COUT << "Naziv: " << obj.GetNaziv() << endl;
        //COUT << "Ocjene:" << endl;
        for (int i = 0; i < obj._ocjene.getTrenutno(); i++)
        {
            COUT << "Ocjena: " << obj._ocjene.getElement1(i) << "  Datum: " << obj._ocjene.getElement2(i) << endl;
        }
        COUT << "Prosjecna ocjena: " << obj.prosjecnaOcjenaPredmeta() << endl;
        return COUT;
    }
};
bool operator == (const Predmet& p1, const Predmet& p2)
{
    if (strcmp(p1.GetNaziv(), p2.GetNaziv()) != 0)
        return false;
    if (const_cast<Predmet&>(p1).GetOcjene().getTrenutno() != const_cast<Predmet&>(p2).GetOcjene().getTrenutno())
        return false;
    for (int i = 0; i < const_cast<Predmet&>(p1).GetOcjene().getTrenutno(); i++)
    {
        if (const_cast<Predmet&>(p1).GetOcjene().getElement1(i) != const_cast<Predmet&>(p2).GetOcjene().getElement1(i) || const_cast<Predmet&>(p1).GetOcjene().getElement2(i) != const_cast<Predmet&>(p2).GetOcjene().getElement2(i))
            return false;
    }
    return true;
}
bool operator != (const Predmet& p1, const Predmet& p2)
{
    return !(p1 == p2);
}
class Uspjeh {
    eRazred _razred;
    //string se odnosi na napomenu o polozenom predmetu
    Kolekcija<Predmet, string>* _polozeniPredmeti;
public:

    Uspjeh(eRazred razred = PRVI)
    {
        _razred = razred;
        _polozeniPredmeti = nullptr;
    }
    Uspjeh(eRazred razred, Predmet predmet, string napomena)
    {
        _razred = razred;
        _polozeniPredmeti = new Kolekcija<Predmet, string>();
        _polozeniPredmeti->AddElement(predmet, napomena);
    }
    Uspjeh(const Uspjeh& obj)
        :_polozeniPredmeti(new Kolekcija<Predmet, string>(*obj._polozeniPredmeti))
    {
        _razred = obj.GetERazred();
    }
    Uspjeh(Uspjeh&& obj)noexcept
        :_polozeniPredmeti(new Kolekcija<Predmet, string>(move(*obj._polozeniPredmeti)))
    {
        _razred = obj._razred;
    }
    Uspjeh& operator = (const Uspjeh& obj)
    {
        if (this == &obj)
            return *this;
        _razred = obj.GetERazred();
        if (_polozeniPredmeti == nullptr)
            _polozeniPredmeti = new Kolekcija<Predmet, string>();
        *_polozeniPredmeti = *obj._polozeniPredmeti;
        return *this;
    }
    ~Uspjeh()
    {
        delete _polozeniPredmeti;
        _polozeniPredmeti = nullptr;
    }

    Kolekcija<Predmet, string>* GetPredmeti() const { return _polozeniPredmeti; }
    eRazred GetERazred() const { return _razred; }
    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
        COUT << obj._razred << " " << *obj._polozeniPredmeti << endl;
        return COUT;
    }
    float ukupanProsjek()const
    {
        float prosjek = 0.0f;
        for (int i = 0; i < _polozeniPredmeti->getTrenutno(); i++)
        {
            prosjek += _polozeniPredmeti->getElement1(i).prosjecnaOcjenaPredmeta();
        }
        if (prosjek > 0)
            return prosjek /= _polozeniPredmeti->getTrenutno();
        return prosjek;
    }
};
bool operator == (const Uspjeh& u1, const Uspjeh& u2)
{
    if (u1.GetERazred() != u2.GetERazred())
        return false;
    if (u1.GetPredmeti()->getTrenutno() != u2.GetPredmeti()->getTrenutno())
        return false;
    for (int i = 0; i < u1.GetPredmeti()->getTrenutno(); i++)
    {
        if (u1.GetPredmeti()->getElement1(i) != u2.GetPredmeti()->getElement1(i) || u1.GetPredmeti()->getElement2(i) != u2.GetPredmeti()->getElement2(i))
            return false;
    }
    return true;
}
bool operator != (const Uspjeh& u1, const Uspjeh& u2)
{
    return !(u1 == u2);
}
mutex m;
class Kandidat {
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh> _uspjeh;
    void posaljiEmail(eRazred raz, float prosjek, float ukupniProsjek)
    {
        /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
        FROM:info@fit.ba
        TO: emailKorisnika
        Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
        na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
        Pozdrav.
        FIT Team.
        ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
        slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
        */
        m.lock();
        cout << "FROM:info@fit.ba" << endl;
        cout << "TO: " << this->GetEmail() << endl;
        cout << " Postovani " << this->GetImePrezime() << ", evidentirali ste uspjeh za " << raz << " razred. Dosadasnji uspjeh (prosjek) na nivou " << raz << " razreda iznosi " << prosjek << ", a ukupni uspjeh u toku skolovanja iznosi " << ukupniProsjek << " ." << endl;
        cout << "Pozdrav." << endl;
        cout << "FIT Team." << endl;
        m.unlock();
    }
    void posaljiSMS(eRazred raz, float prosjek)
    {
        m.lock();
        cout << "Svaka cast za uspjeh " << prosjek << " u " << raz << " razredu" << endl;
        m.unlock();
    }
public:
    Kandidat()
    {
        _imePrezime = nullptr;
    }
    Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona)
    {
        _imePrezime = GetNizKaraktera(imePrezime);
        _emailAdresa = ValidirajEmail(emailAdresa) ? emailAdresa : "notSet@edu.fit.ba";
        _brojTelefona = brojTelefona;
    }
    Kandidat(const Kandidat& obj)
        :_emailAdresa(obj.GetEmail()), _brojTelefona(obj.GetBrojTelefona()), _uspjeh(obj._uspjeh)
    {
        _imePrezime = GetNizKaraktera(obj.GetImePrezime());
    }
    Kandidat(Kandidat&& obj)noexcept
        :_emailAdresa(move(obj._emailAdresa)), _brojTelefona(move(obj._brojTelefona)),
        _uspjeh(move(obj._uspjeh))
    {
        _imePrezime = obj._imePrezime;
        obj._imePrezime = nullptr;
    }
    ~Kandidat()
    {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
        for (size_t i = 0; i < obj._uspjeh.size(); i++)
            COUT << obj._uspjeh[i];
        return COUT;
    }
    vector<Uspjeh>& GetUspjeh() { return _uspjeh; }
    string GetEmail() const { return _emailAdresa; }
    string GetBrojTelefona()const { return _brojTelefona; }
    char* GetImePrezime() const { return _imePrezime; }
    bool AddPredmet(eRazred razred, Predmet predmet, string napomena)
    {
        /*
    uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda,
    - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
    - dodavanje vise od 5 predmeta na nivou jednog razreda
    */
        if (_uspjeh.size() == 0)
        {
            _uspjeh.push_back(Uspjeh(razred, predmet, napomena));
            float prosjek = 0.0f;
            float ukupanProsjek = 0.0f;
            for (auto uspjeh : _uspjeh)
            {
                ukupanProsjek = uspjeh.ukupanProsjek();
                for (int j = 0; j < uspjeh.GetPredmeti()->getTrenutno(); j++)
                {
                    prosjek = uspjeh.GetPredmeti()->getElement1(j).prosjecnaOcjenaPredmeta();
                }
                if (uspjeh.GetERazred() == razred)
                {
                    for (int j = 0; j < uspjeh.GetPredmeti()->getTrenutno(); j++)
                    {
                        if (uspjeh.GetPredmeti()->getElement1(j).prosjecnaOcjenaPredmeta() > 4.5f)
                        {
                            thread t1(&Kandidat::posaljiSMS, this, razred, prosjek);
                            t1.join();
                        }
                    }
                }
            }
            thread t(&Kandidat::posaljiEmail, this, razred, prosjek, ukupanProsjek);
            t.join();
            return true;
        }
        auto checkDuplikatiIBrojDodanih = [this, &razred, &predmet]()
        {
            for (auto uspjeh : _uspjeh)
            {
                if (uspjeh.GetERazred() == razred)
                {
                    for (int i = 0; i < uspjeh.GetPredmeti()->getTrenutno(); i++)
                    {
                        if (uspjeh.GetPredmeti()->getElement1(i) == predmet)
                            return false;//Nema dodavanja istih predmeta na nivou jednog razreda;
                    }
                    if (uspjeh.GetPredmeti()->getTrenutno() > 5)
                        return false;//Nema dodavanj vise od pet predmeta na nivou jednog razreda;
                }
            }
            return true;
        };
        auto checkProsjecnaOcjena = [this, &predmet]()
        {
            if (predmet.prosjecnaOcjenaPredmeta() < 2.5f)
                return false;//Nema dodavanja predmeta cija je prosjecna ocjena manja od 2.5;
            return true;
        };
        if (checkDuplikatiIBrojDodanih() && checkProsjecnaOcjena())
        {
            _uspjeh.push_back(Uspjeh(razred, predmet, napomena));
            float prosjek = 0.0f;
            float ukupanProsjek = 0.0f;
            for (auto uspjeh : _uspjeh)
            {
                ukupanProsjek = uspjeh.ukupanProsjek();
                for (int j = 0; j < uspjeh.GetPredmeti()->getTrenutno(); j++)
                {
                    prosjek = uspjeh.GetPredmeti()->getElement1(j).prosjecnaOcjenaPredmeta();
                }
                if (uspjeh.GetERazred() == razred)
                {
                    for (int j = 0; j < uspjeh.GetPredmeti()->getTrenutno(); j++)
                    {
                        if (uspjeh.GetPredmeti()->getElement1(j).prosjecnaOcjenaPredmeta() > 4.5f)
                        {
                            thread t1(&Kandidat::posaljiSMS, this, razred, prosjek);
                            t1.join();
                        }
                    }
                }
            }
            thread t(&Kandidat::posaljiEmail, this, razred, prosjek, ukupanProsjek);
            t.join();
            return true;
        }
        return false;
    }
    Kolekcija<Predmet, float> operator () (Datum from, Datum to)
    {
        Kolekcija<Predmet, float> povratna;
        float prosjecanBrdana = 0.0f;
        for (auto uspjeh : _uspjeh)
        {
            for (int i = 0; i < uspjeh.GetPredmeti()->getTrenutno(); i++)
            {
                for (int j = 0; j < uspjeh.GetPredmeti()->getElement1(i).GetOcjene().getTrenutno(); j++)
                {
                    if (uspjeh.GetPredmeti()->getElement1(i).GetOcjene().getElement2(j) > from && to > uspjeh.GetPredmeti()->getElement1(i).GetOcjene().getElement2(j))
                        if (uspjeh.GetPredmeti()->getElement1(i).GetOcjene().getTrenutno() >= 1)
                        {
                            prosjecanBrdana++;
                            if (prosjecanBrdana > 0)
                                prosjecanBrdana /= uspjeh.GetPredmeti()->getElement1(i).GetOcjene().getTrenutno();
                            povratna.AddElement(uspjeh.GetPredmeti()->getElement1(i), prosjecanBrdana);
                        }
                    prosjecanBrdana = 0.0f;
                }
            }
        }
        return povratna;
    }
    Uspjeh* operator [](eRazred key)
    {
        Uspjeh* temp = nullptr;
        for (int i = 0; i < _uspjeh.size(); i++)
        {
            if (_uspjeh[i].GetERazred() == key)
                temp = &_uspjeh[i];
        }
        return temp;
    }
};

const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Na sta se odnosi pojam reprezentacije tipa?\n";
    return "Odgovor -> Reprezentacija tipa predstavlja nacin na koji klasa ili primitivni tip podatka moze biti kreiran (konstruktovan)";
}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Na koji nacin se moze izbjeci pojavljivanje vise podobjekata bazne klase u slucaju visestrukog nasljedjivanja?\n";
    return "Odgovor -> Ovaj problem se izbjegne tako sto se u najizvedenijoj klasi naslijede bazne klase pomocu virtual nasljedjivanja te onda se njihova bazna klasa inicijalizuje samo jednim konstruktorom koji ce zadovoljiti sve ostale.";
}

void main() {

    cout << PORUKA;
    cin.get();

    cout << GetOdgovorNaPrvoPitanje() << endl;
    cin.get();
    cout << GetOdgovorNaDrugoPitanje() << endl;
    cin.get();

    Datum temp,
        datum19062019(19, 6, 2019),
        datum20062019(20, 6, 2019),
        datum30062019(30, 6, 2019),
        datum05072019(5, 7, 2019);

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

    //parametri: nazivPredmeta, prva ocjena, datum
    Predmet Matematika("Matematika", 5, datum19062019),
        Fizika("Fizika", 5, datum20062019),
        Hemija("Hemija", 2, datum30062019),
        Engleski("Engleski", 5, datum05072019);

    Matematika.AddOcjena(3, datum05072019);
    Matematika.AddOcjena(5, datum05072019);

    // ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
    // ukoliko predmet nema niti jednu ocjenu prosjecna treba biti 0
    cout << Matematika << endl;

    if (ValidirajEmail("text.text@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.com"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.org"))
        cout << "Email validan" << crt;

    /*
    email adresa mora biti u formatu text.text (sa ili bez tacke),
    dok domena moze biti fit.ba ili edu.fit.ba
    nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.
    za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
    validacija email adrese ce se vrsiti unutar konstruktora klase Kandidat, a u slucaju da nije validna
    postaviti je na defaultnu adresu: notSet@edu.fit.ba
    */

    Kandidat jasmin("Jasmin Azemovic", "jasmin@fit.ba", "033 281 172");
    Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
    Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

    /*
    uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda,
    - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
    - dodavanje vise od 5 predmeta na nivou jednog razreda
    razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
    Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
    if (jasmin.AddPredmet(DRUGI, Fizika, "Napomena 1"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(DRUGI, Hemija, "Napomena 2"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Engleski, "Napomena 3"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 4"))
        cout << "Predmet uspjesno dodan!" << crt;
    //ne treba dodati Matematiku jer je vec dodana u prvom razredu
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 5"))
        cout << "Predmet uspjesno dodan!" << crt;

    /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
    FROM:info@fit.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
    na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
    Pozdrav.
    FIT Team.
    ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    cout << jasmin << crt;

    //vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
    //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
    Kolekcija<Predmet, float> jasminUspjeh = jasmin(Datum(18, 06, 2019), Datum(21, 06, 2019));
    cout << jasminUspjeh << crt;

    Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
    if (uspjeh_Irazred != nullptr)
        cout << *uspjeh_Irazred << crt;

    cin.get();
    system("pause>0");
}