#include <vector>
#include <iostream>
#include <cassert>
using namespace std;

int Min(int a, int b) { return (a <= b) ? a : b; }
int Max(int a, int b) { return (a >= b) ? a : b; }

//:::::::::::ZADACI::::::::::::
//Z0.1 
char* Alokator(const char* tekst)
{
    if (tekst == nullptr)
        return nullptr;
    unsigned int size = strlen(tekst) + 1;
    char* noviTxt = new char[size];
    strcpy_s(noviTxt, size, tekst);
    return noviTxt;
}
//Z0.2 :: Funkcija za alokaciju pametnog (shared_ptr) pokazivaca na niz karaktera
shared_ptr<char> SharedAlokator(const char* tekst)
{
    if (tekst == nullptr)
        return nullptr;
    unsigned int size = strlen(tekst) + 1;
    shared_ptr<char> noviTxt(new char[size]);//Alociramo shared ptr na niz karaktera size velicine;
    strcpy_s(noviTxt.get(), size, tekst);//Posto je smart ptr izvucemo raw ptr iz njega sa get();
    return noviTxt;
}
//Z0.3 :: Funkcija za alokaciju pametnog (unique_ptr) pokazivaca na niz karaktera
unique_ptr<char> UniqueAlokator(const char* tekst)
{
    if (tekst == nullptr)
        return nullptr;
    unsigned int size = strlen(tekst) + 1;
    unique_ptr<char> noviTxt(new char[size]);//Alociramo novi unique ptr na  niz karaktera;
    strcpy_s(noviTxt.get(), size, tekst);//Posto je smart ptr izvucemo raw ptr iz njega sa get();
    return noviTxt;
}
//Z0.4:: Provjeriti da li su dva niza karaktera ista (po vrijednosti)
bool CheckIfIsti(const char* tekst1, const char* tekst2)
{
    if(tekst1==nullptr || tekts2==nullptr)
        return false;
    if (strcmp(tekst1, tekst2) == 0)
        return true;
    return false;
}

//Z0.5 Provjeriti da li su dva vektora jednaka (da li imaju iste elemente)
template<class T>
bool operator == (const vector<T>& v1, const vector<T>& v2)
{
    if (v1.size() == v2.size())
        return true;
    for (int i = 0; i < v1.size(); i++)//Svejedno je do kojeg size idemo, tj od kojeg vektora jer su ovde isti;
    {
        if (v1[i] == v2[i])
            return true;
    }
    return false;
}

template<class T>
bool operator != (const vector<T>& v1, const vector<T>& v2)
{
    if (!(v1 == v2))
        return true;
    return false;
}
//Poredjenje vektora nizova karaktera jer su oni specificni za porediti:
bool operator == (const vector<const char*>& v1, const vector<const char*>& v2)
{
    if (v1.size() == v2.size())
        return true;
    for (int i = 0; i < v1.size(); i++)
    {
        if (CheckIfIsti(v1[i], v2[i]))
            return true;
    }
    return false;
}
bool operator != (const vector<const char*>& v1, const vector<const char*>& v2)
{
    if (!(v1 == v2))
        return true;
    return false;
}

//Simulacija ratne stateske igre
enum class Moral { Nizak = 1, Srednji, Visok };//Enumeracijska klasa pristupa joj se scope operatorom;

class Vojnik {
protected:
    unique_ptr<int> _zdravlje; //[0-100] 
    unique_ptr<int> _izdrzljivost; //[0-100]
    Moral* _moral;
    vector<const char*> _oruzja;
    bool _statusZiv;
public:
    __declspec(property (get = GetZdravlje, put = SetZdravlje)) int Zdravlje;
    __declspec(property (get = GetIzdrzljivost, put = SetIzdrzljivost)) int Izdrzljivost;
    __declspec(property (get = GetMoral, put = SetMoral)) Moral Moral_;
    __declspec(property (get = GetOruzja, put = SetOruzja)) vector<const char*> Oruzja;
    __declspec(property (get = GetStatusZiv, put = SetStatusZiv)) bool StatusZiv;

    //Z1.1
    Vojnik(int zdravlje = 100, int izdrzljivost = 100, Moral moral = Moral::Visok, bool statusZiv = true)//Dflt.ctor & Usr-def.ctor;
    {
        _zdravlje = make_unique<int>(zdravlje);
        _izdrzljivost = make_unique<int>(izdrzljivost);
        _moral = new Moral(moral);
        _statusZiv = statusZiv;
    }
    //Z1.2
    Vojnik(const Vojnik& obj)//Copy ctor;Koristimo properties od rvalue jer postoji;
        :_oruzja(obj.Oruzja)//Copy ctor class vector;
    {
        _zdravlje = make_unique<int>(obj.Zdravlje);
        _izdrzljivost = make_unique<int>(obj.Izdrzljivost);
        _moral = new Moral(obj.Moral_);
        _statusZiv = obj.StatusZiv;
    }
    //Z1.3
    Vojnik(Vojnik&& obj) noexcept//Move ctor;Mogu se koristiti properties ali nije preporuceno jer se onda ne radi sa actual vrijednostima;
        :_oruzja(move(obj._oruzja)), _zdravlje(move(obj._zdravlje)), _izdrzljivost(move(obj._izdrzljivost))//Move ctor od vectora i smart ptr;
    {
        _moral = obj._moral;
        obj._moral = nullptr;
        _statusZiv = obj._statusZiv;
    }
    //Z1.4 :: Getteri
    int GetZdravlje() const { return *_zdravlje; }
    int GetIzdrzljivost() const { return *_izdrzljivost; }
    Moral GetMoral() const { return *_moral; }
    vector<const char*> GetOruzja() const { return _oruzja; }
    bool GetStatusZiv() const { return _statusZiv; }
    //Z1.5 :: Setteri
    void SetZdravlje(int zdravlje)
    {
        if (_zdravlje == nullptr)
            _zdravlje = make_unique<int>();
        *_zdravlje = zdravlje;//Ne treba assertacija jer smo se sa if pobrinuli da nije nullptr;
    }
    void SetIzdrzljivost(int izdrzljivost)
    {
        if (_izdrzljivost == nullptr)
            _izdrzljivost = make_unique<int>();
        *_izdrzljivost = izdrzljivost;//Ne treba assertacija jer smo se sa if pobrinuli da nije nullptr;
    }
    void SetMoral(Moral moral)
    {
        if (_moral == nullptr)
            _moral = new Moral;
        *_moral = moral;//Ne treba assertacija jer smo se sa if pobrinuli da nije nullptr;
    }
    void SetOruzja(vector<const char*> oruzja)
    {
        _oruzja = oruzja;//Preklopljen operator = class vector;
    }
    void SetStatusZiv(bool statusZiv)
    {
        _statusZiv = statusZiv;
    }
    //Z1.6
    Vojnik& operator = (const Vojnik& obj)
    {
        if (this == &obj)
            return *this;//Ako su na istim memor. lokacijama vrati obj koji je pozvao metodu;
        //Koristimo propertije i lvalue i rvalue jer oba postoje i alocirani su:
        Zdravlje = obj.Zdravlje;
        Izdrzljivost = obj.Izdrzljivost;
        StatusZiv = obj.StatusZiv;
        Oruzja = obj.Oruzja;//Preklopljen operator = class vector;
        Moral_ = obj.Moral_;
        return *this;//Vrati obj koji je pozvao metodu;
    }
    //Z1.7
    virtual void Ispis()
    {
        cout << "Zdravlje: " << Zdravlje << endl;
        cout << "Izdrzljivost: " << Izdrzljivost << endl;
        cout << "Moral: ";
        switch (Moral_)
        {
        case Moral::Nizak:
        {
            cout << "Nizak" << endl;
            break;
        }
        case Moral::Srednji:
        {
            cout << "Srednji" << endl;
            break;
        } case Moral::Visok:
        {
            cout << "Visok" << endl;
            break;
        }
        default:
            break;
        }
        cout << "Oruzja: ";
        for (int i = 0; i < Oruzja.size(); i++)
        {
            cout << Oruzja[i] << ",";
        }
        cout << endl;
        cout << "Status ziv: " << ((StatusZiv) ? "Ziv" : "Mrtav") << endl;
    }
    //Z1.8
    virtual ~Vojnik()
    {
        _zdravlje.reset();//Nepotrebno ali je good practice;
        _izdrzljivost.reset();
        delete _moral;
        _moral = nullptr;
    }
};
//Z1.9
bool operator == (const Vojnik& v1, const Vojnik& v2)
{
    if (v1.Zdravlje == v2.Zdravlje)
        return true;
    if (v1.Izdrzljivost == v2.Izdrzljivost)
        return true;
    if (v1.Moral_ == v2.Moral_)
        return true;
    if (v1.StatusZiv == v2.StatusZiv)
        return true;
    if (v1.Oruzja == v2.Oruzja)//Preklopljen operator == za nizove karaktera u vektorima;
        return true;
    return false;
}
bool operator != (const Vojnik& v1, const Vojnik& v2)
{
    if (!(v1 == v2))
        return true;
    return false;
}

//interfejs
class IPonasanje {
public:
    //U C++ nema pravih interfejsa kao u C# ali ako svaka fija je cista virtuelna onda se dobija ta ista funkcionalnost;
    //U sustini svaka klasa koja nasljedi ovu klasu mora imati sve tri fije implementirane, ukoliko ih nema
    //Onda i ta klasa postaje apstraktna implicitno;
    virtual void OglasiSe() = 0;
    virtual void Napadaj(Vojnik* meta) = 0;
    virtual void OporavljajSe() = 0;
};

enum class Kategorija { Laka = 1, Srednja, Teska };//Enumeracijska klasa pristup preko ::;

class Pjesak : public Vojnik, public IPonasanje {
protected:
    Kategorija _kategorija;
    unique_ptr<char> _oklop;
public:
    __declspec(property (get = GetKategorija, put = SetKategorija)) Kategorija Kategorija_;
    __declspec(property (get = GetOklop, put = SetOklop)) const char* Oklop;
    //Z2.1
    Pjesak(Kategorija kategorija = Kategorija::Srednja, const char* oklop = "metalni")//Dflt ctor i Usr-def ctor;
        :Vojnik(), IPonasanje()//Dflt ctorovi obe klase;
    {
        _kategorija = kategorija;
        _oklop = UniqueAlokator(oklop);
    }
    //Z2.2
    Pjesak(const Pjesak& obj)//Copy ctor;Mozemo koristiti properties od rvalue jer postoji;
        :Vojnik(obj)//Copy ctor base class;Nema potrebe za IPonasanje ctorom jer je ona u ovom sluc. 'Interfejs' a ne klasa;
    {
        _kategorija = obj.Kategorija_;
        _oklop = UniqueAlokator(obj.Oklop);//Ili _oklop.get();
    }
    //Z2.3
    Pjesak(Pjesak&& obj) noexcept //Move ctor;Ne koristi propertije jer izmjene nisu onda direktne
        :Vojnik(move(obj)),//Move ctor base class;
        _oklop(move(obj._oklop)),//Move ctor smart ptr;
        _kategorija(move(obj._kategorija))//Move ctor enum. klase;
    {
    }
    //Z2.4 :: Getteri
    Kategorija GetKategorija() const { return _kategorija; }
    const char* GetOklop() const { return _oklop.get(); }//Get() kako bi se izvukao raw ptr smart ptr objekta;
    //Z2.5 :: Setteri
    void SetKategorija(Kategorija kategorija)
    {
        _kategorija = kategorija;
    }
    void SetOklop(const char* oklop)
    {
        _oklop = UniqueAlokator(oklop);
    }
    //Z2.6
    Pjesak& operator = (const Pjesak& obj)
    {
        if (this == &obj)
            return *this;
        //Oba su objekta alocirana stoga se mogu koristiti propertiji:
        static_cast<Vojnik&>(*this) = static_cast<Vojnik>(obj);//Ili samo obj;
        Kategorija_ = obj.Kategorija_;
        Oklop = obj.Oklop;
        return *this;
    }
    //Z2.7
    virtual void Ispis() override //Virtual se ne mora stavljati ali je good practice isto za override vazi;
    {
        //Vojnik::Ispis();
               static_cast<Vojnik>(*this).Ispis();//Moze na oba nacina;SAMO BEZ & jer se sa & desava stack overflow, iznova i iznova se poziva fija;
        cout << "Kategorija: ";
        switch (Kategorija_)
        {
        case Kategorija::Laka: cout << "Laka" << endl;
            break;
        case Kategorija::Srednja: cout << "Srednja" << endl;
            break;
        case Kategorija::Teska:cout << "Teska" << endl;
            break;
        default:
            break;
        }
        cout << "Oklop: " << Oklop << endl;
    }
    //Z2.8 :: Ispisati poruku "Ja sam tipa <Tip Podatka>" ... 
    //Za utvrdjivanje tipa podatka, koristiti funkciju 'typeid'
    virtual void OglasiSe() override
    {
        cout << "Ja sam tipa: " << typeid(*this).name();
    }
    //Z2.9 :: Ovu funkciju implementirati izvan scope-a
    virtual void Napadaj(Vojnik* meta) override;
    //Z2.10 :: Povecati vrijednost atributa Zdravlje za slucajnu vrijednost izmedju [0-25]
    //Pri cemu isti atribut mora ostati u opsegu vrijednosti od 0-100
    virtual void OporavljajSe() override
    {
        Zdravlje = Min(100, Zdravlje + rand() % 26);
    }
    //Z2.11
    virtual ~Pjesak() override
    {
        //Nasljedjene klase same rjesavaju svoj posao;
        _oklop.reset();//Nepotrebno ali dobra praksa;
    }
};
//Z2.12
bool operator == (const Pjesak& p1, const Pjesak& p2)
{
    if (static_cast<Vojnik>(p1) == static_cast<Vojnik>(p2))//Preklopljen operator == class Vojnik;
        return true;
    if (CheckIfIsti(p1.Oklop, p2.Oklop))
        return true;
    if (p1.Kategorija_ == p2.Kategorija_)
        return true;
    return false;
}
//Z2.13
bool operator != (const Pjesak& p1, const Pjesak& p2)
{
    if (!(p1 == p2))
        return true;
    return false;
}

class Strijelac : public Vojnik, public IPonasanje {
protected:
    int* _municija;
public:
    __declspec(property (get = GetMunicija, put = SetMunicija)) int Municija;
    //Z3.1
    Strijelac(int municija = 50)//Dflt ctor i Usr-def ctor;
        : Vojnik(), IPonasanje()//Dflt ctor obe base class mada druga nije potrebna jer ona glumi interfejs;
    {
        _municija = new int(municija);
    }
    //Z3.2
    Strijelac(const Strijelac& obj)//Copy ctor;Koristimo property jer rvalue postoji;
        : Vojnik(obj)//Copy ctor base class;
    {
        _municija = new int(obj.Municija);//Ili *obj._municija;
    }
    //Z3.3
    Strijelac(Strijelac&& obj) noexcept//Move ctor;Ne koristimo propertije jer izmjene zelimo nad samim propertijima ne kopijama;
        : Vojnik(move(obj))//Move ctor base class
    {
        _municija = obj._municija;
        obj._municija = nullptr;
    }
    //Z3.4 :: Getteri
    int GetMunicija() const { return *_municija; }
    //Z3.5 :: Setteri
    void SetMunicija(int municija)
    {
        if (_municija == nullptr)
            _municija = new int();
        *_municija = municija;//Ne trebamo vrsiti assert jer smo if-om to obuhvatili;
    }
    //Z3.6
    Strijelac& operator =(const Strijelac& obj)
    {
        if (this == &obj)
            return *this;
        //Mozemo koristiti propertije oba objekta jer oba su alocirana;
        static_cast<Vojnik&>(*this) = static_cast<Vojnik>(obj);//Ili samo obj;
        Municija = obj.Municija;
        return *this;
    }
    //Z3.7
    virtual void Ispis() override
    {
        //Vojnik::Ispis();
          //Ili:
        static_cast<Vojnik>(*this).Ispis();
        cout << "Municija: " << Municija << endl;
    }

    //Z3.8 :: Ispisati poruku "Ja sam tipa <Tip Podatka>" ... 
    //Za utvrdjivanje tipa podatka, koristiti funkciju 'typeid'
    virtual void OglasiSe()override
    {
        cout << "Ja sam tipa: " << typeid(*this).name() << endl;
    }
    //Z3.9 :: Ovu funkciju implementirati izvan scope-a
    virtual void Napadaj(Vojnik* meta) override;

    //Z3.10  Povecati vrijednost atributa zdravlje za slucajnu vrijednost izmedju [0-35]
    //Pri cemu isti atribut mora ostati u opsegu vrijednosti od 0-100
    void OporavljajSe()
    {
        Zdravlje = Min(100, Zdravlje + rand() % 36);
        //Ili bez propertija:
       //*_zdravlje = Min(100, *_zdravlje + rand() % 36);
    }
    //Z3.11
    virtual ~Strijelac() override
    {
        //Base class se brine o svom dealociranju;
        delete _municija;
        _municija = nullptr;
    }
};
//Z3.12
bool operator == (const Strijelac& s1, const Strijelac& s2)
{
    if (static_cast<Vojnik>(s1) == static_cast<Vojnik>(s2))//Preklopljen operator class Vojnik;
        return true;
    if (s1.Municija == s2.Municija)
        return true;
    return false;
}
//Z3.13
bool operator != (const Strijelac& s1, const Strijelac& s2)
{
    if (!(s1 == s2))
        return true;
    return false;
}

class Konjanik : public Vojnik, public IPonasanje {
protected:
    unique_ptr<char> _oklop;
    const char* _vrstaKonja;
public:
    __declspec(property (get = GetOklop, put = SetOklop)) const char* Oklop;
    __declspec(property (get = GetVrstaKonja, put = SetVrstaKonja)) const char* VrstaKonja;
    //Z4.1
    Konjanik(const char* oklop = "metalni", const char* vrstaKonja = "")//Dflt i Usr-def ctor;
        :Vojnik(), IPonasanje()//Dflt ctor obe base class iako od interfejsa ne treba jer glumi interfejs a ne class;
    {
        _oklop = UniqueAlokator(oklop);
        _vrstaKonja = Alokator(vrstaKonja);
    }
    //Z4.2
    Konjanik(const Konjanik& obj)//Copy ctor;
        :Vojnik(obj)//Copy ctor base class;
    {
        _oklop = UniqueAlokator(obj.Oklop);//Ili _oklop.get() da se dobije raw ptr;
        //Takodjer se moze koristiti ovo:
        //_oklop(obj._oklop.get())//u headeru ctor-a, ovo je copy ctor smart ptr-a;Mora se koristiti get() da se dobije raw ptr;
        _vrstaKonja = Alokator(obj.VrstaKonja);
    }
    //Z4.3
    Konjanik(Konjanik&& obj)noexcept//Move ctor;
        :Vojnik(move(obj)),//Move ctor base class;
        _oklop(move(obj._oklop))//Move ctor smart ptr-a;
    {
        _vrstaKonja = obj._vrstaKonja;
        obj._vrstaKonja = nullptr;
    }
    //Z4.4 :: Getteri
    const char* GetOklop() const { return _oklop.get(); }
    const char* GetVrstaKonja() const { return _vrstaKonja; }
    //Z4.5 :: Setteri
    void SetOklop(const char* oklop)
    {
        _oklop = UniqueAlokator(oklop);
    }
    void SetVrstaKonja(const char* vrstaKonja)
    {
        delete[]_vrstaKonja;
        _vrstaKonja = Alokator(vrstaKonja);
    }
    //Z4.6
    Konjanik& operator = (const Konjanik& obj)
    {
        if (this == &obj)
            return *this;
        static_cast<Vojnik&>(*this) = static_cast<Vojnik>(obj);//Ili samo obj;
        //Mozemo koristiti propertije od oba jer su alocirana:
        VrstaKonja = obj.VrstaKonja;
        Oklop = obj.Oklop;
        return *this;
    }
    //Z4.7  
    virtual void Ispis() override
    {
        //Vojnik::Ispis();//Ili:
        static_cast<Vojnik>(*this).Ispis();
        cout << "Oklop: " << Oklop << endl;
        cout << "Vrsta konja: " << VrstaKonja << endl;
    }

    //Z4.8 :: Ispisati poruku "Ja sam tipa <Tip Podatka>" ... 
    //Za utvrdjivanje tipa podatka, koristiti funkciju 'typeid'
    virtual void OglasiSe() override
    {
        cout << "Ja sam tipa: " << typeid(*this).name() << endl;
    }
    //4.9 :: Ovu funkciju implementirati izvan scope-a
    virtual void Napadaj(Vojnik* meta) override;
    //Z4.10  Povecati vrijednost atributa zdravlje za slucajnu vrijednost izmedju [0-15]
    //Pri cemu isti atribut mora ostati u opsegu vrijednosti od 0-100
    virtual void OporavljajSe() override
    {
        Zdravlje = Min(100, Zdravlje + rand() % 16);
        //*_zdravlje = Min(100, *_zdravlje + rand() % 16);
    }
    //4.11
    virtual ~Konjanik() override
    {
        delete[]_vrstaKonja;
        _vrstaKonja = nullptr;
        _oklop.reset();//Nepotrebni ali good practice;
        //Base class Vojnik sam izvrsi dealokaciju;
    }
};
//4.12
bool operator == (const Konjanik& k1, const Konjanik& k2)
{
    if (static_cast<Vojnik>(k1) == static_cast<Vojnik>(k2))//Preklopljen operator == class Vojnik;
        return true;
    if (CheckIfIsti(k1.VrstaKonja, k2.VrstaKonja))
        return true;
    if (CheckIfIsti(k1.Oklop, k2.Oklop))
        return true;
    return false;
}
//4.13
bool operator != (const Konjanik& k1, const Konjanik& k2)
{
    if (!(k1 == k2))
        return true;
    return false;
}

//Z2.9 ::
//Pjesak ima vjerovatnocu od 50% da ce uspjesno sprovesti napad
//*Utvrditi tip mete koristenjem 'dynamic_cast'
//U slucaju da je napad uspjesan, steta koja se nanosi je:
// ->30 (ukoliko je meta drugi pjesak)
// ->50 (ukoliko je meta strijelac)
// ->10 (ukoliko je meta konjanik)
//*Umanjiti zdravlje mete za nacinjenu stetu
//*Ukoliko je zdravlje mete palo na 0, proglasiti metu nezivom (StatusAlive = false)
void Pjesak::Napadaj(Vojnik* meta)
{
    if (meta == nullptr)//Ako je nullptr
        return;//Zavrsi metodu jer nema se ko napasti;
    bool napad = rand() % 2;//Daje 1 ili 0;
    if (napad)
    {
        int steta = 0;
        if (dynamic_cast<Pjesak*>(meta) != nullptr)
        {
            steta = 30;
            cout << "Pjesak napada Pjesaka !!!" << endl;
        }
        else if (dynamic_cast<Strijelac*>(meta) != nullptr)
        {
            steta = 50;
            cout << "Pjesak napada Strijelca !!!" << endl;
        }
        else if (dynamic_cast<Konjanik*>(meta) != nullptr)
        {
            steta = 10;
            cout << "Pjesak napada Konjanika !!!" << endl;
        }
        meta->Zdravlje = Max(0, meta->Zdravlje - steta);//Posto se oduzima, ako je Zdravlje negativno onda je van opsega i treba se dati najveca moguca tada a to je 0;
        if (meta->Zdravlje == 0)
            meta->StatusZiv = false;//Mrtav je;
    }
    else
    {
        cout << "Neuspijesan napad..." << endl;
    }
}

//Z3.9 ::
//Strijelac ima vjerovatnocu od 50% da ce uspjesno sprovesti napad
//*Utvrditi tip mete koristenjem 'dynamic_cast'
//U slucaju da je napad uspjesan, steta koja se nanosi je:
//->35 (ukoliko je meta pjesak)
//->50 (ukoliko je meta drugi strijelac)
//->65 (ukoliko je meta konjanik)
//*Umanjiti zdravlje mete za nacinjenu stetu
//*Ukoliko je zdravlje mete palo na 0, proglasiti metu nezivom (StatusAlive = false)
void Strijelac::Napadaj(Vojnik* meta)
{
    if (meta == nullptr)
        return;
    bool napad = rand() % 2;
    if (napad)
    {
        int steta = 0;
        if (dynamic_cast<Pjesak*>(meta) != nullptr)
        {
            steta = 35;
            cout << "Strijelac napada Pjesaka !!!" << endl;
        }
        else if (dynamic_cast<Strijelac*>(meta) != nullptr)
        {
            steta = 50;
            cout << "Strijelac napada Strijelca !!!" << endl;
        }
        else if (dynamic_cast<Konjanik*>(meta) != nullptr)
        {
            steta = 65;
            cout << "Strijelac napada Konjanika !!!" << endl;
        }
        meta->Zdravlje = Max(0, meta->Zdravlje - steta);
        if (meta->Zdravlje == 0)
            meta->StatusZiv = false;
    }
    else
        cout << "Neuspijesan napad..." << endl;
}
//Z4.9 ::
//Konjanik ima vjerovatnocu od 50% da ce uspjesno sprovesti napad
//*Utvrditi tip mete koristenjem 'dynamic_cast'
//U slucaju da je napad uspjesan, steta koja se nanosi je:
//->50 (ukoliko je meta pjesak)
//->80 (ukoliko je meta strijelac)
//->20 (ukoliko je meta konjanik)
//*Umanjiti zdravlje mete za nacinjenu stetu
//*Ukoliko je zdravlje mete palo na 0, proglasiti metu nezivom (StatusAlive = false)
void Konjanik::Napadaj(Vojnik* meta)
{
    if (meta == nullptr)
        return;
    bool napad = rand() % 2;
    if (napad)
    {
        int steta = 0;
        if (dynamic_cast<Pjesak*>(meta) != nullptr)
        {
            steta = 50;
            cout << "Konjanik napada Pjesaka !!!" << endl;
        }
        else if (dynamic_cast<Strijelac*>(meta) != nullptr)
        {
            steta = 80;
            cout << "Konjanik napada Strijelca !!!" << endl;
        }
        else if (dynamic_cast<Konjanik*>(meta) != nullptr)
        {
            steta = 20;
            cout << "Konjanik napada Konjanika !!!" << endl;
        }
        meta->Zdravlje = Max(0, meta->Zdravlje - steta);
        if (meta->Zdravlje == 0)
            meta->StatusZiv = false;
    }
    else
        cout << "Neuspijesan napad..." << endl;
}

template<class T>
class VojnaJedinica {
protected:
    const char* _naziv;
    vector<T> _pripadnici;
    unique_ptr<float> _cijenaObuke;
    unique_ptr<float> _cijenaOdrzavanja;
public:
    __declspec(property (get = GetNaziv, put = SetNaziv)) const char* Naziv;
    __declspec(property (get = GetPripadnici, put = SetPripadnici)) vector<T> Pripadnici;
    __declspec(property (get = GetCijenaObuke, put = SetCijenaObuke)) float CijenaObuke;
    __declspec(property (get = GetCijenaOdrzavanja, put = SetCijenaOdrzavanja)) float CijenaOdrzavanja;
    //Z5.1
    VojnaJedinica(const char* naziv = "", float cijenaObuke = 500, float cijenaOdrzavanja = 110)//Dflt ctor & Usr-def ctor;
    {
        _naziv = Alokator(naziv);
        _cijenaObuke = make_unique<float>(cijenaObuke);
        _cijenaOdrzavanja = make_unique<float>(cijenaOdrzavanja);
    }
    //Z5.2
    VojnaJedinica(const VojnaJedinica<T>& obj)//Copy ctor;
        : _pripadnici(obj.Pripadnici)//Copy ctor class vector (koristimo property jer rvalue postoji);
    {
        _cijenaObuke = make_unique<float>(obj.CijenaObuke);
        _cijenaOdrzavanja = make_unique<float>(obj.CijenaOdrzavanja);
        _naziv = Alokator(obj.Naziv);
    }
    //Z5.3
    VojnaJedinica(VojnaJedinica<T>&& obj)noexcept //Move ctor;
        :_pripadnici(move(obj._pripadnici)),//Move ctor class vector;
        _cijenaObuke(move(obj._cijenaObuke)), _cijenaOdrzavanja(move(obj._cijenaOdrzavanja))//Move ctor smart ptr-a;
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
    }
    //Z5.4 :: Getteri
    const char* GetNaziv() const { return _naziv; }
    vector<T> GetPripadnici() const { return _pripadnici; }
    float GetCijenaObuke() const
    {
        assert(_cijenaObuke != nullptr && "Greska u: VojnaJedinica::GetCijenaObuke");//Nastavi se ako nije nullptr, a ako jeste terminira se code;
        return *_cijenaObuke;
    }
    float GetCijenaOdrzavanja() const
    {
        assert(_cijenaOdrzavanja != nullptr && "Greska u : VojnaJedinica::GetCijenaOdrzavanja");//Nastavi se ako nije nullptr, a ako jeste terminira se code;
        return *_cijenaOdrzavanja;
    }
    //Z5.5 :: Setteri
    void SetNaziv(const char* naziv)
    {
        delete[]_naziv;
        _naziv = Alokator(naziv);
    }
    void SetPripadnici(vector<T> pripadnici)
    {
        _pripadnici = pripadnici;
    }
    void SetCijenaObuke(float cijenaObuke)
    {
        if (_cijenaObuke == nullptr)
            _cijenaObuke = make_unique<float>();
        *_cijenaObuke = cijenaObuke;//Nema potrebe za assert jer je if to obuhvatio;
    }
    void SetCijenaOdrzavanja(float cijenaOdrzavanja)
    {
        if (_cijenaOdrzavanja == nullptr)
            _cijenaOdrzavanja = make_unique<float>();
        *_cijenaOdrzavanja = cijenaOdrzavanja;//Nema potrebe za assert jer je if to obuhvatio;
    }
    //Z5.6
    VojnaJedinica<T>& operator = (const VojnaJedinica<T>& obj)
    {
        if (this == &obj)
            return *this;
        Naziv = obj.Naziv;
        CijenaObuke = obj.CijenaObuke;
        CijenaOdrzavanja = obj.CijenaOdrzavanja;
        Pripadnici = obj.Pripadnici;//Operator = class vector;
        return *this;
    }
    //Z5.7
    template<class T>//Nepotrebno ponavljanje jer se ova genericka fija vec nalazi u scope genericke klase;
    void DodajPripadnika(const T pripadnik)
    {
        _pripadnici.push_back(pripadnik);//Koristimo metodu u class vector za dodavanje na kraj niza;
    }
    //Z5.8 :: Ispisati ukupan broj pripadnika umjesto ispisa svakog pojedinacnog pripadnika
    void Ispis()
    {
        cout << "Naziv: " << Naziv << endl;
        cout << "Vojni red: " << typeid(T).name() << endl;
        cout << "Broj pripadnika: " << Pripadnici.size() << endl;
        cout << "Cijena obuke: " << CijenaObuke << endl;
        cout << "Cijena odrzavanja: " << CijenaOdrzavanja << endl;
    }
    //Z5.9
    ~VojnaJedinica()
    {
        _cijenaObuke.reset();//Nepotrebno ali good practice;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
        _cijenaOdrzavanja.reset();
        delete[]_naziv;
        _naziv = nullptr;
        //Vector se sam dealocira;
    }
};
//Z5.11
template<class T>
bool operator == (const VojnaJedinica<T>& obj1, const VojnaJedinica<T>& obj2)
{
    if (CheckIfIsti(obj1.Naziv, obj2.Naziv))
        return true;
    if (obj1.CijenaObuke == obj2.CijenaObuke)
        return true;
    if (obj1.CijenaOdrzavanja == obj2.CijenaOdrzavanja)
        return true;
    if (obj1.Pripadnici == obj2.Pripadnici)//Zavisno od toga kojeg su tipa pripadnici (konjanik,strijelac,pjesak) porede se  shodno svojim preklopljenim operatorima ==;
        return true;
    return false;
}
//Z5.12
template<class T>
bool operator != (const VojnaJedinica<T>& obj1, const VojnaJedinica<T>& obj2)
{
    if (!(obj1 == obj2))
        return true;
    return false;
}

class Armija {
private:
    const char* _ime;
    vector<VojnaJedinica<Pjesak>> _pjesadija;
    vector<VojnaJedinica<Strijelac>> _strijelci;
    vector<VojnaJedinica<Konjanik>> _konjica;
public:
    __declspec(property (get = GetIme, put = SetIme)) const char* Ime;
    __declspec(property (get = GetPjesadija, put = SetPjesadija)) vector<VojnaJedinica<Pjesak>> Pjesadija;
    __declspec(property (get = GetStrijelci, put = SetStrijelci)) vector<VojnaJedinica<Strijelac>> Strijelci;
    __declspec(property (get = GetKonjica, put = SetKonjica)) vector<VojnaJedinica<Konjanik>> Konjica;
    //Z6.1
    Armija(const char* ime = "")//Dflt ctor & User-def ctor;
    {
        _ime = Alokator(ime);
        //Sve ostalo je dflt ctorom kreirano;
    }
    //Z6.2
    Armija(const Armija& obj)//Copy ctor;
        : _pjesadija(obj.Pjesadija), _strijelci(obj.Strijelci), _konjica(obj.Konjica)//Copy ctor class vector;
    {
        _ime = Alokator(obj.Ime);
    }
    //Z6.3
    Armija(Armija&& obj) noexcept //Move ctor;
        :_pjesadija(move(obj._pjesadija)), _strijelci(move(obj._strijelci)), _konjica(move(obj._konjica))//Move ctor od class vector;
    {
        _ime = obj._ime;
        obj._ime = nullptr;
    }
    //Z6.4 :: Getteri
    const char* GetIme() const { return _ime; }
    vector<VojnaJedinica<Pjesak>> GetPjesadija() const { return _pjesadija; }
    vector<VojnaJedinica<Strijelac>> GetStrijelci() const { return _strijelci; }
    vector<VojnaJedinica<Konjanik>> GetKonjica() const { return _konjica; }
    //Z6.5 :: Setteri
    void SetIme(const char* ime)
    {
        delete[]_ime;
        _ime = Alokator(ime);
    }
    void SetPjesadija(vector<VojnaJedinica<Pjesak>> pjesadija)
    {
        _pjesadija = pjesadija;//Preklopljen operator = class vector;
    }
    void SetStrijelci(vector<VojnaJedinica<Strijelac>> strijelci)
    {
        _strijelci = strijelci;//Preklopljen operator = class vector;
    }
    void SetKonjica(vector<VojnaJedinica<Konjanik>> konjica)
    {
        _konjica = konjica;//Preklopljen operator = class vector;
    }

    //Z6.6
    Armija& operator =(const Armija& obj)
    {
        if (this == &obj)
            return *this;
        Ime = obj.Ime;
        Pjesadija = obj.Pjesadija;//Preklopljen operator = class vector;
        Strijelci = obj.Strijelci;//Preklopljen operator = class vector;
        Konjica = obj.Konjica;//Preklopljen operator = class vector;
        return *this;
    }
    //Z6.7
    //Overloadane metode (istog su potpisa samo se razlikuju u tipu ulaznog parametra);
    void DodajJedinicu(VojnaJedinica<Pjesak> jedinica)
    {
        _pjesadija.push_back(jedinica);//Metoda class vector koja doda element na kraj niza;
    }
    void DodajJedinicu(VojnaJedinica<Strijelac> jedinica)
    {
        _strijelci.push_back(jedinica);//Metoda class vector koja doda element na kraj niza;
    }
    void DodajJedinicu(VojnaJedinica<Konjanik> jedinica)
    {
        _konjica.push_back(jedinica);//Metoda class vector koja doda element na kraj niza;
    }
    //Z6.8
    void Ispis()
    {
        cout << "Naziv armije: " << Ime << endl;
        cout << "----------------------------------\n";
        cout << "Pjesadija: " << endl;
        cout << "----------------------------------\n";
        for (int i = 0; i < Pjesadija.size(); i++)
        {
            Pjesadija[i].Ispis();
        }
        cout << "----------------------------------\n";
        cout << "Strijelci: " << endl;
        cout << "----------------------------------\n";
        for (int i = 0; i < Strijelci.size(); i++)
        {
            Strijelci[i].Ispis();
        }
        cout << "----------------------------------\n";
        cout << "Konjica: " << endl;
        cout << "----------------------------------\n";
        for (int i = 0; i < Konjica.size(); i++)
        {
            Konjica[i].Ispis();
        }
        cout << "----------------------------------\n";
    }
    //Z6.9
    ~Armija()
    {
        delete[]_ime;
        _ime = nullptr;
        //Svi vectori se sami brinu o dealokaciji;
    }
};
//Z6.10
bool operator == (const Armija& a1, const Armija& a2)
{
    if (CheckIfIsti(a1.Ime, a2.Ime))
        return true;
    if (a1.Pjesadija == a2.Pjesadija)//Preklopljen operator == dva vectora;
        return true;
    if (a1.Strijelci == a2.Strijelci)//Preklopljen operator == dva vectora;
        return true;
    if (a1.Konjica == a2.Konjica)//Preklopljen operator == dva vectora;
        return true;
    return false;
}
auto IspisiPoruku = [](const char* tipFunkcije, bool success) {
    cout << tipFunkcije << " se izvrsio " << ((success) ? "uspjesno!" : "bezuspjesno!") << endl;
};
void Zadatak1() {
    vector<const char*> oruzja{ "mac", "dugi mac", "ratna sjekira", "buzdovan", "helebarda", "koplje" };
    Vojnik univerzalniVojnik;
    univerzalniVojnik.Zdravlje = 85;
    univerzalniVojnik.Izdrzljivost = 75;
    univerzalniVojnik.Moral_ = Moral::Srednji;
    univerzalniVojnik.Oruzja = oruzja;
    univerzalniVojnik.StatusZiv = true;
    univerzalniVojnik.Ispis();

    Vojnik u1(univerzalniVojnik);
    IspisiPoruku("Copy ctor", univerzalniVojnik == u1);
    Vojnik u2(move(u1));
    IspisiPoruku("Move ctor", univerzalniVojnik == u2);
    Vojnik u3;
    u3 = univerzalniVojnik;
    IspisiPoruku("Operator '='", univerzalniVojnik == u3);
    cout << endl << "Dealokacija..." << endl;
}
//Passed!
void Zadatak2() {
    vector<const char*> oruzja{ "velika ratna sjekira", "mac s dvije ruke" };
    Pjesak gimly;
    gimly.Oruzja = oruzja;
    gimly.Oklop = "teski celicni oklop";
    gimly.Kategorija_ = Kategorija::Teska;
    gimly.Ispis();

    Pjesak anotherDwarf(gimly);
    IspisiPoruku("Copy ctor", anotherDwarf == gimly);
    Pjesak anotherDwarf2(move(anotherDwarf));
    IspisiPoruku("Move ctor", gimly == anotherDwarf2);
    Pjesak p3;
    p3 = gimly;
    IspisiPoruku("Operator '='", gimly == p3);
    cout << endl << "Dealokacija..." << endl;
}
//Passed!
void Zadatak3() {
    vector<const char*> oruzja{ "velski dugacki luk", "samostrijel", "kratki mac" };
    Strijelac legolas;
    legolas.Oruzja = oruzja;
    legolas.Municija = 150;
    legolas.Ispis();
    Strijelac anotherElf(legolas);
    IspisiPoruku("Copy ctor", legolas == anotherElf);
    Strijelac anotherElf2(move(anotherElf));
    IspisiPoruku("Move ctor", legolas == anotherElf2);
    Strijelac s3;
    s3 = legolas;
    IspisiPoruku("Operator '='", legolas == s3);
    cout << endl << "Dealokacija..." << endl;
}
//Passed!
void Zadatak4() {
    vector<const char*> oruzja{ "dugacko koplje", "buzdovan" };
    Konjanik eomer;
    eomer.Oruzja = oruzja;
    eomer.Oklop = "celicni oklop";
    eomer.VrstaKonja = "frizijski konj";
    eomer.Ispis();
    Konjanik anotherHorserider(eomer);
    IspisiPoruku("Copy ctor", anotherHorserider == eomer);
    Konjanik anotherHorserider2(move(anotherHorserider));
    IspisiPoruku("Move ctor", eomer == anotherHorserider2);
    Konjanik ghostRider;
    ghostRider = eomer;
    IspisiPoruku("Operator '='", eomer == ghostRider);

    Pjesak p1, p2, p3, p4, p5;
    Strijelac s1, s2, s3, s4, s5;
    Konjanik k1, k2, k3, k4, k5;
    cout << "Bitka pocinje: -->" << endl;
    p1.Napadaj(&s1); //Pjesak napada strijelca (50% vjerovatnoca da uspije)
    p2.Napadaj(&p1); //Pjesak napada pjesaka (50% vjerovatnoca da uspije)
    p1.Napadaj(&k1); //Pjesak napada konjanika (50% vjerovatnoca da uspije)
    //
    s1.Napadaj(&s2); // Strijelac napada strijelca (50% vjerovatnoca da uspije)
    s2.Napadaj(&k5); // Strijelac napada konjanika (50% vjerovatnoca da uspije)
    s4.Napadaj(&p5); // Strijelac napada pjesaka (50% vjerovatnoca da uspije)
    //
    k4.Napadaj(&p3); // Konjanika napada pjesaka (50% vjerovatnoca da uspije)
    k4.Napadaj(&k3); // Konjanik napada konjanika (50% vjerovatnoca da uspije)
    k4.Napadaj(&s4); // Konjanik napada strijelca (50% vjerovatnoca da uspije)
    cout << endl << "Dealokacija..." << endl;
}
//Passed!
void Zadatak5() {
    VojnaJedinica<Pjesak> svicarskaGarda("Svicarska garda", 560.5f, 120.5f);
    svicarskaGarda.Naziv = "Svicarska Garda";
    svicarskaGarda.CijenaObuke = 570.0f;
    svicarskaGarda.CijenaOdrzavanja = 120.0f;
    for (int i = 0; i < 100; i++)
        svicarskaGarda.DodajPripadnika(Pjesak(Kategorija::Srednja, "lagani oklop"));
    svicarskaGarda.Ispis();
    VojnaJedinica<Pjesak> kopija(svicarskaGarda);
    IspisiPoruku("Copy ctor", svicarskaGarda == kopija);
    VojnaJedinica<Pjesak> pjesadija(move(kopija));
    IspisiPoruku("Move ctor", svicarskaGarda == pjesadija);
    VojnaJedinica<Pjesak> p3;
    p3 = svicarskaGarda;
    IspisiPoruku("Operator '='", svicarskaGarda == p3);
    cout << endl << "Dealokacija..." << endl;
}
//Passsed!
void Zadatak6() {

    VojnaJedinica<Pjesak> svicarskaGarda("Svicarska garda", 550.0f, 130.0f);
    VojnaJedinica<Pjesak> varangijanskaGarda("Varangijanska garda", 670.0f, 160.0f);
    VojnaJedinica<Pjesak> manAtArms("Man At Arms", 520.0f, 110.0f);
    for (int i = 0; i < 250; i++)
        svicarskaGarda.DodajPripadnika(Pjesak(Kategorija::Srednja, "lagani oklop"));
    for (int i = 0; i < 150; i++)
        varangijanskaGarda.DodajPripadnika(Pjesak(Kategorija::Teska, "celicni oklop"));
    for (int i = 0; i < 100; i++)
        manAtArms.DodajPripadnika(Pjesak(Kategorija::Srednja, "lagani oklop"));

    VojnaJedinica<Strijelac> englishLongbowman("Enlish Longbowman", 350.0f, 110.0f);
    for (int i = 0; i < 250; i++)
        englishLongbowman.DodajPripadnika(Strijelac(100));

    VojnaJedinica<Konjanik> bosanskaElitnaKonjica("Bosanska elitna konjica", 950.0f, 250.0f);
    for (int i = 0; i < 80; i++)
        bosanskaElitnaKonjica.DodajPripadnika(Konjanik("celicni oklop", "bosanski brdski konj"));
    VojnaJedinica<Konjanik> katafrakt("Katafrakt", 850.0f, 230.0f);
    for (int i = 0; i < 100; i++)
        katafrakt.DodajPripadnika(Konjanik("teski celicni oklop", "teski bizantijski konj"));

    Armija sjevernaArmija("Army Of The North");
    sjevernaArmija.DodajJedinicu(svicarskaGarda);
    sjevernaArmija.DodajJedinicu(varangijanskaGarda);
    sjevernaArmija.DodajJedinicu(manAtArms);
    sjevernaArmija.DodajJedinicu(englishLongbowman);
    sjevernaArmija.DodajJedinicu(bosanskaElitnaKonjica);
    sjevernaArmija.DodajJedinicu(katafrakt);
    sjevernaArmija.Ispis();

    Armija juznaArmija(sjevernaArmija);
    IspisiPoruku("Copy ctor", sjevernaArmija == juznaArmija);
    Armija pirane(move(juznaArmija));
    IspisiPoruku("Move ctor", sjevernaArmija == pirane);
    Armija a3;
    a3 = sjevernaArmija;
    IspisiPoruku("Operator '='", sjevernaArmija == a3);
    cout << endl << "Dealokacija..." << endl;
}
//Passed!;
void Menu() {
    int nastaviDalje = 1;
    while (nastaviDalje == 1) {
        int izbor = 0;
        do {
            system("cls");
            cout << "::Zadaci::" << endl;
            cout << "(1) Zadatak 1" << endl;
            cout << "(2) Zadatak 2" << endl;
            cout << "(3) Zadatak 3" << endl;
            cout << "(4) Zadatak 4" << endl;
            cout << "(5) Zadatak 5" << endl;
            cout << "(6) Zadatak 6" << endl;
            cout << "Unesite odgovarajuci broj zadatka za testiranje: -->: ";
            cin >> izbor;
            cout << endl;
        } while (izbor < 1 || izbor > 6);
        switch (izbor) {
        case 1: Zadatak1(); cout << "Zadatak 1. Done." << endl; break;
        case 2: Zadatak2(); cout << "Zadatak 2. Done." << endl; break;
        case 3: Zadatak3(); cout << "Zadatak 3. Done." << endl; break;
        case 4: Zadatak4(); cout << "Zadatak 4. Done." << endl; break;
        case 5: Zadatak5(); cout << "Zadatak 5. Done." << endl; break;
        case 6: Zadatak6(); cout << "Zadatak 6. Done." << endl; break;
        default:break;
        }
        do {
            cout << "DA LI ZELITE NASTAVITI DALJE? (1/0): ";
            cin >> nastaviDalje;
        } while (nastaviDalje != 0 && nastaviDalje != 1);
    }
}
int main() {
    Menu();
    return 0;
}
