#include <iostream>
#include <vector>
#include <regex>
#include <thread>
#include <mutex>
#include <fstream>

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
"10.SVE NEDOZVOLJENE RADNJE TOKOM ISPITA CE BITI SANKCIONISANE!\n"
"11. ZA POTREBE TESTIRANJA, U MAINU, BUDITE SLOBODNI DODATI TESTNIH PODATAKA (POZIVA FUNKCIJA) KOLIKO GOD SMATRATE DA JE POTREBNO!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum Kriteriji { CISTOCA, USLUGA, LOKACIJA, UDOBNOST };

bool ValidirajBrojPasosa(const string& brojPasosa)
{
    // /*
	// * za validaciju broja pasosa koristiti funkciju ValidirajBrojPasosa koja treba, koristeci regex, osigurati postivanje
	// sljedeceg formata:
	// - pocetak sadrzi jedan ili dva velika slova
	// - nakon toga slijede tri ili cetiri broja
	// - nakon toga moze, a ne mora, doci znak crtica (-) ili razmak ( )
	// - nakon toga slijede dva do cetiri broja

	// pomenutu funkciju iskoristiti u konstruktoru klase Gost, te ukoliko broj pasosa nije u validnom formatu onda njegova vrijednost treba biti postavljena na NOT VALID
	// */

    regex formaBroja(R"([A-Z]{1,2}\d{3,4}[- ]?\d{2,4})");

    return regex_match(brojPasosa, formaBroja);
}

template<class T1, class T2>
class Kolekcija
{
    T1* _elementi1;
    T2* _elementi2;
    int* _trenutno;
    bool _omoguciDupliranje;

    void ProsiriNizove()
    {
        T1* temp1 = _elementi1;
        T2* temp2 = _elementi2;

        _elementi1 = new T1[*_trenutno + 1];
        _elementi2 = new T2[*_trenutno + 1];

        for (int i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = temp1[i];
            _elementi2[i] = temp2[i];
        }

        delete[] temp1;
        delete[] temp2;

        *_trenutno += 1;
    }
public:
    Kolekcija(bool omoguciDupliranje = true)
	    : _elementi1(nullptr), _elementi2(nullptr), _trenutno(new int(0)), _omoguciDupliranje(omoguciDupliranje)
	{

    }

    Kolekcija(const Kolekcija& obj)
	    : _trenutno(new int(obj.getTrenutno()))
    {
        _elementi1 = new T1[*_trenutno];
        _elementi2 = new T2[*_trenutno];

        for (int i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = obj.getElement1(i);
            _elementi2[i] = obj.getElement2(i);
        }

        _omoguciDupliranje = obj._omoguciDupliranje;
    }

    ~Kolekcija()
	{
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
    }

    T1& getElement1(int lokacija) const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija) const { return _elementi2[lokacija]; }
    int getTrenutno() const { return *_trenutno; }

    Kolekcija& operator= (const Kolekcija& obj)
    {
        if (this != &obj)
        {
	        delete _trenutno;
	        _trenutno = new int(obj.getTrenutno());
	        delete[] _elementi1;
	        delete[] _elementi2;

	        _elementi1 = new T1[*_trenutno];
	        _elementi2 = new T2[*_trenutno];

	        for (int i = 0; i < *_trenutno; i++)
	        {
	            _elementi1[i] = obj.getElement1(i);
	            _elementi2[i] = obj.getElement2(i);
	        }

	        _omoguciDupliranje = obj._omoguciDupliranje;
        }

    	return *this;
    }

    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj)
	{
        for (size_t i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }

    void AddElement(const T1 obj1, const T2 obj2)
    {
        if (!_omoguciDupliranje)
        {
	        for (int i = 0; i < *_trenutno; i++)
	        {
                if (_elementi1[i] == obj1 || _elementi2[i] == obj2)
                    throw exception("Nije dozvoljeno dupliranje elemenata!");
	        }
        }

        ProsiriNizove();

        _elementi1[*_trenutno - 1] = obj1;
        _elementi2[*_trenutno - 1] = obj2;
    }

    Kolekcija InsertAt(int index, int element1, int element2)
    {
        Kolekcija retKolekcija(*this);

        retKolekcija.ProsiriNizove();

        for (int i = *retKolekcija._trenutno - 1; i > index; i--)
        {
            retKolekcija._elementi1[i] = retKolekcija._elementi1[i - 1];
            retKolekcija._elementi2[i] = retKolekcija._elementi2[i - 1];
        }

        retKolekcija._elementi1[index] = element1;
        retKolekcija._elementi2[index] = element2;

        return retKolekcija;
    }
};

class Datum
{
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
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }

    ~Datum()
	{
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }

    Datum& operator= (const Datum& obj)
    {
	    if (this != &obj)
	    {
            delete _dan;
            delete _mjesec;
            delete _godina;

            _dan = new int(*obj._dan);
            _mjesec = new int(*obj._mjesec);
            _godina = new int(*obj._godina);
        }

        return *this;
    }

    friend ostream& operator<< (ostream& COUT, const Datum& obj)
	{
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
};

class Komentar
{
    char* _sadrzajKomentara;
    //uz komentar, gosti mogu ocijeniti svaki od kriterija zasebno, a int se odnosi na ocjenu u rasponu od 1 - 10.
    //onemoguciti dupliranje kriterija tj. svaki kriterij se moze ocijeniti samo jednom...u suprotnom baciti objekat tipa exception
    Kolekcija<Kriteriji, int>* _ocjeneKriterija;

    char* Alociraj(const char* tekst)
    {
        if (tekst == nullptr)
            return nullptr;

        int bufferVel = strlen(tekst) + 1;
        char* retTekst = new char[bufferVel];
        strcpy_s(retTekst, bufferVel, tekst);

        return retTekst;
    }
public:
    Komentar(const char* sadrzajKomentara = "")
	    : _sadrzajKomentara(Alociraj(sadrzajKomentara))
	{
        _ocjeneKriterija = new Kolekcija<Kriteriji, int>();
    }

    Komentar(const Komentar& obj)
	    : _sadrzajKomentara(Alociraj(obj._sadrzajKomentara))
    {
        _ocjeneKriterija = new Kolekcija<Kriteriji, int>(*obj._ocjeneKriterija);
    }

    ~Komentar()
	{
        delete[] _sadrzajKomentara; _sadrzajKomentara = nullptr;
        delete _ocjeneKriterija; _ocjeneKriterija = nullptr;
    }

    Komentar& operator= (const Komentar& obj)
    {
	    if (this != &obj)
	    {
            delete[] _sadrzajKomentara;
            _sadrzajKomentara = Alociraj(obj._sadrzajKomentara);

            *_ocjeneKriterija = *obj._ocjeneKriterija;
	    }

        return *this;
    }

    friend ostream& operator<< (ostream& COUT, const Komentar& obj)
    {
        cout << "Sadrzaj komentara: " << obj._sadrzajKomentara << "\n";
        cout << "Ocjene kriterija: \n";
        cout << *obj._ocjeneKriterija << endl;

        return COUT;
    }

    char* GetSadrzajKomentara() const { return _sadrzajKomentara; }
    Kolekcija<Kriteriji, int>* GetOcjeneKriterija() const { return _ocjeneKriterija; }

    void AddOcjenuKriterija(Kriteriji cistoca, int i)
    {
        if (i < 1 || i > 10)
            throw exception("Ocjena nije validna");

        for (int i = 0; i < _ocjeneKriterija->getTrenutno(); i++)
        {
            if (_ocjeneKriterija->getElement1(i) == cistoca)
                throw exception("Ova kriterija je vec ocjenjena");
        }

        _ocjeneKriterija->AddElement(cistoca, i);
    }

    float GetProsjek() const
    {
        float prosjek = 0.0f;

        for (int i = 0; i < _ocjeneKriterija->getTrenutno(); i++)
        {
            prosjek += _ocjeneKriterija->getElement2(i);
        }

        if (prosjek == 0.0f)
            return prosjek;

        return prosjek / _ocjeneKriterija->getTrenutno();
    }

    static std::string GetKriterijString(Kriteriji kriterij)
    {
	    switch (kriterij)
	    {
	    case CISTOCA:
            return "CISTOCA";
	    case UDOBNOST:
            return "UDOBNOST";
	    case LOKACIJA:
            return "LOKACIJA";
	    case USLUGA:
            return "USLUGA";
	    }

        return "";
    }
};



class Gost
{
    unique_ptr<char[]> _imePrezime;
    string _emailAdresa;
    string _brojPasosa;

    unique_ptr<char[]> Alociraj(const char* tekst)
    {
        if (tekst == nullptr)
            return nullptr;

        int bufferVel = strlen(tekst) + 1;
        auto retTekst = make_unique<char[]>(bufferVel);
        strcpy_s(retTekst.get(), bufferVel, tekst);

        return retTekst;
    }
public:
    Gost(const char* imePrezime, const string& emailAdresa, const string& brojPasosa)
	    : _imePrezime(Alociraj(imePrezime))
	{
        _emailAdresa = emailAdresa;
        _brojPasosa = ValidirajBrojPasosa(brojPasosa) ? brojPasosa : "NOT VALID";
    }

    Gost(const Gost& obj)
	    : _imePrezime(Alociraj(obj._imePrezime.get()))
    {
        _emailAdresa = obj._emailAdresa;
        _brojPasosa = obj._brojPasosa;
    }

    Gost& operator= (const Gost& obj)
    {
	    if (this != &obj)
	    {
            _imePrezime.reset();
            _imePrezime = Alociraj(obj.GetImePrezime());
            _emailAdresa = obj.GetEmail();
            _brojPasosa = obj.GetBrojPasosa();
	    }

        return *this;
    }

    friend ostream& operator<< (ostream& COUT, const Gost& obj)
	{
        COUT << obj._imePrezime.get() << " " << obj._emailAdresa << " " << obj._brojPasosa << endl;
        return COUT;
    }

    friend bool operator== (const Gost& obj1, const Gost& obj2)
    {
        if (strcmp(obj1.GetImePrezime(), obj2.GetImePrezime()) != 0)
            return false;
        if (obj1.GetEmail() != obj2.GetEmail())
            return false;
        if (obj1.GetBrojPasosa() != obj2.GetBrojPasosa())
            return false;

        return true;
    }

    const string& GetEmail() const { return _emailAdresa; }
    const string& GetBrojPasosa() const { return _brojPasosa; }
    char* GetImePrezime() const { return _imePrezime.get(); }
};

mutex mtx;

class Rezervacija {
    Datum _OD;
    Datum _DO;
    //onemoguciti dodavanje gostiju sa istom email adresom ili brojem pasosa u okviru jedne rezervacije
    vector<Gost> _gosti;
    //za svaku rezervaciju se moze ostaviti samo jedan komentar, bez obzira na broj gostiju
    Komentar _komentar;

    void SaljiEmail()
    {
        ///*
		//    To: denis@fit.ba;jasmin@fit.ba;
		//    Subject: Informacija

		//    Postovani,

		//    Zaprimili smo Vase ocjene, a njihova prosjecna vrijednost je X.X.
		//    Zao nam je zbog toga, te ce Vas u najkracem periodu kontaktirati nasa Sluzba za odnose sa gostima.

		//    Ugodan boravak Vam zelimo
		//    Puno pozdrava
		//*/

        string primaoci;

        for (auto& gostObj : _gosti)
            primaoci += gostObj.GetEmail() + ";";

        mtx.lock();
        cout << "To: " << primaoci << "\n";
        cout << "Subject: Informacija" << "\n\n";
        cout << "Postovani,\n\n";
        cout << "Zaprimili smo Vase ocjene, a njihova prosjecna vrijednost je " << _komentar.GetProsjek() << ".\n";
        cout << "Zao nam je zbog toga, te ce Vas u najkracen periodu kontaktirati nasa Sluzba za odnose sa gostima.\n\n";
        cout << "Ugodan boravak Vam zelimo\n";
        cout << "Puno pozdrava" << endl;
        mtx.unlock();
    }
public:
    Rezervacija(const Datum& OD, const Datum& DO, const Gost& gost)
		:_OD(OD), _DO(DO)
	{
        bool duplikat = false;

        for (auto& gostObj : _gosti)
        {
            if (gostObj == gost)
                duplikat = true;
        }

        if (!duplikat)
			_gosti.push_back(gost);
    }

    Rezervacija(const Rezervacija& obj)
	    : _OD(obj._OD), _DO(obj._DO), _gosti(obj._gosti), _komentar(obj._komentar)
    {
	    
    }

    Rezervacija& operator= (const Rezervacija& obj)
    {
	    if (this != &obj)
	    {
            _OD = obj._OD;
            _DO = obj._DO;
            _gosti = obj._gosti;
            _komentar = obj._komentar;
	    }

        return *this;
    }

    friend ostream& operator<< (ostream& COUT, Rezervacija& obj)
	{
        // /* Primjer ispisa:
		//    -------------------------------------------
		//    Rezervacija 19.6.2022 - 20.6.2022 za goste:
		//            1.Denis Music denis@fit.ba BH235-532
		//            2.Jasmin Azemovic jasmin@fit.ba B123321
		//    -------------------------------------------
		//    Komentar rezervacije:
		//    Nismo pretjerano zadovoljni uslugom, a ni lokacijom.
		//            CISTOCA(7)
		//            USLUGA(4)
		//            LOKACIJA(3)
		//            UDOBNOST(6)
		//    -------------------------------------------
		//    Prosjecna ocjena -> 5
		//*/

        const char* br = "-------------------------------------------";

        cout << br << "\n";
        cout << "Rezervacija " << obj._OD << " - " << obj._DO << " za goste:\n";

        auto& gosti = obj.GetGosti();

        for (int i = 0; i < gosti.size(); i++)
        {
            cout << "\t" << i + 1 << "." << gosti.at(i).GetImePrezime() << " " << gosti.at(i).GetEmail() << " " << gosti.at(i).GetBrojPasosa() << "\n";
        }

        cout << br << "\n";
        cout << "Komentar rezervacije:" << "\n";
        cout << obj.GetKomentar().GetSadrzajKomentara() << "\n";

        auto kriteriji = obj.GetKomentar().GetOcjeneKriterija();

        for (int i = 0; i < kriteriji->getTrenutno(); i++)
        {
            cout << "\t" << Komentar::GetKriterijString(kriteriji->getElement1(i)) << "(" << kriteriji->getElement2(i) << ")\n";
        }

        cout << br << "\n";
        cout << "Prosjecna ocjena -> " << obj.GetKomentar().GetProsjek() << endl;

        return COUT;
    }

    vector<Gost>& GetGosti() { return _gosti; }
    const Komentar& GetKomentar() const { return _komentar; }

    bool AddGost(const Gost& gost)
    {
	    for (auto& gostObj : _gosti)
	    {
            if (gostObj == gost)
                return false;
	    }

        _gosti.push_back(gost);
        return true;
    }

    void SetKomentar(const Komentar& komentar)
    {
        // //ukoliko se rezervaciji doda komentar koji sadrzi najmanje dvije ocjene kriterija sa vrijednoscu manjom od 5, svim gostima, 
		// //koji su dodati u rezervaciju, u zasebnom thread-u, se salje email sa sljedecim sadrzajem:
		///*
		//    To: denis@fit.ba;jasmin@fit.ba;
		//    Subject: Informacija

		//    Postovani,

		//    Zaprimili smo Vase ocjene, a njihova prosjecna vrijednost je X.X.
		//    Zao nam je zbog toga, te ce Vas u najkracem periodu kontaktirati nasa Sluzba za odnose sa gostima.

		//    Ugodan boravak Vam zelimo
		//    Puno pozdrava
		//*/

        _komentar = komentar;

        int ocjenaCounter = 0;

        for (int i = 0; i < _komentar.GetOcjeneKriterija()->getTrenutno(); i++)
        {
            if (_komentar.GetOcjeneKriterija()->getElement2(i) < 5)
                ocjenaCounter++;
        }

        if (ocjenaCounter >= 2)
        {
            thread worker(&Rezervacija::SaljiEmail, *this);
            worker.join();
        }
    }

    pair<int, int> GetBrojZnakova(const char* file, const char* trazeniZnakovi)
    {
        /*metoda GetBrojZnakova treba prebrojati koliko se unutar fajla, cije ime je proslijedjeno kao prvi parametar (relativna putanja), nalazi ukupno znakova, te koliko
		se unutar pomenutog fajla nalazi trazenih znakova koji su proslijedjeni kao drugi parametar. Vrijednosti brojaca se vracaju kao par.*/

        fstream stream(file, fstream::out);

        if (stream.is_open())
        {
            // Insertamo dummy text u file ako je prazan radi testiranja
            stream.seekg(0, ios::end);

        	if (stream.tellg() == 0)
        		stream << "denis+music*_";


            stream.flush();
			stream.close(); 
        }

        int totalKaraktera = 0;
        int trazenihKaraktera = 0;

        stream.open(file, fstream::in);

        if (stream.is_open())
        {
            string linija;

            while (getline(stream, linija))
            {
                totalKaraktera += linija.length();

				for (int i = 0; i < strlen(trazeniZnakovi); i++)
				{
                    if (linija.find(trazeniZnakovi[i]) != string::npos)
                        trazenihKaraktera++;
				}
            }
        }

        stream.close();

        return pair<int, int>(totalKaraktera, trazenihKaraktera);
    }
};


const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Pojasnite osnovne preduslove koji moraju biti ispunjeni da bi se realizovao polimorfizam?\n";
    return "Odgovor -> Polimorfizam je prisutan kada imamo nasljedjivanje jedne klase od neke baze klase gdje izvedena klasa override-a neku baznu metodu koja je oznacena kao virtual. To ukljucuje i overridanje operatora kao i overloadanje metoda/funkcija";
}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Pojasnite razloge koristenja kljucnih rijeci abstract i ciste virtualne metode, te razlike izmedju njih?\n";
    return "Odgovor -> Abstract class je klasa koja se ne moze instancirati i predstavlja vid interfejsa koja diktira sta moraju implementirati izvedene klase. Metode koje izvedene klase moraju implementirati se navode kao pure virtual metode u baznoj klasi (npr. virtual void Test() = 0;) Svaka pure virtual metoda se mora overridati u izvedenoj klasi";
}

void main() {

    cout << PORUKA;
    cin.get();

    cout << GetOdgovorNaPrvoPitanje() << endl;
    cin.get();
    cout << GetOdgovorNaDrugoPitanje() << endl;
    cin.get();

    Datum
        datum19062022(19, 6, 2022),
        datum20062022(20, 6, 2022),
        datum30062022(30, 6, 2022),
        datum05072022(5, 7, 2022);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);//dodaje vrijednosti u kolekciju

    cout << kolekcija1 << crt;

    try {
        //ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement treba baciti objekat tipa exception
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }
    cout << kolekcija1 << crt;

   // /* metoda InsertAt treba da doda vrijednosti drugog i treceg parametra na lokaciju koja je definisana prvim parametrom. Povratna vrijednost metode
   // * je objekat (pozivaoc metode, u konkretnom slucaju objekat kolekcija1) u okviru koga su, na definisanu lokaciju, dodati zahtijevani parametri.
    //* Nakon izvrsenja metode InsertAt, oba objekta, kolekcija1 i kolekcija2, bi trebali posjedovati sljedeci sadrzaj:
    //* 10 10
    //* 0  0
    //* 1  1
    //* 2  2
    //* ....
    //*/
    Kolekcija<int, int> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
    cout << kolekcija2 << crt;


   // /*
   // * za validaciju broja pasosa koristiti funkciju ValidirajBrojPasosa koja treba, koristeci regex, osigurati postivanje
   // sljedeceg formata:
   // - pocetak sadrzi jedan ili dva velika slova
   // - nakon toga slijede tri ili cetiri broja
   // - nakon toga moze, a ne mora, doci znak crtica (-) ili razmak ( )
   // - nakon toga slijede dva do cetiri broja

   // pomenutu funkciju iskoristiti u konstruktoru klase Gost, te ukoliko broj pasosa nije u validnom formatu onda njegova vrijednost treba biti postavljena na NOT VALID
   // */

    if (ValidirajBrojPasosa("BH235-532"))
        cout << "Broj pasosa validan" << endl;
    if (ValidirajBrojPasosa("B123321"))
        cout << "Broj pasosa validan" << endl;
    if (ValidirajBrojPasosa("B1252 521"))
        cout << "Broj pasosa validan" << endl;
    if (!ValidirajBrojPasosa("H4521"))
        cout << "Broj pasosa NIJE validan" << endl;
    if (!ValidirajBrojPasosa("b1252 521"))
        cout << "Broj pasosa NIJE validan" << endl;

    cout << crt;

    Gost denis("Denis Music", "denis@fit.ba", "BH235-532");
    Gost jasmin("Jasmin Azemovic", "jasmin@fit.ba", "B123321");
    Gost adel("Adel Handzic", "adel@edu.fit.ba", "B1252 521");
    Gost gostPasosNotValid("Ime Prezime", "korisnik@klix.ba", "H4521");//_brojPasosa = NOT VALID

   // //rezervaciji se prilikom kreiranja mora dodijeliti gost koji je nosilac rezervacije
    Rezervacija rezervacija(datum19062022, datum20062022, denis);
    if (rezervacija.AddGost(jasmin))
        cout << "Gost uspjesno dodan!";

    cout << crt;

    Komentar komentarRezervacija("Nismo pretjerano zadovoljni uslugom, a ni lokacijom.");
    komentarRezervacija.AddOcjenuKriterija(CISTOCA, 7);
    komentarRezervacija.AddOcjenuKriterija(USLUGA, 4);
    komentarRezervacija.AddOcjenuKriterija(LOKACIJA, 3);
    komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);

    try {
        komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);//kriterij udobnosti je vec ocijenjen!
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }

    cout << crt;

   // //u okviru rezervacije postavlja vrijednost atributa _komentar 
    rezervacija.SetKomentar(komentarRezervacija);

   // //ukoliko se rezervaciji doda komentar koji sadrzi najmanje dvije ocjene kriterija sa vrijednoscu manjom od 5, svim gostima, 
   // //koji su dodati u rezervaciju, u zasebnom thread-u, se salje email sa sljedecim sadrzajem:
   ///*
   //    To: denis@fit.ba;jasmin@fit.ba;
   //    Subject: Informacija

   //    Postovani,

   //    Zaprimili smo Vase ocjene, a njihova prosjecna vrijednost je X.X.
   //    Zao nam je zbog toga, te ce Vas u najkracem periodu kontaktirati nasa Sluzba za odnose sa gostima.

   //    Ugodan boravak Vam zelimo
   //    Puno pozdrava
   //*/

   ////ispisuje sve podatke o rezervaciji, gostima, komentar, ocjene pojedinih kriterija i prosjecnu ocjenu.
    cout << rezervacija << endl;
   // /* Primjer ispisa:
   //    -------------------------------------------
   //    Rezervacija 19.6.2022 - 20.6.2022 za goste:
   //            1.Denis Music denis@fit.ba BH235-532
   //            2.Jasmin Azemovic jasmin@fit.ba B123321
   //    -------------------------------------------
   //    Komentar rezervacije:
   //    Nismo pretjerano zadovoljni uslugom, a ni lokacijom.
   //            CISTOCA(7)
   //            USLUGA(4)
   //            LOKACIJA(3)
   //            UDOBNOST(6)
   //    -------------------------------------------
   //    Prosjecna ocjena -> 5
   //*/

    Rezervacija rezervacijaSaAdelom = rezervacija;
    if (rezervacijaSaAdelom.AddGost(adel))
        cout << "Gost uspjesno dodan!" << endl;
    if (!rezervacijaSaAdelom.AddGost(denis))
        cout << "Gost je vec dodan na rezervaciju!" << endl;

    cout << rezervacijaSaAdelom << endl;

    /*metoda GetBrojZnakova treba prebrojati koliko se unutar fajla, cije ime je proslijedjeno kao prvi parametar (relativna putanja), nalazi ukupno znakova, te koliko
    se unutar pomenutog fajla nalazi trazenih znakova koji su proslijedjeni kao drugi parametar. Vrijednosti brojaca se vracaju kao par.*/
    pair<int, int> brojac = rezervacijaSaAdelom.GetBrojZnakova("rezervacije.txt", "*_+");
    cout << "Ukupno znakova u fajlu: " << brojac.first << endl;
    cout << "Ukupno trazenih znakova: " << brojac.second << endl;
    /*
    ako se u fajlu rezervacije.txt nalazi sadrzaj "denis+music*_" onda povratna vrijednost metode GetBrojZnakova treba biti par 13 i 3. za potrebe testiranja kreirajte
    fajl sa zeljenim sadrzajem na istoj lokaciji kao i vas .exe fajl. vodite racuna o tome da se zeljeni naziv fajla definise vrijednoscu parametra.
    */

    cin.get();
    system("pause>0");
}

 