#include <iostream>
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

class Datum
{
private:
    shared_ptr<int> _dan;//Smart pointeri (shared_ptr);
    shared_ptr<int> _mjesec;
    shared_ptr<int> _godina;
public:
    //Z1.1
    Datum()
    {
        //Moze se napisati :
        /*_dan = nullptr;
        _mjesec = nullptr;
        _godina = nullptr;*/
        //Ali nepotrebno je jer su po defaultu vec nulirani;
    }

    //Z1.2
    Datum(int dan, int mjesec, int godina)
    {
        _dan = make_shared<int>(dan);//Pozvana genericka funkcija koja alocira prostor na heap za jedan int;
        //_dan = make_shared<int>(*new int(dan));//Moze i ovako ali se vrsi bezpotrebna alokacija !!!;
        _mjesec = make_shared<int>(mjesec);
        _godina = make_shared<int>(godina);
    }

    //Z1.3
    Datum(const Datum& obj)//Cpy.ctor;
    {
        _dan = make_shared<int>(*obj._dan);//Alocira se novi prostor sa *obj._dan vrijednoscu;
        _mjesec = make_shared<int>(*obj._mjesec);
        _godina = make_shared<int>(*obj._godina);
    }

    //Z1.4
    Datum(Datum&& obj)noexcept//Move.ctor;
    {
        _dan = obj._dan;
        obj._dan.reset();//Basically obj._dan=nullptr;
        _mjesec = obj._mjesec;
        obj._mjesec.reset();
        _godina = obj._godina;
        obj._godina.reset();
    }

    //Z1.5
    Datum& operator =(const Datum& obj)
    {
        if (this == &obj)
            return *this;
        if (_dan == nullptr)
            _dan = make_shared<int>();
        if (_mjesec == nullptr)
            _mjesec = make_shared<int>();
        if (_godina == nullptr)
            _godina = make_shared<int>();
        //Prostor na heap je vec alociran, stoga se samo dodijele vrijednosti:
        *_dan = *obj._dan;
        *_mjesec = *obj._mjesec;
        *_godina = *obj._godina;
        return *this;//Vrati objekat koji je pozvao ovu fiju;
    }

    //Z1.6
    //Setteri:
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
    //Getteri:
    int GetDan() const { return *_dan; }
    int GetMjesec() const { return *_mjesec; }
    int GetGodina() const { return *_godina; }
    //Z1.7
    ~Datum()
    {
        _dan.reset();//U sustini reset obrise (dealocira memoriju);
        _mjesec.reset();
        _godina.reset();
    }

    friend ostream& operator << (ostream& COUT, const Datum& datum);
    friend bool operator == (const Datum& d1, const Datum& d2);
    friend bool operator > (const Datum&, const Datum&);
};
//Z1.8
bool operator == (const Datum& d1, const Datum& d2)
{
    return (*d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina);
}
//Z1.9
bool operator > (const Datum& d1, const Datum& d2)
{
    if (*d1._godina > * d2._godina)
        return true;
    else if (*d1._godina == *d2._godina)
    {
        if (*d1._mjesec > * d2._mjesec)
        {
            return true;
        }
        else if (*d1._mjesec == *d2._mjesec)
        {
            if (*d1._dan > * d2._dan)
            {
                return true;
            }
            //Ako su isti dani :
            return false;
        }
        return false;
    }
    return false;
}
bool operator >= (const Datum& d1, const Datum& d2)
{
    return (d1 > d2) || (d1 == d2);//Oba operatora su vec preklopljena;
}
bool operator < (const Datum& d1, const Datum& d2)
{
    return !(d1 >= d2);
}
bool operator <= (const Datum& d1, const Datum& d2)
{
    return !(d1 > d2);
}
//Z1.10
ostream& operator << (ostream& COUT, const Datum& datum)
{
    COUT << *datum._dan << "." << *datum._mjesec << "." << *datum._godina << "." << endl;
    return COUT;
}

//Kolekcija elemenata (tip T) sa mogucnoscu dinamickog prosirivanja
template<class T>
class DinamickaKolekcija {
private:
    int _trenutno; //oznacava velicinu niza
    int _maxElemenata;
    T* _elementi;
public:
    //Z2.1
    DinamickaKolekcija(int maxElemenata = 100)
    {
        _maxElemenata = maxElemenata;//Po default ce bit 100 ako se ne prosljedi drugacije;
        _trenutno = 0;
        //_elementi = nullptr;//Ovo bi bilo okej da je dflt.ctor ali je user-def.ctor;
        _elementi = new T[_maxElemenata];//Alocira se niz T elemenata velicine _maxelemenata;
    }
    //Z2.2
    DinamickaKolekcija(const DinamickaKolekcija<T>& kolekcija)//Copy.ctor;
    {
        _maxElemenata = kolekcija._maxElemenata;
        _trenutno = kolekcija._trenutno;
        _elementi = new T[_maxElemenata];//Alocira se novi niz;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = kolekcija._elementi[i];//Podrazumijeva se da class T ima  preklopljen operator =;
        }
    }
    //Z2.3
    DinamickaKolekcija(DinamickaKolekcija<T>&& kolekcija)noexcept//Move.ctor;
    {
        _maxElemenata = kolekcija._maxElemenata;
        //kolekcija._maxElemenata = 0;//Nema svrhe da se radi jer bi se negdje opet morala postaviti;
        _trenutno = kolekcija._trenutno;
        kolekcija._trenutno = 0;
        _elementi = kolekcija._elementi;
        kolekcija._elementi = nullptr;
    }
    //Z2.4
    //Vraca se REFERENCA na T objekat (time se gubi potreba kopiranja);
    DinamickaKolekcija<T>& operator = (const DinamickaKolekcija<T>& kolekcija)
    {
        if (this == &kolekcija)
            return *this;//Ako su iste adrese vrati obj koji je pozvao fiju;
        _trenutno = kolekcija._trenutno;
        _maxElemenata = kolekcija._maxElemenata;
        delete[]_elementi;//Pozove se dtor za svaki element niza tipa T;
        _elementi = new T[_maxElemenata];
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = kolekcija._elementi[i];//Podrazumijeva se da je implementiran operator = za class T;
        }
        return *this;//Vrati se objekat koji je pozvao fiju;
    }

    //Z2.5 :: Osigurati da je index unutar opsega [0,_trenutno-1]. Koristiti genericke funkcije 'Min' i 'Max' 
    T& operator [] (int index) const
    {
        index = Min<int>(0, index);//Ako je index - vratiti ce 0;//Genericka fija  je pozvana manuelno;
        index = Max(index, _trenutno - 1);//Ako je index 20 a trenutno ima 15 el vratiti ce 15-i element;
        //Genericka fija je pozvana implicitno za tip int;
        return _elementi[index];
    }

    //Z2.6
    void ProsiriKolekciju(int prosiriZa)
    {
        int newSize = _maxElemenata + prosiriZa;
        T* temp = _elementi;//Kreiramo novi niz koji pokazuje na stari;
        _elementi = new T[newSize];//Alocira se novi prostor;
        for (int i = 0; i < _trenutno; i++)//Jer jos uvijek je ostao isti br TRENUTNIH elemenata u nizu;
        {
            _elementi[i] = temp[i];//Preklopljeni operator = za klasu T mora postojati;
        }
        delete[] temp;
        temp = nullptr;
    }

    //Z2.7 :: Dodavanje novog elementa (ukoliko je _trenutno < _maxElemenata, uraditi prosirivanje niza za 10)
    bool operator += (T noviElement)
    {
        if (_trenutno == _maxElemenata)//Znaci da smo dostigli granicu, te se niz mora siriti;
            ProsiriKolekciju(10);
        _elementi[_trenutno] = noviElement;//Mora postojati preklopljen operator za class T;
        _trenutno++;
        return true;
    }

    //Z2.8
    bool UkloniZadnjiElement()
    {
        if (_trenutno == 0)
            return false;//Nema se sta ukloniti;
        _trenutno--;//Dekrementovanjem brojaca se ne brise elemenat niza, vec on samo postane kraci, brisanje se desava na kraju tako sto se brise citav niz;
        return true;
    }
    //Z2.9
    int GetTrenutno() const { return _trenutno; }
    int GetMaxElemenata() const { return _maxElemenata; }

    ~DinamickaKolekcija()
    {
        delete[]_elementi;
        _elementi = nullptr;
    }
    //Znak <> kod dole deklarisane funkcije je specijalni znak koji se koristi samo kod generickih istream i ostream preklopnih opratora;
    friend ostream& operator << <> (ostream& COUT, const DinamickaKolekcija<T>& kolekcija);
};
//Z2.10 :: Ispisuje elemente kolekcije. Za pristup elementima koristiti preklopljeni operator []
template<class T>
ostream& operator << <> (ostream& COUT, const DinamickaKolekcija<T>& kolekcija)
{
    COUT << "Elementi kolekcije su: " << endl;
    for (int i = 0; i < kolekcija._trenutno; i++)
    {
        COUT << kolekcija._elementi[i] << " ";//Operator = class T;
    }
    cout << endl;
    return COUT;
}

template<class T, int max> //max oznacava velicinu niza
class StatickaKolekcija {
private:
    int _trenutno; //trenutno elemenata 
    T* _elementi[max] = { nullptr }; //staticki niz pokazivaca
public:
    //Z3.1
    StatickaKolekcija()
    {
        _trenutno = 0;
    }
    //Z3.2
    StatickaKolekcija(const StatickaKolekcija<T, max>& kolekcija)//Cpy.ctor;
    {
        _trenutno = kolekcija._trenutno;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = new T(*kolekcija._elementi[i]);//Novi niz pokazivaca na el se alocira i svaki pok pokazuje na alocirani obj koji je alociran pomocu cpy. ctor od T;
        }
        //Nemoj zab dereferencirati i sami pokazivac !!!;
    }
    //Z3.3
    StatickaKolekcija(StatickaKolekcija<T, max>&& kolekcija) noexcept //Move.ctor;
    {
        _trenutno = kolekcija._trenutno;
        kolekcija._trenutno = 0;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = kolekcija._elementi[i];//Preklopljen operator = za class T;
            kolekcija._elementi[i] = nullptr;//Svaki stari nuliramo;
        }
    }
    //Z3.4
    StatickaKolekcija<T, max>& operator=(const StatickaKolekcija<T, max>& kolekcija)
    {
        if (this == &kolekcija)
            return *this;//Ako su na istim adresama oba obekta, vrati onaj obekat koji je pozvao fiju;
        //Obrisemo sve stare elemente niza:
        for (int i = 0; i < _trenutno; i++)
        {
            delete _elementi[i];
            _elementi[i] = nullptr;
        }
        _trenutno = kolekcija._trenutno;
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi[i] = new T(*kolekcija._elementi[i]);//Copy.ctor; BITNO je da se dereferencira i sami pok na objekat;
        }
        return *this;
    }

    //Z3.5 :: Osigurati da je index unutar opsega [0,_trenutno-1]. Koristiti genericke funkcije 'Min' i 'Max' 
    T& operator [] (int index) const
    {
        index = Min<int>(0, index);
        index = Max(index, _trenutno - 1);
        return *_elementi[index];//OPET NE ZAB DEREFERENCIRATI I SAMI POK NA NIZ POKAZIVACA !!!
    }

    //Z3.6 :: Dodati (alocirati) novi element
    bool operator +=(T noviElement)
    {
        if (_trenutno == max)
            return false;
        _elementi[_trenutno] = new T(noviElement);//Podrazumijeva se da class T ima copy.ctor
        _trenutno++;
        return true;
    }

    //Z3.7 :: Izbrisati (dealocirati) zadnji element i umanjiti brojac
    bool UkloniZadnjiElement()
    {
        if (_trenutno == 0)
            return false;//Nemamo sta da izbrisemo;
        delete _elementi[_trenutno];//Mora se actual objekat izbrisati, tj obj na koji pok pokazuje;
        _elementi[_trenutno] = nullptr;
        _trenutno--;
        return true;
    }

    //Z3.8
    int GetTrenutno() const { return _trenutno; }
    int GetMaxElemenata() const { return max; }

    ~StatickaKolekcija()
    {
        for (int i = 0; i < _trenutno; i++)//Idemo do trenutno jer oni poslije su vec nullpointeri tkd nema svrhe;
        {
            delete _elementi[i];
            _elementi[i] = nullptr;
        }
    }

    friend ostream& operator << <>(ostream& COUT, const StatickaKolekcija<T, max>& kolekcija);
};

//Z3.9
template<class T, int max>
ostream& operator << <>(ostream& COUT, const StatickaKolekcija<T, max>& kolekcija)
{
    COUT << "Elementi niza su : " << endl;
    for (int i = 0; i < kolekcija._trenutno; i++)
    {
        COUT << *kolekcija._elementi[i] << " ";//Operator [] class T;
    }
    cout << endl;
    return COUT;
}

class Sahista {
private:
    const char* _imePrezime;
    Datum _datumRodjenja;
    const char* _drzava;
    shared_ptr<bool> _spol;
public:
    //Z4.1
    Sahista()
    {
        _imePrezime = nullptr;
        _drzava = nullptr;
        //_spol je smart ptr pa je vec nuliran;
    }
    //Z4.2
    Sahista(const char* imePrezime, Datum datumRodjenja, const char* drzava, bool spol)
        :_datumRodjenja(datumRodjenja.GetDan(), datumRodjenja.GetMjesec(), datumRodjenja.GetGodina())//Usr-def.ctor
    {
        _imePrezime = AlocirajIKopiraj(imePrezime);
        _drzava = AlocirajIKopiraj(drzava);
        _spol = make_shared<bool>(spol);

    }
    //Z4.3
    Sahista(const Sahista& obj)//Copy.ctor;
        :_datumRodjenja(obj._datumRodjenja)//Copy.ctor;
    {
        _imePrezime = AlocirajIKopiraj(obj._imePrezime);
        _drzava = AlocirajIKopiraj(obj._drzava);
        _spol = make_shared<bool>(*obj._spol);
    }
    //Z4.4
    Sahista(Sahista&& obj)noexcept//Move ctor;
        :_datumRodjenja(move(obj._datumRodjenja))//Move ctor od class Datum;
    {
        _imePrezime = obj._imePrezime;
        obj._imePrezime = nullptr;
        _drzava = obj._drzava;
        obj._drzava = nullptr;
        _spol = obj._spol;
        obj._spol.reset();//Isto kao i nullptr;
    }
    //Z4.5
    Sahista& operator = (const Sahista& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_imePrezime;
        delete[]_drzava;
        if (_spol == nullptr)
            _spol = make_shared<bool>();
        _imePrezime = AlocirajIKopiraj(obj._imePrezime);
        _drzava = AlocirajIKopiraj(obj._drzava);
        *_spol = *obj._spol;
        _datumRodjenja = obj._datumRodjenja;//Preklopljen operator class Datum;
        return *this;
    }
    //Z4.6
    const char* GetImePrezime() const { return _imePrezime; }
    const char* GetDrzava() const { return _drzava; }
    Datum GetDatumRodjenja() const { return _datumRodjenja; }
    bool GetSpol() const { return *_spol; }

    //Z4.7
    void SetImePrezime(const char* imePrezime)
    {
        delete[] _imePrezime;
        _imePrezime = AlocirajIKopiraj(imePrezime);
    }
    void SetDatumRodjenja(Datum datumRodjenja)
    {
        _datumRodjenja = datumRodjenja;//Preklopljen operator class Datum;
    }
    void SetDrzava(const char* drzava)
    {
        delete[] _drzava;
        _drzava = AlocirajIKopiraj(drzava);
    }
    void SetSpol(bool spol)
    {
        if (_spol == nullptr)
            _spol = make_shared<bool>();
        *_spol = spol;
    }
    //Z4.8
    ~Sahista()
    {
        delete[]_imePrezime;
        _imePrezime = nullptr;
        delete[]_drzava;
        _drzava = nullptr;
        _spol.reset();
    }
    friend ostream& operator << (ostream& COUT, const Sahista& s);
    friend bool operator >(const Sahista& obj1, const Sahista& obj2);
    friend bool operator <(const Sahista& obj1, const Sahista& obj2);
    friend bool operator == (const Sahista& obj1, const Sahista& obj2);
};

//Z4.9
ostream& operator << (ostream& COUT, const Sahista& s)
{
    COUT << "Ime i prezime : " << s._imePrezime << endl;
    COUT << "Drzava : " << s._drzava << endl;
    COUT << "Datum rodjenja : " << s._datumRodjenja << endl;//Preklopljen operator za ispis el class Datum;
    COUT << "Spol : " << ((*s._spol) ? "Musko" : "Zensko") << endl;
    return COUT;
}

//Z4.10 :: Odgovara na pitanje: "Da li je prvi sahista mladji od drugog?"
bool operator >(const Sahista& obj1, const Sahista& obj2)
{
    if (obj1._datumRodjenja > obj2._datumRodjenja)//Preklopljen operator > class Datum;
        return true;
    return false;
}

//Z4.11 :: Odgovara na pitanje: "Da li je drugi sahista mladji od prvog?"
bool operator <(const Sahista& obj1, const Sahista& obj2)
{
    if (obj1._datumRodjenja < obj2._datumRodjenja)//Preklopljen operator < class Datum;
        return true;
    return false;
}

//Vraca true ako sahisti imaju isto ime i prezime
bool operator == (const Sahista& obj1, const Sahista& obj2)
{
    if (strcmp(obj1._imePrezime, obj2._imePrezime) == 0)
        return true;
    return false;
}

bool operator != (const Sahista& obj1, const Sahista& obj2)
{
    return !(obj1 == obj2);//Samo negiramo sta vrati operator ==;
}

//Z4.12 :: Koristiti genericku funkciju Max
Sahista* VratiNajmladjegSahistu(DinamickaKolekcija<Sahista>& sahisti)
{
    Sahista* najmladji = &sahisti[0];
    for (int i = 0; i < sahisti.GetTrenutno(); i++)
    {
        if (Max<Sahista>(sahisti[i], *najmladji) != *najmladji)
        {
            najmladji = &sahisti[i];
        }
    }
    return najmladji;//Vrati pokazivac na najmladjeg elementa;
}

template<class T1, class T2>
class Par {
    T1 _lijevi;
    T2 _desni;
public:
    //Z5.1
    Par()
    {
        //Ostavi se prazan;
    }
    Par(T1 el1, T2 el2)
    {
        _lijevi = el1;
        _desni = el2;
    }
    //Z5.2
    Par(const Par<T1, T2>& obj)//Copy.ctor;
        : _lijevi(obj._lijevi), _desni(obj._desni)//Isto je;
    {
 
    }
    Par(Par<T1, T2>&& obj)noexcept//Move.ctor;
        : _lijevi(move(obj._lijevi)), _desni(move(obj._desni))
    {
    }

    //Z5.3
    Par<T1, T2>& operator = (const Par<T1, T2>& obj)
    {
        if (this == &obj)
            return *this;//Ako su dva objekta na istoj adresi, vrati objekat koji je pozvao fiju;
        _lijevi = obj._lijevi;
        _desni = obj._desni;
        return *this;//Vrati obj koji je pozvao fiju;
    }
    //Z5.4
    T1 GetLijevi() const { return _lijevi; }
    T2 GetDesni() const { return _desni; }
};

//Z5.5
template<class T1, class T2>
ostream& operator << (ostream& COUT, const Par<T1, T2>& par)
{
    COUT << "(lijevi)" << endl;
    COUT << par._lijevi << endl;
    COUT << "(lijevi)" << endl;
    COUT << "(desni)" << endl;
    COUT << par._desni << endl;
    COUT << "(desni)" << endl;
}

class SahovskiTurnir {
private:
    char* _naziv;
    char* _mjestoOdrzavanja;
    StatickaKolekcija<Par<Sahista, Sahista>, 20> _dueli;//Staticki niz parova gdje su parovi sahista i sahista;
public:
    //Z5.6
    SahovskiTurnir()
    {
        _naziv = nullptr;
        _mjestoOdrzavanja = nullptr;
    }
    //Z5.7
    SahovskiTurnir(const char* naziv, const char* mjestoOdrzavanja)//User-def.ctor;
    {
        _naziv = AlocirajIKopiraj(naziv);
        _mjestoOdrzavanja = AlocirajIKopiraj(mjestoOdrzavanja);
    }
    //Z5.8
    bool DodajDuel(Sahista& s1, Sahista& s2)
    {
        if (_dueli += Par<Sahista, Sahista>(s1, s2))//Moguce je ovo zbog preklopljenog operatora += u generickoj klasi StatickaKolekcija;
            return true;
        return false;
    }
    //Z5.9
    ~SahovskiTurnir()
    {
        delete[]_naziv;
        _naziv = nullptr;
        delete[]_mjestoOdrzavanja;
        _mjestoOdrzavanja = nullptr;
    }
    friend ostream& operator <<(ostream& COUT, SahovskiTurnir& st);
};
//Z5.10 :: Ispisati naziv, mjesto, te sve duele
ostream& operator <<(ostream& COUT, SahovskiTurnir& st)
{
    COUT << "Naziv : " << st._naziv << endl;
    COUT << "Mjesto : " << st._mjestoOdrzavanja << endl;
    for (int i = 0; i < st._dueli.GetTrenutno(); i++)
    {
        Sahista prvi = st._dueli[i].GetLijevi();//Podrazumijeva se preklopljen operator [] kao i =  class Sahista;
        Sahista drugi = st._dueli[i].GetDesni();
        cout << "Dueli :( " << i + 1 << " )" << prvi.GetImePrezime() << "--" << drugi.GetImePrezime() << endl;
    }
    return COUT;
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
//Zadatak1 passed the test !;
void Zadatak2() {
    cout << "Kolekcija datuma..." << endl;
    DinamickaKolekcija<Datum> datumi;
    cout << "Dodajemo 5 elemenata: " << endl;
    datumi += Datum(1, 5, 2020);
    datumi += Datum(7, 7, 2020);
    datumi += Datum(1, 3, 2020);
    datumi += Datum(7, 10, 2020);
    datumi += Datum(8, 12, 2020);
    cout << "Ispis elemenata..." << endl;
    cout << datumi << endl;
    cout << "Uklanjamo zadnji element...." << endl;
    datumi.UkloniZadnjiElement();
    cout << datumi << endl;

    cout << "Pravimo kopiju kolekcije: " << endl;
    DinamickaKolekcija<Datum> kopijaDatuma(datumi);
    cout << "Dodajemo jedan element (11.3.2019)" << endl;
    kopijaDatuma += Datum(11, 3, 2019);
    cout << kopijaDatuma << endl;

    cout << "Zatim premjestamo elemente (iz kopije) na novu lokaciju..." << endl;
    DinamickaKolekcija<Datum> noviDatumi(move(kopijaDatuma));
    cout << "Ispis nakon premjestanja elemenata..." << endl;
    cout << noviDatumi << endl;

    cout << "Kreiramo novu kolekciju i vrsimo naknadno kopiranje.. " << endl;
    DinamickaKolekcija<Datum> nova;
    nova = noviDatumi;
    cout << nova << endl;
    cout << "Dealokacija..." << endl;
}
//Zadatak2 passed the test !;
void Zadatak3() {
    cout << "Kreiramo 'tudjeOcjene'" << endl;
    cout << "Dodajemo nove elemente u 'tudjeOcjene'..." << endl;
    StatickaKolekcija<int, 100> tudjeOcjene;
    tudjeOcjene += 9;
    tudjeOcjene += 10;
    tudjeOcjene += 10;
    tudjeOcjene += 10;
    tudjeOcjene += 9;
    tudjeOcjene += 10;
    tudjeOcjene += 10;
    cout << tudjeOcjene << endl;

    cout << "Kreiramo 'mojeOcjene' na osnovu 'tudjeOcjene'" << endl;
    StatickaKolekcija<int, 100> mojeOcjene(tudjeOcjene);
    cout << mojeOcjene << endl;
    cout << "Brisemo sve elemente iz 'mojeOcjene'" << endl;
    for (size_t i = 0; i < 10; i++)
        mojeOcjene.UkloniZadnjiElement();
    cout << "Dodajemo nove (losije) ocjene u 'mojeOcjene'  .... " << endl;
    mojeOcjene += 6;
    mojeOcjene += 7;
    mojeOcjene += 6;
    mojeOcjene += 6;
    mojeOcjene += 7;
    mojeOcjene += 8;
    cout << mojeOcjene << endl;

    cout << "Kreiramo 'josGoreOcjene' na osnovu 'mojeOcjene' " << endl;
    StatickaKolekcija<int, 100> josGoreOcjene(move(mojeOcjene));
    cout << josGoreOcjene << endl;
    cout << "Brisemo sve elemente iz 'josGoreOcjene'..." << endl;
    for (size_t i = 0; i < 10; i++)
        josGoreOcjene.UkloniZadnjiElement();
    cout << "Dodajemo nove (losije) ocjene u 'josGoreOcjene' .... " << endl;
    for (size_t i = 0; i < 5; i++)
        josGoreOcjene += 6;
    cout << josGoreOcjene << endl;

    cout << "Kreiramo 'josGoreOcjene2' na osnovu 'josGoreOcjene' " << endl;
    StatickaKolekcija<int, 100> josGoreOcjene2;
    josGoreOcjene2 = josGoreOcjene;
    cout << josGoreOcjene << endl;
    cout << "Dealokacija..." << endl;
}
//Zadatak3 passed the test !;
void Zadatak4() {
    Sahista s1("Gary Kasparov", Datum(2, 2, 1963), "Rusija", 1);
    Sahista s2(s1);
    Sahista s3(move(s2));
    Sahista garyKasparov;
    garyKasparov = s3;

    Sahista bobbyFischer("Bobby Fischer", Datum(4, 4, 1943), "SAD", 1);
    Sahista carlsen("Magnus Carlsen", Datum(3, 3, 1990), "Norveska", 1);
    Sahista judithPolgar("Judith Polgar", Datum(5, 5, 1976), "Madjarska", 0);

    DinamickaKolekcija<Sahista> sahisti;
    sahisti += garyKasparov;
    sahisti += bobbyFischer;
    sahisti += carlsen;
    sahisti += judithPolgar;

    cout << sahisti << endl;
    cout << "Najmladji sahista: " << endl << (VratiNajmladjegSahistu(sahisti)->GetImePrezime()) << endl;
    cout << "Dealokacija..." << endl;
}
//Zadatak4 passed the test !;
void Zadatak5() {
    Sahista kingBobby("Bobby Fischek", Datum(4, 4, 1943), "SAD", 1);
    Sahista sahmatov("Viktor Sahmatov", Datum(4, 4, 1966), "Rusija", 1);
    Sahista chuckNorris("Chuck Mate", Datum(4, 4, 1966), "SAD", 1);
    SahovskiTurnir bugojno2021("MVST Bugojno (2021)", "Bugojno, BiH");
    bugojno2021.DodajDuel(kingBobby, sahmatov);
    bugojno2021.DodajDuel(sahmatov, chuckNorris);
    bugojno2021.DodajDuel(chuckNorris, kingBobby);
    cout << bugojno2021 << endl;
    cout << "Dealokacija..." << endl;
}
//Zadatak5 passed the test !;
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
