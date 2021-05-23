#include <cassert>
#include <iostream>
#include <exception>
#include <functional>
using namespace std;
int Max(int a, int b) { return (a >= b) ? a : b; }
int Min(int a, int b) { return (a <= b) ? a : b; }
//Z0.1
bool CheckIfIsti(const char* str1, const char* str2) {
    if (str1 == nullptr || str2 == nullptr)
        return false;
    if (strcmp(str1, str2) == 0)
        return true;
    return false;
}
//Z0.2
char* Alokator(const char* str)
{
    if (str == nullptr)
        return nullptr;
    unsigned int size = strlen(str) + 1;
    char* noviStr = new char[size];
    strcpy_s(noviStr, size, str);
    return noviStr;
}
//Z0.3
unique_ptr<char> UniqueAlokator(const char* str)
{
    if (str == nullptr)
        return nullptr;
    unsigned int size = strlen(str) + 1;
    unique_ptr<char> noviStr(new char[size]);
    strcpy_s(noviStr.get(), size, str);//get() kako bi dobili raw ptr iz smart ptr-a;
    return noviStr;
}

class Greska : public exception {
    int _linija;
    const char*  _funkcija;
public:
    _declspec (property (get = GetPoruka)) const char* Poruka;
    _declspec (property (get = GetLinija, put = SetLinija)) int Linija;
    _declspec (property (get = GetFunkcija, put = SetFunkcija)) const char* Funkcija;
    //Z0.4
    Greska(const char* poruka, int linija, const char* funkcija)//Dflt & Usr-def ctor;
        :exception(poruka)//Usr-def ctor base class;
    {
        _linija = linija;
        _funkcija = Alokator(funkcija);
    }
    //Z0.5
    Greska(const Greska& obj)//Copy ctor;
        :exception(obj)//Copy ctor base class;
    {
        _linija = obj.Linija;
        _funkcija = Alokator(obj.Funkcija);//Koristimo propertije jer je rvalue alociran;
    }
    //Z0.6
    //noexcept stavljamo kada znamo da metoda (ctor) u ovom slucaju, nece u sebi baciti nikakve izuzetke tj. garantujemo da ce se izvrsiti do kraja. Ovo je karakteristicno za Move ctor jer on ako bi se prekinuo dolazi se do gubitka podataka stoga mora biti noexcept;
    Greska(Greska&& obj) noexcept//Move ctor;
        :exception(move(obj))//Move ctor base class;
    {
        _linija = obj._linija;
        obj._linija = 0;
        _funkcija = obj._funkcija;
        obj._funkcija = nullptr;
        //Ne koristimo propertije jer je move ctor; Moramo raditi sa pravim adresama varijabli, ne sa kopijama;
    }
    //Z0.7
    const char* GetPoruka() const { return exception::what(); }//Posto smo nasljedili class exception imamo pristup metodi what();
    int GetLinija() const { return _linija; }
    const char* GetFunkcija() const { return _funkcija; }
    //Z0.8
    void SetLinija(int linija) { _linija = linija; }
    void SetFunkcija(const char* funkcija)
    {
        delete[]_funkcija;
        _funkcija = Alokator(funkcija);
    }
    //Z0.9
    Greska& operator =(const Greska& obj)
    {
        if (this == &obj)//Ako su oba objekta na istim memorijskim lokacijama, vrati obj koji je pozvao metodu;
            return *this;
        static_cast<exception&>(*this) = static_cast<exception>(obj);//Ili samo obj;Preklopljen operator = class exception;
        Linija = obj.Linija;
        Funkcija = obj.Funkcija;
        return  *this;//Vrati obj koji je pozvao metodu;
    }
    //Z0.10
    ~Greska() override//Jer je base class virtuelna samim time joj je virtuelan i Dtor;
    {
        delete[]_funkcija;
        _funkcija = nullptr;
    }
};
//Z0.11
ostream& operator <<(ostream& COUT, const Greska& obj)
{
    COUT << "Poruka: " << obj.what() << endl;
    COUT << "Linija: " << obj.Linija << endl;
    COUT << "Funkcija: " << obj.Funkcija << endl;
    return COUT;
}
template<class T>
class Vektor {
private:
    int _trenutno;//Brojac trenutno el;
    int _maxElemenata;//Velicina niza;
    T* _elementi;//Niz;
public:
    _declspec(property (get = GetTrenutno)) int Length;
    //Z1.1
    Vektor(int maxElemenata = 10)//Dflt & Usr-def ctor;
    {
        _maxElemenata = maxElemenata;
        _trenutno = 0;
        _elementi = new T[_maxElemenata];//Alociramo niz;
    }
    //Z1.2
    Vektor(const Vektor<T>& obj)//Copy ctor;
    {
        _trenutno = obj._trenutno;//Mozemo korisiti i property Length;
        _maxElemenata = obj._maxElemenata;
        _elementi = new T[_maxElemenata];//Alociramo niz;
        assert(_trenutno <= _maxElemenata);//Tvrdim da je _trenutno<_maxElemenata;Ovo radimo kako bi se izbjegao warning kompajlera jer misli da je _trenutno vece od _maxElemenata;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = obj._elementi[i];//Prekopiramo vrijednosti;
        }
    }
    //Z1.3
    Vektor(Vektor<T>&& obj)noexcept//Move ctor;
    {
        _maxElemenata = obj._maxElemenata;
        _trenutno = obj._trenutno;
        obj._trenutno = 0;//Trenutno resetujemo na 0, jer mozda nekad poslije ce se koristiti taj Vektor i nece imati elemenata jer smo ih pobrisali pa mora i brojac _trenutno biti 0;Velicina nije bitna da se resetuje;
        _elementi = obj._elementi;
        obj._elementi = nullptr;
    }
    //Z1.4
    Vektor<T>& operator = (const Vektor<T>& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_elementi;
        _trenutno = obj._trenutno;
        _maxElemenata = obj._maxElemenata;
        _elementi = new T[_maxElemenata];
        assert(_trenutno <= _maxElemenata);//Tvrdim da je _trenutno<_maxElemenata;Ovo radimo kako bi se izbjegao warning kompajlera jer misli da je _trenutno vece od _maxElemenata;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = obj._elementi[i];
        }
        return *this;
    }
    //Z1.5
    T& operator [] (int index) const
    {
        if (_trenutno == 0)
            throw Greska("Vektor je prazan", __LINE__, __FUNCTION__);
        if (index < 0 || index >= _trenutno)
            throw Greska("Index nije validan", __LINE__, __FUNCTION__);
        return _elementi[index];//Referenca na elemenat na index poziciji;
    }
    T* GetElementi() const { return _elementi; }
    int GetTrenutno()const { return _trenutno; }
    //Z1.6
    void ProsiriVektor(int prosiriZa)
    {
        T* temp = _elementi;//Kreiramo privremeni ptr na niz;
        _maxElemenata = _maxElemenata + prosiriZa;//Nova velicina;
        _elementi = new T[_maxElemenata];//Alociramo novi niz, nove velicine;
        assert(_trenutno <= _maxElemenata);//Tvrdim da je _trenutno<_maxElemenata;Ovo radimo kako bi se izbjegao warning kompajlera jer misli da je _trenutno vece od _maxElemenata;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = temp[i];
        }
        delete[]temp;//Obrisemo privremeni niz;
        temp = nullptr;//Nuliramo ptr na privremeni niz;
    }

    //Z1.7 :: Dodavanje novog elementa 
    //Ukoliko je _elementi == nullptr, uraditi ponovnu alokaciju (_maxElemenata = 10)
    //Ukoliko je _trenutno == _maxElemenata, prosiriti niz za 10
    void operator += (T noviElement)
    {
        if (_elementi == nullptr)//Ovo je moguce zbog move ctor-a. Mozda nekada budemo koristili neki stari niz koji je izbrisan stoga se mora ponovno kreirati;
        {
            _trenutno = 0;
            _maxElemenata = 10;
            _elementi = new T[_maxElemenata];//Alociramo niz;
        }
        if (_trenutno == _maxElemenata)
            ProsiriVektor(10);//Sirimo za 10;
        _elementi[_trenutno] = noviElement;//Operator = class T;
        _trenutno++;
    }
    //Z1.8
    bool UkloniZadnjiElement()
    {
        if (_trenutno == 0)
            return false;
        _trenutno--;//Posto je dinamicki niz samo smanjimo brojac trenutnih el. jer cemo preko starih vrj. samo prepisivati;
        return true;
    }
    //Z1.9
    T* CheckIfSadrzi(T element)
    {
        for (int i = 0; i < _trenutno; i++)
        {
            if (_elementi[i] == element)
                return &_elementi[i];//_elementi[i] vrati vrijednost i-tog clana a nama treba pok na taj clan stoga vratimo njegovu adresu;
        }
        return nullptr;
    }
    //Z1.10
    bool Ukloni(T element)
    {
        if (_elementi == nullptr)
            return false;//Prazan niz;
        auto adresaE = CheckIfSadrzi(element);
        if (adresaE == nullptr)
            return false;//Nije nadjen element;
        auto lokacijaElementa = adresaE - _elementi;//Aritmetika ptr;Oduzmemo dvije adrese;
        for (int i = lokacijaElementa; i < _trenutno - 1; i++)//For petlja krece od nadjenog elementa;
        {
            _elementi[i] = _elementi[i + 1];//Trenutni el. mjenjamo sa narednim;For ide do _trenutno-1 upravo kako zadnji el ne bi bio junkvrijednost;
        }
        _trenutno--;//Uklonili smo jedan el;
        return true;
    }
    //Z1.11
    bool UkloniAt(int index)
    {
        if (_trenutno == 0)
            return false;
        if (index < 0 || index >= _trenutno)
            return false;
        for (int i = index; i < _trenutno - 1; i++)//For petlja krece od index-a (pozicije) na kojem zelimo da maknemo element;
        {
            _elementi[i] = _elementi[i + 1];
        }
        _trenutno--;//Jer je uklonjen jedan element niza;
        return true;
    }
    //Z1.12
    //Map je funkcija viseg reda (higher-order function) koja aplicira datu funkciju 'f' na svaki element niza.
    //Niz :[e1, e2, e3, e4, e5, ..., en]
    //Rezultat => [f(e1), f(e2), f(e3), f(e4), f(e5), ..., f(en)]
    void Map(function<void(T&)> f)
    {
        for (int i = 0; i < _trenutno; i++)
        {
            f(_elementi[i]);
        }
    }
    //Z1.13
    //Filter je funkcija viseg reda (higher-order function) koja aplicira datu filter funkciju 'f' na svaki element niza.
    //Rezultat aplikacije filter funkcije je niz elemenata koji zadovoljavaju uslov (odredjen tom istom funkcijom)
    //Niz :[e1, e2, e3, e4, e5, ..., en]
    //Rezultat => [svaki element niza za kojeg vazi f(x) == true]
    Vektor<T> Filter(function<bool(T&)> f)
    {
        Vektor<T> temp;//Dflt ctor;
        for (int i = 0; i < _trenutno; i++)
        {
            if (f(_elementi[i]))
                temp += _elementi[i];//Preklopljen operator +=;
        }
        return temp;
    }
    //Z1.14
    //Reduce (accumulate) je funkcija viseg reda (higher-order function) zaduzena za agregaciju vrijednosti niza. 
    //Funkcija reduce primjenjuje datu funkciju 'f' izmjedju susjednih elemenata da dobije medjurezultate.
    //----------------------------------------------
    //Primjer: Suma :: f(a,b) = a+b
    //Niz vrijednosti: [5, 1, 3, 4, 2]
    // f(5, 1), 3, 4, 2 => 6, 3, 4, 2
    // f(6, 3), 4, 2 => 9, 4, 2
    // f(9, 4), 2] => 13, 2
    // f(13, 2) => 15
    //----------------------------------------------
    //Primjer: Max :: f(a,b) = (a>=b)?a:b;
    //Niz vrijednosti: [5, 6, 3, 8, 2]
    // f(5, 6), 3, 8, 2 => 6, 3, 8, 2
    // f(6, 3), 8, 2 => 6, 8, 2
    // f(6, 8), 2 => 8, 2
    // f(8, 2) => 8
    //----------------------------------------------
    T Reduce(function<T(T, T)> f)
    {
        if (_trenutno == 0)
            throw Greska("Vektor je prazan!", __LINE__, __FUNCTION__);
        T temp = _elementi[0];//Settujemo neku nasumicnu vrj;
        for (int i = 0; i < _trenutno; i++)
        {
            temp = f(temp, _elementi[i]);//Reducira svakim korakom prosli najreduciraniji (temp) i sljedeci u nizu;
        }
        return temp;
    }
    //Z.15
    ~Vektor()
    {
        delete[]_elementi;
        _elementi = nullptr;
    }
};
//Z1.16
template<class T>
ostream& operator << <>(ostream& COUT, const Vektor<T>& obj)
{
    for (int i = 0; i < obj.Length; i++)
    {
        COUT << obj[i] << endl;
    }
    return COUT;
}
//Z1.17
template<class T>
void Ispis(const Vektor<T>& obj, const char* delimiter = " ")
{
    for (int i = 0; i < obj.Length; i++)
    {
        cout << obj[i] << delimiter;
    }
}
//Z1.18
template<class T>
bool operator == <>(const Vektor<T>& v1, const Vektor<T>& v2)
{
    if (v1.Length == v2.Length)
        return true;
    for (int i = 0; i < v1.Length; i++)//Svejedno je do cije duzine vektora idemo jer  su ovdje isti;
    {
        if (v1[i] == v2[i])
            return true;
    }
    return false;
}
//Z1.19
template<class T>
bool operator != <>(const Vektor<T>& v1, const Vektor<T>& v2)
{
    if (!(v1 == v2))
        return true;
    return false;
}
//Z1.20
template<class T>
bool operator == <>(const Vektor<const char*>& v1, const Vektor<const char*>& v2)
{
    if (v1.Length == v2.Length)
        return true;
    for (int i = 0; i < v1.Length; i++)//Svejedno je do cije duzine vektora idemo jer  su ovdje isti;
    {
        if (CheckIfIsti(v1[i], v2[i]))
            return true;
    }
    return false;
}
//Z1.21
template<class T>
bool operator != <>(const Vektor<const char*>& v1, const Vektor<const char*>& v2)
{
    if (!(v1 == v2))
        return true;
    return false;
}



enum class Spol { Muski = 1, Zenski = 0 };//Enumeracijska klasa;
enum class Boja { Plava, Zelena, Zuta, Crvena, Crna, Bijela };//Enumeracijska klasa;
//Z1.22
ostream& operator << (ostream& COUT, const Boja& obj)
{
    switch (obj)
    {
    case Boja::Plava:COUT << "Plava" << endl; break;
    case Boja::Zelena:COUT << "Zelena" << endl; break;
    case Boja::Zuta:COUT << "Zuta" << endl; break;
    case Boja::Crvena:COUT << "Crvena" << endl; break;
    case Boja::Crna:COUT << "Crna" << endl; break;
    case Boja::Bijela:COUT << "Bijela" << endl; break;
    default:
        break;
    }
    return COUT;
}

class Osoba {
protected:
    const char* _jmbg;
    pair<unique_ptr<char>, unique_ptr<char>> _imePrezime;
    unique_ptr<Spol> _spol;
    float* _visina;
    Boja* _bojaOciju;
    Boja* _bojaKose;
public:
    _declspec(property (get = GetJmbg, put = SetJmbg)) const char* Jmbg;
    _declspec(property (get = GetIme, put = SetIme)) const char* Ime;
    _declspec(property (get = GetPrezime, put = SetPrezime)) const char* Prezime;
    _declspec(property (get = GetSpol, put = SetSpol)) Spol Spol_;
    _declspec(property (get = GetVisina, put = SetVisina)) float Visina;
    _declspec(property (get = GetBojaOciju, put = SetBojaOciju)) Boja BojaOciju;
    _declspec(property (get = GetBojaKose, put = SetBojaKose)) Boja BojaKose;
    //Z3.1
    Osoba()//Dflt ctor;
    {
        _jmbg = nullptr;
        _visina = nullptr;
        _bojaOciju = nullptr;
        _bojaKose = nullptr;
    }
    //Z3.2
    Osoba(const char* jmbg, const char* ime, const char* prezime, Spol spol, float visina, Boja bojaOciju, Boja bojaKose)//Usr-def ctor;
    {
        _jmbg = Alokator(jmbg);
        _imePrezime = make_pair(UniqueAlokator(ime), UniqueAlokator(prezime));//Ili odvojeno sa .first i .second;
        _spol = make_unique<Spol>(spol);//Copy ctor;
        _visina = new float(visina);//Copy ctor;
        _bojaOciju = new Boja(bojaOciju);//Copy ctor;
        _bojaKose = new Boja(bojaKose);//Copy ctor;
    }
    //Z3.3
    Osoba(const Osoba& obj)//Copy ctor;
    {
        //Koristimo propertije jer rvalue postoji;
        _jmbg = Alokator(obj.Jmbg);
        _imePrezime = make_pair(UniqueAlokator(obj.Ime), UniqueAlokator(obj.Prezime));
        _spol = make_unique<Spol>(obj.Spol_);
        _visina = new float(obj.Visina);
        _bojaOciju = new Boja(obj.BojaOciju);
        _bojaKose = new Boja(obj.BojaKose);
    }
    //Z3.4
    Osoba(Osoba&& obj)noexcept //Move ctor;
        :_imePrezime(move(obj._imePrezime)), _spol(move(obj._spol))//Move ctor oba objekta;
    {
        _jmbg = obj._jmbg;
        obj._jmbg = nullptr;
        _visina = obj._visina;
        obj._visina = nullptr;
        _bojaOciju = obj._bojaOciju;
        obj._bojaOciju = nullptr;
        _bojaKose = obj._bojaKose;
        obj._bojaKose = nullptr;
    }
    //Z3.5
    const char* GetJmbg() const { return _jmbg; }
    const char* GetIme() const { return _imePrezime.first.get(); }//.get() da bi dobili raw ptr;
    const char* GetPrezime() const { return _imePrezime.second.get(); }
    Spol GetSpol() const
    {
        if (_spol == nullptr)
            throw Greska("Spol je nullptr!", __LINE__, __FUNCTION__);
        return *_spol;
    }
    float GetVisina() const
    {
        if (_visina == nullptr)
            throw Greska("Visina je nullptr!", __LINE__, __FUNCTION__);
        return *_visina;
    }
    Boja GetBojaOciju() const
    {
        if (_bojaOciju == nullptr)
            throw Greska("Boja ociju je nullptr!", __LINE__, __FUNCTION__);
        return *_bojaOciju;
    }
    Boja GetBojaKose() const
    {
        if (_bojaKose == nullptr)
            throw Greska("Boja kose je nullptr!", __LINE__, __FUNCTION__);
        return *_bojaKose;
    }
    //Z3.6
    void SetJmbg(const char* jmbg)
    {
        delete[]_jmbg;
        _jmbg = Alokator(jmbg);
    }
    void SetIme(const char* ime)
    {
        _imePrezime.first = UniqueAlokator(ime);
    }
    void SetPrezime(const char* prezime)
    {
        _imePrezime.second = UniqueAlokator(prezime);
    }
    void SetSpol(Spol spol)
    {
        if (_spol == nullptr)
            _spol = make_unique<Spol>();//Dflt ctor;
        *_spol = spol;//Ne treba assert jer smo if-om osigurali da nije nullptr;
    }
    void SetVisina(float visina)
    {
        if (_visina == nullptr)
            _visina = new float();//Dflt ctor;
        *_visina = visina;
    }
    void SetBojaOciju(Boja bojaOciju)
    {
        if (_bojaOciju == nullptr)
            _bojaOciju = new Boja();
        *_bojaOciju = bojaOciju;
    }
    void SetBojaKose(Boja bojaKose)
    {
        if (_bojaKose == nullptr)
            _bojaKose = new Boja();
        *_bojaKose = bojaKose;
    }
    //Z3.7
    Osoba& operator = (const Osoba& obj)
    {
        if (this == &obj)
            return *this;
        Jmbg = obj.Jmbg;
        Spol_ = obj.Spol_;
        Ime = obj.Ime;
        Prezime = obj.Prezime;
        Visina = obj.Visina;
        BojaOciju = obj.BojaOciju;
        BojaKose = obj.BojaKose;
        return *this;
    }
    //Z3.8
    ~Osoba()
    {
        delete[]_jmbg;
        _jmbg = nullptr;
        _spol.reset();//Nije potrebno ali je dobra praksa;
        delete _visina;
        _visina = nullptr;
        delete _bojaOciju;
        _bojaOciju = nullptr;
        delete _bojaKose;
        _bojaKose = nullptr;
    }
};
//Z3.9
ostream& operator <<(ostream& COUT, const Osoba& osoba)
{
    COUT << "JMBG: " << osoba.Jmbg << endl;
    COUT << "Ime i prezime: " << osoba.Ime << " " << osoba.Prezime << endl;
    COUT << "Spol: " << ((osoba.Spol_ == Spol::Muski) ? "Musko" : "Zensko") << endl;
    COUT << "Visina (u metrima): " << osoba.Visina << endl;
    COUT << "Boja ociju: " << osoba.BojaOciju;//Preklopljen operator << enum class Boja;
    COUT << "Boja kose: " << osoba.BojaKose;//Preklopljen operator << enum class Boja;
    return COUT;
}
//Z3.10
bool operator == (const Osoba& o1, const Osoba& o2)
{
    if (CheckIfIsti(o1.Jmbg, o2.Jmbg))
        return true;
    if (CheckIfIsti(o1.Ime, o2.Ime) && CheckIfIsti(o1.Prezime, o2.Prezime))
        return true;
    if (o1.Spol_ == o2.Spol_)
        return true;
    if (o1.Visina == o2.Visina)
        return true;
    if (o1.BojaOciju == o2.BojaOciju)
        return true;
    if (o1.BojaKose == o2.BojaKose)
        return true;
    return false;
}
bool operator != (const Osoba& o1, const Osoba& o2)
{
    if (!(o1 == o2))
        return true;
    return false;
}

class Teniser : public Osoba {
protected:
    Vektor<const char*> _osvojeniNaslovi;
    unique_ptr<int> _ranking;
public:
    _declspec(property (get = GetOsvojeniNaslovi, put = SetOsvojeniNaslovi)) Vektor<const char*> OsvojeniNaslovi;
    _declspec(property (get = GetRanking, put = SetRanking)) int Ranking;
    //Z4.1
    Teniser()//Dflt ctor;
        :Osoba()//Dflt ctor base class;
    {

    }
    //Z4.2
    Teniser(const char* jmbg, const char* ime, const char* prezime, Spol spol, float visina, Boja bojaOciju, Boja bojaKose,
        Vektor<const char*> naslovi, int ranking)//Usr-def ctor;
        :Osoba(jmbg, ime, prezime, spol, visina, bojaOciju, bojaKose),//Usr-def ctor base class;
        _osvojeniNaslovi(naslovi)//Copy ctor class Vektor;
    {
        //_osvojeniNaslovi = naslovi;//Ili u header;
        _ranking = make_unique<int>(ranking);
    }
    //Z4.3
    Teniser(const Teniser& obj)//Copy ctor;
        :Osoba(obj),//Copy ctor base class;
        _osvojeniNaslovi(obj.OsvojeniNaslovi)//Copy ctor class Vektor;
    {
        _ranking = make_unique<int>(obj.Ranking);//Korisitimo propertije jer mozemo, rvalue je kreiran;
    }
    //Z4.4
    Teniser(Teniser&& obj)noexcept//Move ctor;
        :Osoba(move(obj)),//Move ctor base class;
        _osvojeniNaslovi(move(obj._osvojeniNaslovi)),//Move ctor class Vektor;
        _ranking(move(obj._ranking))//Move ctor smart ptr;
    {
    }
    //Z4.5
    Vektor<const char*> GetOsvojeniNaslovi() const { return _osvojeniNaslovi; }
    int GetRanking() const
    {
        if (_ranking == nullptr)
            throw Greska("Ranking je nullptr!", __LINE__, __FUNCTION__);
        return *_ranking;
    }
    //Z4.6
    void SetOsvojeniNaslovi(Vektor<const char*> naslovi)
    {
        _osvojeniNaslovi = naslovi;//Operator u class Vektor;
    }
    void SetRanking(int ranking)
    {
        if (_ranking == nullptr)
            _ranking = make_unique<int>();//Dflt ctor;
        *_ranking = ranking;//Ne treba nam assert jer je rjeseno dereferenciranje sa if-om;
    }
    //Z4.7
    Teniser& operator = (const Teniser& obj)
    {
        if (this == &obj)
            return *this;
        //Mozemo koristiti propertije jer oba objekta postoje;
        static_cast<Osoba&>(*this) = static_cast<Osoba>(obj);//Ili samo osoba;Preklopljen operator = base class Osoba;
        OsvojeniNaslovi = obj.OsvojeniNaslovi;//Preklopljen operator = class Vektor;
        Ranking = obj.Ranking;
        return *this;
    }
    //Z4.8
    void DodajOsvojeniNaslov(const char* naslov)
    {
        _osvojeniNaslovi += naslov;//Preklopljen operator += class Vektor;
    }
    //Z4.9
    ~Teniser()
    {
        //Base class ima svoje destruktore kao i Vektor class;
        _ranking.reset();//Nije potrebno ali samo radi dobre prakse;
    }
};
//Z4.10
ostream& operator <<(ostream& COUT, const Teniser& teniser)
{
    COUT << static_cast<Osoba>(teniser);//Ispisemo base class Osoba;
    COUT << "Osvojeni naslovi: " << endl;
    Ispis(teniser.OsvojeniNaslovi, ",");//Operator << class Vektor;Koristena globalna fija za ispis;
    COUT << "Ranking: " << teniser.Ranking << endl;
    return COUT;
}
//Z4.11
bool operator == (const Teniser& t1, const Teniser& t2)
{
    if (static_cast<Osoba>(t1) == static_cast<Osoba>(t2))//Operator == class Osoba;
        return true;
    if (t1.OsvojeniNaslovi == t2.OsvojeniNaslovi)//Preklopljen operator == class Vektor;
        return true;
    if (t1.Ranking == t2.Ranking)
        return true;
    return false;
}
//Z4.12
bool operator != (const Teniser& t1, const Teniser& t2)
{
    if (!(t1 == t2))
        return true;
    return false;
}
class Mec {
protected:
    Teniser* _prvi;
    Teniser* _drugi;
    Vektor<pair<int, int>> _setovi;
public:
    _declspec(property (get = GetPrvi, put = SetPrvi)) Teniser Prvi;
    _declspec(property (get = GetDrugi, put = SetDrugi)) Teniser Drugi;
    _declspec(property (get = GetSetovi, put = SetSetovi)) Vektor<pair<int, int>> Setovi;
    //Z5.1
    Mec() // Dflt ctor;
    {
        _prvi = nullptr;
        _drugi = nullptr;
    }
    //Z5.2
    Mec(Teniser prvi, Teniser drugi)//Usr-def ctorl\;
    {
        _prvi = new Teniser(prvi);//Copy ctor class Teniser;
        _drugi = new Teniser(drugi);//Copy ctor class Teniser;
    }
    //Z5.3
    Mec(const Mec& obj)//Copy ctor;
        :_setovi(obj.Setovi)//Copy ctor class Vektor;Mozemo koristiti propertije jer rvalue postoji;
    {
        _prvi = new Teniser(obj.Prvi);
        _drugi = new Teniser(obj.Drugi);
    }
    //Z5.4
    Mec(Mec&& obj)noexcept//Move ctor;
        :_setovi(move(obj._setovi))//Move ctor class Vektor;
    {
        _prvi = obj._prvi;
        obj._prvi = nullptr;
        _drugi = obj._drugi;
        obj._drugi = nullptr;
    }
    //Z5.5
    Teniser GetPrvi() const
    {
        if (_prvi == nullptr)
            throw Greska("Prvi teniser je nullptr!", __LINE__, __FUNCTION__);
        return *_prvi;
    }
    Teniser GetDrugi() const
    {
        if (_drugi == nullptr)
            throw Greska("Drugi teniser je nullptr!", __LINE__, __FUNCTION__);
        return *_drugi;
    }
    Vektor<pair<int, int>> GetSetovi() const { return _setovi; }
    //Z5.6
    void SetPrvi(Teniser teniser)
    {
        if (_prvi == nullptr)
            _prvi = new Teniser();//Dflt ctor;
        *_prvi = teniser;//Preklopljen operator = class Teniser;Nema potrebe za assert jer smo sa if rijesili sve;
    }
    void SetDrugi(Teniser teniser)
    {
        if (_drugi == nullptr)
            _drugi = new Teniser();
        *_drugi = teniser;//Preklopljen operator = class Teniser;Nema potrebe za assert jer smo sa if rijesili sve;
    }
    void SetSetovi(Vektor<pair<int, int>> setovi)
    {
        _setovi = setovi;//Preklopljen operator = class Vektor;
    }
    //Z5.7
    void DodajSet(int bodovi1, int bodovi2)
    {
        _setovi += make_pair(bodovi1, bodovi2);//Preklopljen operator += class Vektor;
    }
    //Z5.8
    Mec& operator = (const Mec& obj)
    {
        if (this == &obj)
            return *this;
        Prvi = obj.Prvi;//Operator = class Teniser;
        Drugi = obj.Drugi;//Operator = class Teniser;
        Setovi = obj.Setovi;//Operator = class Vektor;
        return *this;
    }
    //Z5.9
    ~Mec()
    {
        delete _prvi;
        _prvi = nullptr;
        delete _drugi;
        _drugi = nullptr;
        //Class Vektor ima svoj destruktor;
    }
};
//Z5.10
ostream& operator <<(ostream& COUT, const Mec& teniskiMec)
{
    COUT << "----------------------------------------------------\n";
    COUT << "Teniser [1]: " << teniskiMec.Prvi;//Ne treba endl jer je operator << preklopljen u class Teniser i vec tu ima endl;
    COUT << "----------------------------------------------------\n";
    COUT << "Teniser [2]: " << teniskiMec.Drugi;
    COUT << "----------------------------------------------------\n";
    COUT << "Rezultat po setovima: " << endl;
    for (int i = 0; i < teniskiMec.Setovi.Length; i++)
    {
        COUT << "[" << teniskiMec.Setovi[i].first << "]" << "-" << "[" << teniskiMec.Setovi[i].second << "]" << endl;//Preklopljen operator << class Vektor;//Operator [] class Vektor;
    }
    COUT << "----------------------------------------------------\n";
    return COUT;
}
//Z5.11
bool operator == (const Mec& m1, const Mec& m2)
{
    if (m1.Prvi == m2.Prvi && m1.Drugi == m2.Drugi)//Preklopljen operator == class Teniser;
        return true;
    if (m1.Setovi.Length == m2.Setovi.Length)
        return true;
    for (int i = 0; i < m1.Setovi.Length; i++)//Svejedno je do cije velicine idemo jer su one iste sada;
    {
        if (m1.Setovi[i].first == m2.Setovi[i].first && m1.Setovi[i].second == m2.Setovi[i].second)//Operator [] class Vektor;
            return true;
    }
    return false;
}
bool operator != (const Mec& m1, const Mec& m2)
{
    if (!(m1 == m2))
        return true;
    return false;
}
auto IspisiPoruku = [](const char* tipFunkcije, bool success) {
    cout << tipFunkcije << " se izvrsio " << ((success) ? "uspjesno!" : "bezuspjesno!") << endl;
};
void Zadatak1() {
    //operacije [za mapiranje]
    auto pomnoziSa3 = [](int& broj) {broj *= 3; };
    auto modulirajSa10 = [](int& broj) {broj %= 10; };
    //operacije [za primjenu filtera]
    auto veciOd5 = [](int& broj) {return broj > 5; };
    auto onlyEven = [](int& broj) {return broj % 2 == 0; };
    //operacije [za primjenu reduce]
    auto sumaBrojeva = [](int broj1, int broj2) {return broj1 + broj2; };

    Vektor<int> slucajniBrojevi;
    for (int i = 0; i < 30; i++)
        slucajniBrojevi += rand() % 40;
    cout << "Svi slucajni brojevi: " << endl;
    Ispis(slucajniBrojevi, " ");
    //Uklanjanje zadnjeg elementa
    cout << endl;
    slucajniBrojevi.UkloniZadnjiElement();
    cout << "Nakon uklanjanja zadnjeg ... " << endl;
    Ispis(slucajniBrojevi, " ");
    cout << endl;
    //Uklanjanje prvog elementa
    slucajniBrojevi.UkloniAt(0);
    cout << "Nakon uklanjanja prvog ... " << endl;
    Ispis(slucajniBrojevi, " ");
    cout << endl;

    //:::::::::::::::::: MAP ::::::::::::::::::
    slucajniBrojevi.Map(pomnoziSa3);
    cout << "Nakon mnozenja sa 3 ... " << endl;
    Ispis(slucajniBrojevi, " ");
    cout << endl;
    slucajniBrojevi.Map(modulirajSa10);
    cout << "Nakon moduliranja sa 10 ... " << endl;
    Ispis(slucajniBrojevi, " ");
    cout << endl;
    //:::::::::::::::::: FILTER ::::::::::::::::::
    cout << "Filtriranje [veci od 5]... " << endl;
    Ispis(slucajniBrojevi.Filter(veciOd5), " ");
    cout << endl;
    cout << "Filtriranje [veci od 5 i parni]... " << endl;
    Ispis(slucajniBrojevi.Filter(veciOd5).Filter(onlyEven), " ");
    cout << endl;
    //:::::::::::::::::: REDUCE ::::::::::::::::::
    cout << "[veci od 5 i parni] --> Suma()" << endl;
    cout << "Suma je: " << slucajniBrojevi.Filter(veciOd5).Filter(onlyEven).Reduce(sumaBrojeva) << endl;
    cout << endl;
    cout << "[veci od 5 i parni] --> Max()" << endl;
    cout << "Max element je: " << slucajniBrojevi.Filter(veciOd5).Filter(onlyEven).Reduce(Max) << endl;
    cout << endl;
    cout << "[veci od 5 i parni] --> Min()" << endl;
    cout << "Min element je: " << slucajniBrojevi.Filter(veciOd5).Filter(onlyEven).Reduce(Min) << endl;
    cout << endl;
    //:::::::::::::::::: COPY CTOR | MOVE CTOR | OPERATOR '=' ::::::::::::::::::
    Vektor<int> v2(slucajniBrojevi);
    IspisiPoruku("Copy ctor", slucajniBrojevi == v2);
    Vektor<int> v3(move(v2));
    IspisiPoruku("Move ctor", slucajniBrojevi == v3);
    Vektor<int> v4;
    v4 = slucajniBrojevi;
    IspisiPoruku("Operator '='", slucajniBrojevi == v4);
    //:::::::::::::::::: Testiranje funkcije Vektor<T>::Ukloni ::::::::::::::::::
    cout << endl << "Testiranje funkcije Vektor<T>::Ukloni" << endl;
    Vektor<int> noviVektor;
    noviVektor += 11;
    noviVektor += 2;
    noviVektor += 5;
    noviVektor += 5;
    noviVektor += 6;
    noviVektor += 7;
    noviVektor += 6;
    noviVektor += 11;
    Ispis(noviVektor, " ");
    int zaUklanjanje = 6;
    noviVektor.Ukloni(zaUklanjanje);
    cout << "Nakon uklanjanja (jedne instance) broja " << zaUklanjanje << endl;
    Ispis(noviVektor, " ");
    zaUklanjanje = 11;
    noviVektor.Ukloni(zaUklanjanje);
    cout << "Nakon uklanjanja (jedne instance) broja " << zaUklanjanje << endl;
    Ispis(noviVektor, " ");
    cout << endl << "Dealokacija..." << endl;
}
//Passed!;
void Zadatak2() {
    Vektor<const char*> rijeci;
    const char* kolekcijaRijeci[] = { "reverenda","nakasljati se","podrugljivo", "trgnuti", "zajapuriti",
        "dozlogrditi", "zamajavati", "upravljati", "doskociti", "kalota" };
    for (size_t i = 0; i < size(kolekcijaRijeci); i++)
        rijeci += kolekcijaRijeci[i];
    Ispis(rijeci, ", ");
    //:::::::::::::::::: COPY CTOR | MOVE CTOR | OPERATOR '=' ::::::::::::::::::
    Vektor<const char*> v2(rijeci);
    IspisiPoruku("Copy ctor", rijeci == v2);
    Vektor<const char*> v3(move(v2));
    IspisiPoruku("Move ctor", rijeci == v3);
    Vektor<const char*> v4;
    v4 = rijeci;
    IspisiPoruku("Operator '='", rijeci == v4);

    //:::::::::::::::::: FILTER ::::::::::::::::::
    auto pocinjeSaZ = [](const char* rijec) { return rijec[0] == 'z'; };
    cout << "Rijeci koje pocinju sa 'Z': " << endl << rijeci.Filter(pocinjeSaZ) << endl;
    //:::::::::::::::::: REDUCE ::::::::::::::::::
    auto getDuzaRijec = [](const char* rijec1, const char* rijec2) { return strlen(rijec1) >= strlen(rijec2) ? rijec1 : rijec2; };
    cout << "Najduza rijec: " << endl << rijeci.Reduce(getDuzaRijec) << endl;
    cout << endl << "Dealokacija..." << endl;
}
//Passed!;
void Zadatak3() {
    Osoba putin;
    putin.Jmbg = "0710952215003";
    putin.Ime = "Vladimir";
    putin.Prezime = "Putin";
    putin.Spol_ = Spol::Muski;
    putin.BojaKose = Boja::Plava;
    putin.BojaOciju = Boja::Plava;
    putin.Visina = 1.68f;
    cout << putin << endl;
    //:::::::::::::::::: COPY CTOR | MOVE CTOR | OPERATOR '=' ::::::::::::::::::
    try
    {
        Osoba o2(putin);
        IspisiPoruku("Copy ctor", putin == o2);
        Osoba o3(move(o2));
        IspisiPoruku("Move ctor", putin == o3);
        Osoba o4;
        o4 = putin;
        IspisiPoruku("Operator '='", putin == o4);
        cout << endl << "Dealokacija..." << endl;
    }
    catch (const Greska& greska)
    {
        cout << greska << endl;
    }
}
//Passed!;
void Zadatak4() {
    Vektor<const char*> titule;
    titule += "Australian Open 2020";
    titule += "Italian Open 2020";
    titule += "Cincinnati Masters 2020";
    Teniser djole;
    djole.Jmbg = "2205986345113";
    djole.Ime = "Novak";
    djole.Prezime = "Djokovic";
    djole.Spol_ = Spol::Muski;
    djole.Visina = 1.88f;
    djole.BojaKose = Boja::Crna;
    djole.BojaOciju = Boja::Zelena;
    djole.OsvojeniNaslovi = titule;
    djole.Ranking = 11063;
    cout << djole << endl;
    //:::::::::::::::::: COPY CTOR | MOVE CTOR | OPERATOR '=' ::::::::::::::::::
    try {
        Teniser t2(djole);
        IspisiPoruku("Copy ctor", djole == t2);
        Teniser t3(move(t2));
        IspisiPoruku("Move ctor", djole == t3);
        Teniser t4;
        t4 = djole;
        IspisiPoruku("Operator '='", djole == t4);
        cout << endl << "Dealokacija..." << endl;
    }
    catch (const Greska& greska)
    {
        cout << greska << endl;
    }
}
//Passed!;
void Zadatak5() {
    Teniser djole;
    djole.Jmbg = "2205986345113";
    djole.Ime = "Novak";
    djole.Prezime = "Djokovic";
    djole.Spol_ = Spol::Muski;
    djole.Visina = 1.88f;
    djole.BojaKose = Boja::Crna;
    djole.BojaOciju = Boja::Zelena;
    djole.Ranking = 11063;
    Teniser nadal;
    nadal.Jmbg = "2304984344414";
    nadal.Ime = "Rafael";
    nadal.Prezime = "Nadal";
    nadal.Spol_ = Spol::Muski;
    nadal.Visina = 1.83f;
    nadal.BojaKose = Boja::Crna;
    nadal.BojaOciju = Boja::Crna;
    nadal.Ranking = 9500;
    Mec finale;
    finale.Prvi = djole;
    finale.Drugi = nadal;
    finale.DodajSet(6, 1);
    finale.DodajSet(5, 7);
    finale.DodajSet(6, 4);
    cout << finale;
    //:::::::::::::::::: COPY CTOR | MOVE CTOR | OPERATOR '=' ::::::::::::::::::
    try
    {
        Mec m2(finale);
        IspisiPoruku("Copy ctor", finale == m2);
        Mec m3(move(m2));
        IspisiPoruku("Move ctor", finale == m3);
        Mec m4;
        m4 = finale;
        IspisiPoruku("Operator '='", finale == m4);
        cout << endl << "Dealokacija..." << endl;
    }
    catch (const Greska& greska)
    {
        cout << greska << endl;
    }
}
//Passed!;
int main() {
    int nastaviDalje = 1;
    while (nastaviDalje) {
        int izbor = 0;
        do {
            system("cls");
            cout << "::Zadaci::" << endl;
            cout << "(1) Zadatak 1" << endl;
            cout << "(2) Zadatak 2" << endl;
            cout << "(3) Zadatak 3" << endl;
            cout << "(4) Zadatak 4" << endl;
            cout << "(5) Zadatak 5" << endl;
            cout << "Unesite broj za testiranje zadatka: -->: ";
            cin >> izbor;
            cout << endl;
        } while (izbor <= 0 || izbor > 5);
        switch (izbor) {
        case 1: Zadatak1(); cout << "Done." << endl; break;
        case 2: Zadatak2(); cout << "Done." << endl; break;
        case 3: Zadatak3(); cout << "Done." << endl; break;
        case 4: Zadatak4(); cout << "Done." << endl; break;
        case 5: Zadatak5(); cout << "Done." << endl; break;
        default:
            break;
        }
        do {
            cout << "DA LI ZELITE NASTAVITI DALJE? (1/0): ";
            cin >> nastaviDalje;
        } while (nastaviDalje != 0 && nastaviDalje != 1);
    }
    return 0;
}
