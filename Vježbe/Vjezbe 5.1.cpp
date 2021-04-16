#include <iostream>
#include <memory>
#include <functional>
using namespace std;

//Z0.1
char* AlocirajIKopiraj(const char* izvor)
{
    if (izvor == nullptr)
        return nullptr;
    int size = strlen(izvor) + 1;
    char* noviTxt = new char[size];
    strcpy_s(noviTxt, size, izvor);
    return noviTxt;
}

//Z0.2 :: Funkcija vraca max. od dva elementa
template<typename T>
T Max(T el1, T el2)
{
    if (el1 > el2)
        return el1;
    return el2;
}

//Z0.3 :: Funkcija vraca min. od dva elementa
template<typename T>
T Min(T el1, T el2)
{
    if (el1 < el2)
        return el1;
    return el2;
}

//Z0.4 :: Ispis niza (tipa T) gdje se delimiter ispisuje izmedju dva susjedna elementa tog niza
template<class T>
void IspisNiza(T niz[], int vel, const char* delimiter = ", ")
{
    for (int i = 0; i < vel; i++)
    {
        cout << niz[i] << delimiter;
    }
    cout << endl;
}

/*Z0.5 :: Napisati genericku funkciju 'BubbleSort' za sortiranje niza elemenata tipa T
       :: Iskoristiti vec postojeci kod za sortiranje niza integera ('BubbleSort.txt') i prepraviti ga
       :: reverse' polje oznacava da li se radi o uzlaznom ili silaznom sortiranju
*/
template<class T>
void BubbleSort(T niz[], int vel, bool reverse = false)
{
    //Lambda fija koja odreduje nacin sortiranja:
    //Auto smo pokazivac koristili jer nam entry lista [] nije prazna;
    auto sort = [&reverse](T& el1, T& el2)//Uzimamo objekte po referenci kako bi se izbjeglo kopiranje a i da imamo kontrolu nad njihovim vrijednostima direktno;
    {
        if (reverse == true)//Ako je u "rikvercu"
            return el1 < el2;//sortiraj niz opadajucim redosljedom,
        return el1 > el2;//Ako nije onda ga sortiraj penjucim redosljedom;
    };
    for (int i = 0; i < vel - 1; i++)//Vel-1 jer je dovoljno da se ide za jedan manje od max velicine, nepotrebno je ici dalje;
    {
        for (int j = 0; j < vel - i - 1; j++)//Idemo do vel-i-1 jer je vazda zadnji el niza sortiran, ovime se optimizira sami sort;
        {
            if (sort(niz[j], niz[j + 1]))
            {
                T temp = niz[j];//Copy ctor za class T;
                niz[j] = niz[j + 1];//Mora postojati preklopljen operator dodjele za class T;
                niz[j + 1] = temp;
            }
        }
    }
}

class Datum
{
private:
    unique_ptr<int> _dan;
    unique_ptr<int> _mjesec;
    unique_ptr<int> _godina;
public:
    //Z1.1
    Datum()
    {
        //_dan = nullptr;
        //_mjesec = nullptr;
        //_godina = nullptr;
        //Mozemo ali je nepotrebno jer su po defaultu nulirani !!!;
    }
    //Z1.2
    Datum(int dan, int mjesec, int godina)//User-def ctor;
    {
        _dan = make_unique<int>(dan);//Make_unique alocira prostor za int i copy.ctor od int se iskoristi;
        _mjesec = make_unique<int>(mjesec);
        _godina = make_unique<int>(godina);
    }
    //Z1.3
    Datum(const Datum& obj)//Copy ctor;
    {
        _dan = make_unique<int>(*obj._dan);
        _mjesec = make_unique<int>(*obj._mjesec);
        _godina = make_unique<int>(*obj._godina);
    }
    //Z1.4
    Datum(Datum&& obj)noexcept //Move ctor;
        :_dan(move(obj._dan)), _mjesec(move(obj._mjesec)), _godina(move(obj._godina))//Ovo moze
    {
        //_dan = obj._dan;//Ne moze ovako jer koristimo unikatne ptr;

        //Ali je best practice koristenje move assignment operatora:
      // _dan = move(obj._dan);
      // _mjesec = move(obj._mjesec);
      // _godina = move(obj._godina);
    }
    //Z1.5
    Datum& operator =(const Datum& obj)
    {
        if (this == &obj)//Ako su objekti na istim adresama vrati objekat koji je pozvao fiju;
            return *this;
        if (_dan == nullptr)//U slucaju da nije alocirana potrebna memorija
            _dan = make_unique<int>();//Alociraj je;
        if (_mjesec == nullptr)
            _mjesec = make_unique<int>();
        if (_godina == nullptr)
            _godina = make_unique<int>();
        *_dan = *obj._dan;
        *_mjesec = *obj._mjesec;
        *_godina = *obj._godina;
        return *this;//Vrati obj koji je pozvao fiju;
    }
    //Z1.6
    void SetDan(int dan)
    {
        if (_dan == nullptr)//Ako nije alocirana memorija
            _dan = make_unique<int>();//Alociraj je;
        *_dan = dan;
    }
    void SetMjesec(int mjesec)
    {
        if (_mjesec == nullptr)//Ako nije alocirana memorija
            _mjesec = make_unique<int>();//Alociraj je;
        *_mjesec = mjesec;
    }
    void SetGodina(int godina)
    {
        if (_godina == nullptr)
            _godina = make_unique<int>();
        *_godina = godina;
    }
    //Z1.7
    int GetDan() const { return *_dan; }
    int GetMjesec() const { return *_mjesec; }
    int GetGodina() const { return *_godina; }
    //Z1.8
    bool CheckIfValid()
    {
        if (*_godina > INT_MAX || *_godina < INT_MIN)//Provjera da se ne bi unjela banalna godina tipa:12354879456...;
            return false;
        if (*_mjesec < 1 || *_mjesec>12)
            return false;
        if (*_dan < 1 || *_dan>31)
            return false;
        //Provjera parnih i neparnih godina:

        bool (*isGodinaParna)(int) = [](int godina)
        {
            if (godina % 4 == 0 && godina % 100 != 0 || (godina % 400 == 0))
                return true;
            return false;
        };

        if (*_mjesec == 2)
        {
            if (isGodinaParna(*_godina))//Da li je parna ili neparna;
            {
                if (*_dan <= 29)//Ako je parna da li ima 29 dana ili manje;
                    return true;
                return false;
            }
            else
            {
                if (*_dan <= 28)//Ako je neparna da li ima 28 dana ili manje;
                    return true;
                return false;
            }

        }
        if (*_dan == 4 || *_dan == 6 || *_dan == 9 || *_dan == 11)
            if (*_dan <= 30)
                return true;
            else
                return false;
        return true;//Finalno je okej formata datum;
    }

    //Z1.9 :: Staticka metoda za generisanje datuma (sa ugradjenom provjerom validnosti tog datuma)
    static Datum GetRandomDatum()
    {
        Datum datum;//Privremena var;
        do
        {
            int dan = rand() % 31 + 1;
            int mjesec = rand() % 12 + 1;
            int godina = rand() % 2020 + 1;
            datum.SetDan(dan);
            datum.SetMjesec(mjesec);
            datum.SetGodina(godina);
        } while (!(datum.CheckIfValid()));//Sve dok nije neki validan datum settovan, ponavljaj generisanje;
        return datum;
    }
    //Z1.10
    ~Datum()
    {
        _dan.reset();//Isto kao da ga mi brisemo i  nuliramo;
        _mjesec.reset();
        _godina.reset();
    }
    friend ostream& operator << (ostream& COUT, const Datum& obj);
    friend bool operator == (const Datum& d1, const Datum& d2);
    friend bool operator > (const Datum&, const Datum&);
};
//Z1.11
bool operator == (const Datum& d1, const Datum& d2)
{
    if (*d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina)
        return true;
    return false;
}
bool operator != (const Datum& d1, const Datum& d2)
{
    if (!(d1 == d2))
        return true;
    return false;
}
//Z1.12
bool operator >(const Datum& d1, const Datum& d2)
{
    if (d1.GetGodina() * 365 + d1.GetMjesec() * 30 + d1.GetDan() > d2.GetGodina() * 365 + d2.GetMjesec() * 30 + d2.GetDan())
        return true;
    return false;
}
bool operator >= (const Datum& d1, const Datum& d2)
{
    if ((d1 > d2) || (d1 == d2))
        return true;
    return false;
}
bool operator <= (const Datum& d1, const Datum& d2)
{
    return !(d1 > d2);
}
bool operator < (const Datum& d1, const Datum& d2)
{
    return !(d1 >= d2);
}

//Z1.13
ostream& operator << (ostream& COUT, const Datum& obj)
{
    if (obj._dan != nullptr && obj._mjesec != nullptr && obj._godina != nullptr)
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << "." << endl;
    return COUT;
}

//Vektor :: Kolekcija elemenata (tipa T) sa mogucnoscu dinamickog prosirivanja
template<class T>
class Vektor {
private:
    int _trenutno;  //brojac;
    int _maxElemenata;//oznacava velicinu niza;
    T* _elementi;
public:
    //Z2.1
    Vektor(int maxElemenata = 10)
    {
        _trenutno = 0;
        _maxElemenata = maxElemenata;
        _elementi = new T[_maxElemenata];//Alociranje niza objekata, dflt ctor class T;
    }
    //Z2.2
    Vektor(const Vektor<T>& kolekcija)//Copy ctor;
    {
        _trenutno = kolekcija._trenutno;
        _maxElemenata = kolekcija._maxElemenata;
        _elementi = new T[_maxElemenata];//Alociranje memorije za novi niz;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = kolekcija._elementi[i];//Operator = class T;
        }
    }
    //Z2.3
    Vektor(Vektor<T>&& kolekcija)//Move ctor
    {
        _trenutno = kolekcija._trenutno;
        kolekcija._trenutno = 0;
        _maxElemenata = kolekcija._maxElemenata;
        //kolekcija._maxElemenata = 0;//Nema smisla jer je vec defaultno settovan u dflt.ctorl
        _elementi = kolekcija._elementi;
        kolekcija._elementi = nullptr;//Nuliranje starog ptr kako ne bi pokazivali na isti niz;
    }
    //Z2.4
    Vektor<T>& operator = (const Vektor<T>& kolekcija)
    {
        if (this == &kolekcija)
            return *this;
        _trenutno = kolekcija._trenutno;
        _maxElemenata = kolekcija._maxElemenata;
        delete[]_elementi;//Brisemo stari niz jer mozda je novi veci;
        _elementi = new T[_maxElemenata];//Alociramo novi, dflt ctor za class T;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = kolekcija._elementi[i];//Operator = za class T;
        }
        return *this;//Vratimo obj koji je pozvao fiju;
    }

    //Z2.5 :: Osigurati da je index unutar opsega [0,_trenutno-1]. Koristiti genericke funkcije 'Min' i 'Max' 
    T& operator [] (int index) const
    {
        index = Min<int>(index, _trenutno - 1);//Vrati najmani od dva;
        index = Max(index, 0);//Vrati najveci od dva, drugi je najveci trenutno moguci;
        //Prva fija je genericki eksplicitno pozvana za int dok je druga implicitno za int;
        return _elementi[index];//Vraca se referenca na obj jer da joj se kasnije moze pristupiti;
    }

    T* GetElementi() const { return _elementi; }
    //Z2.6
    void ProsiriVektor(int prosiriZa)
    {
        T* temp = _elementi;//Kreiramo pok na trenutni niz;
        _maxElemenata += prosiriZa;//Ne moze se npr smjestati nova velicina u neki novi int vec se mora nadodati na stari _maxElemenata kako bi on bio azuran uvijek.
        _elementi = new T[_maxElemenata];//Alociramo novi niz;//Dflt ctor za class T;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = temp[i];//Operator = za class T;
        }
        delete[] temp;
        temp = nullptr;
    }

    //Z2.7 :: Dodavanje novog elementa (ukoliko je _trenutno < _maxElemenata, uraditi prosirivanje niza za 10)
    void operator += (T noviElement)
    {
        if (_elementi == nullptr)//Ako je slucajno bio niz ne-alociran
            _elementi = new T[_maxElemenata];//alociraj ga;

        if (_trenutno == _maxElemenata)
            ProsiriVektor(10);//Prosirimo ukoliko je dostigao max;
        _elementi[_trenutno] = noviElement;//Copy assignment operator class T;
        _trenutno++;
    }

    //Z2.8
    bool UkloniZadnjiElement()
    {
        if (_trenutno == 0)
            return false;//Ne moze se smanjiti ako je nula elemenata u nizu;
        _trenutno--;//U sustini nista ne brisemo jer se pok na niz brise tek u dealokaciji pa onda mozemo samo smanjiti brojac;
        return true;
    }

    //Z2.9
    bool CheckIfSadrzi(T element)
    {
        for (int i = 0; i < _trenutno; i++)
        {
            if (_elementi[i] == element)//Operator == za class T;
                return true;
        }
        return false;
    }

    //Z2.10 :: Sortiranje elemenata na osnovu specificiranog algoritma
    void Sortiraj(void(*algoritamSortiranja)(T[], int, bool), bool reverse = false)
    {
        //Jedan od parametara je vec pokazivac na lambda fiju koja uzima tri parametra, a drugi je bool koji se koristi u lambda fiji;
        algoritamSortiranja(_elementi, _trenutno, reverse);
        //Dakle u samom pozivu se pozove jedan od pokazivaca na lambda funkciju,i neka bool vrijednost dok se parametri dodijele tom pokazivacu u ovoj funkciji;
    }

    //Z2.11
    int GetTrenutno() const { return _trenutno; }
    int GetMaxElemenata() const { return _maxElemenata; }

    //Z.12
    ~Vektor()
    {
        delete[] _elementi;
        _elementi = nullptr;
    }

    friend ostream& operator << <> (ostream& COUT, const Vektor<T>& kolekcija);
};

//Z2.13 :: Ispisuje elemente kolekcije. Za pristup elementima koristiti preklopljeni operator []
template<class T>
ostream& operator << <>(ostream& COUT, const Vektor<T>& kolekcija)
{
    COUT << "Elementi niza su : " << endl;
    for (int i = 0; i < kolekcija._trenutno; i++)
    {
        COUT << kolekcija._elementi[i] << " ";//Preklopljeni operator [] za class T;
    }
    cout << endl;
    return COUT;
}

class Fudbaler {
private:
    char* _ime;
    char* _prezime;
    unique_ptr<Datum> _datumRodjenja;
    float* _visina; // npr. 1.75 m
    char _pozicija[50]; //pozicija na terenu

    int _trenutnoNagrada; //brojac :: krece od 0
    char* _nagrade[100] = { nullptr };//Pokazivac na niz pokazivaca koji su char;
public:
    //Z3.1
    Fudbaler()//Dflt ctor;
    {
        _ime = nullptr;
        _prezime = nullptr;
        _visina = nullptr;
        strcpy_s(_pozicija, 50, " ");//Empty string napravimo;
        _trenutnoNagrada = 0;
    }
    //Z3.2
    Fudbaler(const char* ime, const char* prezime, Datum datum, float visina, const char* pozicija)//Usr-def ctor;

    {
        _ime = AlocirajIKopiraj(ime);
        _prezime = AlocirajIKopiraj(prezime);
        _datumRodjenja = make_unique<Datum>(datum.GetDan(), datum.GetMjesec(), datum.GetGodina());//User def ctor od class Datum;
        _visina = new float(visina);
        strcpy_s(_pozicija, 50, pozicija);
        _trenutnoNagrada = 0;
    }
    //Z3.3
    Fudbaler(const Fudbaler& obj)//Copy ctor;
    {
        _ime = AlocirajIKopiraj(obj._ime);
        _prezime = AlocirajIKopiraj(obj._prezime);
        _datumRodjenja = make_unique<Datum>(*obj._datumRodjenja);//Copy ctor class Datum;
        _visina = new float(*obj._visina);//Copy ctor od float-a;
        strcpy_s(_pozicija, 50, obj._pozicija);
        _trenutnoNagrada = obj._trenutnoNagrada;
        for (int i = 0; i < _trenutnoNagrada; i++)
        {
            _nagrade[i] = AlocirajIKopiraj(obj._nagrade[i]);//Dok se na ovaj nacin ALOCIRA nova memorija za svaki pokazivac sa lside;
            //_nagrade[i] = obj._nagrade[i];//Ovo ne valja jer bi ovime  dva pok pokazivali na iste memorijske lokacije;
        }
    }
    //Z3.4
    Fudbaler(Fudbaler&& obj) noexcept //Move ctor;
        :_datumRodjenja(move(obj._datumRodjenja))//Move ctor koristimo jer ne moze unique ptr pokazivati na neki drugi...;
    {
        _ime = obj._ime;
        obj._ime = nullptr;
        _prezime = obj._prezime;
        obj._prezime = nullptr;
        _visina = obj._visina;
        obj._visina = nullptr;
        strcpy_s(_pozicija, 50, obj._pozicija);
        strcpy_s(obj._pozicija, 50, " ");//Pobrise string;
        _trenutnoNagrada = obj._trenutnoNagrada;
        obj._trenutnoNagrada = 0;
        for (int i = 0; i < _trenutnoNagrada; i++)
        {
            _nagrade[i] = obj._nagrade[i];//Nizovi su vec kod starog alocirani, pa onda samo novi pok pokazuju na stare, jer ce se stari unistiti, nulirati; Operator = za class Fudbaler;
            obj._nagrade[i] = nullptr;//Nuliramo stare pokazivace;
        }

    }
    //Z3.5
    Fudbaler& operator = (const Fudbaler& obj)
    {
        if (this == &obj)
            return *this;
        delete[] _ime;
        delete[] _prezime;
        _ime = AlocirajIKopiraj(obj._ime);
        _prezime = AlocirajIKopiraj(obj._prezime);
        if (_datumRodjenja == nullptr)//Ako je nuliran pokazivac;
            _datumRodjenja = make_unique<Datum>();//Alociraj prostor;
        *_datumRodjenja = *obj._datumRodjenja;//Dereferenciramo ih pa onda izjednacimo, u suprotnom unique ptr ne moze pointat na neki drugi unique ptr;
        if (_visina == nullptr)
            _visina = new float;
        *_visina = *obj._visina;
        strcpy_s(_pozicija, 50, obj._pozicija);
        _trenutnoNagrada = obj._trenutnoNagrada;
        for (int i = 0; i < _trenutnoNagrada; i++)
        {
            delete[]_nagrade[i];//Brisemo pojedinacne nizove svakog pokazivaca;
            _nagrade[i] = nullptr;//Nuliramo ptr;
        }
        for (int i = 0; i < _trenutnoNagrada; i++)
        {
            _nagrade[i] = AlocirajIKopiraj(obj._nagrade[i]);//Vrsi se alokacija na osnovu kopija od starog;
        }
        return *this;//Vratimo objekat koji je pozvao funkciju;
    }

    //Z3.6 :: Dodavanje nove nagrade u niz pokazivaca '_nagrade'. Uraditi alokaciju prilikom dodavanja
    bool operator()(const char* nagrada)
    {
        if (_trenutnoNagrada == 100)
            return false;//Nema se sta dodati;
        _nagrade[_trenutnoNagrada] = AlocirajIKopiraj(nagrada);//Ili new char (*nagrada);
        _trenutnoNagrada++;
        return true;
    }
    //Z3.7
    const char* GetIme() const { return _ime; }
    const char* GetPrezime() const { return _prezime; }
    Datum GetDatumRodjenja() const { return *_datumRodjenja; }
    float GetVisina() const { return *_visina; }
    const char* GetPozicija() const { return _pozicija; }
    int GetTrenutnoNagrada() const { return _trenutnoNagrada; }

    //Z3.8 :: Dohvacanje jednog elementa iz niza '_nagrade'
    const char* operator[] (int index) const
    {
        index = Max<int>(0, index);//Vrati max od dva elementa pri cemu je ako je negativan index dat maximum 0;
        index = Min(index, _trenutnoNagrada - 1);//Vrati min od dva elementa pri cemu ako je index neki ogroman, postace samo trenutni najveci;
        return _nagrade[index];
    }
    //Z3.9
    void SetIme(const char* ime)
    {
        delete[]_ime;
        _ime = AlocirajIKopiraj(ime);
    }
    void SetPrezime(const char* prezime)
    {
        delete[] _prezime;
        _prezime = AlocirajIKopiraj(prezime);
    }
    void SetDatumRodjenja(Datum datumRodjenja)
    {
        if (_datumRodjenja == nullptr)
            _datumRodjenja = make_unique<Datum>();
        *_datumRodjenja = datumRodjenja;
    }
    void SetVisina(float visina)
    {
        if (_visina == nullptr)
            _visina = new float;
        *_visina = visina;
    }
    void SetPozicija(const char* pozicija)
    {
        strcpy_s(_pozicija, 50, pozicija);
    }

    //Z3.10
    ~Fudbaler()
    {
        delete[]_ime;
        _ime = nullptr;
        delete[] _prezime;
        _prezime = nullptr;
        _datumRodjenja.reset();
        delete _visina;
        _visina = nullptr;
        for (int i = 0; i < _trenutnoNagrada; i++)
        {
            delete[]_nagrade[i];//Obirisi niz na i-tom pokazivacu;
            _nagrade[i] = nullptr;//Nuliraj i-ti pokazivac;
        }
    }

    friend ostream& operator <<(ostream& COUT, const Fudbaler& fudbaler);
    friend bool operator == (const Fudbaler& f1, const Fudbaler& f2);
};

//Z3.11
ostream& operator <<(ostream& COUT, const Fudbaler& fudbaler)
{
    COUT << "Ime i prezime : " << fudbaler._ime << " " << fudbaler._prezime << endl;
    COUT << "Datum rodjenja : " << *fudbaler._datumRodjenja << endl;//Preklopljen operator << za class Datum;
    COUT << "Pozicija na terenu : " << fudbaler._pozicija << endl;
    COUT << "-----------------------NAGRADE-------------------------------\n";
    for (int i = 0; i < fudbaler._trenutnoNagrada; i++)
    {
        COUT << fudbaler._nagrade[i] << endl;
    }
    COUT << "---------------------------------------------------------\n";
    return COUT;
}

//Z3.12 :: Poredjenje vrijedosti svih atributa jednog objekta sa vrijednostima atributa drugog objekta
bool operator == (const Fudbaler& f1, const Fudbaler& f2)
{
    if (strcmp(f1._ime, f2._ime) == 0)
        return true;
    if (strcmp(f1._prezime, f2._prezime) == 0)
        return true;
    if (f1._datumRodjenja == f2._datumRodjenja)
        return true;
    if (*f1._visina == *f2._visina)
        return true;
    if (strcmp(f1._pozicija, f1._pozicija) == 0)
        return true;
    if (f1._trenutnoNagrada == f2._trenutnoNagrada)
        return true;
    for (int i = 0; i < f1._trenutnoNagrada; i++)//Podrazumijeva se da su i f1 nagrade i f2 nagrade iste duzine, stoga je svejedno koja se koristi;
    {
        if (strcmp(f1._nagrade[i], f2._nagrade[i]) == 0)//Poredimo pojedinacne stringove;
            return true;
    }
    return false;
}
bool operator != (const Fudbaler& f1, const Fudbaler& f2)
{
    return !(f1 == f2);
}
auto GetRandomBrojIgraca = []() {//Mogao se koristiti i obicni pok na fiju tipa int (*brIgraca)() =[] () {};
    //Ovo je lambda fija koja vraca random int od 1 do 100;
    return rand() % 100 + 1;
};



class FudbalskiTim {
private:
    const char* _naziv;
    Datum _datumOsnivanja;
    char* _domaciStadion;
    Vektor<Fudbaler> _fudbaleri;
    Vektor<int> _brojevi; // brojevi fudbalera na majicama
    function<int()> _rasporedjivac;
public:
    //Z4.1 :: Postaviti defaultne vrijednost
    FudbalskiTim(function<int()> rasporedjivac = GetRandomBrojIgraca)
    {
        _naziv = nullptr;
        _domaciStadion = nullptr;
        _rasporedjivac = rasporedjivac;//Ptr na fiju pokazuje na drugi ptr na fiju;
    }
    //Z4.2
    FudbalskiTim(const char* naziv, Datum datumOsnivanja, const char* domaciStadion, function<int()> rasporedjivac = GetRandomBrojIgraca)
        :_datumOsnivanja(datumOsnivanja.GetDan(), datumOsnivanja.GetMjesec(), datumOsnivanja.GetGodina())//User-def ctor;
    {
        _naziv = AlocirajIKopiraj(naziv);
        //_datumOsnivanja = datumOsnivanja;//Operator dodjele za class Datum;
        _domaciStadion = AlocirajIKopiraj(domaciStadion);
        _rasporedjivac = rasporedjivac;//nema () jer nema parametara nikakvih;
    }
    //Z4.3
    FudbalskiTim(const FudbalskiTim& obj)
        :_datumOsnivanja(obj._datumOsnivanja), _fudbaleri(obj._fudbaleri), _brojevi(obj._brojevi)//Copy.ctor;
    {
        _naziv = AlocirajIKopiraj(obj._naziv);
        _domaciStadion = AlocirajIKopiraj(obj._domaciStadion);
        _rasporedjivac = obj._rasporedjivac;
    }
    //Z4.4
    FudbalskiTim(FudbalskiTim&& obj) noexcept
        :_datumOsnivanja(move(obj._datumOsnivanja)), _fudbaleri(move(obj._fudbaleri)), _brojevi(move(obj._brojevi))//Move ctor;
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
        _domaciStadion = obj._domaciStadion;
        obj._domaciStadion = nullptr;
        _rasporedjivac = obj._rasporedjivac;
    }
    //Z4.5
    FudbalskiTim& operator = (const FudbalskiTim& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_naziv;
        _naziv = AlocirajIKopiraj(obj._naziv);
        delete[]_domaciStadion;
        _domaciStadion = AlocirajIKopiraj(obj._domaciStadion);
        _datumOsnivanja = obj._datumOsnivanja;//Operator = preklopljen
        _rasporedjivac = obj._rasporedjivac;//Operator = preklopljen
        _brojevi = obj._brojevi;//Operator = preklopljen
        _fudbaleri = obj._fudbaleri;//Operator = preklopljen
        return *this;//Vrati objekat koji je pozvao fiju;
    }

    //Z4.5
    const char* GetNaziv() const { return _naziv; }
    Datum GetDatumOsnivanja() const { return _datumOsnivanja; }
    const char* GetDomaciStadion() const { return _domaciStadion; }
    function<int()> GetRasporedjivac() const { return _rasporedjivac; }
    Vektor<Fudbaler> GetFudbalere() const { return _fudbaleri; }
    Vektor<int> GetBrojeve() const { return _brojevi; }

    //Z4.6
    void SetNaziv(const char* naziv)
    {
        delete[]_naziv;
        _naziv = AlocirajIKopiraj(naziv);
    }
    void SetDatumOsnivanja(Datum datumOsnivanja)
    {
        _datumOsnivanja = datumOsnivanja;//Operator = preklopljen
    }
    void SetDomaciStadion(const char* stadion)
    {
        delete[] _domaciStadion;
        _domaciStadion = AlocirajIKopiraj(stadion);
    }

    /*Z4.7 :: Dodati novog fudbalera, i asocirati uz njega novi (neiskoristeni) broj koristenjem pokazivaca na funkciju '_rasporedjivac'
           :: Dodavanje vrsiti u vektore '_fudbaleri' i '_brojevi'*/
    void operator += (Fudbaler fudbaler)
    {
        int noviBr = 0;
        do
        {
            noviBr = _rasporedjivac();//Randomizuje se neki br igraca od 1 do 100;
        } while (_brojevi.CheckIfSadrzi(noviBr));
        _brojevi += noviBr;//Implementiran operator += za class T;
        _fudbaleri += fudbaler;//Implementiran operator += za class T;
    }

    //4.8
    Fudbaler& operator[](int index) const
    {
        index = Min<int>(index, _fudbaleri.GetTrenutno() - 1);
        index = Max(index, 0);
        return _fudbaleri[index];
    }

    //4.9 :: Setter za polje '_brojevi'. Promijeniti vrijednost elementa na lokaciji specificiranoj parametrom 'index'
    void SetBroj(int index, int noviBroj)
    {
        //Radi uvijek provjeru indexa;
        index = Min<int>(index, _brojevi.GetTrenutno() - 1);
        index = Max(0, index);
        _brojevi[index] = noviBroj;//Preklopljen operator [];
    }

    //Z4.10
    ~FudbalskiTim()
    {
        delete[] _naziv;
        _naziv = nullptr;
        delete[] _domaciStadion;
        _domaciStadion = nullptr;
    }
    friend ostream& operator << (ostream& COUT, const FudbalskiTim& tim);
    friend bool operator == (const FudbalskiTim& tim1, const FudbalskiTim& tim2);
};

//Z4.11 :: Ispisati obiljezja '_naziv', '_datumOsnivanja', '_domaciStadion', te imena, prezimena i brojeve fudbalera, kao i pozicije na kojima igraju
ostream& operator << (ostream& COUT, const FudbalskiTim& tim)
{
    COUT << "Naziv : " << tim._naziv << endl;
    COUT << "Datum osnivanja : " << tim._datumOsnivanja << endl;//Preklopljen operator << class Datum;
    COUT << "Domaci stadion : " << tim._domaciStadion << endl;
    COUT << "Trenutno aktivni fudbaleri : " << endl;
    COUT << "--------------------------------------------------------\n";
    for (int i = 0; i < tim._fudbaleri.GetTrenutno(); i++)
    {
        COUT << tim._fudbaleri[i].GetIme() << " " << tim._fudbaleri[i].GetPrezime() << " (" << tim._brojevi[i] << " )" << tim._fudbaleri[i].GetPozicija() << endl;

    }
    COUT << "--------------------------------------------------------\n";
    return COUT;
}

//Z4.12 :: Poredjenje objekata (poredjenje svih atributa)
bool operator == (const FudbalskiTim& tim1, const FudbalskiTim& tim2)
{
    if (strcmp(tim1._naziv, tim2._naziv) == 0)
        return true;
    if (strcmp(tim1._domaciStadion, tim2._domaciStadion) == 0)
        return true;
    if (tim1._datumOsnivanja == tim2._datumOsnivanja)//Preklopljen operator == za class Datum;
        return true;
    if (tim1._fudbaleri.GetTrenutno() == tim2._fudbaleri.GetTrenutno())
        return true;
    for (int i = 0; i < tim1._fudbaleri.GetTrenutno(); i++)//Podrazumeva se da su istih duzina oba vektora
    {
        if (tim1._fudbaleri[i] == tim2._fudbaleri[i])//Preklopljen operator == class Fudbaler;
            return true;
    }
    if (tim1._brojevi.GetTrenutno() == tim2._brojevi.GetTrenutno())
        return true;
    for (int i = 0; i < tim1._brojevi.GetTrenutno(); i++)//Podrazumeva se da su istih duzina oba vektora
    {
        if (tim1._brojevi[i] == tim2._brojevi[i])//Nema preklopljenog == jer je int tip, vec je postojeci;
            return true;
    }
    return false;
}

bool operator != (const FudbalskiTim& tim1, const FudbalskiTim& tim2)
{
    return !(tim1 == tim2);
}

class Utakmica {
private:
    pair<FudbalskiTim, FudbalskiTim> _timovi;
    pair<int, int> _postignutiGolovi;
public:
    //Z5.1 '_postignutiGolovi' postaviti na vrijednosti <0,0>
    Utakmica()
    {
        /*_postignutiGolovi.first = 0;
        _postignutiGolovi.second = 0;*/
        _postignutiGolovi = make_pair(0, 0);
    }
    //Z5.2
    Utakmica(FudbalskiTim domacin, FudbalskiTim gosti)//Usr-def ctor;
    {
        /* _timovi.first = domacin;
         _timovi.second = gosti;*/
        _timovi = make_pair(move(domacin), move(gosti));
        _postignutiGolovi = make_pair(0, 0);
    }
    //Z5.3
    Utakmica(const Utakmica& obj)//Copy.ctor;
        :_timovi(obj._timovi), _postignutiGolovi(obj._postignutiGolovi)//Koristimo analogno copy.ctor
    {

    }
    //Z5.4
    Utakmica(Utakmica&& obj) noexcept //Move ctor;
        :_timovi(move(obj._timovi)), _postignutiGolovi(move(obj._postignutiGolovi))//Analogno tome koristimo move ctor;
    {

    }
    //Z5.5
    Utakmica& operator = (const Utakmica& obj)
    {
        if (this == &obj)
            return *this;
        _timovi = obj._timovi;//Preklopljen operator za parove class pair;
        _postignutiGolovi = obj._postignutiGolovi;//Preklopljen operator za parove class pair;
        return *this;//Vrati objekat koji je pozvao fiju;
    }
    //Z5.6
    FudbalskiTim GetDomacin() const { return _timovi.first; }
    FudbalskiTim GetGosti() const { return _timovi.second; }

    //Z5.7
    void SetDomacin(FudbalskiTim tim)
    {
        _timovi.first = tim;
    }
    void SetGosti(FudbalskiTim tim)
    {
        _timovi.second = tim;
    }

    //Z5.8 :: int golZa = {1 if Domacin, 2 if Gosti}
    void DodajGol(int golZa)
    {
        golZa = Max<int>(golZa, 1);//Ako se unese neki neg broj bice 1;
        golZa = Min(golZa, 2);//Ako se unese neki br veci od 2 bice 2;
        //Ovime se ogranici na 1 ili 2;
        if (golZa == 1)
            _postignutiGolovi.first += 1;
        else
            _postignutiGolovi.second += 1;
    }
    //Z5.9
    ~Utakmica()
    {
        //Nemamo sta dealocirati od heap memorije;
    }
    friend ostream& operator <<(ostream& COUT, const Utakmica& utakmica);
    friend bool operator == (const Utakmica& u1, const Utakmica& u2);
};

/*Z5.10 :: Ispisati utakmicu u sljedecem formatu:
        Stadion: Camp Nou
        Barcelona - Arsenal [2:2]
*/
ostream& operator <<(ostream& COUT, const Utakmica& utakmica)
{
    COUT << "Stadion : " << utakmica._timovi.first.GetDomaciStadion() << endl;
    COUT << utakmica._timovi.first.GetNaziv() << "-" << utakmica._timovi.second.GetNaziv() << " " << " [" << utakmica._postignutiGolovi.first << ":" << utakmica._postignutiGolovi.second << "]" << endl;
    return COUT;
}

//Z5.11
bool operator == (const Utakmica& u1, const Utakmica& u2)
{
    if ((u1._timovi.first == u2._timovi.first) && (u1._timovi.second == u2._timovi.second))
        return true;
    if ((u1._postignutiGolovi.first == u2._postignutiGolovi.first) && (u1._postignutiGolovi.second == u2._postignutiGolovi.second))
        return true;
    return false;
}



void Zadatak1() {
    Datum aprilFoolsDay;
    Datum laborDay(1, 5, 2021);
    Datum newYearsEve(31, 12, 2021);
    aprilFoolsDay.SetDan(1);
    aprilFoolsDay.SetMjesec(4);
    aprilFoolsDay.SetGodina(2021);

    Datum d1(aprilFoolsDay);
    Datum d2(move(laborDay));
    Datum d3;
    d3 = newYearsEve;
    cout << d1 << endl;
    cout << d2 << endl;
    cout << d3 << endl;
    cout << "Dealokacija..." << endl;
}
//Zadatak1 has passed the test;
void Zadatak2() {
    cout << ":::::::::: Vektor<int> :::::::::::\n";
    Vektor<int> ocjeneFilma;
    int brojOcjena = 30;
    for (int i = 0; i < brojOcjena; i++)
        ocjeneFilma += rand() % 10 + 1;
    IspisNiza(ocjeneFilma.GetElementi(), ocjeneFilma.GetTrenutno(), ", ");

    ocjeneFilma.Sortiraj(BubbleSort);
    cout << "Ocjene filma nakon sortiranja [Ascending]:" << endl;
    IspisNiza(ocjeneFilma.GetElementi(), ocjeneFilma.GetTrenutno(), ", ");

    ocjeneFilma.Sortiraj(BubbleSort, true);
    cout << "Ocjene filma nakon sortiranja [Descending]:" << endl;
    IspisNiza(ocjeneFilma.GetElementi(), ocjeneFilma.GetTrenutno(), ", ");

    Vektor<int> ocjeneFilma2(ocjeneFilma);
    Vektor<int> ocjeneFilma3(move(ocjeneFilma));
    Vektor<int> ocjeneFilma4;
    ocjeneFilma4 = ocjeneFilma2;
    auto checkIfIsti = [](Vektor<int>& v1, Vektor<int>& v2) {
        if (v1.GetTrenutno() != v2.GetTrenutno())
            return false;
        for (int i = 0; i < v1.GetTrenutno(); i++)
            if (v1[i] != v2[i])
                return false;
        return true;
    };

    cout << (checkIfIsti(ocjeneFilma, ocjeneFilma2) ? "Isti vektori" : "Nisu isti vektori") << endl;
    cout << (checkIfIsti(ocjeneFilma, ocjeneFilma3) ? "Isti vektori" : "Nisu isti vektori") << endl;
    cout << (checkIfIsti(ocjeneFilma2, ocjeneFilma3) ? "Isti vektori" : "Nisu isti vektori") << endl;
    cout << (checkIfIsti(ocjeneFilma4, ocjeneFilma2) ? "Isti vektori" : "Nisu isti vektori") << endl;
    cout << "\n\n";

    cout << ":::::::::: Vektor<Datum> :::::::::::\n";
    Vektor<Datum> datumi;
    int brojDatuma = 15;
    for (int i = 0; i < brojDatuma; i++)
        datumi += Datum::GetRandomDatum(); // dodavanje novog datuma u vektor 
    cout << datumi << endl;
    datumi.Sortiraj(BubbleSort);
    cout << "Datumi nakon sortiranja [Ascending]:" << endl;
    cout << datumi << endl;
    datumi.Sortiraj(BubbleSort, true);
    cout << "Datumi nakon sortiranja [Descending]:" << endl;
    cout << datumi << endl;
    cout << "Dealokacija..." << endl;
}
//Zadatak2 has passed the test;
void Zadatak3() {
    cout << "::::::::::klasa Fudbaler:::::::::::\n";
    Fudbaler gandhi;
    gandhi.SetIme("Mohandas");
    gandhi.SetPrezime("Gandhi");
    gandhi.SetVisina(1.70f);
    gandhi.SetDatumRodjenja(Datum(5, 5, 1990));
    gandhi.SetPozicija("Obrambeni vezni");
    //Dodjeljivanje nagrada igracu
    gandhi("Fair play award 2018"); // poziv operatora '()' za dodavanje novog elementa u niz '_nagrade'
    gandhi("Fair play award 2019");
    gandhi("Fair play award 2020");
    gandhi("Fair play award 2021");
    gandhi("The best team leader award 2019");
    gandhi("The best team leader award 2020");
    gandhi("The greatest comeback award 2020");
    gandhi("The greatest comeback award 2021");
    gandhi("Sportsmanship award 2019");
    gandhi("Sportsmanship award 2020");
    gandhi("The most effective player vs. England national team 2020");
    gandhi("The most effective player vs. England national team 2021");
    gandhi("The 1st player to score 5 goals vs. England in a single match");
    gandhi("The best Indian football player of all time");
    cout << gandhi << "\n";

    cout << "Testiranje user-def. konstruktora: ";
    Fudbaler gandhi2(gandhi.GetIme(), gandhi.GetPrezime(), gandhi.GetDatumRodjenja(), gandhi.GetVisina(), gandhi.GetPozicija());
    for (int i = 0; i < gandhi.GetTrenutnoNagrada(); i++)
        gandhi2(gandhi[i]); // dodajemo nagrade drugom objektu
    cout << ((gandhi == gandhi2) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;

    cout << "Testiranje konstruktora kopije: ";
    Fudbaler gandhiImpostor(gandhi);
    cout << ((gandhi == gandhiImpostor) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;

    cout << "Testiranje move konstruktora: ";
    Fudbaler gandhiMover(move(gandhiImpostor));
    cout << ((gandhi == gandhiMover) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;

    cout << "Testiranje operatora '=': ";
    Fudbaler martinLutherKing;
    martinLutherKing = gandhi;
    cout << ((gandhi == martinLutherKing) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;
    cout << "Dealokacija..." << endl;
}
//Zadatak3 has passed the test;
void Zadatak4() {
    cout << "::::::::::klasa FudbalskiTim:::::::::::\n";
    Fudbaler trump("Donald J.", "Trump", Datum(1, 3, 2150), 1.66f, "Centralni napadac");
    Fudbaler dalaiLama("Tenzin", "Gyatso", Datum(5, 3, 2151), 1.78f, "Obrambeni vezni");
    Fudbaler superSemir("Dr. Semir", "Osmanagic", Datum(5, 3, 2150), 1.85f, "Golman");
    Fudbaler ciroImpostor("Tarik", "Filipovic", Datum(5, 3, 2149), 1.85f, "Stoper");
    Fudbaler kreso("Kresimir", "Misak", Datum(5, 3, 2158), 1.66f, "Krilni vezni");
    Fudbaler theForbiddenOne("Chuck", "Norris", Datum(32, 12, 2154), 2.5f, "Na svim pozicijama istovremeno");

    FudbalskiTim osvajaciGalaksije;
    osvajaciGalaksije.SetNaziv("Osvajaci Galaksije (F.C)");
    osvajaciGalaksije.SetDomaciStadion("Milenijsko gnijezdo");
    osvajaciGalaksije.SetDatumOsnivanja(Datum(5, 5, 2100));

    osvajaciGalaksije += trump;
    osvajaciGalaksije += dalaiLama;
    osvajaciGalaksije += superSemir;
    osvajaciGalaksije += ciroImpostor;
    osvajaciGalaksije += kreso;
    osvajaciGalaksije += theForbiddenOne;
    cout << osvajaciGalaksije << endl;

    cout << "Testiranje user-def. konstruktora: ";
    FudbalskiTim galaktikosi(osvajaciGalaksije.GetNaziv(), osvajaciGalaksije.GetDatumOsnivanja(), osvajaciGalaksije.GetDomaciStadion(),
        osvajaciGalaksije.GetRasporedjivac());
    Vektor<Fudbaler> fudbaleri = osvajaciGalaksije.GetFudbalere();
    Vektor<int> brojevi = osvajaciGalaksije.GetBrojeve();
    for (int i = 0; i < fudbaleri.GetTrenutno(); i++)
        galaktikosi += fudbaleri[i];
    for (int i = 0; i < brojevi.GetTrenutno(); i++)
        galaktikosi.SetBroj(i, brojevi[i]);
    cout << ((osvajaciGalaksije == galaktikosi) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;

    cout << "Testiranje konstruktora kopije: ";
    FudbalskiTim osvajaci2(osvajaciGalaksije);
    cout << ((osvajaciGalaksije == osvajaci2) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;

    cout << "Testiranje move konstruktora: ";
    FudbalskiTim osvajaci3(move(osvajaci2));
    cout << ((osvajaciGalaksije == osvajaci3) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;

    cout << "Testiranje operatora '=': ";
    FudbalskiTim osvajaci4;
    osvajaci4 = osvajaciGalaksije;
    cout << ((osvajaci4 == osvajaciGalaksije) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;
    cout << "Dealokacija..." << endl;
}
//Zadatak4 has passed the test;
void Zadatak5() {
    cout << "::::::::::klasa Utakmica:::::::::::\n";
    //Tim :: 1
    Fudbaler sleepyJoe("Joe", "Biden", Datum(3, 4, 1950), 1.77f, "Obrambeni vezni");
    Fudbaler stableTrump("Donald J.", "Trump", Datum(5, 4, 1951), 1.88f, "Centralni napadac");
    Fudbaler lyingTed("Ted", "Cruz", Datum(6, 6, 1952), 1.8f, "Centralni vezni");
    Fudbaler theLittleMan("Marco", "Rubio", Datum(7, 7, 1953), 1.6f, "Ofanzivni vezni");
    Fudbaler lowEnergyJeb("Jeb", "Bush", Datum(8, 8, 1954), 1.68f, "Golman");
    //Tim :: 2
    Fudbaler facebookLord("Mark", "Zuckerberg", Datum(3, 4, 1960), 1.74f, "Obrambeni vezni");
    Fudbaler elonTheMartian("Elon", "Musk", Datum(5, 5, 1961), 1.77f, "Leteci napadac");
    Fudbaler codeJeff("Jeff", "Bezos", Datum(5, 3, 1961), 1.8f, "Centralni vezni");
    Fudbaler theVaxGuy("Bill", "Gates", Datum(15, 3, 1955), 1.85f, "Golman");
    Fudbaler cryptoGuru("Satoshi", "Nakamoto", Datum(15, 3, 1955), 1.85f, "Nevidljiv na terenu");

    FudbalskiTim cowboys("Cowboys (F.C)", Datum(1, 3, 1960), "Wild West Park");
    cowboys += sleepyJoe;
    cowboys += stableTrump;
    cowboys += lyingTed;
    cowboys += theLittleMan;
    cowboys += lowEnergyJeb;

    FudbalskiTim aliens("Aliens (F.C)", Datum(5, 6, 1955), "Galaxy Centre");
    aliens += facebookLord;
    aliens += elonTheMartian;
    aliens += codeJeff;
    aliens += theVaxGuy;
    aliens += cryptoGuru;

    Utakmica spaceClassico;
    spaceClassico.SetDomacin(cowboys);
    spaceClassico.SetGosti(aliens);
    spaceClassico.DodajGol(1); //gol za domacina
    spaceClassico.DodajGol(1); //gol za domacina
    spaceClassico.DodajGol(2); //gol za goste
    spaceClassico.DodajGol(2); //gol za goste
    spaceClassico.DodajGol(1); //gol za domacina
    cout << spaceClassico << endl;

    cout << "Testiranje user-def. konstruktora: ";
    Utakmica derbi(cowboys, aliens);
    derbi.DodajGol(1);
    derbi.DodajGol(1);
    derbi.DodajGol(2);
    derbi.DodajGol(2);
    derbi.DodajGol(1);
    cout << ((spaceClassico == derbi) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;

    cout << "Testiranje konstruktora kopije: ";
    Utakmica kopija(spaceClassico);
    cout << ((spaceClassico == kopija) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;

    cout << "Testiranje move konstruktora: ";
    Utakmica clashOfTheTitans(move(kopija));
    cout << ((spaceClassico == clashOfTheTitans) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;

    cout << "Testiranje operatora '=': ";
    Utakmica mecNaseGeneracije;
    mecNaseGeneracije = spaceClassico;
    cout << ((spaceClassico == mecNaseGeneracije) ? "Ispravno uradjen" : "Nije ispravno uradjen") << endl;
    cout << "Dealokacija..." << endl;
}
//Zadataj5 has passed the test;
void Menu() {
    int nastaviDalje = 1;
    while (nastaviDalje == 1) {
        int izbor = 0;
        do {
            system("cls");
            cout << "***Zadaci***" << endl;
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