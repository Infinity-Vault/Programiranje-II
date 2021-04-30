#include <vector>
#include <iostream>
#include <cassert>
#include <functional>
using namespace std;

int MIN_GODINA = 1, MAX_GODINA = 10000;
//Enumeracija ispod u sustini omoguci skladistenje (najbrajanje) vise globalnih varijabli u ovom slucaju mjesece u godini;
enum Mjesec { Januar = 1, Februar, Mart, April, Maj, Juni, Juli, August, Septembar, Oktobar, Novembar, Decembar };
//Enumeracija i enumeracijska klasa su identicne samo se razlikuju u pristupu obiljezja, kod enum. klase je to Ime_klase::property;
enum class Spol { Muski = 1, Zenski = 0 };

//Z0.1 
char* Alokator(const char* tekst)
{
    if (tekst == nullptr)
        return nullptr;
    int size = strlen(tekst) + 1;
    char* noviTxt = new char[size];
    strcpy_s(noviTxt, size, tekst);
    return noviTxt;
}

//Z0.2 :: Funkcija za alokaciju pametnog (shared_ptr) pokazivaca na niz karaktera
shared_ptr<char> SharedAlokator(const char* tekst)
{
    //Provjerimo prvo da li je ulazni parametar prazan (nullptr):
    if (tekst == nullptr)
        return nullptr;
    int size = strlen(tekst) + 1;
    shared_ptr <char> noviTxt(new char[size]);//Alociramo novi memorijski prostor sa  konstruktorom;
    //shared_ptr<char> noviTxt;
    //noviTxt.reset(new char[size]);//Alociramo novi memorijski prostor sa metodom reset();
    strcpy_s(noviTxt.get(), size, tekst);//Posto su smart pointeri "objekti" moramo koristiti metodu get() da dobijemo memo. lokaciju ovog pokazivaca tj regularni ptr tipa char*;
    return noviTxt;
}
//Z0.3 :: Funkcija za alokaciju pametnog (unique_ptr) pokazivaca na niz karaktera
unique_ptr<char[]> UniqueAlokator(const char* tekst)
{
    if (tekst == nullptr)
        return nullptr;
    int size = strlen(tekst) + 1;
    unique_ptr<char[]> noviTxt(new char[size]);//Mozemo koristiti [] ili samo char, svejedno je; Alociramo memoriju sa konstruktorom;
    //unique_ptr<char[]> noviTxt;
    //noviTxt.reset(new char[size]);//Alociramo memoriju sa reset()
    strcpy_s(noviTxt.get(), size, tekst);
    return noviTxt;
}

//Z0.4 :: Funkcija vraca max. od dva elementa
template<typename T>
T Max(T el1, T el2)
{
    if (el1 > el2)
        return el1;
    return el2;
}
//Z0.5 :: Funkcija vraca min. od dva elementa
template<typename T>
T Min(T el1, T el2)
{
    if (el1 < el2)
        return el1;
    return el2;
}

class Vrijeme {
protected:
    int* _sati;
    int* _minute;
    int* _sekunde;
public:
    //Logika iza kreiranja propertija jeste ta da su dostupni svim instancama te klase te da iza sebe u "backend-u" imaju implementirane settere i gettere;
    __declspec(property (get = GetSati, put = SetSati)) int Sati;
    __declspec(property (get = GetMinute, put = SetMinute)) int Minute;
    __declspec(property (get = GetSekunde, put = SetSekunde)) int Sekunde;

    //Z1.1
    //Vraca true ako postoji makar jedno obiljezje sa vrijednoscu nullptr
    static bool CheckIfNull(const Vrijeme& obj)
    {
        if (obj._sati == nullptr || obj._minute == nullptr || obj._sekunde == nullptr)
            return true;
        return false;
    }
    //Z1.2
    Vrijeme()//Dflt ctor;
    {
        _sati = nullptr;
        _minute = nullptr;
        _sekunde = nullptr;
    }
    //Z1.3
    Vrijeme(int sati, int minute, int sekunde)//User-def ctor;
    {
        _sati = new int(sati);
        _minute = new int(minute);
        _sekunde = new int(sekunde);
    }
    //Z1.4
    Vrijeme(const Vrijeme& obj)//Copy ctor;
    {
        assert(!CheckIfNull(obj));//Ako je svaki ptr alociran uradi sljedece, ako nije prekini program;
        _sati = new int(*obj._sati);
        _minute = new int(*obj._minute);
        _sekunde = new int(*obj._sekunde);
    }
    //Z1.5
    Vrijeme(Vrijeme&& obj) noexcept//Move ctor;
    {
        _sati = obj._sati;
        obj._sati = nullptr;
        _minute = obj._minute;
        obj._minute = nullptr;
        _sekunde = obj._sekunde;
        obj._sekunde = nullptr;
    }
    //Z1.6
    Vrijeme& operator =(const Vrijeme& obj)
    {
        if (this == &obj)//Provjera da li su dva objekta na istim memorijskim lokacijama;
            return *this;
        //Ako su lvaluve pointeri nulirani alociramo memoriju za njih;
        if (_sati == nullptr)
            _sati = new int;
        if (_minute == nullptr)
            _minute = new int;
        if (_sekunde == nullptr)
            _sekunde = new int;
        assert(!CheckIfNull(obj));//Jer je moguce da su ptr-i od obj nulirani pa bi pri dereferenciranju doslo do pada programa;
        *_sati = *obj._sati;
        *_minute = *obj._minute;
        *_sekunde = *obj._sekunde;
        return *this;//Vrati obj koji je pozvao metodu;
    }
    //Z1.7
    void SetSati(int sati)
    {
        if (_sati == nullptr)
            _sati = new int;
        *_sati = sati;
    }
    void SetMinute(int minute)
    {
        if (_minute == nullptr)
            _minute = new int;
        *_minute = minute;
    }
    void SetSekunde(int sekunde)
    {
        if (_sekunde == nullptr)
            _sekunde = new int;
        *_sekunde = sekunde;
    }
    //Z1.8
    int GetSati() const
    {
        assert(_sati != nullptr);//Ako je ptr razlicit od nullptr odradi kod, ako ne baci gresku;
        return *_sati;
    }
    int GetMinute() const
    {
        assert(_minute != nullptr);
        return *_minute;
    }
    int GetSekunde() const
    {
        assert(_sekunde != nullptr);
        return *_sekunde;
    }
    //Z1.9 :: koristenjem rand() funkcije generisati slucajne vrijednosti za novi objekat(Vrijeme)
    static Vrijeme GetRandomVrijeme()
    {
        Vrijeme temp;//Dflt ctor pozvan;
        //Lakse je da se koriste propertiji jer oni ce pozivati settere te shodno time alocirati memoriju za nas;
        temp.Sati = rand() % 24;
        temp.Minute = rand() % 60;
        temp.Sekunde = rand() % 60;
        return temp;
    }
    //Z1.10
    bool CheckIfValidnoVrijeme()
    {
        if (Sati < 0 || Sati >23)
            return false;
        if (Minute < 0 || Minute>60)
            return false;
        if (Sekunde < 0 || Sekunde>60)
            return false;
        return true;
    }
    //Z1.11
    ~Vrijeme()//Dtor;
    {
        delete _sati;
        delete _minute;
        delete _sekunde;
        _sati = nullptr;
        _minute = nullptr;
        _sekunde = nullptr;
    }
};
//Z1.12
ostream& operator << (ostream& COUT, const Vrijeme& obj)
{
    //Lakse je koristiti propertije jer imaju getter, te time ovaj operator ne mora biti friend;
    COUT << "Sati: " << obj.Sati << "  Minute:  " << obj.Minute << "  Sekunde: " << obj.Sekunde << endl;
    return COUT;
}
//Z1.13
bool operator == (const Vrijeme& v1, const Vrijeme& v2)
{
    //Lakse je koristiti propertije jer imaju getter, te time ovaj operator ne mora biti friend;
    if (v1.Sati == v2.Sati && v1.Minute == v2.Minute && v1.Sekunde == v2.Sekunde)
        return true;
    return false;
}
bool operator != (const Vrijeme& v1, const Vrijeme& v2)
{
    if (!(v1 == v2))
        return true;
    return false;
}
bool operator > (const Vrijeme& v1, const Vrijeme& v2)
{
    if (v1.Sati * 3600 + v1.Minute * 60 + v1.Sekunde > v2.Sati * 3600 + v2.Minute * 60 + v2.Sekunde)
        return true;
    return false;
}
bool operator >= (const Vrijeme& v1, const Vrijeme& v2)
{
    if (v1 == v2 || v1 > v2)
        return true;
    return false;
}
bool operator < (const Vrijeme& v1, const Vrijeme& v2)
{
    if (!(v1 >= v2))
        return true;
    return false;
}
bool operator <= (const Vrijeme& v1, const Vrijeme& v2)
{
    if (!(v1 > v2))
        return true;
    return false;
}

class DatumVrijeme : public Vrijeme {//DatumVrijeme je child klasa parent klase Vrijeme u public odnosu;
private:
    shared_ptr<int> _dan;
    shared_ptr<int> _mjesec;
    shared_ptr<int> _godina;
public:
    __declspec(property (get = GetDan, put = SetDan)) int Dan;
    __declspec(property (get = GetMjesec, put = SetMjesec)) int Mjesec;
    __declspec(property (get = GetGodina, put = SetGodina)) int Godina;
    //Z2.1
    static bool CheckIfNull(const DatumVrijeme& obj)
    {
        if (obj._dan == nullptr || obj._mjesec == nullptr || obj._godina == nullptr)
            return true;//Ako makar jedan ptr nuliran, vrati true;
        return false;
    }
    //Z2.2
    DatumVrijeme()//Dflt ctor;
        :Vrijeme()//Mozemo pozvati i dflt ctor od Vrijeme komponente klase ali je to vec po defaultu;
    {
        //Posto radimo sa smart pointerima nema potrebe da ista nuliramo vec su nulirani;
    }
    //Z2.3
    DatumVrijeme(int dan, int mjesec, int godina, int sati, int minute, int sekunde)//Dflt ctor;
        :Vrijeme(sati, minute, sekunde)//Pozivamo user-def ctor od komponente Vrijeme;
    {
        _dan = make_shared<int>(dan);
        _mjesec = make_shared<int>(mjesec);
        _godina = make_shared<int>(godina);
    }
    //Z2.4
    DatumVrijeme(const DatumVrijeme& obj)//Copy ctor;
        :Vrijeme(obj)//Copy ctor od komponente Vrijeme;
    {
        assert(!CheckIfNull(obj));//Provjeri da li su rvalue pointeri nulirani ili ne;Ako nisu izvrsava se dalje, ako jesu prekida se program;
        _dan = make_shared<int>(*obj._dan);
        _mjesec = make_shared<int>(*obj._mjesec);
        _godina = make_shared<int>(*obj._godina);
    }
    //Z2.5
    DatumVrijeme(DatumVrijeme&& obj) noexcept //Move ctor;
        :Vrijeme(move(obj))//Pozivamo move ctor komponente Vrijeme;
    {
        _dan = obj._dan;
        obj._dan = nullptr;
        _mjesec = obj._mjesec;
        obj._mjesec = nullptr;
        _godina = obj._godina;
        obj._godina = nullptr;
    }
    //Z2.6 
    DatumVrijeme& operator =(const DatumVrijeme& obj)
    {
        if (this == &obj)//Provjeri da li su iste memorijske adrese;
            return *this;
        //Prvo izjednacimo komponentu Vrijeme:
        //Vrijeme::operator=(obj);//Prvi nacin;
        (Vrijeme&)*this = (Vrijeme&)obj;//Drugi nacin gdje se lvalue pristupi sa this;
        //Provjerimo ima li praznih nealociranih pointera:
        if (_dan == nullptr)
            _dan = make_shared<int>();
        if (_mjesec == nullptr)
            _mjesec = make_shared<int>();
        if (_godina == nullptr)
            _godina = make_shared<int>();
        //Provjerimo da slucajno rvalue nisu nulirani ptr jer bi pri dereferenciranju pao program:
        assert(!CheckIfNull(obj));//Ako nema nullptr nastavi se ako ima onda zaustavi execution programa;
        *_dan = *obj._dan;
        *_mjesec = *obj._mjesec;
        *_godina = *obj._godina;
        return *this;//Vrati obj koji je pozvao metodu;
    }
    //Z2.7
    void SetDan(int dan)
    {
        if (_dan == nullptr)
            _dan = make_shared<int>();
        *_dan = dan;
    }
    void SetMjesec(int mjesec)
    {
        if (_mjesec == nullptr)
            _mjesec = make_shared<int>();
        *_mjesec = mjesec;
    }
    void SetGodina(int godina)
    {
        if (_godina == nullptr)
            _godina = make_shared<int>();
        *_godina = godina;
    }
    //Z2.8
    int GetDan() const
    {
        assert(_dan != nullptr);//Izvrsi radnju ispod samo ako nije nuliran ptr;
        return *_dan;
    }
    int GetMjesec() const
    {
        assert(_mjesec != nullptr);//Izvrsi radnju ispod samo ako nije nuliran ptr;
        return *_mjesec;
    }
    int GetGodina() const
    {
        assert(_godina != nullptr);//Izvrsi radnju ispod samo ako nije nuliran ptr;
        return *_godina;
    }
    //Z2.9
    bool CheckIfValidnoDatumVrijeme()
    {
        if (!CheckIfValidnoVrijeme())//Pozvana bool metoda izvedene parent klase Vrijeme
            return false;
        if (Godina<INT_MIN || Godina>INT_MAX)
            return false;
        if (Mjesec < Januar || Mjesec>Decembar)//Koristimo enumeraciju Mjesec
            return false;
        if (Dan < 0 || Dan > 31)
            return false;
        //Kreiranje lambda fiju za provjeru prijestupne godine;
        bool (*JelPrijestupna) (int) = [](int godina)
        {
            if ((godina % 4 == 0 && godina % 100 != 0) || godina % 400 == 0)
                return true;//Prijestupna;
            return false;//Nije prijestupna;
        };
        if (Mjesec == Februar)//Koristimo enumeraciju
        {
            if (JelPrijestupna(Godina))
            {
                if (Dan <= 29)
                    return true;
                return false;
            }
            else
            {
                if (Dan <= 28)
                    return true;
                return false;
            }
        }
        if (Mjesec == April || Mjesec == Juni || Mjesec == Septembar || Mjesec == Novembar)//Koristimo enumeraciju
        {
            if (Mjesec <= 30)
                return true;
            return false;
        }
        return true;
    }
    //Z2.10 Obratiti paznju na validnost novokreiranog objekta (DatumVrijeme) sa slucajnim vrijednostima
    static DatumVrijeme GetRandomDatumVrijeme()
    {
        //Mozemo koristiti Vrijeme randomVrijeme ili kljucnu rijec auto jer rvalue znamo da ce vratiti neki tip;
        auto randomVrijeme = Vrijeme::GetRandomVrijeme();
        DatumVrijeme temp(1, 1, 2021, randomVrijeme.Sati, randomVrijeme.Minute, randomVrijeme.Sekunde);//Kreiramo temp vrijeme
        do
        {
            //Randomizuje vrijednosti sve dok ne dodje do neke random validne vrijednosti;
            //Lakse je koristiti propertije jer imaju settere te samim time alociraju memoriju za nas;
            temp.Dan = rand() % 31 + 1;
            temp.Mjesec = rand() % 12 + 1;
            temp.Godina = rand() % 2021 + 1;
        } while (!temp.CheckIfValidnoDatumVrijeme());//Vrti sve dok nije validan;
        return temp;
    }
    //Z2.11
    ~DatumVrijeme()
    {
        //Ovo se cak po automatizmu uradi ali dobra je praksa ipak pisati;
        _dan.reset();
        _mjesec.reset();
        _godina.reset();
        //Dtor od komponente Vrijeme se sam pozove;
    }
};

//Z2.12
ostream& operator << (ostream& COUT, const DatumVrijeme& obj)
{
    //Lakse je korisiti propertije jer imaju gettere te operator << ne mora biti friend;
    COUT << "Dan: " << obj.Dan << " Mjesec: " << obj.Mjesec << " Godina: " << obj.Godina << endl;
    return COUT;
}
//Z2.13
bool operator == (const DatumVrijeme& d1, const DatumVrijeme& d2)
{
    if ((Vrijeme&)d1 == (Vrijeme&)d2 && (d1.Dan == d2.Dan && d1.Mjesec == d2.Mjesec && d1.Godina == d2.Godina))
        return true;
    return false;
}
bool operator !=(const DatumVrijeme& d1, const DatumVrijeme& d2)
{
    if (!(d1 == d2))
        return true;
    return false;
}
//Z2.14
bool operator >(const DatumVrijeme& d1, const DatumVrijeme& d2)
{
    //Lakse je koristiti propertije jer imamo settere dostupne;

    if (d1.Godina * 365 + d1.Mjesec * 30 + d1.Dan > d2.Godina * 365 + d2.Mjesec * 30 + d2.Dan)
        return true;//Ako je veca suma d1 od d2 vrati true;
    else if (d1.Godina * 365 + d1.Mjesec * 30 + d1.Dan == d2.Godina * 365 + d2.Mjesec * 30 + d2.Dan)//Ako nije gornji if  true pitaj da li su d1 i d2 jednaki;
        if ((Vrijeme&)d1 > (Vrijeme&)d2)//Pitaj onda da li je d1 > d2;
            return true;//ako jeste;
        else
            return false;//ako nije;
    return false;//Nije onda sig d1 veci od d2;
}
bool operator >= (const DatumVrijeme& d1, const DatumVrijeme& d2)
{
    if (d1 == d2 || d1 > d2)
        return true;
    return false;
}
bool operator < (const DatumVrijeme& d1, const DatumVrijeme& d2)
{
    if (!(d1 >= d2))
        return true;
    return false;
}
bool operator <= (const DatumVrijeme& d1, const DatumVrijeme& d2)
{
    if (!(d1 > d2))
        return true;
    return false;
}

template<class T, int max> //max oznacava velicinu niza
class Array {
private:
    int _trenutno; //trenutno elemenata 
    T* _elementi[max] = { nullptr }; //staticki niz pokazivaca
    bool _omoguciDupliranje;
public:
    __declspec(property (get = GetTrenutno)) int Length;
    //Z3.1
    Array(bool omoguciDupliranje = true)//User-def ctor;
    {
        _trenutno = 0;
        _omoguciDupliranje = omoguciDupliranje;
    }
    //Z3.2
    Array(const Array<T, max>& obj)//Copy ctor;
    {
        _trenutno = obj._trenutno;
        _omoguciDupliranje = obj._omoguciDupliranje;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = new T(*obj._elementi[i]);//Lvalue niz je vec alociran stoga samo njegove pokazivace postavimo na kopije od obj pokazivaca;
            //Copy ctor klase T mora postojati;
        }
    }
    //Z3.3
    Array(Array<T, max>&& obj) noexcept//Move ctor;
    {
        _trenutno = obj._trenutno;
        obj._trenutno = 0;
        _omoguciDupliranje = obj._omoguciDupliranje;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = obj._elementi[i];//Jedan pokazivac pokazuje na drugi;
            obj._elementi[i] = nullptr;//Nuliramo rvalue ptr da ne bi doslo do pada;
        }
    }
    //Z3.4
    Array<T, max>& operator=(const Array<T, max>& obj)
    {
        if (this == &obj)
            return *this;//Ako su na istim lokacijama vrati obj koji je pozvao metodu;
        for (int i = 0; i < _trenutno; i++)//Bitno je da prije nismo mijenjali brojac jer prvo moramo pobrisati;
        {
            delete _elementi[i];
            _elementi[i] = nullptr;
        }
        _trenutno = obj._trenutno;
        _omoguciDupliranje = obj._omoguciDupliranje;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = new T(*obj._elementi[i]);//Alociramo novi prostor na koji ce lvalue pointer pokazivati;
            //Copy ctor klase T mora postojati;
        }
        return *this;//Vrati obj koji je pozvao metodu;
    }

    //Z3.5 :: Osigurati da je index unutar opsega [0,_trenutno-1]. Koristiti genericke funkcije 'Min' i 'Max' 
    T& operator [] (int index) const
    {
        index = Max<int>(index, 0);//Ako je index negativan vrati se najveci  a to je 0;
        index = Min(index, _trenutno - 1);//Ako je index neki ogroman br veci od brojaca, on vrati zadnji moguci validan index;
        //U jednoj gen. fiji je naglasen tip podatka <int> dok u drugoj nije vec je implicitno odradio to kompajler;
        assert(_elementi[index] != nullptr);//Ako nije nuliran pok, izvrsi se kod ako jeste terminira se program;
        return *_elementi[index];
    }
    //Z3.6 :: Dodati (alocirati u niz) novi element
    bool operator +=(T noviElement)
    {
        if (_trenutno == max)
            return false;
        if (_omoguciDupliranje == false)//Nisu dozvoljeni duplikati;
        {
            for (int i = 0; i < _trenutno; i++)
            {
                if (*_elementi[i] == noviElement)
                    return false;//Nece i ne moze biti dodan;
            }
        }
        //Sljedeci kod se izvrsava ako gornji if nije ispunjen, tj. ako je dupliciranje dozvoljeno ILI ako nije nadjen nijedan duplikat u nizu:
        _elementi[_trenutno] = new T(noviElement);//Copy ctor class T;
        _trenutno++;
        return true;
    }
    //Z3.7 :: Izbrisati (dealocirati) zadnji element i umanjiti brojac
    bool UkloniZadnjiElement()
    {
        if (_trenutno == 0)
            return false;//Ne mozemo izbrisati nesto sto ne postoji;
        delete _elementi[_trenutno - 1];//Trenutno nam pokazuje zadnji aktuelni element, stoga moramo oduzeti jer indeksacija ide od 0 do _trenutno; Npr( _trenutno=100) bice elemenata od 0 do 99 stoga imamo oduzimanje;
        _elementi[_trenutno - 1] = nullptr;//Nuliramo pokazivac na obj;
        _trenutno--;
        return true;
    }
    //Z3.8
    bool CheckIfSadrzi(T element)
    {
        for (int i = 0; i < _trenutno; i++)
        {
            if (*_elementi[i] == element)//Obavezno dereferencirati drugi pokazivac;
                return true;
        }
        return false;
    }
    //Z3.9 :: Sortiranje elemenata pomocu algoritma za sortiranje "Bubble Sort"
    void Sortiraj(bool reverse = false)
    {
        auto sort = [&reverse](T& el1, T& el2)//OBAVEZNO uzimati po & da nema kopiranja nepotrebnog;
        {
            if (reverse)
                return el1 < el2;//Opadajuci sort;
            else
                return el1 > el2;//Penjuci sort;
        };
        for (int i = 0; i < _trenutno - 1; i++)
        {
            for (int j = 0; j < _trenutno - i - 1; j++)
            {
                if (sort(*_elementi[j], *_elementi[j + 1]))
                {
                    T temp = *_elementi[j];
                    *_elementi[j] = *_elementi[j + 1];
                    *_elementi[j + 1] = temp;
                }
            }
        }
    }
    //Z3.10
    int GetTrenutno() const { return _trenutno; }
    int GetMaxElemenata() const { return max; }
    bool GetOmoguciDupliranje() const { return _omoguciDupliranje; }
    //Z3.11
    //Dtor;
    ~Array()
    {
        for (int i = 0; i < _trenutno; i++)
        {
            delete _elementi[i];
            _elementi[i] = nullptr;
        }
    }
};
//Z3.12
template<class T, int max>
ostream& operator << <>(ostream& COUT, const Array<T, max>& obj)
{
    for (int i = 0; i < obj.Length; i++)
    {
        COUT << obj[i] << endl;//Preklopljen operator [] class T;
    }
    return COUT;
}
//Z3.13
template<class T, int max>
void Ispis(const Array<T, max>& obj, const char* delimiter = " ")
{
    for (int i = 0; i < obj.Length; i++)
    {
        cout << obj[i] << delimiter;
    }
    cout << endl;
}
//Z3.14
template<class T, int max>
bool operator == (const Array<T, max>& arr1, const Array<T, max>& arr2)
{
    if (arr1.Length != arr2.Length)
        return false;
    for (int i = 0; i < arr1.Length; i++)//Podrazumijeva se da su sada iste duzine oba arraya;Stoga je svejedno do kojeg idemo;
    {
        if (arr1[i] != arr2[i])//Poredi dva objekta tipa Array;
            return false;
    }
    return true;
}
template<class T, int max>
bool operator != (const Array<T, max>& arr1, const Array<T, max>& arr2)
{
    if (!(arr1 == arr2))//Poredi dva objekta tipa Array;
        return true;
    return false;
}

class Osoba {
protected:
    shared_ptr<char> _ime;
    shared_ptr<char> _prezime;
    unique_ptr<DatumVrijeme> _datumRodjenja;
    Spol _spol;
public:
    __declspec(property (get = GetIme, put = SetIme)) const char* Ime;
    __declspec(property (get = GetPrezime, put = SetPrezime)) const char* Prezime;
    __declspec(property (get = GetDatumRodjenja, put = SetDatumRodjenja)) DatumVrijeme DatumRodjenja;
    __declspec(property (get = GetSpol, put = SetSpol)) Spol Spol_;//Spol je enumeracija;

    //Z4.1
    Osoba(Spol spol = Spol::Zenski)//Dflt ctor;
    {
        _spol = spol;
        //Svi ostali ptr su smart ptr stoga nije potrebno nista;
    }
    //Z4.2
    Osoba(const char* ime, const char* prezime, DatumVrijeme datumRodjenja, Spol spol)//User-def ctor;
    {
        _ime = SharedAlokator(ime);
        _prezime = SharedAlokator(prezime);
        _datumRodjenja = make_unique<DatumVrijeme>(datumRodjenja);//Copy ctor DatumVrijeme klase;
        _spol = spol;
    }
    //Z4.3
    Osoba(const Osoba& obj)//Copy ctor;
    {
        _ime = SharedAlokator(obj.Ime);//Koristimo property kako bi vratili getterom const char*
        _prezime = SharedAlokator(obj.Prezime);
        _datumRodjenja = make_unique<DatumVrijeme>(*obj._datumRodjenja);
        _spol = obj._spol;
    }
    //Z4.4
    Osoba(Osoba&& obj) noexcept//Move ctor;
        : _datumRodjenja(move(obj._datumRodjenja))//Move ctor od unique_ptr jer ne mogu oba pok na isti objekat;
    {
        _ime = obj._ime;
        obj._ime = nullptr;
        _prezime = obj._prezime;
        obj._prezime = nullptr;
        _spol = obj._spol;
    }
    //Z4.5
    Osoba& operator = (const Osoba& obj)
    {
        if (this == &obj)
            return *this;
        _ime = SharedAlokator(obj.Ime);//Moze sa propertijem
        _prezime = SharedAlokator(obj._prezime.get());//Moze i sa get() metodom;
        if (_datumRodjenja == nullptr)
            _datumRodjenja = make_unique<DatumVrijeme>();
        assert(obj._datumRodjenja != nullptr);//Ako rvalue ptr nije nuliran izvrsava se code ako jeste terminira se;
        *_datumRodjenja = *obj._datumRodjenja;
        _spol = obj._spol;
        return *this;
    }
    //Z4.6
    const char* GetIme() const { return _ime.get(); }//Mora vratiti "raw" pointer;
    const char* GetPrezime() const { return _prezime.get(); }//Mora vratiti "raw" pointer;
    DatumVrijeme GetDatumRodjenja() const
    {
        assert(_datumRodjenja != nullptr);//Ako  ptr nije nuliran izvrsava se code ako jeste terminira se;
        return *_datumRodjenja;
    }
    Spol GetSpol() const { return _spol; }
    //Z4.7
    void SetIme(const char* ime)
    {
        // _ime.reset();//Ekvivalent delete [] _niz; Ali je nepotrebno, jer je smart shared ptr;Shared ptr moze pokazivati na vise objekata;
        _ime = SharedAlokator(ime);
    }
    void SetPrezime(const char* prezime)
    {
        _prezime = SharedAlokator(prezime);
    }
    void SetDatumRodjenja(DatumVrijeme datumRodjenja)
    {
        if (_datumRodjenja == nullptr)
            _datumRodjenja = make_unique<DatumVrijeme>();
        //Ovdje bi assert nepotreban bio jer smo if-om gore vec osigurali da nije nullptr;
        *_datumRodjenja = datumRodjenja;
    }
    void SetSpol(Spol spol)
    {
        _spol = spol;
    }
    //4.8
    ~Osoba()
    {
        //Nepotrebno ali dobar practise;
        _ime.reset();
        _prezime.reset();
        _datumRodjenja.reset();
    }
};
//Z4.9
ostream& operator << (ostream& COUT, const Osoba& osoba)
{
    COUT << "Ime i prezime: " << osoba.Ime << " " << osoba.Prezime << endl;
    COUT << "Datum rodjenja: " << osoba.DatumRodjenja << endl;
    COUT << "Spol : " << ((osoba.Spol_ == Spol::Muski) ? "Musko" : "Zensko") << endl;
    return COUT;
}
//Z4.10
bool operator == (const Osoba& o1, const Osoba& o2)
{
    if (strcmp(o1.Ime, o2.Ime) == 0)
        return true;
    if (strcmp(o1.Prezime, o2.Prezime) == 0)
        return true;
    if (o1.DatumRodjenja == o2.DatumRodjenja)//Operator == class DatumVrijeme;
        return true;
    if (o1.Spol_ == o2.Spol_)
        return true;
    return false;
}
bool operator != (const Osoba& o1, const Osoba& o2)
{
    if (!(o1 == o2))
        return true;
    return false;
}

class Pjevac : public Osoba {
    const char* _zanr;
    vector<const char*> _instrumenti;
    unique_ptr<char[]> _aktivan; // Npr. 1978 - present
public:
    __declspec(property (get = GetZanr, put = SetZanr)) const char* Zanr;
    __declspec(property (get = GetInstrumenti)) vector<const char*> Instrumenti;
    __declspec(property (get = GetAktivan, put = SetAktivan)) const char* Aktivan;
    //Z5.1
    Pjevac()//Dflt ctor;
        :Osoba()//Nema potrebe ali eto dobra je praksa;
    {
        _zanr = nullptr;
    }
    //Z5.2
    Pjevac(const char* ime, const char* prezime, DatumVrijeme dv, Spol sp, const char* zanr, const char* aktivan)//Usr-def ctor;
        :Osoba(ime, prezime, dv, sp)//User-def ctor komponente Osoba;
    {
        _zanr = Alokator(zanr);
        _aktivan = UniqueAlokator(aktivan);
    }
    //Z5.3
    Pjevac(const Pjevac& obj)//Copy ctor;
        :Osoba(obj), _instrumenti(obj._instrumenti)//Copy ctor komponente Osoba;Copy ctor vector klase;
    {
        _zanr = Alokator(obj.Zanr);
        _aktivan = UniqueAlokator(obj._aktivan.get());//Ili property Aktivan pozvati;
    }
    //Z5.4
    Pjevac(Pjevac&& obj) noexcept//Move ctor;
        :Osoba(move(obj)), _instrumenti(move(obj._instrumenti))//Move ctor komponente Osoba;Move ctor vector klase;
    {
        _zanr = obj._zanr;
        obj._zanr = nullptr;
        _aktivan = move(obj._aktivan);//Move assignment operator class vector;Moze i u Header
    }
    //Z5.5
    Pjevac& operator = (const Pjevac& obj)
    {
        if (this == &obj)
            return *this;
        (Osoba&)*this = (Osoba&)obj;//Preklopljen operator = za komponentu klase Osoba;
        delete[] _zanr;
        _zanr = Alokator(obj._zanr);
        _aktivan = UniqueAlokator(obj._aktivan.get());//Ili get() ili property Aktivan;
        _instrumenti = obj._instrumenti;//Operator == class vector;
        return *this;
    }
    //Z5.6
    const char* GetZanr()const
    {
        return _zanr;
    }
    const char* GetAktivan()const
    {
        return _aktivan.get();//Mora vratiti "raw" pointer const char*;
    }
    vector<const char*> GetInstrumenti() const { return _instrumenti; }
    //Z5.7
    void SetZanr(const char* zanr)
    {
        delete[]_zanr;
        _zanr = Alokator(zanr);
    }
    void SetAktivan(const char* aktivan)
    {
        _aktivan = UniqueAlokator(aktivan);
    }
    //Z5.8
    void DodajInstrument(const char* instrument)
    {
        _instrumenti.push_back(instrument);//Metoda klase vector;
    }
    //Z5.9
    ~Pjevac()
    {
        delete[]_zanr;
        _zanr = nullptr;
        _aktivan.reset();//Nepotrebno;
    }
};
//Z5.10
ostream& operator << (ostream& COUT, const Pjevac& pjevac)
{
    COUT << (Osoba&)pjevac;
    COUT << " " << "Muzicki zanr: " << pjevac.Zanr << endl;
    COUT << "Instrumenti: ";
    for (int i = 0; i < pjevac.Instrumenti.size(); i++)
    {
        COUT << pjevac.Instrumenti[i] << ",";
    }
    return COUT;
}
//Z5.11
bool operator == (const Pjevac& p1, const Pjevac& p2)
{
    if ((Osoba&)p1 == (Osoba&)p2)//Preklopljen operator == komponente Osoba;
        return true;
    if (strcmp(p1.Zanr, p2.Zanr) == 0)
        return true;
    if (strcmp(p1.Aktivan, p2.Aktivan) == 0)
        return true;
    if (p1.Instrumenti == p2.Instrumenti)//Preklopljen == class vector;
        return true;
    return false;
}
bool operator != (const Pjevac& p1, const Pjevac& p2)
{
    if (!(p1 == p2))
        return true;
    return false;
}

class Pjesma {
    const char* _naziv;
    char* _tekst;
    pair<int, int> _trajanje; // <minute, sekunde> [Po defaultu postaviti na <0,0>]
    shared_ptr<char> _tekstopisac;
    int _trenutnoZanrova;
    char* _zanrovi[5] = { nullptr };
public:
    __declspec(property (get = GetNaziv, put = SetNaziv)) const char* Naziv;
    __declspec(property (get = GetTekst, put = SetTekst)) const char* Tekst;
    __declspec(property (get = GetTrajanje, put = SetTrajanje)) pair<int, int> Trajanje;
    __declspec(property (get = GetTekstopisac, put = SetTekstopisac)) const char* Tekstopisac;

    //Z6.1
    Pjesma()//Dflt ctor;
        //: _trajanje(0,0)//Isto kao i dole samo sto je ovdje pozvan copy ctor;
    {
        _naziv = nullptr;
        _tekst = nullptr;
        _trajanje = make_pair<int, int>(0, 0);
        _trenutnoZanrova = 0;
    }
    //Z6.2
    Pjesma(const char* naziv, const char* tekst, pair<int, int> trajanje, const char* tekstopisac)//Usr-def ctor;
        :_trajanje(trajanje.first, trajanje.second)//Usr-def ctor od pair;
    {
        _naziv = Alokator(naziv);
        _tekst = Alokator(tekst);
        _trenutnoZanrova = 0;
        _tekstopisac = SharedAlokator(tekstopisac);
    }
    //Z6.3
    Pjesma(const Pjesma& obj)//Copy ctor;
        :_trajanje(obj._trajanje)//Copy ctor od pair;
    {
        _naziv = Alokator(obj._naziv);
        _tekst = Alokator(obj._tekst);
        _trenutnoZanrova = obj._trenutnoZanrova;
        for (int i = 0; i < _trenutnoZanrova; i++)
        {
            _zanrovi[i] = Alokator(obj._zanrovi[i]);//Alociranje  i prekopiranje stringa;
        }
        _tekstopisac = SharedAlokator(obj._tekstopisac.get());//Moze i get() i property;
    }
    //Z6.4
    Pjesma(Pjesma&& obj)noexcept //Move ctor;
        :_trajanje(move(obj._trajanje))//Move ctor od pair;
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
        _tekst = obj._tekst;
        obj._tekst = nullptr;
        _trenutnoZanrova = obj._trenutnoZanrova;
        obj._trenutnoZanrova = 0;
        for (int i = 0; i < _trenutnoZanrova; i++)
        {
            _zanrovi[i] = obj._zanrovi[i];
            obj._zanrovi[i] = nullptr;
        }
        _tekstopisac = obj._tekstopisac;//Moze i u header sa move jer je smart ptr;
        obj._tekstopisac = nullptr;
        //Ovo je samo da bi se vrijednosti integera stavile na 0 jer move to ne moze uraditi kod primitivnog tipa podatka (int);
        //obj._trajanje = make_pair(0, 0);
        //Ili:
        obj._trajanje.first = 0;
        obj._trajanje.second = 0;
    }
    //Z6.5
    Pjesma& operator = (const Pjesma& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_naziv;
        delete[]_tekst;
        _naziv = Alokator(obj._naziv);
        _tekst = Alokator(obj._tekst);
        _tekstopisac = SharedAlokator(obj._tekstopisac.get());//Moze i get() i property;
        _trajanje = obj._trajanje;
        for (int i = 0; i < _trenutnoZanrova; i++)
        {
            delete _zanrovi[i];
            _zanrovi[i] = nullptr;
        }
        _trenutnoZanrova = obj._trenutnoZanrova;//Tek sada trenutno postane azuriran;
        for (int i = 0; i < _trenutnoZanrova; i++)
        {
            _zanrovi[i] = Alokator(obj._zanrovi[i]);//Alociranje i prekopiranje stringa;
        }
        return *this;
    }
    //Z6.6
    const char* GetNaziv() const { return _naziv; }
    const char* GetTekst() const { return _tekst; }
    pair<int, int> GetTrajanje()const { return _trajanje; }
    const char* GetTekstopisac()const { return _tekstopisac.get(); }//Da bi se vratio "raw" pointer;
    int GetTrenutno()const { return _trenutnoZanrova; }
    const char* GetZanr(int index) const
    {
        index = Max<int>(index, 0);
        index = Min(index, _trenutnoZanrova - 1);
        return _zanrovi[index];
    }
    //Z6.7
    void SetNaziv(const char* naziv)
    {
        delete[] _naziv;
        _naziv = Alokator(naziv);
    }
    void SetTekst(const char* tekst)
    {
        delete[] _tekst;
        _tekst = Alokator(tekst);
    }
    void SetTrajanje(pair<int, int> trajanje)
    {
        _trajanje = trajanje;//Assignment od pair;
    }
    void SetTekstopisac(const char* tekstopisac)
    {
        _tekstopisac = SharedAlokator(tekstopisac);
    }
    //Z6.8
    bool DodajZanr(const char* zanr)
    {
        if (_trenutnoZanrova == 5)
            return false;
        _zanrovi[_trenutnoZanrova] = Alokator(zanr);
        _trenutnoZanrova++;
        return true;
    }
    //Z6.9
    ~Pjesma()
    {
        delete[] _naziv;
        _naziv = nullptr;
        delete[] _tekst;
        _tekst = nullptr;
        _tekstopisac.reset();//Nepotrebno ali good practice;
        for (int i = 0; i < _trenutnoZanrova; i++)
        {
            delete _zanrovi[i];
            _zanrovi[i] = nullptr;
        }
    }
};
//Z6.10
ostream& operator << (ostream& COUT, const Pjesma& obj)
{
    //Lakse je koristiti propertije jer imaju settere i ne mora biti friend ovaj operator kako bi mu bio omogucen pristup;
    COUT << "Naziv: " << obj.Naziv << endl;
    COUT << "Tekst: " << obj.Tekst << endl;
    COUT << "Trajanje: " << obj.Trajanje.first << ":" << obj.Trajanje.second << endl;
    COUT << "Tekstopisac: " << obj.Tekstopisac << endl;
    COUT << "Zanrovi: ";
    for (int i = 0; i < obj.GetTrenutno(); i++)
    {
        COUT << obj.GetZanr(i) << ",";
    }
    COUT << endl;
    return COUT;
}
//Z6.11
bool operator == (const Pjesma& p1, const Pjesma& p2)
{
    if (strcmp(p1.Naziv, p2.Naziv) == 0)
        return true;
    if (strcmp(p1.Tekst, p2.Tekst) == 0)
        return true;
    if (strcmp(p1.Tekstopisac, p2.Tekstopisac) == 0)
        return true;
    if (p1.Trajanje == p2.Trajanje)
        return true;
    if (p1.GetTrenutno() == p2.GetTrenutno())
        return true;
    for (int i = 0; i < p1.GetTrenutno(); i++)
    {
        if (strcmp(p1.GetZanr(i), p2.GetZanr(i)) == 0)
            return true;
    }
    return false;
}
bool operator != (const Pjesma& p1, const Pjesma& p2)
{
    if (!(p1 == p2))
        return true;
    return false;
}

class Album {
    char* _naziv;
    Pjevac* _pjevac;
    Array<Pjesma, 100> _pjesme;
    vector<const char*> _izdavackeKuce;
public:
    __declspec(property (get = GetNaziv, put = SetNaziv)) const char* Naziv;
    __declspec(property (get = GetPjevac, put = SetPjevac)) Pjevac Pjevac_;
    __declspec(property (get = GetPjesme)) Array<Pjesma, 100> Pjesme;
    __declspec(property (get = GetIzdavackeKuce)) vector<const char*> IzdavackeKuce;
    //Z7.1
    Album()//Dflt ctor;
    {
        _naziv = nullptr;
        _pjevac = nullptr;
    }
    //Z7.2
    Album(const char* naziv, Pjevac pjevac)//Usr-def ctor;
    {
        _naziv = Alokator(naziv);
        _pjevac = new Pjevac(pjevac);//Copy ctor class Pjevac;
    }
    //Z7.3
    Album(const Album& obj)//Copy ctor;
        :_pjesme(obj._pjesme), _izdavackeKuce(obj._izdavackeKuce)//Copy ctor class Pjesme, Copy ctor clas vector;
    {
        _naziv = Alokator(obj._naziv);
        assert(obj._pjevac != nullptr);//Ako rvalue ptr nije nuliran izvrsit ce se code ako jeste terminirat ce se proces;
        _pjevac = new Pjevac(*obj._pjevac);//Copy ctor class Pjevac;
    }
    //Z7.4
    Album(Album&& obj) noexcept //Move ctor;
        :_pjesme(move(obj._pjesme)), _izdavackeKuce(move(obj._izdavackeKuce))//Move ctor class Pjesme, Move ctor clas vector;
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
        _pjevac = obj._pjevac;
        obj._pjevac = nullptr;
    }
    //Z7.5
    Album& operator =(const Album& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_naziv;
        if (_pjevac == nullptr)
            _pjevac = new Pjevac();//Dflt ctor;
        _naziv = Alokator(obj._naziv);
        assert(obj._pjevac != nullptr);//Ako nije nullptr izvrsi se code ako jeste terminira se proces;
        *_pjevac = *obj._pjevac;//Assignment operator class Pjevac;
        _pjesme = obj._pjesme;//Assignment operator class Pjesma;
        _izdavackeKuce = obj._izdavackeKuce;//Assignment operator class vector;
        return *this;
    }
    //Z7.6
    const char* GetNaziv() const { return _naziv; }
    Pjevac GetPjevac()const
    {
        assert(_pjevac != nullptr);
        return *_pjevac;
    }
    Array<Pjesma, 100> GetPjesme() const { return _pjesme; }
    vector<const char*> GetIzdavackeKuce() const { return _izdavackeKuce; }
    //Z7.7
    void SetNaziv(const char* naziv)
    {
        delete[]_naziv;
        _naziv = Alokator(naziv);
    }
    void SetPjevac(Pjevac pjevac)
    {
        if (_pjevac == nullptr)
            _pjevac = new Pjevac();//Dflt ctor;
        *_pjevac = pjevac;
    }
    //Z7.8
    bool operator+= (Pjesma novaPjesma)
    {
        if (_pjesme.operator+=(novaPjesma))//Implementiran operator za dodjelu clana niza u class Array;
            return true;
        return false;
    }
    //Z7.9
    void operator+= (const char* izdavackaKuca)
    {
        _izdavackeKuce.push_back(izdavackaKuca);//Metoda class vector;
    }
    //Z7.10
    ~Album()
    {
        delete[]_naziv;
        _naziv = nullptr;
        delete _pjevac;
        _pjevac = nullptr;
    }
};
//Z7.11
ostream& operator <<(ostream& COUT, const Album& obj)
{
    COUT << "Naziv: " << obj.Naziv << endl;
    COUT << "Pjevac: " << obj.Pjevac_ << endl;
    COUT << ".............Pjesme ........................." << endl;
    Ispis(obj.Pjesme, "\n");//Isto se moglo ici for petljom i koristiti preklopljeni operator [] class Array;
    COUT << "Izdavacke kuce: " << endl;
    for (int i = 0; i < obj.IzdavackeKuce.size(); i++)
    {
        COUT << obj.IzdavackeKuce[i] << ",";//Preklopljen operator [] u class vector;
    }
    COUT << endl;
    return COUT;
}
//Z7.12
bool operator == (const Album& a1, const Album& a2)
{
    if (strcmp(a1.Naziv, a2.Naziv) == 0)
        return true;
    if (a1.Pjevac_ == a2.Pjevac_)//Preklopljen operator == u class Pjevac;
        return true;
    if (a1.Pjesme == a2.Pjesme)//Preklopljen operator u class Array;
        return true;
    if (a1.IzdavackeKuce == a2.IzdavackeKuce)//Preklopljen operator == class vector;
        return true;
    return false;
}
bool operator !=(const Album& a1, const Album& a2)
{
    if (!(a1 == a2))
        return true;
    return false;
}

auto IspisiPoruku = [](const char* tipFunkcije, bool success) {
    cout << tipFunkcije << " se izvrsio " << ((success) ? "uspjesno!" : "bezuspjesno!") << endl;
};

void Zadatak1() {
    cout << "Generisanje slucajnog vremena: " << endl;
    Vrijeme vrijeme = Vrijeme::GetRandomVrijeme();
    cout << vrijeme << endl;

    Vrijeme vrijeme2(vrijeme.Sati, vrijeme.Minute, vrijeme.Sekunde);
    IspisiPoruku("User-defined ctor", vrijeme2 == vrijeme);
    Vrijeme vrijeme3(vrijeme);
    IspisiPoruku("Copy ctor", vrijeme3 == vrijeme);
    Vrijeme vrijeme4(move(vrijeme2));
    IspisiPoruku("Move ctor", vrijeme4 == vrijeme);
    Vrijeme vrijeme5;
    vrijeme5 = vrijeme;
    IspisiPoruku("Copy assignment operator", vrijeme5 == vrijeme);

    //Za [27.4]
    cout << endl;
    Vrijeme izlazakSunca(5, 48, 0);
    cout << izlazakSunca << " => " << (izlazakSunca.CheckIfValidnoVrijeme() ? "validno vrijeme" : "nevalidno vrijeme") << endl;
    Vrijeme podne(12, 47, 0);
    cout << podne << " => " << (podne.CheckIfValidnoVrijeme() ? "validno vrijeme" : "nevalidno vrijeme") << endl;
    Vrijeme zalazakSunca(19, 50, 0);
    cout << zalazakSunca << " => " << (zalazakSunca.CheckIfValidnoVrijeme() ? "validno vrijeme" : "nevalidno vrijeme") << endl;
    cout << "\nDealokacija" << endl;
}
//Zadatak1 passed the test !;
void Zadatak2() {
    cout << "Generisanje slucajnog datuma i vremena: " << endl;
    DatumVrijeme dv = DatumVrijeme::GetRandomDatumVrijeme();
    cout << dv << endl;

    DatumVrijeme dv2(dv.Dan, dv.Mjesec, dv.Godina, dv.Sati, dv.Minute, dv.Sekunde);
    IspisiPoruku("User-defined ctor", dv2 == dv);
    DatumVrijeme dv3(dv);
    IspisiPoruku("Copy ctor", dv3 == dv);
    DatumVrijeme dv4(move(dv2));
    IspisiPoruku("Move ctor", dv4 == dv);
    DatumVrijeme dv5;
    dv5 = dv;
    IspisiPoruku("Copy assignment operator", dv5 == dv);

    cout << "\nDealokacija" << endl;
}
//Zadatak2 passed the test !;
void Zadatak3() {
    const int maxElemenata = 100;
    Array<int, maxElemenata> randomNumbers;
    cout << "Dodavanje " << maxElemenata << " elemenata!" << endl;
    for (int i = 0; i < maxElemenata; i++)
        randomNumbers += rand() % 100;
    int toRemove = 75;
    cout << "Uklanjanje " << toRemove << " elemenata!" << endl;
    for (int i = 0; i < 75; i++)
        randomNumbers.UkloniZadnjiElement();
    cout << "Ostalo nam je " << maxElemenata - toRemove << " elemenata!" << endl;
    Ispis(randomNumbers, ", ");

    //Sortiranje Array<int,max>
    cout << "Sortiranje [ASCENDING]: " << endl;
    randomNumbers.Sortiraj(false);
    Ispis(randomNumbers, ", ");
    cout << "Sortiranje [DESCENDING]: " << endl;
    randomNumbers.Sortiraj(true);
    Ispis(randomNumbers, ", ");

    Array<int, maxElemenata> arr2(randomNumbers);
    IspisiPoruku("Copy ctor", arr2 == randomNumbers);

    Array<int, maxElemenata> arr3(move(arr2));
    IspisiPoruku("Move ctor", arr3 == randomNumbers);

    Array<int, maxElemenata> arr4;
    arr4 = randomNumbers;
    IspisiPoruku("Copy assignment operator", arr4 == randomNumbers);
    cout << "\nTestiranje Array<DatumVrijeme,int> \n";
    Array<DatumVrijeme, 10> datumi;
    for (int i = 0; i < 10; i++)
        datumi += DatumVrijeme::GetRandomDatumVrijeme();
    Ispis(datumi, "\n");

    //Sortiranje Array<Datum,max>
    cout << "Sortiranje [ASCENDING]: " << endl;
    datumi.Sortiraj(false);
    Ispis(datumi, "\n");
    cout << "Sortiranje [DESCENDING]: " << endl;
    datumi.Sortiraj(true);
    Ispis(datumi, "\n");
    cout << endl;
    cout << "\nDealokacija" << endl;
}
//Zadatak3 passed the test !;
void Zadatak4() {
    Osoba realDonaldTrump;
    realDonaldTrump.Ime = "Donald J.";
    realDonaldTrump.Prezime = "Trump";
    realDonaldTrump.DatumRodjenja = DatumVrijeme(13, 5, 1945, 17, 30, 0);
    realDonaldTrump.Spol_ = Spol::Muski;
    cout << realDonaldTrump << endl;

    Osoba jobsPresident(realDonaldTrump.Ime, realDonaldTrump.Prezime, realDonaldTrump.DatumRodjenja, realDonaldTrump.Spol_);
    IspisiPoruku("User-defined ctor", jobsPresident == realDonaldTrump);
    Osoba borisJohnson(realDonaldTrump);
    IspisiPoruku("Copy ctor", borisJohnson == realDonaldTrump);
    Osoba magaPresident(move(jobsPresident));
    IspisiPoruku("Move ctor", magaPresident == realDonaldTrump);
    Osoba o4;
    o4 = realDonaldTrump;
    IspisiPoruku("Copy assignment", o4 == realDonaldTrump);
    cout << endl;
    cout << "\nDealokacija" << endl;
}
//Zadatak4 passed the test !;
void Zadatak5() {
    Pjevac theRocketMan;
    theRocketMan.Ime = "Kim";
    theRocketMan.Prezime = "Jong-un";
    theRocketMan.DatumRodjenja = DatumVrijeme(32, 12, 1940, 01, 00, 00);
    theRocketMan.Spol_ = Spol::Muski;
    theRocketMan.Zanr = "K-pop";
    theRocketMan.Aktivan = "2011-present";
    theRocketMan.DodajInstrument("bubnjevi");
    theRocketMan.DodajInstrument("bongo");
    theRocketMan.DodajInstrument("klavir");
    cout << theRocketMan << endl;

    Pjevac supremeFrontman(theRocketMan.Ime, theRocketMan.Prezime,
        theRocketMan.DatumRodjenja, theRocketMan.Spol_, theRocketMan.Zanr, theRocketMan.Aktivan);
    supremeFrontman.DodajInstrument("bubnjevi");
    supremeFrontman.DodajInstrument("bongo");
    supremeFrontman.DodajInstrument("klavir");

    IspisiPoruku("User-defined ctor", supremeFrontman == theRocketMan);
    Pjevac kPopPrince(theRocketMan);
    IspisiPoruku("Copy ctor", kPopPrince == theRocketMan);
    Pjevac sendMeNukes(move(supremeFrontman));
    IspisiPoruku("Move ctor", sendMeNukes == theRocketMan);
    Pjevac p4;
    p4 = theRocketMan;
    IspisiPoruku("Copy assignment", p4 == theRocketMan);
    cout << endl;
    cout << "Dealokacija" << endl;
}
//Zadatak5 passed the test !;
void Zadatak6() {
    Pjesma horseWithNoName;
    horseWithNoName.Naziv = "America - A Horse With No Name";
    horseWithNoName.Tekst = "On the first part of the journey I was looking at all the life";
    horseWithNoName.Trajanje = make_pair<int, int>(4, 7);
    horseWithNoName.Tekstopisac = "Dewey Bunnell";
    horseWithNoName.DodajZanr("folk rock");
    horseWithNoName.DodajZanr("soft rock");
    horseWithNoName.DodajZanr("country rock");
    cout << horseWithNoName;

    Pjesma gtaSaSong(horseWithNoName.Naziv, horseWithNoName.Tekst, horseWithNoName.Trajanje, horseWithNoName.Tekstopisac);
    gtaSaSong.DodajZanr("folk rock");
    gtaSaSong.DodajZanr("soft rock");
    gtaSaSong.DodajZanr("country rock");
    IspisiPoruku("User-defined ctor", gtaSaSong == horseWithNoName);
    Pjesma kdistFTW(horseWithNoName);
    IspisiPoruku("Copy ctor", kdistFTW == horseWithNoName);
    Pjesma breakingBadSong(move(gtaSaSong));
    IspisiPoruku("Move ctor", breakingBadSong == horseWithNoName);
    Pjesma joeysFailedAudition;
    joeysFailedAudition = horseWithNoName;
    IspisiPoruku("Copy assignment", joeysFailedAudition == horseWithNoName);
    cout << endl;
    cout << "Dealokacija" << endl;
}
//Zadatak6 passed the test !;
void Zadatak7() {
    Album finalHour;
    finalHour.Naziv = "Face to face";
    finalHour.Pjevac_ = Pjevac("Phil", "Collins", DatumVrijeme(13, 5, 1951, 14, 30, 00), Spol::Muski, "Pop, soft rock", "1963-2011");

    Pjesma inTheAirTonight("In the air tonight", "I can feel it coming in the air tonight...", make_pair(5, 34), "Phil Collins");
    inTheAirTonight.DodajZanr("Experimental pop");
    inTheAirTonight.DodajZanr("Soft rock");
    Pjesma anotherDay("Another day in paradise", "She calls out to the man on the street...", make_pair(4, 40), "Phil Collins");
    anotherDay.DodajZanr("Soft rock");
    Pjesma rainItDown("I Wish It Would Rain Down", "You know I never meant to see you again...", make_pair(4, 11), "Phil Collins");
    rainItDown.DodajZanr("Pop");

    finalHour += inTheAirTonight;
    finalHour += anotherDay;
    finalHour += rainItDown;
    finalHour += "Atlas Music Publishing";
    finalHour += "Concord Music Publising";
    finalHour += "Big Deal Music";
    finalHour += "Bluewater Music";
    cout << finalHour << endl;

    Album ritual(finalHour.Naziv, finalHour.Pjevac_);
    ritual += inTheAirTonight;
    ritual += anotherDay;
    ritual += rainItDown;
    ritual += "Atlas Music Publishing";
    ritual += "Concord Music Publising";
    ritual += "Big Deal Music";
    ritual += "Bluewater Music";
    IspisiPoruku("User-defined ctor", ritual == finalHour);
    Album copycat(finalHour);
    IspisiPoruku("Copy ctor", copycat == finalHour);
    Album stealCredit(move(ritual));
    IspisiPoruku("Move ctor", stealCredit == finalHour);
    Album imaginationLevelZero;
    imaginationLevelZero = finalHour;
    IspisiPoruku("Copy assignment", imaginationLevelZero == finalHour);
    cout << endl;
    cout << "Dealokacija" << endl;
}
//Zadatak7 passed the test !;
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
            cout << "(7) Zadatak 7" << endl;
            cout << "Unesite odgovarajuci broj zadatka za testiranje: -->: ";
            cin >> izbor;
            cout << endl;
        } while (izbor < 1 || izbor > 7);
        switch (izbor) {
        case 1: Zadatak1(); cout << "Zadatak 1. Done." << endl; break;
        case 2: Zadatak2(); cout << "Zadatak 2. Done." << endl; break;
        case 3: Zadatak3(); cout << "Zadatak 3. Done." << endl; break;
        case 4: Zadatak4(); cout << "Zadatak 4. Done." << endl; break;
        case 5: Zadatak5(); cout << "Zadatak 5. Done." << endl; break;
        case 6: Zadatak6(); cout << "Zadatak 6. Done." << endl; break;
        case 7: Zadatak7(); cout << "Zadatak 7. Done." << endl; break;
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
