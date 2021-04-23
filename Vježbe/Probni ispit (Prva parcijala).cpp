#include <iostream>
using namespace std;

//Fija za alociranje novog niza karaktera i prekopiranje:
char* AlocirajiKopiraj(const char* txt)
{
    if (txt == nullptr)
        return nullptr;
    int size = strlen(txt) + 1;
    char* noviTxt = new char[size];
    strcpy_s(noviTxt, size, txt);
    return noviTxt;
}

const char* crt = "\n----------------------------------------------------\n";
/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/


class Datum {
    int* _dan, * _mjesec, * _godina;//Radimo sa ptr;
public:
    Datum()//Dflt ctor;
    {
        _dan = nullptr;
        _mjesec = nullptr;
        _godina = nullptr;
    }
    Datum(int d, int m, int g)//User-def ctor
    {
        _dan = new int(d);
        _mjesec = new int(m);
        _godina = new int(g);
    }
    Datum(const Datum& obj)//Copy ctor;
    {
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }
    Datum(Datum&& obj) noexcept //Move ctor;
    {
        //Apsolutno nije potreban ali sam ga implementirao vjezbe radi;
        _dan = obj._dan;
        obj._dan = nullptr;
        _mjesec = obj._mjesec;
        obj._mjesec = nullptr;
        _godina = obj._godina;
        obj._godina = nullptr;
    }
    Datum& operator=(const Datum& obj)//Operator dodjele;
    {
        if (this == &obj)
            return *this;//Ako su na istim lokacijama vrati obj koji je pozvao fiju;
        //Provjerimo da li su prazni:
        if (_dan == nullptr)
            _dan = new int;
        if (_mjesec == nullptr)
            _mjesec = new int;
        if (_godina == nullptr)
            _godina = new int;
        //Dodijelimo vrijednosti:
        *_dan = *obj._dan;
        *_mjesec = *obj._mjesec;
        *_godina = *obj._godina;
        return *this;//Vratimo obj koji je pozvao fiju;
    }
    //Getteri:
    int getDan()const { return *_dan; }
    int getMjesec()const { return *_mjesec; }
    int getGodina()const { return *_godina; }
    ~Datum()//Dtor;
    {
        delete _dan;
        _dan = nullptr;
        delete _mjesec;
        _mjesec = nullptr;
        delete _godina;
        _godina = nullptr;
    }
    friend ostream& operator<<(ostream& COUT, Datum obj) {
        COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina << endl;
        return COUT;
    }
};
//Datum passed the test;

template<class T, int velicina>
class FITArray {
    T _niz[velicina];//Staticki niz objekata tipa T;
    int _trenutno;//Brojac;
    bool _dozvoliDupliciranje;
public:

    FITArray(bool dozvoliDupliciranje = true)//Usr-def ctor kod kojeg je default za duplikate true;
    {
        //Ovaj ctor se vec smatra kao dflt jer ako se ne proslijedi parametar on je ctor bez parametara (aka Dflt.ctor);
        _dozvoliDupliciranje = dozvoliDupliciranje;
        _trenutno = 0;
    }
    FITArray(const FITArray& obj)//Copy ctor;
    {
        _trenutno = obj._trenutno;
        _dozvoliDupliciranje = obj._dozvoliDupliciranje;
        for (int i = 0; i < _trenutno; i++)
        {
            _niz[i] = obj._niz[i];//Operator = class T;
        }
    }

    //Getteri:
    int GetTrenutno() const { return _trenutno; }//Vrati size niza;
    T* GetNiz() { return _niz; }//Vrati pok na niz;

    //Kreiranje fije koja ce provjeravati ima li  tog clana u nizu:
    bool ProvjeridatiClan(T obj)
    {
        for (int i = 0; i < _trenutno; i++)
        {
            if (_niz[i] == obj)//Operator == za class T mora biti implementiran;
                return true;
        }
        return false;
    }
    //Kreiranje operatora dodjele za novi clan niza: (tipa je bool jer se u main testira kroz if()):
    	bool operator +=(const T& obj)
    {
        if (_trenutno >= velicina)
            return false;//Jer se ne moze dodati na nealociranu memoriju, tj prekoraciti opseg niza;
    	if (!(_dozvoliDupliciranje && ProvjeridatiClan(obj)))//Jedino ako su oba true ce se zaobici dodavanje clana na niz;
    	{
            _niz[_trenutno] = obj;//Operator = za class T;
            _trenutno++;//Povecaj broj clanova;
            return true; //Vrati info da je dodan element;
    	}
        return false;
    }
    //Kreiranje operatora -= 
    bool operator -=(const T& obj)
    {
        bool uklonjen = false;
        for (int i = 0; i < _trenutno; i++)
        {
            if (_niz[i] == obj)
            {
                for (int j = i; j < _trenutno - 1; j++)//Idemo do trenutno-1 kako bi se premjestanje izvrsilo adekvatno;
                {
                    //Bez _trenutno-1 bi otisli u nealociranu memoriju i dobili -84321542 (JUNK);
                    _niz[j] = _niz[j + 1];//Izvrsimo izmjestanje vrijednosti; Operator = class T;
                    _niz[j + 1] = 0;//Samo da se nekako postavi neka vrijednost na obrisane clanove;
                }
                uklonjen = true;
            }
        }
        return uklonjen;
    }
    //Operator dodijele za  FITArray:
    FITArray<T, velicina>& operator=(const FITArray<T, velicina>& obj)
    {
        if (this == &obj)//Ako su iste memorijske lokacije
            return *this;//Vrati obj koji je pozvao fiju;
        _trenutno = obj._trenutno;
        _dozvoliDupliciranje = obj._dozvoliDupliciranje;
        for (int i = 0; i < _trenutno; i++)
        {
            _niz[i] = obj._niz[i];
        }
        return *this;//Vrati obj koji je pozvao fiju;
    }
    //Kreiranje operatora () OD DO:
    FITArray<T, velicina> operator () (int OD, int DO)
    {
        FITArray<T, velicina>temp;//Poziv default/user-def ctor-a;
        if (OD > 0 && OD < DO)
        {
            if (DO > _trenutno)
                DO = _trenutno;
            for (int i = OD; i < DO; i++)//Ako ukljucujemo i zadnju lokaciju onda bi stavili i<=DO;
            {
                temp += _niz[i];//Operator += je implementiran za FITArray stoga ga ovdje i iskoristimo;
            }
        }
        return temp;
    }
    ~FITArray()
    {
        //Destruktor nema neke zadatke jer nije nista na heap-u;
    }
    friend ostream& operator <<(ostream& COUT, const FITArray<T, velicina>& obj)
    {
        for (int i = 0; i < obj.GetTrenutno(); i++)
        {
            COUT << obj._niz[i] << ", ";
        }
        cout << endl;
        return COUT;
    }

};

//Preklopljen operator == za class T;
template<class T>
bool operator ==(T& obj1, T& obj2)
{
    return (obj1 == obj2);
}


class PolozeniPredmet {
    char* _naziv;
    int _ocjena;
    Datum _datumPolaganja;
public:

    PolozeniPredmet()
    {
        _naziv = nullptr;
        _ocjena = 0;

    }
    PolozeniPredmet(Datum& polaganje, const char* naziv, int ocjena)//User def ctor;
        :_datumPolaganja(polaganje.getDan(), polaganje.getMjesec(), polaganje.getGodina())//User-def ctor;
    {
        _naziv = AlocirajiKopiraj(naziv);
        _ocjena = ocjena;
    }
    PolozeniPredmet(const PolozeniPredmet& obj)//copy ctor;
        :_datumPolaganja(obj._datumPolaganja)//Copy ctor
    {
        _naziv = AlocirajiKopiraj(obj._naziv);
        _ocjena = obj._ocjena;
    }
    PolozeniPredmet(PolozeniPredmet&& obj) noexcept //Move ctor;
        :_datumPolaganja(move(obj._datumPolaganja))//Move ctor;
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
        _ocjena = obj._ocjena;
        obj._ocjena = 0;
    }

    PolozeniPredmet& operator=(const PolozeniPredmet& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_naziv;
        _naziv = AlocirajiKopiraj(obj._naziv);
        _ocjena = obj._ocjena;
        _datumPolaganja = obj._datumPolaganja;//Preklopljen operator za  class Datum
        return *this;
    }

    int getOcjena()const { return _ocjena; }

    ~PolozeniPredmet()//Dtor;
    {
        delete[] _naziv;
        _naziv = nullptr;
    }
    friend ostream& operator<<(ostream& COUT, const PolozeniPredmet& obj) {
        COUT << "Naziv predmeta : " << " Ocena: " << obj._naziv << "(" << obj._ocjena << ") " << " Datum polaganja : " << obj._datumPolaganja << endl;
        return COUT;
    }
    friend bool operator==(PolozeniPredmet& pr1, PolozeniPredmet& pr2);
};
//PolozeniPredmet passed the test;
bool operator==(PolozeniPredmet& pr1, PolozeniPredmet& pr2)
{
    return (strcmp(pr1._naziv, pr2._naziv) == 0);
}

class Student {
    static int _indeksBrojac; //sluzi za odredjivanje narednog broja indeksa koji ce biti dodijeljen studentu, pocevsi od 200000
    const int _indeks;
    char* _imePrezime;
    FITArray<PolozeniPredmet, 40> _polozeniPredmeti;
public:

    Student()//Dflt ctor;
        :_indeks(_indeksBrojac)
    {
        _imePrezime = nullptr;
    }
    Student(const char* imePrezime) //User def ctor
        :_indeks(_indeksBrojac++)
    {
        _imePrezime = AlocirajiKopiraj(imePrezime);
    }

    Student(const Student& obj)//Copy ctor;
        :_indeks(obj._indeks), _polozeniPredmeti(obj._polozeniPredmeti)//FITArray copy ctor;
    {
        _imePrezime = AlocirajiKopiraj(obj._imePrezime);
    }
    Student(Student&& obj) noexcept //Move ctor;
        :_indeks(obj._indeks), _polozeniPredmeti(move(obj._polozeniPredmeti))//Move ctor FITArray ;
    {
        _imePrezime = obj._imePrezime;
        obj._imePrezime = nullptr;
    }
    Student& operator= (const Student& obj)

    {
        if (this == &obj)
            return *this;
        delete[]_imePrezime;
        _imePrezime = AlocirajiKopiraj(obj._imePrezime);
        _indeksBrojac = obj._indeksBrojac;
        _polozeniPredmeti = obj._polozeniPredmeti;//Preklopljen operator =  FITArray;
        return *this;//Vrati obj koji je pozvao fiju;
    }
    ~Student()//Dtor;
    {
        delete[] _imePrezime;
        _imePrezime = nullptr;
    }

    //Kreiranje fije dodaj predmet:
    void DodajPredmet(PolozeniPredmet pr)
    {
        _polozeniPredmeti += pr;//Preklopljen operator += i ==
    }

    //Fija koja racuna prosjek:
    float GetProsjek()
    {
        float suma = 0.0;
        int trenutnoEl = _polozeniPredmeti.GetTrenutno();
        for (int i = 0; i < trenutnoEl; i++)
        {
            suma += _polozeniPredmeti.GetNiz()[i].getOcjena();
        }
        return suma /= trenutnoEl;
    }
    //Ispis
    friend ostream& operator<<(ostream& COUT, Student& obj) {
        COUT << "Indeks : " << obj._indeks << " " << "Ime i prezime : " << obj._imePrezime << endl << obj._polozeniPredmeti;
        return COUT;
    }
    friend bool operator == (Student& obj1, Student& obj2);

};
//Student passed the test;

//Operator komparacije;
bool operator == (Student& obj1, Student& obj2)
{
    //Preklopljen operator == za class T;
    return (strcmp(obj1._imePrezime, obj2._imePrezime) == 0);
}
//Inizijaliziranje statickog clana :
int Student::_indeksBrojac = 200000;


void main() {
    const int max = 20;

    Datum jucer(19, 4, 2021), danas(20, 4, 2021), sutra(21, 4, 2021);
    cout << jucer << danas << sutra << endl;
    jucer = danas;
    cout << jucer << danas << sutra << endl;
    //Datum today(move(danas));//Provjera move ctor-a;

   //parametar tipa bool odredjuje da li je u nizu dozvoljeno dupliciranje elemenata
    FITArray<int, max> nizIntegera(true);

    for (size_t i = 0; i < max - 1; i++)
        nizIntegera += i;//dodaje novog clana niza; Operator  +=

    if (!(nizIntegera += 6))//pokusavamo dodati dupli clan niza; Operator  +=
        cout << "Element nije dodan u niz" << endl;

    cout << crt << nizIntegera << crt; //ispisuje sve clanove niza;Operator <<


    nizIntegera -= 9; //uklanja clan niza sa vrijednoscu 9
    nizIntegera -= 17;
    cout << "Niz nakon uklanjanja brojeva 9 i 17 : " << endl;
    cout << crt << nizIntegera << crt;

    FITArray<int, max> noviNizIntegera(nizIntegera);//Copy ctor;

    cout << crt << noviNizIntegera << crt;//Operator <<

    ///*parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti. u slucaju da u nizu ne postoji trazeni broj elemenata funkcija treba da vrati sve element od lokacije OD pa do posljednje dostupnog elementa */
    cout << "Clanovi niza od lokacije 2 do lokacije 5 su -> " << noviNizIntegera(2, 5) << crt;

    Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");

    PolozeniPredmet prII(jucer, "PRII", 8), prIII(sutra, "PRIII", 7);

    adel.DodajPredmet(prII);
    adel.DodajPredmet(prIII);
    cout << adel << endl; //Operator <<
    //vraca prosjecnu ocjenu studenta
    cout << "Prosjecna ocjena -> " << adel.GetProsjek() << crt;//Operator <<

    jasmin = adel;//Operator =
    cout << jasmin << endl;//Operator <<
    cout << "Prosjecna ocjena -> " << jasmin.GetProsjek() << crt;
}
