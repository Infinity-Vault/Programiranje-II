#include <iostream>
using namespace std;

const char* crt = "\n-------------------------------------------------\n";
//Fija alociranja i prekopiranja niza karakera:
char* Alokator(const char* txt)
{
	if (txt == nullptr)
		return nullptr;
	unsigned int size = strlen(txt) + 1;
	char* noviTxt = new char[size];
	strcpy_s(noviTxt, size, txt);
	return noviTxt;
}

//Kreiranje klase Dokument:
class Dokument
{
private:
	char* _sadrzaj;
public:
	Dokument()
	{
		_sadrzaj = nullptr;
	}
	Dokument(const char* sadrzaj)//Usr-def ctor;
	{
		_sadrzaj = Alokator(sadrzaj);
	}
	Dokument(const Dokument& obj)//Copy ctor;
	{
		_sadrzaj = Alokator(obj._sadrzaj);
	}
	//Operator <<:
	friend ostream& operator << (ostream& COUT, const Dokument& obj)
	{
		COUT << "Sadrzaj dokumenta: " << obj._sadrzaj << endl;
		return COUT;
	}
	~Dokument()
	{
		delete[] _sadrzaj;
		_sadrzaj = nullptr;
	}
};
//Kreiranje bazne klase Uredjaj:
class Uredjaj
{
private:
	char* _proizvodjac;
	char* _model;
	int _godinaGarancije;
public:
	Uredjaj()//Dflt ctor;
	{
		_proizvodjac = nullptr;
		_model = nullptr;
		_godinaGarancije = 0;
	}
	Uredjaj(const char* proizvodjac, const char* model, int godinaG)//Usr-def ctor;
	{
		_proizvodjac = Alokator(proizvodjac);
		_model = Alokator(model);
		_godinaGarancije = godinaG;
	}
	Uredjaj(const Uredjaj& obj)//Copy ctor;
	{
		_proizvodjac = Alokator(obj._proizvodjac);
		_model = Alokator(obj._model);
		_godinaGarancije = obj._godinaGarancije;
	}
	//Operator = (copy assignment):
	Uredjaj& operator = (const Uredjaj& obj)
	{
		if (this == &obj)//Ako se oba nalaze na istim memorijskim adresama
			return *this;//vrati onaj koji je pozvao metodu;
		delete[]_proizvodjac;
		delete[]_model;
		_proizvodjac = Alokator(obj._proizvodjac);
		_model = Alokator(obj._model);
		_godinaGarancije = obj._godinaGarancije;
		return *this;//Vrati obj koji je pozvao metodu;
	}
	//Oerator redirekcije << (ispisa):
	friend  ostream& operator << (ostream& COUT, const Uredjaj& obj)
	{
		COUT << "Proizvodjac: " << obj._proizvodjac << endl;
		COUT << "Model: " << obj._model << endl;
		COUT << "Broj godina garancije: " << obj._godinaGarancije << endl;
		return COUT;
	}
	//Dtor:
	~Uredjaj()
	{
		delete[]_proizvodjac;
		delete[]_model;
		_proizvodjac = nullptr;
		_model = nullptr;
	}
};
//Kreiranje klase Printer
class Printer : virtual public Uredjaj //Koristenjem virtual keyword-a pri specificiranju tipa nasljedjivanja klase nam u sustini omogucava da se uvijek poziva optimalna metoda/operator ili bilo sta drugo od trenutne najizvedenije klase. Time se osigurava da ce se vijek pozvati tacno ono sto treba u tom momentu;
{
private:
	bool _laserski;
public:
	Printer()//Dflt ctor;
		:Uredjaj()//Dflt ctor;
	{
		_laserski = false;
	}
	Printer(const char* proizvodjac, const char* model, int brojG, bool laserski)//Usr-def ctor;
		:Uredjaj(proizvodjac, model, brojG)//Usr-def ctor;
	{
		_laserski = laserski;
	}
	Printer(const Printer& obj)//Copy ctor;
		:Uredjaj(obj)//Copy ctor;
	{
		_laserski = obj._laserski;
	}
	~Printer()
	{
		//Nemamo neku dealokaciju jer nista nije na heap-u;
	}
	//Operator <<:
	friend ostream& operator << (ostream& COUT, const Printer& obj)
	{
		COUT << static_cast<Uredjaj>(obj);
		COUT << "Laserski: " << boolalpha << obj._laserski << endl;
		return COUT;
	}
	void Isprintaj(Dokument doc)//Copy ctor od Dokument
	{
		cout << "Printam: " << doc << endl;
	}
};
//Kreiranje klase Fax:
class Fax : virtual public Uredjaj
{
private:
	int _brojStranica;
public:
	Fax()//Dflt ctor;
		:Uredjaj()//Dflt ctor;
	{
		_brojStranica = 0;
	}
	Fax(const char* proizvodjac, const char* model, int godinaG, int brojStranica)//Usr-def ctor;
		:Uredjaj(proizvodjac, model, godinaG)//Usr-def ctor;
	{
		_brojStranica = brojStranica;
	}
	Fax(const Fax& obj)//Copy ctor;
		:Uredjaj(obj)//Copy ctor;
	{
		_brojStranica = obj._brojStranica;
	}
	//Operator << :
	friend ostream& operator << (ostream& COUT, const Fax& obj)
	{
		COUT << static_cast<Uredjaj>(obj);
		COUT << "Broj stranica u minuti: " << obj._brojStranica << endl;
		return COUT;
	}
	~Fax()
	{
		//Nemamo sta pobrisati;
	}
	void  posaljiFax(Dokument doc)//Copy ctor Dokument
	{
		cout << "Saljem dokument..." << doc << endl;
	}
};

//Kreiranje klase Skener
class Skener : virtual public Uredjaj
{
private:
	int _dpi;
public:
	Skener()//Dflt ctor;
		:Uredjaj()//Dflt ctor;
	{
		_dpi = 0;
	}
	Skener(const char* proizvodjac, const char* model, int godinaG, int dpi)//Usr-def ctor;
		:Uredjaj(proizvodjac, model, godinaG)//Usr-def ctor;
	{
		_dpi = dpi;
	}
	Skener(const Skener& obj)//Copy ctor;
		:Uredjaj(obj)//Copy ctor;
	{
		_dpi = obj._dpi;
	}
	//Operator <<:
	friend ostream& operator << (ostream& COUT, const Skener& obj)
	{
		COUT << static_cast<Uredjaj>(obj);
		COUT << "Dots per inch: " << obj._dpi << endl;
		return COUT;
	}
	~Skener()
	{
		//Nemamo sta obrisati jer nista nije na heap;
	}
	Dokument Skeniraj()//Vrati kopiju tipa  Dokument
	{
		cout << "Skeniram dokument..." << endl;
		return Dokument("Neki skenirani doc");
		//Ako zelimo vratiti ref na objekat:
		/*Dokument  doc("Neki skenirani doc");
		return doc;*/ //Mora biti ovako jer referenca mora postojati kako bi se vratila;
	}
};
//Kreiranje klase AllInOne
class AllInOne : public Printer, public Skener, public Fax//U ovoj liniji primjenjujemo koncept visenasljednosti u smislu da sada klasa AllInOne ima tri bazne klase, Printer,Skener i Fax;
{
private:
	//Klasa AllInOne nema apsolutno potrebe da ista od svojih atributa ima, sve ce nasljediti iz baznih klasa sto joj treba;
public:
	AllInOne()//Dflt ctor;
		:Printer(), Skener(), Fax()//Dflt ctor od svake bazne klase;
	{
	}
	AllInOne(const char* proizvodjac, const char* model, int godinaG, bool laserski, int dpi, int brojStranica)//Usr-def ctor;
		: Uredjaj(proizvodjac, model, godinaG), Printer(proizvodjac, "envy", godinaG, laserski), Skener(proizvodjac, "envy", godinaG, dpi), Fax(proizvodjac, "envy", godinaG, brojStranica)//Usr-def ctor za svaku od izvedene klase;
	{
		//Ukoliko izvodjenja gore navedenih klasa, iz njihove bazne klase, nisu virtuelna, za svaku pojedinacnu baznu klasu, ovih klasa(Prin.Sken.Fax)
		//ce se pozvati ctor od njene bazne klase (koja je u ovom slucaju svima Uredjaj) stoga cemo imati tri poziva dok bi koristenjem
		//virtualnog izvodjenja imali samo jedan poziv, koji bi se eksplicitno naveo sa Uredjaj(proizvodjac,model,godinaG);
		//Mozemo primjetiti, da iako se mijenja property  model rucno, u ctorovima baznih klasa klase AllInOne
		//pri ispisu samo vidimo ono sto smo poslali nasem jedinom ctoru bazne klase baznih klasa(Printer,Sken,Fax);


		//Drugi primjer jeste, ako se pokusa izbaciti tip nasljedjivanja virtual iz klase npr. Skener, onda dolazimo do problema
		//da ce se Uredjaj pozvati i za class Skener ali i za ove dvije klase (Printer,Fax) koje su ostale virtuelne po nasljedjivanju
		//stoga, imati cemo dva poziva,a prvi za dvije virtualne klase pozivamo preko neke od izvedenih klasa tipa  Printer::Uredjaj( proizvodjac,model,godinaG);
		//Drugi poziv ce se sam izvrsiti za ne virtuelnu klasu Skener;
	}
	~AllInOne()
	{
		//Nemamo sta brisati jer se svaka klasa brine o svojim  dealociranjima;
	}
	//Operator <<:
	friend ostream& operator << (ostream& COUT, const AllInOne& obj)
	{
		//Pozovemo ispis za svaki dio objekta neke od baznih klasa;
		COUT << static_cast<Printer>(obj) << crt;
		COUT << static_cast<Skener>(obj) << crt;
		COUT << static_cast<Fax>(obj) << crt;
		return COUT;
	}
};
void testingPojedinacne()
{
	Uredjaj laptop("Dell", "Arteon", 3);
	cout << laptop << endl;

	Printer epson("Epson", "Envy 235hq", 2, true);
	cout << epson << endl;

	Skener hp("HP", "DeskJet 231dg", 5, 500);
	cout << hp << endl;

	Fax mojo("Mojo", "Runaga 556fh", 2, 5);
	cout << mojo << endl;

	//Testiranje funkcionalnosti:
	cout << "Testiranje funkcionalnosti:";
	cout << crt << endl;
	Dokument potvrda("Potvrda za bla blala blablabla bla blabla bla.");
	epson.Isprintaj(potvrda);
	Dokument skeniraniDoc = hp.Skeniraj();
	cout << skeniraniDoc;
	mojo.posaljiFax(potvrda);
}
void testinAllInOneKlase()
{
	Dokument potvrda("Potvrda...............");
	AllInOne office1("HP", "LaserJet Pro MFP", 5, true, 1000, 500);
	cout << office1;
	cout << crt;
	//Posto je AllInOne klasa od tri bazne klase, sve metode tih triju baznih klasa su nam na raspolaganju preko AllInOne objekata:
	office1.Isprintaj(potvrda);
	office1.posaljiFax(potvrda);
	Dokument skenirani = office1.Skeniraj();
	cout << skenirani;
}
int main()
{
	//testingPojedinacne();
	cout << crt;
	testinAllInOneKlase();



	cin.get();
	return 0;
}