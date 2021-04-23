#include<iostream>
#include<vector>
#include<functional>
using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO RE\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";

//Fija za alokaciju i kopiranje stringa:
char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false)
{
    if (sadrzaj == nullptr)//Ako je sadrzaj "empty" 
        return nullptr;//Vrati nullptr;
    int size = strlen(sadrzaj) + 1;
    char* noviTxt = new char[size];
    strcpy_s(noviTxt, size, sadrzaj);//Prekopiraj sadrzaj starog u novi
    if (dealociraj)//Ako je dealokacija odobrena izbrisi niz;
    {
        delete[] sadrzaj;
        sadrzaj = nullptr;//Nije potrebno jer svakako se anulira kad se zavrsi scope funkcije;
    }
    return noviTxt;
}

class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    //Nema potrebe da se pravi default ctor jer jedan u sustini vec imamo (to je ovaj sa default settovanim vrijednostima);

    Datum(int dan = 1, int mjesec = 1, int godina = 2000)//Usr-def ctor;
    {
        //Svaki ptr alocira memoriju i dodijeli vrijednost sa cpy ctorom od int-a;
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& obj)//Copy ctor;
    {
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }
    Datum(Datum&& obj)noexcept//Move ctor;
    {
        _dan = obj._dan;
        obj._dan = nullptr;
        _mjesec = obj._mjesec;
        obj._mjesec = nullptr;
        _godina = obj._godina;
        obj._godina = nullptr;
    }
    Datum& operator = (const Datum& obj)
    {
        if (this == &obj)//Ako je memo lokacija i jednog i drugog ista, vrati obj koji je pozvao fiju;
            return *this;
        //Treba provjeriti da slucajno  memor loakcije ptr nisu alocirane i u tom slucaju ih alocirati
        if (_dan == nullptr)
            _dan = new int;
        if (_mjesec == nullptr)
            _mjesec = new int;
        if (_godina == nullptr)
            _godina = new int;
        *_dan = *obj._dan;
        *_mjesec = *obj._mjesec;
        *_godina = *obj._godina;
        return *this;//Vrati obj koji je pozvao fiju;
    }

    ~Datum()//Dtor;
    {
        delete _dan, _dan = nullptr;
        delete _mjesec, _mjesec = nullptr;
        delete _godina, _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
    friend bool operator == (const Datum& d1, const Datum& d2);
    friend bool operator != (const Datum& d1, const Datum& d2);
};
//Operator == za class Datum:
bool operator == (const Datum& d1, const Datum& d2)
{
    if (*d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina)
        return true;
    return false;
}
//Operator != za class Datum:
bool operator != (const Datum& d1, const Datum& d2)
{
    if (!(d1 == d2))
        return true;
    return false;
}
//Sve za class Datum radi !!!;


template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;//Pok na niz;
    T2* _elementi2;//Pok na niz;
    int _trenutno;//Brojac;
    bool _omoguciDupliranje;//Bool provjera;
public:
    //Nema potrebe za Dflt ctorom jer je ovaj Usr-def ctor ujedno i dflt ako se nista ne proslijedi;
    Kolekcija(bool omoguciDupliranje = true)//Usr-def ctor
    {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _omoguciDupliranje = omoguciDupliranje;
        _trenutno = 0;
    }
    Kolekcija(const Kolekcija<T1, T2>& obj)//Copy ctor;
    {
        _omoguciDupliranje = obj._omoguciDupliranje;
        _trenutno = obj._trenutno;
        //Alociranje lvalue nizova:
        _elementi1 = new T1[_trenutno];
        _elementi2 = new T2[_trenutno];
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];//Kopiranje vrijednosti;
            _elementi2[i] = obj._elementi2[i];
        }
    }
    Kolekcija(Kolekcija<T1, T2>&& obj)noexcept//Move ctor;
    {
        _omoguciDupliranje = obj._omoguciDupliranje;
        _trenutno = obj._trenutno;
        obj._trenutno = 0;//Kako bi se resetovao brojac rvalue objekta;
        _elementi1 = obj._elementi1;
        obj._elementi1 = nullptr;
        _elementi2 = obj._elementi2;
        obj._elementi2 = nullptr;
    }
    Kolekcija<T1, T2>& operator =(const Kolekcija<T1, T2>& obj)
    {
        if (this == &obj)
            return *this;
        delete[] _elementi1;
        delete[] _elementi2;
        _trenutno = obj._trenutno;
        _elementi1 = new T1[_trenutno];
        _elementi2 = new T2[_trenutno];
        for (int i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];//Operator = class T1;
            _elementi2[i] = obj._elementi2[i];//Operator = class T2;
        }
        _omoguciDupliranje = obj._omoguciDupliranje;
        return *this;//Vrati obj koji je pozvao fiju;
    }


    ~Kolekcija()//Dtor;
    {
        delete[]_elementi1;
        _elementi1 = nullptr;
        delete[] _elementi2;
        _elementi2 = nullptr;
    }
    //Getteri:
    T1* GetElementi1() const { return _elementi1; }
    T2* GetElementi2() const { return _elementi2; }
    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() const { return _trenutno; }
    bool DupliranjeOmoguceno()const { return _omoguciDupliranje; }

    //Fija AddElement:

    void AddElement(T1 element1, T2 element2)
    {
        if (_omoguciDupliranje == false)//Ako dupliciranje nije dozvoljeno;
        {
            //Provjera ima li duplikata:
            for (int i = 0; i < _trenutno; i++)
            {
                if (_elementi1[i] == element1 && _elementi2[i] == element2)//Ako ima istih i u 1 nizu i u 2,
                    return;//Izadji iz fije;
            }
        }
        //Ako se ovo gore ne izvrsi onda se mogu dodati elementi, jer je, dupliciranje ili omoguceno ili nema nadjenih duplikata;
        //Ali prvo prosirujemo nizove:

        //Pokazivaci na stare nizove:
        T1* temp1 = _elementi1;
        T2* temp2 = _elementi2;
        //Alociranje novih nizova koji su za jedan veci:
        _elementi1 = new T1[_trenutno + 1];
        _elementi2 = new T2[_trenutno + 1];
        for (int i = 0; i < _trenutno; i++)
        {
            //Prekopiranje:
            _elementi1[i] = temp1[i];
            _elementi2[i] = temp2[i];
        }
        delete[] temp1;
        delete[] temp2;
        temp1 = nullptr;//Opcionalno jer u ovom scope-u se svakako anulira;
        temp2 = nullptr;//Opcionalno jer u ovom scope-u se svakako anulira;
        _elementi1[_trenutno] = element1;
        _elementi2[_trenutno] = element2;
        _trenutno++;
    }
    //Operator == za class  Kolekciju:
    //Iako je unutar klase, ovaj operator je globalna fija
    friend bool operator ==(const Kolekcija<T1, T2>& k1, const Kolekcija<T1, T2>& k2)
    {
        if (k1._trenutno == k2._trenutno)
            return true;
        //Bool nije potrebno provjeravati, a svakako bi bila greska jer ne bi mogao pristupiti k1 i k2 jer su const a fija DupliranjeOmoguceno nije const, ovo se izbjegne ako se napravi fija DupliranjeOmoguceno const;
        if (k1.DupliranjeOmoguceno() == k2.DupliranjeOmoguceno())
            return true;
        for (int i = 0; i < k1._trenutno; i++)//Svejedno je do kojeg _trenutno idemo jer su oba po pretpostavci isti;
        {
            if (k1.getElement1(i) == k2.getElement1(i) || k1.getElement2(i) == k2.getElement2(i))//Podrazumijeva se preklopljen operator == za klase T1 i T2;
                return true;
        }
        return false;
    }
    //Operator !=:
    friend bool operator !=(const Kolekcija<T1, T2>& k1, const Kolekcija<T1, T2>& k2)
    {
        if (!(k1 == k2))
            return true;
        return false;
    }
    //Operator () koji vraca novu kolekciju od stare, "pod-kolekciju" od kolekcije:
    Kolekcija<T1, T2> operator ()(int OD, int DO)
    {
        Kolekcija<T1, T2> temp;//Pomocna kolekcija koja ce se kasnije vratiti, pozvan  je Dflt ctor;

        for (int i = OD; i <= DO; i++)//Idemo OD (pocetni indeks) DO (zavrsni indeks) <= ukljucujuci DO !!! ;
        {
            temp.AddElement(_elementi1[i], _elementi2[i]);//Koristimo AddElement metodu kako bi smo dodali el tipa T1 na prvi niz i el tipa T2 na drugi niz;

            //Ovaj kod baca gresku jer pisemo po memoriji koja nije alocirana a i nizovi se nigdje ne sire po potrebama;
            /*temp.GetElementi1()[i] = _elementi1[i];
            temp.GetElementi2()[i] = _elementi2[i];*/
        }
        return temp;//Vratimo novokreiranu kolekciju;
    }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (int i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
};
//ISTO SE MOGLO I OVAKO :
//template<class T1, class T2>
//bool operator ==(const Kolekcija<T1, T2>& k1, const Kolekcija<T1, T2>& k2)
//{
//
//}
//Sve za class Kolekcija radi !!!;



class Predmet {
    const char* _naziv;//Konstantan niz karaktera;
    char* _detalji;//Niz karaktera;
    char _akademskaGodina[100];//Staticki niz karaktera;
    unique_ptr<int> _brojECTS;//Smart ptr tipa unique_ptr;
    Kolekcija<int, Datum>* _ocjene;//Pokazivac tipa genericke klase Kolekcija;
public:

    //Dflt ctor nije potreban jer u Usr-def ctor-u vec imamo defaulte parametre stoga se on ponasa kao i Dflt ctor.
    Predmet(const char* naziv = "", const char* detalji = "", const char* akademskaGodina = "", int brojECTS = 5)
    {
        _naziv = GetNizKaraktera(naziv);
        _detalji = GetNizKaraktera(detalji);
        strcpy_s(_akademskaGodina, 100, akademskaGodina);
        _brojECTS = make_unique<int>(brojECTS);
        _ocjene = new Kolekcija<int, Datum>;//Pokazivac alocira novu kolekciju tipa int i Datum;
    }
    Predmet(const Predmet& obj)//Copy ctor;

    {
        _naziv = GetNizKaraktera(obj._naziv);
        _detalji = GetNizKaraktera(obj._detalji);
        strcpy_s(_akademskaGodina, 100, obj._akademskaGodina);
        _brojECTS = make_unique<int>(*obj._brojECTS);

        //Opet Copy ctor klase Kolekcija nece raditi kako treba jer je kolekcija _ocjene sada pokazivac !!!;
         //:_ocjene(obj._ocjene)//Copy ctor za class Kolekcija;
        //Stoga moramo koristiti alociranje nove kolekcije na osnovu stare kao i u regularnim  Copy ctor;
        _ocjene = new Kolekcija<int, Datum>(obj._ocjene);//Sa ili bez dereferenciranja (*), isto je;
        //Moze i :
        //_brojECTS.reset(new int(*obj._brojECTS));
    }
    Predmet(Predmet&& obj)noexcept //Move ctor;
        :_brojECTS(move(obj._brojECTS))
    {
        _naziv = obj._naziv;
        obj._naziv = nullptr;
        _detalji = obj._detalji;
        obj._detalji = nullptr;
        strcpy_s(_akademskaGodina, 100, obj._akademskaGodina);
        strcpy_s(obj._akademskaGodina, 100, " ");//"Brisanje" starog stringa;

         //Move ctor od klase kolekcija ne mozemo da koristimo jer je _ocjene pokazivac;
        //_ocjene(move(obj._ocjene))//Koristimo move ctor za class Kolekcija;
        _ocjene = obj._ocjene;
        obj._ocjene = nullptr;//Stoga se mora kao pokazivac nulirati manuelno kako ne bi ostale vrijednosti i stanje tog objekta kod rvalue;
        //_brojECTS = make_unique<int>(*obj._brojECTS);//Ovako samo ostaje u memoriji ne brise se !!!;
        //_brojECTS = move(obj._brojECTS);//Ovako se izbrise stanje u memoriji;
    }
    Predmet& operator=(const Predmet& obj)
    {
        if (this == &obj)
            return *this;
        delete[]_naziv;//Pobrisemo stare;
        delete[] _detalji;//Pobrisemo stare;
        if (_brojECTS == nullptr)//Provjerimo da li je na pokazivacu alociranja memorija, ako nije alociramo je;
            _brojECTS = make_unique<int>();
        if (_ocjene == nullptr)
            _ocjene = new Kolekcija<int, Datum>(obj._ocjene->DupliranjeOmoguceno());//Dflt/Usr-def ctor;
        _naziv = GetNizKaraktera(obj._naziv);
        _detalji = GetNizKaraktera(obj._detalji);
        strcpy_s(_akademskaGodina, 100, obj._akademskaGodina);
        *_brojECTS = *obj._brojECTS;
        *_ocjene = *obj._ocjene;//Preklopljen operator = u generickoj klasi Kolekcija;
        return *this;//Vrati obj koji je pozvao metodu;
    }
    friend bool operator==(const Predmet& p1, const Predmet& p2)
    {
        if (strcmp(p1._naziv, p2._naziv) == 0)
            return true;
        if (strcmp(p1._detalji, p2._detalji) == 0)
            return true;
        if (strcmp(p1._akademskaGodina, p2._akademskaGodina) == 0)
            return true;
        if (*p1._brojECTS == *p2._brojECTS)
            return true;
        if (p1._ocjene == p2._ocjene)//Operator == u gener. klasi Kolekcija;
            return true;
        return false;
    }
    friend bool operator !=(const Predmet& p1, const Predmet& p2)
    {
        if (!(p1 == p2))
            return true;
        return false;
    }

    ~Predmet()//Dtor;
    {
        delete[] _naziv;
        _naziv = nullptr;
        delete[] _detalji;
        _detalji = nullptr;
        _brojECTS.reset();
        delete _ocjene;//Iako se destruktor klase Kolekcija poziva sam, ovo je bio pokazivac tipa te klase, stoga je potrebno i njega obrisati i nulirati;
        _ocjene = nullptr;
    }
    void AddOcjena(int ocjena, Datum datum) {
        _ocjene->AddElement(ocjena, datum);//Dereferenciramo i "otpakujemo"/pristupimo  sa ->;
        //Alternativa:
        //(*_ocjene).AddElement(ocjena, datum);
    }
    //Getteri:
    const char* GetNaziv()const { return _naziv; }
    Kolekcija<int, Datum>& GetOcjene()const { return *_ocjene; }

    friend ostream& operator <<(ostream& COUT, const Predmet& predmet) {
        COUT << "Naziv: " << predmet._naziv << endl;
        COUT << "Detalji: " << predmet._detalji << endl;
        COUT << "Akademska godina: " << predmet._akademskaGodina << endl;
        COUT << "Broj ECTS:" << *predmet._brojECTS << endl;//Dereferenciramo ptr;
        COUT << "Ocjene: " << endl;
        COUT << *predmet._ocjene << endl;//Dereferenciramo ptr na Kolekciju;
        return COUT;
    }
};

//Sve radi za class Predmet !!!;

//formula: (x1 + x2 + x3 + ... + xn)/n;
//Pok na fiju moze isto tako biti i ovakav : float (*aritmetickaSredina)(int[], int);
function<float(int[], int)> aritmetickaSredina = [](int niz[], int size)->float //-> float samo oznaci povratni tip (moze se i izostaviti, kompajler ce znati automatski);
{
    float suma = 0.0f;
    if (size == 0)//Ako je velicina niza 0 vrati samo sumu;
        return suma;
    for (int i = 0; i < size; i++)
    {
        suma += niz[i];
    }
    return suma / size;
    //suma/=size ce spremiti rezultat u suma;
};
//formula:  (x1 * x2 * x3 * ... * xn)^(1/n);
function<float(int[], int)> geometrijskaSredina = [](int niz[], int size)
{
    float proizvod = 1.0f;
    if (size == 0)//Ako je velicina niza 0 vrati samo proizvod;
        return proizvod;
    for (int i = 0; i < size; i++)
    {
        proizvod *= niz[i];
    }
    return pow(proizvod, 1.0f / size);//1.0f  samo zato da ne bi morali castati u float radi preciznosti;
};

//formula:  n/((1/x1) + (1/x2) + ... + (1/xn))
function<float(int[], int)> harmonijskaSredina = [](int niz[], int size)
{
    float suma = 0.0f;
    if (size == 0)//Ako je velicina niza 0 vrati samo sumu;
        return suma;
    for (int i = 0; i < size; i++)
    {
        suma += (1.0f / niz[i]);//1.0f  samo zato da ne bi morali castati u float radi preciznosti;
    }
    return size / suma;
    //size/=suma ce spremiti rezultat u size;
};

//Fija koja vraca random broj:
int GetRandomBroj(int a, int b) {
    return rand() % b + a;
}

void main() {
    cout << PORUKA;
    Datum datum1(1, 1, 2021), datum2(2, 2, 2021), datum3(3, 3, 2021), datum4(4, 4, 2021);

    Datum temp(datum1);
    cout << ((datum1 == temp) ? "Objekti sa istim vrijednostima" : "Objekti sa razlicitim vrijednostima") << endl;
    Datum temp2(move(temp));
    cout << ((datum1 == temp2) ? "Objekti sa istim vrijednostima" : "Objekti sa razlicitim vrijednostima") << endl;
    Datum temp3;
    temp3 = datum1;
    cout << ((datum1 == temp3) ? "Objekti sa istim vrijednostima" : "Objekti sa razlicitim vrijednostima") << endl;
    cout << "\n";

    Kolekcija<Datum, int> ocjene(false);
    ocjene.AddElement(datum1, 10);
    ocjene.AddElement(datum1, 10);
    ocjene.AddElement(datum1, 8);
    ocjene.AddElement(datum2, 6);
    ocjene.AddElement(datum3, 6);
    ocjene.AddElement(datum4, 7);

    Kolekcija<Datum, int> ocjene2(ocjene);
    cout << ((ocjene == ocjene2) ? "Kolekcije sa istim vrijednostima" : "Kolekcije sa razlicitim vrijednostima") << endl;

    Kolekcija<Datum, int> ocjene3(move(ocjene2));
    cout << ((ocjene == ocjene3) ? "Kolekcije sa istim vrijednostima" : "Kolekcije sa razlicitim vrijednostima") << endl;

    Kolekcija<Datum, int> ocjene4;
    ocjene4 = ocjene;
    cout << ((ocjene == ocjene4) ? "Kolekcije sa istim vrijednostima" : "Kolekcije sa razlicitim vrijednostima") << endl;

    cout << ocjene4 << endl;

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> brojevi(false);
    for (int i = 0; i <= kolekcijaTestSize; i++) {
        int broj1 = GetRandomBroj(1, 3);
        int broj2 = GetRandomBroj(1, 3);
        brojevi.AddElement(broj1, broj2);
    }
    cout << brojevi << endl;
    ///*objekat 'brojevi2' ce biti inicijalizovan elementima koji se u objektu 'brojevi' nalaze na lokacijama 1 - 4
    //ukljucujuci i te lokacije.
    Kolekcija<int, int> brojevi2 = brojevi(1, 4);
    cout << brojevi2 << crt;

    Predmet operativniSistemi("Operativni sistemi", "Neki detalji...", "2020/2021", 7),
        diskretnaMatematika("Diskretna matematika", "Neki detalji...", "2020/2021", 6),
        engleskiJezik1("Engleski Jezik I", "Neki detalji...", "2020/2021", 6),
        programiranje1("Programiranje I", "Neki detalji...", "2020/2021", 7);
    programiranje1.AddOcjena(7, Datum(5, 4, 2021));
    programiranje1.AddOcjena(8, Datum(11, 4, 2021));
    programiranje1.AddOcjena(10, Datum(26, 4, 2021));

    Predmet pr1(programiranje1);
    cout << ((pr1 == programiranje1) ? "Objekti sa istim vrijednostima" : "Objekti sa razlicitim vrijednostima") << endl;

    Predmet pr_1(move(pr1));
    cout << ((pr_1 == programiranje1) ? "Objekti sa istim vrijednostima" : "Objekti sa razlicitim vrijednostima") << endl;

    Predmet PR1;
    PR1 = programiranje1;
    cout << ((PR1 == programiranje1) ? "Objekti sa istim vrijednostima" : "Objekti sa razlicitim vrijednostima") << endl;
    cout << PR1 << endl;

    cout << aritmetickaSredina(PR1.GetOcjene().GetElementi1(), PR1.GetOcjene().getTrenutno()) << endl;
    cout << geometrijskaSredina(PR1.GetOcjene().GetElementi1(), PR1.GetOcjene().getTrenutno()) << endl;
    cout << harmonijskaSredina(PR1.GetOcjene().GetElementi1(), PR1.GetOcjene().getTrenutno()) << endl;
    system("pause>0");
}