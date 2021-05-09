#include <vector>
#include <iostream>
#include <cassert>
#include <functional>
using namespace std;

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
//Z0.2
int IzracunajBrojZnamenki(int broj)
{
    int brojZnamenki = 0;
    while (broj > 0)
    {
        broj /= 10;//Skinemo zadnju cifru;
        brojZnamenki++;//Povecamo brojac;
    }
    return brojZnamenki;//Vratimo brojac sa brojem znamenki;
}
//Z0.3 :: Pretvoriti (int) u (char*).
char* IntToStr(int broj)
{
    int size = IzracunajBrojZnamenki(broj) + 1;
    char* string = new char[size];
    _itoa_s(broj, string, size, 10);//Funkcija sama stavi '\0' znak;
    return string;
}

//Z0.4 :: Funkcija za alokaciju pametnog (shared_ptr) pokazivaca na niz karaktera
shared_ptr<char> SharedAlokator(const char* tekst)
{
    if (tekst == nullptr)
        return nullptr;
    unsigned int size = strlen(tekst) + 1;
    //Ili u jednoj liniji:
    shared_ptr<char> noviText(new char[size]);
    strcpy_s(noviText.get(), size, tekst);
    /* shared_ptr<char> noviText;
     noviText.reset(new char[size]);*///Reset fija moze alocirati jedan tip podatka ili niz;
     // shared_ptr<char> noviText = make_shared<char>(new char[size]);//Ne moze jer se sa make_shared gen  fijom  ovako inicijaliziraju samo pojedinacne varijable a ne nizovi;
    return noviText;
}
//Z0.5 :: Funkcija za alokaciju pametnog (unique_ptr) pokazivaca na niz karaktera
unique_ptr<char> UniqueAlokator(const char* tekst)
{
    if (tekst == nullptr)
        return nullptr;
    unsigned int size = strlen(tekst) + 1;
    unique_ptr<char> noviText(new char[size]);
    strcpy_s(noviText.get(), size, tekst);
    //Ili duzi nacin:
   /* unique_ptr<char> noviText;
   noviText.reset(new char[size]);*///Reset fija moze alocirati jedan tip podatka ili niz;
    return noviText;
}

//Z0.6:: Provjeriti da li su dva niza karaktera ista (po vrijednosti)
bool CheckIfIsti(const char* tekst1, const char* tekst2)
{
    if (strcmp(tekst1, tekst2) == 0)//Strcmp vrati 0 ako su oba ista, -1 ako je prvi duzi ili 1 ako je prvi kraci;
        return true;
    return false;
}
//Z0.7 Provjeriti da li su dva vektora jednaka (da li imaju iste elemente)
template<class T>
bool operator == (const vector<T>& v1, const vector<T>& v2)
{
    if (v1.size() != v2.size())//Preklopljen operator != class T;
        return false;//Ako su im razlicite velicine odmah vrati false (Vektori su razliciti);
    for (int i = 0; i < v1.size(); i++)//Svejedno je do cije velicine vektora idemo, jer su sada u ovoj liniji isti, jer smo gore osigurali return ako su razliciti;
    {
        if (v1[i] != v2[i])//Preklopljen operator != class T;
            return false;//Ako su im elementi razliciti jedno od drugog, vrati false (nisu jednaki);
    }
    return true;//Ako se nista odozgo ne izvrsi, vektori su jednaki;
}
template<class T>
bool operator != (const vector<T>& v1, const vector<T>& v2)
{
    if (!(v1 == v2))
        return true;
    return false;
}
//Z0.8 Provjeriti da li su dva vektora nizova karaktera jednaka (da li imaju iste elemente)
bool CheckIfIsti(const vector<const char*>& v1, const vector<const char*>& v2)
{
    //Nizovi karaktera su posebni jer ako imamo vekton nizova karaktera ne mozemo ih regularno porediti sa ==
    //vec koristimo strcmp, jer, su nizovi karaktera;
    if (v1.size() != v2.size())
        return false;
    for (int i = 0; i < v1.size(); i++)
    {
        if (!CheckIfIsti(v1[i], v2[i]))//Negiramo ono sto vrati fija jer smo obrnuli logiku; Ako se vrati true isti su stoga bude false, ako se vrati false razliciti su negira se stoga se vrati true;
            return false;
    }
    return true;
}

//Po mogucnosti postavljati 'assert' tamo gdje se radi dereferenciranje pokazivaca
//Za postojece objekte mozete koristiti property-je za pristup vrijednostima atributa
//Za ispravno koristenje property-ja, uraditi implementaciju odgovarajucih gettera i settera
class Uredjaj {
private:
    const char* _id;
protected:
    unique_ptr<char> _naziv;
    int _godinaProizvodnje;
    shared_ptr<float> _cijena;
    int* _garancija;
    vector<int> _ocjene;
public:
    __declspec(property (get = GetId, put = SetId)) const char* Id;
    __declspec(property (get = GetNaziv, put = SetNaziv)) const char* Naziv;
    __declspec(property (get = GetGodinaProizvodnje, put = SetGodinaProizvodnje)) int GodinaProizvodnje;
    __declspec(property (get = GetCijena, put = SetCijena)) float Cijena;
    __declspec(property (get = GetGarancija, put = SetGarancija)) int Garancija;
    __declspec(property (get = GetOcjene, put = SetOcjene)) vector<int> Ocjene;

    //Z1.0 :: Staticka metoda koja vraca niz karaktera (brojeva) u opsegu "10000" do "99999"
    static const char* GetRandomId()
    {
        char* nizKaraktera = IntToStr(rand() % 90000 + 10000);//Vrati rand od 10 000 do 99 999;
        return nizKaraktera;
    }
    //Z1.1 :: Postaviti '_id' na rezultat koji vrati staticka funkcija 'GetRandomId'
    Uredjaj()//Dflt ctor;
        :_id(GetRandomId())//Moze i u body;
    {
        _godinaProizvodnje = 0;
        _garancija = nullptr;
    }
    //Z1.2
    Uredjaj(const char* naziv, int godinaProizvodnje, float cijena, int garancija)//User-def ctor;
        :_id(GetRandomId())//Moze i u body;
    {
        _naziv = UniqueAlokator(naziv);
        _godinaProizvodnje = godinaProizvodnje;
        _cijena = make_shared<float>(cijena);
        _garancija = new int(garancija);//Moze i u header;
    }
    //Z1.3 :: mozete koristiti property-je drugog (vec postojeceg) objekta za pristup vrijednostima njegovih atributa
    //Kopirati sve atribute, ukljucujuci i polje '_id'
    Uredjaj(const Uredjaj& obj)//Copy ctor;
        :_ocjene(obj.Ocjene)//Koristimo copy  za vector, mozemo koristiti propertije jer je desni objekat vec postojeci;
    {
        //_id(obj.Id)//Ovo ne mozemo staviti u header jer se nece alocirati novi prostor za lvalue, samo ce se prekopirati !!!;
        _id = Alokator(obj._id);
        _naziv = UniqueAlokator(obj.Naziv);//Ili _naziv.get() kako bi se izvukao raw pointer;
        _godinaProizvodnje = obj.GodinaProizvodnje;
        _cijena = make_shared<float>(obj.Cijena);//Ili *obj._cijena;
        _garancija = new int(obj.Garancija);//Ili *obj._garancija;
    }
    //Z1.4
    Uredjaj(Uredjaj&& obj) noexcept//Move ctor;
        :_naziv(move(obj._naziv)),
        _cijena(move(obj._cijena)), _ocjene(move(obj._ocjene))//Koristimo move ctor  vector ocjene, kao i za sve objekte jer posjeduju move mehanizme;
    {
        _id = obj._id;
        obj._id = nullptr;//Takodjer nema svrhe da se move ctor radi na const char* jer ce se samo uraditi copy ctor u sustini;
        _godinaProizvodnje = obj._godinaProizvodnje;
        obj._godinaProizvodnje = 0;//Ovo je int (primitivni tip podatka) stoga move ctor u sustini samo prekopira vrijednosti kao sto bi i copy ctor;
        _garancija = obj._garancija;
        obj._garancija = nullptr;
    }

    //Z1.5
    const char* GetId() const { return _id; }
    const char* GetNaziv() const { return _naziv.get(); }//Koristimo get() kako bi dobili raw pointer;
    int GetGodinaProizvodnje() const { return  _godinaProizvodnje; }
    float GetCijena() const
    {
        assert(_cijena != nullptr);//Ako nije nullptr nastavlja se, ako jeste terminira se program;
        return *_cijena;
    }
    int GetGarancija() const
    {
        assert(_garancija != nullptr);//Ako nije nullptr nastavlja se, ako jeste terminira se program;
        return *_garancija;
    }
    vector<int> GetOcjene() const { return _ocjene; }

    //Z1.6
    void SetId(const char* id)
    {
        delete[]_id;
        _id = Alokator(id);
    }
    void SetNaziv(const char* naziv)
    {
        //Unique ptr svakako mora da alocira uvijek novi prostor jer moze da pokazuje samo na jednu memorijsku lokaciju;
        _naziv = UniqueAlokator(naziv);//Ne brisemo stari niz karaktera niti radimo ikakve promjene jer je smart ptr;
    }
    void SetGodinaProizvodnje(int godinaProizvodnje)
    {
        _godinaProizvodnje = godinaProizvodnje;
    }
    void SetCijena(float cijena)
    {
        //Assert ne treba pri dereferenciranju jer  lvalue ptr sigurno je != nullptr;
        if (_cijena == nullptr)
            _cijena = make_shared<float>();
        *_cijena = cijena;
    }
    void SetGarancija(int garancija)
    {
        //Assert ne treba pri dereferenciranju jer  lvalue ptr sigurno je != nullptr;
        if (_garancija == nullptr)
            _garancija = new int();
        *_garancija = garancija;
    }
    void SetOcjene(vector<int> ocjene)
    {
        _ocjene = ocjene;//Preklopljen operator = u class vector;
    }
    //Z1.7 :: mozete koristiti property-je za oba objekta (obzirom da vec postoje)
    Uredjaj& operator =(const Uredjaj& obj)
    {
        if (this == &obj)//Ako su na istim memor. lokacijama, 
            return *this;//Vrati obj koji je pozvao metodu;
        Id = obj.Id;
        Naziv = obj.Naziv;
        GodinaProizvodnje = obj.GodinaProizvodnje;
        Garancija = obj.Garancija;
        Ocjene = obj.Ocjene;
        Cijena = obj.Cijena;
        return *this;//Vrati obj koji je pozvao metodu;
    }
    //Z1.8
    void AddOcjena(int ocjena)
    {
        _ocjene.push_back(ocjena);//Metoda u class vector za dodavanje na kraj niza;
    }

    //Z1.9 :: Mozete koristiti property-je obzirom da objekt vec postoji
    float GetProsjecnaOcjena(function<float(vector<int>)> tipProsjeka)
    {
        return tipProsjeka(_ocjene);//Mozemo koristiti i property Ocjene;
        //Posalje se pokazivac na fiju koja je vec negdje implementirana;
    }
    //Z1.10 :: implementacija funkcije 'Ispis' se prepisuje (override-a) sa implementacijom istoimene funkcije  najizvedenije klase
    // Mozete koristiti property-je obzirom da objekt vec postoji
    virtual void Ispis()
    {
        cout << "Id: " << Id << endl;
        cout << "Naziv: " << Naziv << endl;
        cout << "Godina proizvodnje: " << GodinaProizvodnje << endl;
        cout << "Cijena: " << Cijena << endl;
        cout << "Garancija u godinama: " << Garancija << endl;
        cout << "Ocjene: ";
        for (int i = 0; i < Ocjene.size(); i++)
        {
            cout << Ocjene[i] << ",";
        }
        cout << endl;
    }
    //Z1.11
    virtual ~Uredjaj()//Shodno tome da imamo bar jednu virtuelnu metodu, "moramo" imati i virtuelan destruktor koji ce se pozivati za najizvedeniju klasu;
    {
        _naziv.reset();//Nije potrebno ali je dobra praksa;
        delete[]_id;
        _id = nullptr;
        _cijena.reset();//Nije potrebno ali je dobra praksa;
        delete _garancija;
        _garancija = nullptr;
    }
};

//Z1.12
//Iskljuciti polje 'Id' iz uporedbe
bool operator == (const Uredjaj& u1, const Uredjaj& u2)
{
    if (CheckIfIsti(u1.Naziv, u2.Naziv))
        return true;
    if (u1.GodinaProizvodnje == u2.GodinaProizvodnje)
        return true;
    if (u1.Cijena == u2.Cijena)
        return true;
    if (u1.Garancija == u2.Garancija)
        return true;
    if (u1.Ocjene == u2.Ocjene)//Preklopljen operator == za dva vectora;
        return true;
    return false;
}
bool operator != (const Uredjaj& u1, const Uredjaj& u2)
{
    if (!(u1 == u2))
        return true;
    return false;
}

class Mobitel : public Uredjaj {
private:
    char _operativniSistem[50];
    unique_ptr<float> _velicinaEkrana;//u incima Npr. 6.1
    int* _memorija; //GB
public:
    __declspec(property (get = GetOperativniSistem, put = SetOperativniSistem)) const char* OperativniSistem;
    __declspec(property (get = GetVelicinaEkrana, put = SetVelicinaEkrana)) float VelicinaEkrana;
    __declspec(property (get = GetMemorija, put = SetMemorija)) int Memorija;
    //Z3.1
    Mobitel()//Dflt ctor;
        :Uredjaj()//Dflt ctor;
    {
        strcpy_s(_operativniSistem, 50, "");
        _memorija = nullptr;
    }
    //Z2.2
    Mobitel(const char* naziv, int godinaP, float cijena, int garancija, const char* operativniS, float velicinaE, int memorija)//Usr-def ctor;
        :Uredjaj(naziv, godinaP, cijena, garancija)//User-def ctor base class;
    {
        strcpy_s(_operativniSistem, 50, operativniS);
        _velicinaEkrana = make_unique<float>(velicinaE);
        _memorija = new int(memorija);
    }
    //Z2.3
    Mobitel(const Mobitel& mobitel)//Copy ctor;
        :Uredjaj(mobitel)//Copy ctor base class;
    {
        strcpy_s(_operativniSistem, 50, mobitel._operativniSistem);
        _velicinaEkrana = make_unique<float>(mobitel.VelicinaEkrana);//Koristimo property jer vec  postoji rvalue ili mozemo *mobitel._velicinaEkrana;
        _memorija = new int(mobitel.Memorija);
    }
    //Z2.4
    Mobitel(Mobitel&& obj) noexcept//Move ctor;
        :Uredjaj(move(obj)),//Move ctor base class;
        _velicinaEkrana(move(obj._velicinaEkrana))//Move ctor objekta unique ptr;
    {
        strcpy_s(_operativniSistem, 50, obj._operativniSistem);
        strcpy_s(obj._operativniSistem, 50, "");//Nepotrebno ali dobra praksa;
        _memorija = obj._memorija;
        obj._memorija = nullptr;
    }

    const char* GetOperativniSistem()const { return _operativniSistem; }
    float GetVelicinaEkrana()const
    {
        assert(_velicinaEkrana != nullptr && "Doslo je do greske u Mobitel:: GetVelicinaEkrana ");//Ako nije nullptr nastavi ako jeste terminira proces;
        return *_velicinaEkrana;
    }
    int GetMemorija()const
    {
        assert(_memorija != nullptr && "Doslo je do greske u Mobitel: GetMemorija ");//Ako nije nullptr nastavi ako jeste terminira proces;
        return *_memorija;
    }

    void SetOperativniSistem(const char* operativniSistem)
    {
        strcpy_s(_operativniSistem, 50, operativniSistem);
    }
    void SetVelicinaEkrana(float velicinaEkrana)
    {
        if (_velicinaEkrana == nullptr)
            _velicinaEkrana = make_unique<float>();
        *_velicinaEkrana = velicinaEkrana;
    }
    void SetMemorija(int memorija)
    {
        if (_memorija == nullptr)
            _memorija = new int();
        *_memorija = memorija;
    }
    //Z2.5
    Mobitel& operator = (const Mobitel& obj)
    {
        if (this == &obj)
            return *this;
        static_cast<Uredjaj&>(*this) = static_cast<Uredjaj>(obj);//Ili samo = obj
        //Mozemo koristiti propertije jer su i lvalue i rvalue alocirani (kreirani);
        OperativniSistem = obj.OperativniSistem;
        VelicinaEkrana = obj.VelicinaEkrana;
        Memorija = obj.Memorija;
        return *this;
    }
    //Z2.6
    //Ispisati id, naziv mobitela, velicinu ekrana, operativni sistem, kolicinu memorije, cijenu te broj godina garancije
    void Ispis()
    {
        cout << "Id: " << Id << endl;
        cout << "Naziv: " << Naziv << endl;
        cout << "Velicina ekrana: " << VelicinaEkrana << " incha" << endl;
        cout << "Operativni sistem: " << OperativniSistem << endl;
        cout << "Kolicina memorije: " << Memorija << " GB" << endl;
        cout << "Cijena: " << Cijena << " KM" << endl;
        cout << "Garancija: " << Garancija << " godina" << endl;
    }
    //Z2.7
    ~Mobitel()
    {
        _velicinaEkrana.reset();
        delete _memorija;
        _memorija = nullptr;
        //Uredjaj destruktor se sam pozove;
    }
};
//Z2.8
bool operator == (const Mobitel& m1, const Mobitel& m2)
{
    if (static_cast<Uredjaj>(m1) == static_cast<Uredjaj>(m2))//Preklopljen operator == class Uredjaj;
    //U static_cast ne mozemo & staviti jer u operatoru == kazemo da su const & sto znaci da se nece ti objekti mjenjati a mi ih mjenjamo te time krsimo to pravilo;
        return true;
    if (m1.Cijena == m2.Cijena)
        return true;
    if (CheckIfIsti(m1.OperativniSistem, m2.OperativniSistem))
        return true;
    if (m1.Memorija == m2.Memorija)
        return true;
    return false;
}
bool operator != (const Mobitel& m1, const Mobitel& m2)
{
    if (!(m1 == m2))
        return true;
    return false;
}

class Televizor : public Uredjaj {
    unique_ptr<float> _velicinaEkrana;
    unique_ptr<char> _tipEkrana;
    pair<int, int> _rezolucija;
    vector<const char*> _prikljucci;
public:
    __declspec(property (get = GetVelicinaEkrana, put = SetVelicinaEkrana)) float VelicinaEkrana;
    __declspec(property (get = GetTipEkrana, put = SetTipEkrana)) const char* TipEkrana;
    __declspec(property (get = GetRezolucija, put = SetRezolucija)) pair<int, int> Rezolucija;
    __declspec(property (get = GetPrikljucci, put = SetPrikljucci)) vector<const char*> Prikljucci;

    //Z3.1
    Televizor()//Dflt ctro;
        :Uredjaj()//Dflt ctro;
    {
        _rezolucija = make_pair(0, 0);
    }
    //Z3.2
    Televizor(const char* naziv, int godinaP, float cijena, int garancija,
        float velicinaEkrana, const char* tipEkrana, pair<int, int> rezolucija)//Usr-def ctor;
        :Uredjaj(naziv, godinaP, cijena, garancija),//Usr-def ctor od  base class;
        _rezolucija(rezolucija.first, rezolucija.second)
    {
        _velicinaEkrana = make_unique<float>(velicinaEkrana);
        _tipEkrana = UniqueAlokator(tipEkrana);
    }
    //Z3.3
    Televizor(const Televizor& obj)//Copy ctor;
        :Uredjaj(obj), _rezolucija(obj.Rezolucija),//Copy ctor base class i copy ctor class pair
        _prikljucci(obj.Prikljucci)//Copy ctor class vector;
    {
        _velicinaEkrana = make_unique<float>(obj.VelicinaEkrana);//Ili *obj._velicinaEkrana;
        _tipEkrana = UniqueAlokator(obj.TipEkrana);//Ili obj._tipEkrana.get() kako bi izvukli raw poiner;

    }
    //Z3.4
    Televizor(Televizor&& obj)noexcept//Move ctor;
        :Uredjaj(move(obj)), _rezolucija(move(obj._rezolucija)), _prikljucci(move(obj._prikljucci)),//Move ctor base class, move ctor class pair, move ctor clas vector;Ako se koriste propertiji nema onda na rvalue move vidljivih promjena;
        _velicinaEkrana(move(obj._velicinaEkrana)), _tipEkrana(move(obj._tipEkrana))//Move ctor unique smart ptr,moraju imati tj vratiti unique ptr a ne raw ptr, stoga  ne koristimo propertije za njih;
    {
        obj._rezolucija = make_pair(0, 0);//Radi nuliranja vrijednosti;
    }

    float GetVelicinaEkrana()const
    {
        assert(_velicinaEkrana != nullptr && "Greska u Televizor:: GetVelicinaEkrana");//Ako nije nullptr nastavi ako jeste, terminira kod;
        return *_velicinaEkrana;
    }
    const char* GetTipEkrana()const { return _tipEkrana.get(); }//Get() da bi vratili raw ptr;
    pair<int, int> GetRezolucija()const { return _rezolucija; }
    vector<const char*> GetPrikljucci()const { return _prikljucci; }

    void SetVelicinaEkrana(float velicinaEkrana)
    {
        if (_velicinaEkrana == nullptr)
            _velicinaEkrana = make_unique<float>();
        *_velicinaEkrana = velicinaEkrana;
    }
    void SetTipEkrana(const char* tipEkrana)
    {
        _tipEkrana = UniqueAlokator(tipEkrana);
    }
    void SetRezolucija(pair<int, int> rezolucija)
    {
        _rezolucija = rezolucija;//Operator = class pair;
    }
    void SetPrikljucci(vector<const char*>prikljucci)
    {
        _prikljucci = prikljucci;//Operator = class vector;
    }

    //Z3.5
    Televizor& operator = (const Televizor& obj)
    {
        if (this == &obj)
            return *this;
        static_cast<Uredjaj&>(*this) = static_cast<Uredjaj>(obj);//Ili samo obj;
        VelicinaEkrana = obj.VelicinaEkrana;
        Rezolucija = obj.Rezolucija;//Operator = class pair;
        Prikljucci = obj.Prikljucci;//Operator = class vector;
        TipEkrana = obj.TipEkrana;
        return *this;
    }
    void operator+=(const char* prikljucak)
    {
        _prikljucci.push_back(prikljucak);//Implementirana metoda class vector za dodavanje na kraj niza;
    }

    //Z3.6
    //Ispisati id, naziv TV-a, velicinu ekrana, tip ekrana, rezoluciju, cijenu, broj godina garancije, te sve prikljucke
    void Ispis()
    {
        cout << "Id: " << Id << endl;
        cout << "Naziv: " << Naziv << endl;
        cout << "Velicina ekrana: " << VelicinaEkrana << "  incha" << endl;
        cout << "Tip ekrana: " << TipEkrana << endl;
        cout << "Rezolucija: " << Rezolucija.first << "x" << Rezolucija.second << endl;//Jer je pair;
        cout << "Cijena: " << Cijena << " KM" << endl;
        cout << "Garancija: " << Garancija << " godina" << endl;
        cout << "Prikljucci: ";
        for (int i = 0; i < Prikljucci.size(); i++)
        {
            cout << Prikljucci[i] << ",";
        }
        cout << endl;
    }
    //Z3.7
    ~Televizor()
    {
        _tipEkrana.reset();
        _velicinaEkrana.reset();//Nema potrebe ali dobra je praksa;
    }
};

bool operator == (const Televizor& t1, const Televizor& t2)
{
    if (static_cast<Uredjaj>(t1) == static_cast<Uredjaj>(t2))//Operator == class Uredjaj;
        return true;
    if (CheckIfIsti(t1.TipEkrana, t2.TipEkrana))
        return true;
    if (t1.VelicinaEkrana == t2.VelicinaEkrana)
        return true;
    if (t1.Rezolucija == t2.Rezolucija)//Operator == class pair;
        return true;
    if (t1.Prikljucci == t2.Prikljucci)//Operator == za dva vectora;
        return true;
    return false;
}
bool operator != (const Televizor& t1, const Televizor& t2)
{
    if (!(t1 == t2))
        return true;
    return false;
}

class Printer : public Uredjaj {
    int* _brzinaPrintanja; // br. str. po minuti
public:
    __declspec(property (get = GetBrzinaPrintanja, put = SetBrzinaPrintanja)) int BrzinaPrintanja;
    //Z4.1
    Printer()//Dflt ctor;
        :Uredjaj()//Dflt ctor;
    {
        _brzinaPrintanja = nullptr;
    }
    //Z4.2
    Printer(const char* naziv, int godinaP, float cijena, int garancija, int brzinaPrintanja)//Usr-def ctor;
        :Uredjaj(naziv, godinaP, cijena, garancija)//Usr-def ctor od base class;
    {
        _brzinaPrintanja = new int(brzinaPrintanja);
    }
    //Z4.3
    Printer(const Printer& obj) //Copy ctor;
        :Uredjaj(obj)//Copy ctor base class;
    {
        _brzinaPrintanja = new int(*obj._brzinaPrintanja);//Ili property;
    }
    //Z4.4
    Printer(Printer&& obj)noexcept//Move ctor
        :Uredjaj(move(obj))//Move ctor base class;
    {
        _brzinaPrintanja = obj._brzinaPrintanja;
        obj._brzinaPrintanja = nullptr;
    }
    int GetBrzinaPrintanja() const
    {
        assert(_brzinaPrintanja != nullptr && "Greska u Printer:: GetBrzinaPrintanja");//Nastavi ako nije nullptr a ako jeste terminira proces;
        return *_brzinaPrintanja;
    }
    void SetBrzinaPrintanja(int brzinaPrintanja)
    {
        if (_brzinaPrintanja == nullptr)
            _brzinaPrintanja = new int();
        *_brzinaPrintanja = brzinaPrintanja;
    }
    //Z4.5
    Printer& operator= (const Printer& obj)
    {
        if (this == &obj)
            return *this;
        static_cast<Uredjaj&>(*this) = static_cast<Uredjaj>(obj);//Ili samo = obj;
        BrzinaPrintanja = obj.BrzinaPrintanja;
        return *this;
    }
    //Z4.6
    //Ispisati id, naziv, brzinu printanja, cijenu te broj godina garancije
    void Ispis()
    {
        cout << "Naziv: " << Naziv << endl;
        cout << "Brzina printanja: " << BrzinaPrintanja << endl;
        cout << "Cijena: " << Cijena << " KM" << endl;
        cout << "Garancija: " << Garancija << " godina" << endl;
    }
    //Z4.7
    ~Printer()
    {
        delete _brzinaPrintanja;
        _brzinaPrintanja = nullptr;
    }
};
//Z4.8
bool operator == (const Printer& p1, const Printer& p2)
{
    if (static_cast<Uredjaj>(p1) == static_cast<Uredjaj>(p2))//Preklopljen operator == class Uredjaj;
        return true;
    if (p1.BrzinaPrintanja == p2.BrzinaPrintanja)
        return true;
    return false;
}
bool operator != (const Printer& p1, const Printer& p2)
{
    if (!(p1 == p2))
        return true;
    return false;
}

class WebShop {
    char* _naziv;
    char _emailAdresa[100];
    int _trenutnoUredjaja;
    Uredjaj* _uredjaji[100] = { nullptr };//Pokazivac na niz pokazivaca class Uredjaj (a posto je ona parent class svima oni mogu pokazivati na bilo koju child klasu);
public:
    __declspec(property (get = GetNaziv, put = SetNaziv)) const char* Naziv;
    __declspec(property (get = GetEmailAdresa, put = SetEmailAdresa)) const char* EmailAdresa;
    __declspec(property (get = GetTrenutnoUredjaja)) int TrenutnoUredjaja;

    //Z5.1
    WebShop()//Dflt ctor;
    {
        _naziv = nullptr;
        strcpy_s(_emailAdresa, 100, " ");
        _trenutnoUredjaja = 0;//Brojac;
    }
    //Z5.2
    WebShop(const char* naziv, const char* emailAdresa)//Usr-def ctor;
    {
        _naziv = Alokator(naziv);
        strcpy_s(_emailAdresa, 100, emailAdresa);
        _trenutnoUredjaja = 0;//Brojac;
    }
    //Z5.3
    WebShop(const WebShop& obj)//Copy ctor;
    {
        _naziv = Alokator(obj._naziv);//Ili obj.Naziv;
        strcpy_s(_emailAdresa, 100, obj.EmailAdresa);//Ili obj._emailAdresa;
        _trenutnoUredjaja = obj._trenutnoUredjaja;
        for (int i = 0; i < _trenutnoUredjaja; i++)
        {
            //Za svaki moguci slucaj pravimo pointer i alociramo prostor sa njime:
            //Nepisano pravilo je da idemo od najizvedenije klase;
            Mobitel* mob = dynamic_cast<Mobitel*>(_uredjaji[i]);//Vrati nullptr ako nema izvedene klase, a ako ima vrati ptr  te izvedene klase;
            if (mob != nullptr)
            {
                _uredjaji[i] = new Mobitel(*mob);
                continue;//Izadje iz petlje i ide dalje, jer je pogodjen odredjeni slucaj;
            }
            Televizor* tel = dynamic_cast<Televizor*>(_uredjaji[i]);
            if (tel != nullptr)
            {
                _uredjaji[i] = new Televizor(*tel);
                continue;//Izadje iz petlje i ide dalje, jer je pogodjen odredjeni slucaj;
            }
            //Bolje je koristiti auto jer je vec prepoznat  povratni tip;A i ovime se ne duplicira typename;
            auto* print = dynamic_cast<Printer*>(_uredjaji[i]);
            if (print != nullptr)
            {
                _uredjaji[i] = new Printer(*print);
                continue;//Izadje iz petlje i ide dalje, jer je pogodjen odredjeni slucaj;
            }
            //Zadnji slucaj jeste da to bude bazna klasa, sto u sustini moze svaka izvedena klasa (da bude bazna);
            auto* uredjaj = dynamic_cast<Uredjaj*>(_uredjaji[i]);
            if (uredjaj != nullptr)
            {
                _uredjaji[i] = new Uredjaj(*uredjaj);
                continue;//Iako je nepotrebno jer je zadnji slucaj;
            }
        }
    }
    //Z5.4
    WebShop(WebShop&& obj)noexcept//Move ctor;
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
        strcpy_s(_emailAdresa, 100, obj._emailAdresa);
        strcpy_s(obj._emailAdresa, 100, " ");//Nepotrebno ali je dobra praksa;
        _trenutnoUredjaja = obj._trenutnoUredjaja;
        obj._trenutnoUredjaja = 0;
        for (int i = 0; i < _trenutnoUredjaja; i++)
        {
            _uredjaji[i] = obj._uredjaji[i];//Moguce je da odredjeni tip sada ne bude podudaran ali tako je sada implementirano rjesenje;
            obj._uredjaji[i] = nullptr;
        }
    }

    const char* GetNaziv() const { return _naziv; }
    const char* GetEmailAdresa() const { return _emailAdresa; }
    int GetTrenutnoUredjaja() const { return _trenutnoUredjaja; }
    Uredjaj* GetElementAt(int index) const { return _uredjaji[index]; }

    void SetNaziv(const char* naziv)
    {
        delete[] _naziv;
        _naziv = Alokator(naziv);
    }
    void SetEmailAdresa(const char* emailAdresa)
    {
        strcpy_s(_emailAdresa, 100, emailAdresa);
    }
    //Z5.5
    WebShop& operator = (const WebShop& obj)
    {
        if (this == &obj)
            return *this;
        Naziv = obj.Naziv;
        EmailAdresa = obj.EmailAdresa;
        for (int i = 0; i < _trenutnoUredjaja; i++)
        {
            delete _uredjaji[i];//Dtor odredjenog uredjaja (mob,tel etc...).Ovo je moguce jer je base class Dtor virtual;
            _uredjaji[i] = nullptr;
        }
        _trenutnoUredjaja = obj._trenutnoUredjaja;
        for (int i = 0; i < _trenutnoUredjaja; i++)
        {
            //Ponovno je potrebno ispitati o kojoj child class se radi:
            auto* mob = dynamic_cast<Mobitel*>(_uredjaji[i]);
            if (mob != nullptr)
            {
                _uredjaji[i] = new Mobitel(*mob);
                continue;
            }
            auto* tel = dynamic_cast<Televizor*>(_uredjaji[i]);
            if (tel != nullptr)
            {
                _uredjaji[i] = new Televizor(*tel);
                continue;
            }
            auto* print = dynamic_cast<Printer*>(_uredjaji[i]);
            if (print != nullptr)
            {
                _uredjaji[i] = new Printer(*print);
                continue;
            }
            auto* uredjaj = dynamic_cast<Uredjaj*>(_uredjaji[i]);
            if (uredjaj != nullptr)
            {
                _uredjaji[i] = new Uredjaj(*uredjaj);
                continue;//Iako je bezpotrebno jer se svakako doslo do zadnjeg slucaja;
            }
        }
        return *this;
    }
    //Z5.6 :: Dodavanje novog elementa u niz pokazivaca
    template <class T>
    bool DodajUredjaj(T uredjaj)
    {
        if (_trenutnoUredjaja == 100)
            return false;//Ne moze se vise dodavati, dostigao se max limit niza;
        _uredjaji[_trenutnoUredjaja] = new T(uredjaj);//Copy ctor class T;(mob,tel,print etc...);
        _trenutnoUredjaja++;
        return true;
    }
    //Z5.7
    ~WebShop()
    {
        delete[]_naziv;
        _naziv = nullptr;
        for (int i = 0; i < _trenutnoUredjaja; i++)
        {
            delete _uredjaji[i];//Pozove se adekvatni dtor child klase, jer je bazna Uredjaj (parent);
            _uredjaji[i] = nullptr;
        }
    }
    friend ostream& operator << (ostream& COUT, const WebShop& webShop);
};
//Z5.8
ostream& operator << (ostream& COUT, const WebShop& obj)
{
    COUT << "Naziv Webshop-a: " << obj.Naziv << endl;
    COUT << "Email adresa: " << obj.EmailAdresa << endl;
    COUT << "Uredjaji u ponudi: " << endl;
    for (int i = 0; i < obj.TrenutnoUredjaja; i++)
    {
        cout << "---------------------------------------------\n";
        //obj._uredjaji[i]->Ispis();
        //Ili:
        obj.GetElementAt(i)->Ispis();//Pozove se ispis za svaki pokazivac, shodno tome koje je child klase;
    }
    return COUT;
}
//Z5.9
bool operator == (const WebShop& shop1, const WebShop& shop2)
{
    if (CheckIfIsti(shop1.Naziv, shop2.Naziv))
        return true;
    if (shop1.TrenutnoUredjaja == shop2.TrenutnoUredjaja)
        return true;
    if (CheckIfIsti(shop1.EmailAdresa, shop2.EmailAdresa))
        return true;
    for (int i = 0; i < shop1.TrenutnoUredjaja; i++)//Svejedno je do kojeg trenutno idemo jer se podrazumijeva da su isti;
    {
        if (*shop1.GetElementAt(i) == *shop2.GetElementAt(i))//operator == za specific uredjaj;DEREFERENCIRANJE JE OBAVEZNO KAKO BI SE POREDILE VRIJEDNOSTI A NE ADRESE;
            return true;
    }
    return false;
}


auto IspisiPoruku = [](const char* tipFunkcije, bool success) {
    cout << tipFunkcije << " se izvrsio " << ((success) ? "uspjesno!" : "bezuspjesno!") << endl;
};
void Zadatak1() {
    //Generator ocjena
    const int max = 20;
    vector<int> ocjene;
    for (int i = 0; i < max; i++)
        ocjene.push_back(rand() % 5 + 1);
    //Testiranje klase 'Uredjaj'
    Uredjaj monitor;
    monitor.Naziv = "LED monitor";
    monitor.GodinaProizvodnje = 2017;
    monitor.Cijena = 250;
    monitor.Garancija = 2;
    monitor.Ocjene = ocjene;
    monitor.Ispis();
    cout << endl;
    Uredjaj monitor2("LED monitor", 2017, 250, 2);
    monitor2.Ocjene = monitor.Ocjene;
    IspisiPoruku("User-def. ctor", monitor == monitor2);
    Uredjaj monitor3(monitor);
    IspisiPoruku("Copy ctor", monitor == monitor3);
    Uredjaj monitor4(move(monitor2));
    IspisiPoruku("Move ctor", monitor == monitor4);
    Uredjaj monitor5;
    monitor5 = monitor;
    IspisiPoruku("Operator '='", monitor == monitor5);
    auto aritmetickaSredina = [](vector<int> brojevi) {
        float suma = 0.0f;
        if (brojevi.size() == 0)
            return suma;
        for (size_t i = 0; i < brojevi.size(); i++)
            suma += brojevi[i];
        return suma / brojevi.size();
    };
    cout << "Prosjecna ocjena: " << monitor.GetProsjecnaOcjena(aritmetickaSredina) << endl;
    cout << endl << "Dealokacija..." << endl;
}
//Zadatak1 passed the test;
void Zadatak2() {
    Mobitel ironPhone;
    ironPhone.Naziv = "Iron Phone S Ultra";
    ironPhone.GodinaProizvodnje = 2021;
    ironPhone.Cijena = 1200;
    ironPhone.Garancija = 2;
    ironPhone.OperativniSistem = "Onyx";
    ironPhone.VelicinaEkrana = 6.1f;
    ironPhone.Memorija = 16;
    ironPhone.Ispis();
    cout << endl;
    Mobitel ironPhone2("Iron Phone S Ultra", 2021, 1200, 2, "Onyx", 6.1f, 16);
    IspisiPoruku("User-def. ctor", ironPhone == ironPhone2);
    Mobitel ironPhone3(ironPhone);
    IspisiPoruku("Copy ctor", ironPhone == ironPhone3);
    Mobitel ironPhone4(move(ironPhone2));
    IspisiPoruku("Move ctor", ironPhone == ironPhone4);
    Mobitel ironPhone5;
    ironPhone5 = ironPhone;
    IspisiPoruku("Operator '='", ironPhone == ironPhone5);
    cout << endl << "Dealokacija..." << endl;
}
//Zadatak2 passed the test;
void Zadatak3() {
    vector<const char*> prikljucci;
    prikljucci.push_back("USB-C");
    prikljucci.push_back("HDMI");
    prikljucci.push_back("S-Video");
    Televizor chromethius;
    chromethius.Naziv = "Chromethius";
    chromethius.GodinaProizvodnje = 2020;
    chromethius.Cijena = 700;
    chromethius.Garancija = 2;
    chromethius.VelicinaEkrana = 43.5;
    chromethius.TipEkrana = "LED";
    chromethius.Rezolucija = pair<int, int>(3840, 2160);
    chromethius.Prikljucci = prikljucci;
    chromethius.Ispis();
    cout << endl;
    Televizor chromethius2("Chromethius", 2020, 700, 2, 43.5, "LED", pair<int, int>(3840, 2160));
    chromethius2.Prikljucci = chromethius.Prikljucci;
    IspisiPoruku("User-def. ctor", chromethius == chromethius2);
    Televizor chromethius3(chromethius);
    IspisiPoruku("Copy ctor", chromethius == chromethius3);
    Televizor chromethius4(move(chromethius2));
    IspisiPoruku("Move ctor", chromethius == chromethius4);
    Televizor chromethius5;
    chromethius5 = chromethius;
    IspisiPoruku("Operator '='", chromethius == chromethius5);
    cout << endl << "Dealokacija..." << endl;
}
//Zadatak3 passed the test;
void Zadatak4() {
    Printer optomec;
    optomec.Naziv = "Optomec X-40";
    optomec.GodinaProizvodnje = 2021;
    optomec.Cijena = 550;
    optomec.Garancija = 2;
    optomec.BrzinaPrintanja = 30;
    optomec.Ispis();
    //Testiranje user-def. ctora
    Printer optomec2("Optomec X-40", 2021, 550, 2, 30);
    IspisiPoruku("User-def. ctor", optomec == optomec2);
    //Testiranje copy ctora
    Printer optomec3(optomec);
    IspisiPoruku("Copy ctor", optomec == optomec3);
    //Testiranje move ctora
    Printer optomec4(move(optomec2));
    IspisiPoruku("Move ctor", optomec == optomec4);
    //Testiranje operatora '='
    Printer optomec5;
    optomec5 = optomec;
    IspisiPoruku("Operator '='", optomec == optomec5);
    cout << endl << "Dealokacija..." << endl;
}
//Zadatak4 passed the test;
void Zadatak5() {
    //WebShop
    WebShop bosnianVortex;
    bosnianVortex.Naziv = "Bosnian Vortex";
    bosnianVortex.EmailAdresa = "bosnian.vortex@gmail.com";
    //Uredjaji
    Uredjaj napojna("Corsair Napojna jedinica", 2018, 159.0, 2);
    Mobitel wikkoJerry("WIKO JERRY 3 ANTHRACITE", 2019, 149.0, 2, "Android 8.1", 5.45f, 16);
    Televizor phillipsTV("PHILIPS LED TV AMBILIGHT", 2019, 899.0, 3, 43.0, "LED", pair<int, int>(3840, 2160));
    Printer officeJet("B-HP OFFICEJET 202", 2019, 549.0, 2, 20);
    //Dodavanje razlicitih tipova uredjaja
    bosnianVortex.DodajUredjaj<Uredjaj>(napojna); //Mozete izostaviti naziv generickog tipa tj: bosnianVortex.DodajUredjaj(napojna);
    bosnianVortex.DodajUredjaj<Mobitel>(wikkoJerry);
    bosnianVortex.DodajUredjaj<Televizor>(phillipsTV);
    bosnianVortex.DodajUredjaj<Printer>(officeJet);
    //Testiranje user-def. ctora
    WebShop bosnianVortex2("Bosnian Vortex", "bosnian.vortex@gmail.com");
    bosnianVortex2.DodajUredjaj(napojna);
    bosnianVortex2.DodajUredjaj(wikkoJerry);
    bosnianVortex2.DodajUredjaj(phillipsTV);
    bosnianVortex2.DodajUredjaj(officeJet);
    cout << bosnianVortex << endl;
    IspisiPoruku("User-def. ctor", bosnianVortex2 == bosnianVortex);
    //Testiranje copy ctora
    WebShop bosnianVortex3(bosnianVortex);
    IspisiPoruku("Copy ctor", bosnianVortex3 == bosnianVortex);
    //Testiranje move ctora
    WebShop bosnianVortex4(move(bosnianVortex2));
    IspisiPoruku("Move ctor", bosnianVortex4 == bosnianVortex);
    //Testiranje operatora '='
    WebShop bosnianVortex5;
    bosnianVortex5 = bosnianVortex;
    IspisiPoruku("Operator '='", bosnianVortex5 == bosnianVortex);
    cout << endl << "Dealokacija..." << endl;
}
//Zadatak5 passed the test;
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
            cout << "Unesite odgovarajuci broj zadatka za testiranje: -->: ";
            cin >> izbor;
            cout << endl;
        } while (izbor < 1 || izbor > 5);
        switch (izbor) {
        case 1: Zadatak1(); cout << "Zadatak 1. Done." << endl; break;
        case 2: Zadatak2(); cout << "Zadatak 2. Done." << endl; break;
        case 3: Zadatak3(); cout << "Zadatak 3. Done." << endl; break;
        case 4: Zadatak4(); cout << "Zadatak 4. Done." << endl; break;
        case 5: Zadatak5(); cout << "Zadatak 5. Done." << endl; break;
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