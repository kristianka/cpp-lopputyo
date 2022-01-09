// hotellityo.cpp : This file contains the 'main' function. Program execution begins and ends there.
// kristian kähkönen, 21tietoB

// aikaa käytetty: 26 tuntia

#include <iostream>
#include <string>

using namespace std;

const int HUONEMAARA = 300;

struct Asiakas {
    string nimi;
    double huone_numero = 0;
    long long int varaus_numero = 0;
    int yo = 0;    // öiden määrä
    int yo_hinta = 0; // vierailun lopullinen hinta
    long int pinkoodi = 0; // huoneen avauskoodi
    char huone_koko = '0'; // 1 on yhden hengen, 2 kahden hengen.
    bool check = false; // katsotaan onko huone varattu. 0 jos ei ole, 1 jos on.
};

Asiakas asiakkaan_tiedot(Asiakas *a, int randomi);
int haku(Asiakas *a);                               // haetaan varattua huonetta käyttäjän nimellä
int tulostus(Asiakas *a, int input);                // tulostetaan asiakkaan tiedot
bool nimiTarkistus(string nimi);                    // tarkistetaan että pelkkiä kirjaimia
bool numeroTarkistus(int numero);                   // tarkistetaan että pelkkiä numeroita



// panostin että ohjelma toimii kaikilla käyttöjärjestelmillä, jonka takia en käytä system("cls") -komentoa.
// käytän vain paljon endl. ei ole yhtä hieno, mutta toimii myös linuxilla ja macilla nyt.
// ohjelman olisi voinut modularisoida ehkä enemmän alaohjelmiin, esimerkiksi jotkin syötteet.
// huoneet eivät ole täydellisesti 50-50 vapaana, 40-169 ovat yhden hengen, huoneet 170 - 300 ovat kahden.
// ohjelma sisältää paljon logiikkaa, jonka takia koodi on pitkä.
// 
// lisäosia, jotka itse lisäsin:
// 
// yleisesti tulostuksia hifistelty, esimerkiksi haku() funktiossa.
// kun menussa painaa 3, näyttää jokaisen huoneen varajaan nimen. debug tarkoituksiin tottakai,
// aika iso tietoturvapuute jos aidossa varausjärjestelmässä olisi sellainen. :D
// lisäksi ohjelma arvoo pinkoodin huoneelle väliltä 2999 - 9999
// 
// olen tyytyväinen projektiin itse, toivon tästä arvosanaa 5. panostin tähän paljon aikaa.



int main()
{
    Asiakas a[HUONEMAARA];
    setlocale(LC_ALL, "fi_FI");
    srand(time(NULL));

    bool virhe;
    char input;
    int max = 150; // jaettuna kahdella jotta saadaan parillinen randomi
    int min = 20; // jaettuna kahdella jotta saadaan parillinen randomi
    int random_varaus = rand() % HUONEMAARA + 1;
    int vapaat_huoneet = 2*(rand() % (max - min + 1) + min);
    int varatut_huoneet = HUONEMAARA - vapaat_huoneet;
    bool pariton = true;


    // arvotaan randomi kappaletta huoneita jotka varataan. jos huone on jo varattu, niin arvotaan uusiksi.
    // tämä osuus käy  vain kerran. ei ole menu loopin sisällä.

    for (int i = 40; i < 170; i++)
    {
        int x = rand() % 299 + 1;
        a[i].huone_koko = '1';
    }
    for (int i = 170; i < 300; i++)
    {
        int x = rand() % 299 + 1;
        a[i].huone_koko = '2';
    }
    for (int i = 0; i < 40; i++)
        a[i].check = true;

    // täytetään satunnaisia huoneita
    for (int i = 0; i < varatut_huoneet; i++)
    {
        int x = rand() % 299 + 1;
        if (a[x].check == true && (a[x].huone_koko != '1' && a[x].huone_koko != '2'))
        {
            i--;
        }
        else {
            a[x].huone_numero = x;
            a[x].check = true;
        }
    }


    do {
        cout << "-----------------------------------------------------------" << endl;
        cout << "| Tervetuloa hotellihuoneen varausohjelmaan.              |" << endl;
        cout << "| Mitä haluaisit tehdä? (Syötä numero 1-4)                |" << endl;
        cout << "| 1: Varaa huone                                          |" << endl;
        cout << "| 2: Hae varausta nimellä                                 |" << endl;
        cout << "| 3: Näytä varattujen huoneiden nimet                     |" << endl;
        cout << "| 4: Sammuta ohjelma                                      |" << endl;
        cout << "-----------------------------------------------------------" << endl;

        do
        {
            virhe = false;
            cin >> input;
            // katsotaan että syöte saa olla vain 1 - 4
            if (input != '1' && input != '2' && input !='3' && input != '4'  ) 
            {
                cin.ignore();
                cin.clear();
                cout << "Virhe. Syöte voi olla väliltä 1 - 4." << endl;
                virhe = true;
            }
        } while (virhe);

        switch (input)
        {
        case '1': {

            // otetaan asiakaan tiedot ylös, lähetetään sinne taulukko ja vapaat huoneet määrä
            asiakkaan_tiedot(a, vapaat_huoneet); 
            break;
        }
        case '2': {
            cout << "Valitsit 2" << endl << endl;
            haku(a);
            break;
        }
        case '3': {
            // debug tarkoitukseen, näyttää jokaisen huoneen varaajan nimen
            cout << "Valitsit 3" << endl << endl;
            int laskin = 0;
            for (int i = 0; i < HUONEMAARA; i++) {
                if (a[i].varaus_numero != 0)
                {
                    cout << "Huoneessa " << a[i].huone_numero << " vierailee asiakas " << a[i].nimi << endl;
                    laskin++;
                }
                if (i == 299 && laskin == 0) 
                    cout << "Kukaan ei ole varannut huonetta.";
            }

            cout << endl << endl; 
            break;
        }
        default: {
            break;
        }
    }
    } while (input != '4');
    cout << "Kiitos ohjelman käytöstä. Tervetuloa uudelleen!";
    return 0;
}


Asiakas asiakkaan_tiedot(Asiakas* a, int randomi) {

    long long int random_varaus = rand() % 99999 + 10000;
    char valinta;
    int input = 0;
    bool virhe;
    bool varaustarkistus = true;
    int satunnainen_huone = 0;
    int yhdenhuonenhinta = 100;
    int kahdenhuonenhinta = 150;
    long int pinkoodi = (rand() % (9999 - 2999 + 1) + 2999);


    for (int i = 0; i < HUONEMAARA; i++)
    {
        a[i].huone_numero = i + 1;
    }

    cin.ignore();
    
    cout << endl << "Haluatko valita itse huoneen, vai haluatko satunnaisen huoneen?" << endl;
    cout << "Jos haluat päättää itse, syötä 1. Jos haluat satunnaisen, syötä 2." << endl;

    // syötteen varmistus
    do
    {
        virhe = false;
        cin >> valinta;
        if (valinta != '1' && valinta != '2')
        {
            cin.ignore();
            cin.clear();
            cout << "Virhe. Syöte pitää olla 1 tai 2." << endl;
            virhe = true;
        }
    } while (virhe == true);
     

    switch (valinta)
    {
    case '1': {
        // lasketaan paljonko huoneita on vapaana ja varattu
        int varatut_huone_yhden = 0;
        int varatut_huone_kahden = 0;

        cout << endl << "Huomioithan, että tällä hetkellä vapaita huoneita on rajattu määrä kysynnän vuoksi." << endl;
        cout << "Vapaana on " << randomi << " huonetta, joista puolet ovat yhden hengen huoneita ja toinen puoli kahden hengen huoneita. " << endl;
        cout << "Yhden hengen huoneet, jotka ovat  vapaita:" << endl << endl;
        for (int i = 40; i < HUONEMAARA; i++)
        {
            if (a[i].check == false)
            {
                if (a[i].huone_koko == '1')
                {
                    cout << a[i].huone_numero - 1 << ", ";
                    varatut_huone_yhden++;
                }
            }
        }
        cout << endl << endl << "Kahden hengen huoneet, jotka ovat  vapaita:" << endl << endl;
        for (int i = 170; i < HUONEMAARA; i++)
        {
            if (a[i].check == false)
            {
                if (a[i].huone_koko == '2')
                {
                    cout << a[i].huone_numero - 1 << ", ";
                    varatut_huone_kahden++;
                }
            }
        }
        cout << endl << endl << "Vapaita yhden hengen huoneita on " << varatut_huone_yhden;
        cout << endl << "Vapaita kahden hengen huoneita on " << varatut_huone_kahden;
        // käyttäjä valitsee huoneen
        cout << endl << "Minkä huoneen haluat varata? " << endl;
        bool varaustarkistus = true;
        while (varaustarkistus == true)
        {
            cin >> input;
            while (!(numeroTarkistus(input)))
            {
                cout << "Virhe. Syötä vain numeroita. " << endl;
                cin.clear();
                cin.ignore();
                cin >> input;
            }
            if (a[input].check == true)
            {
                cout << "Huone on varattu. Kokeile eri huoneen varaamista. " << endl;
                cout << "Minkä huoneen haluat varata? ";
            }
            if (a[input].check == false) {
                varaustarkistus = false;
            }
        }

        cout << "Syötä nimesi: ";
        cin.ignore();
        getline(cin, a[input].nimi);

        // katsotaan ettei ole ihme merkkejä syötteessä, pelkkiä aakkosia tai välilyöntejä
        while (!nimiTarkistus(a[input].nimi))
        {
            cout << "Virhe. Syötä vain kirjaimia. " << endl;
            cin.clear();
            getline(cin, a[input].nimi);
        }

        a[input].varaus_numero = random_varaus;
        a[input].pinkoodi = pinkoodi;

        cout << "Montako yötä aiot olla? Tämän hetkinen hinta on " << yhdenhuonenhinta << "e/yö." << endl;
        cin >> a[input].yo;

        // katsotaan että pelkkiä numeroita. käytän aiempaa aliohjelmaa, joten maksimiyömäärä on 300.
        while (!(numeroTarkistus(a[input].yo)))
        {
            cout << "Virhe. Syötä vain numeroita. Voit varata huoneen korkeintaan 300 yöksi. " << endl;
            cin.clear();
            cin.ignore();
            cin >> a[input].yo;
        }

        tulostus(a, input);

        // merkataan että tämä huone on varattu
        a[input].check = true;

        // palaamme mainiin tällä tiedolla
        return a[input];
        break;
    }

    case  '2': {

        // satunnainen
        cout << "Haluatko yhden vai kahden hengen huoneen?" << endl;
        cout << "Jos haluat yhden hengen huoneen, syötä 1. Jos haluat kahden hengen huoneen, syötä 2." << endl;

        do
        {
            cin >> valinta;
            virhe = false;
            // katsotaan että syöte saa olla vain 1 tai 2
            if (valinta != '1' && valinta != '2')
            {
                cin.ignore();
                cin.clear();
                cout << "Virhe. Syöte voi olla 1 tai 2." << endl;
                virhe = true;
            }
        } while (virhe);

        if (valinta == '1')
        {
            // yhden hengen satunnainen huone
            while (varaustarkistus == true)
            {
                satunnainen_huone = rand() % (HUONEMAARA - 1 + 1) + 1;
                // jos huone on vapaana ja huoneen koko on yhden hengen
                if (a[satunnainen_huone].check == false && a[satunnainen_huone].huone_koko == '1') {
                    varaustarkistus = false;
                    break;
                }
            }
            cout << "Syötä nimesi: ";
            cin.ignore();
            getline(cin, a[satunnainen_huone].nimi);

            // katsotaan ettei ole ihme merkkejä syötteessä
            while (!nimiTarkistus(a[satunnainen_huone].nimi))
            {
                cout << "Virhe. Syötä vain kirjaimia. " << endl;
                cin.clear();
                getline(cin, a[satunnainen_huone].nimi);
            }

            // annetaan varausnumero ja ovikoodi
            a[satunnainen_huone].varaus_numero = random_varaus;
            a[satunnainen_huone].pinkoodi = pinkoodi;


            cout << "Montako yötä aiot olla? Tämän hetkinen hinta on " << yhdenhuonenhinta << "e/yö." << endl;
            cin >> a[satunnainen_huone].yo;

            // katsotaan että pelkkiä numeroita. käytän aiempaa aliohjelmaa, joten maksimiyömäärä on 300.
            while (!(numeroTarkistus(a[satunnainen_huone].yo)))
            {
                cout << "Virhe. Syötä vain numeroita. Voit varata huoneen korkeintaan 300 yöksi. " << endl;
                cin.clear();
                cin.ignore();
                cin >> a[satunnainen_huone].yo;
            }
            tulostus(a, satunnainen_huone);

            // merkataan että tämä huone on varattu
            a[satunnainen_huone].check = true;
            // palaamme mainiin tällä tiedolla
            return a[satunnainen_huone];
        }
        // kahden hengen satunnainen huone
        if (valinta == '2')
        {
            while (varaustarkistus == true)
            {
                satunnainen_huone = rand() % (HUONEMAARA - 1 + 1) + 1;

                if (a[satunnainen_huone].check == false && a[satunnainen_huone].huone_koko == '2') {
                    varaustarkistus = false;
                    break;
                }
            }

            cout << "Syötä nimesi: ";
            cin.ignore();
            getline(cin, a[satunnainen_huone].nimi);
            // katsotaan ettei ole ihme merkkejä syötteessä
            while (!nimiTarkistus(a[satunnainen_huone].nimi))
            {
                cout << "Virhe. Syötä vain kirjaimia. " << endl;
                cin.clear();
                getline(cin, a[satunnainen_huone].nimi);
            }

            // annetaan varausnumero ja ovikoodi
            a[satunnainen_huone].varaus_numero = random_varaus;
            a[satunnainen_huone].pinkoodi = pinkoodi;

            cout << "Montako yötä aiot olla? Tämän hetkinen hinta on " << kahdenhuonenhinta << "e/yö." << endl;
            cin >> a[satunnainen_huone].yo;

            // katsotaan että pelkkiä numeroita. käytän aiempaa aliohjelmaa, joten maksimiyömäärä on 300.
            while (!(numeroTarkistus(a[satunnainen_huone].yo)))
            {
                cout << "Virhe. Syötä vain numeroita. Voit varata huoneen korkeintaan 300 yöksi. " << endl;
                cin.clear();
                cin.ignore();
                cin >> a[satunnainen_huone].yo;
            }

            tulostus(a, satunnainen_huone);

            // merkataan että tämä huone on varattu
            a[satunnainen_huone].check = true;
            // palaamme mainiin tällä tiedolla
            return a[satunnainen_huone];
             }
        }
    }
} 

int haku(Asiakas *a)
{
    string varaus_syote;
    bool varmistus = false;

    cout << "Hae varausta nimellä: ";
    cin.ignore();
    getline(cin, varaus_syote);
    cout << endl;

    // syötteen tarkistus
    while (!nimiTarkistus(varaus_syote))
    {
        cout << "Virhe. Syötä vain kirjaimia. " << endl;
        cin.clear();
        getline(cin, varaus_syote);
    }

    for (int i = 0; i < HUONEMAARA; i++)
    {
        if (varaus_syote == a[i].nimi)
        {
            cout << endl << "Varaus löytyi nimelläsi." << endl;
            cout << "Olet varannut huoneen " << a[i].huone_numero - 1 << "." << endl;
            cout << "Huoneen ovikoodi on " << a[i].pinkoodi << "." << endl;
                // katsotaan huonekoko
                if (a[i].huone_koko == '1')
                    cout << "Huone on yhdelle hengelle." << endl;
                
                if (a[i].huone_koko == '2') 
                    cout << "Olet varannut kahdelle hengelle." << endl;
                
                if (a[i].huone_koko != '1' && a[i].huone_koko != '2')
                    cout << "Huoneen koko ei ole tiedossa.";
                
            cout << "Huone on varattu sinulle " << a[i].yo << " yöksi. " << endl;
            cout << "Vierailusi tulee maksamaan " << a[i].yo_hinta << " euroa." << endl << endl;
            varmistus = true;
        }
        // kun ollaan käyty kaikki huoneet läpi ja ei ole löytynyt yhtään niin 
        if (i == HUONEMAARA - 1 && varmistus == false) 
            cout << "Varausta ei löytynyt." << endl << endl;
    }
    return 0;
} 

bool nimiTarkistus(string nimi) {
    char c;
    // jos syöte ei ole mitään, palautuu virhe
    if (nimi.length() == 0) {
        return false;
    }

    // katsotaan stringin jokainen kohta jos jokin ei ole numero niin returnataan virhe
    for (int i = 0; i < nimi.length(); i++) {
        if (nimi[i] >= '0' && nimi[i] <= '9')
            return false;
    }
    for (int i = 0; i < nimi.length(); i++) {

        c = nimi.at(i);

        // tarkistetaan että on vain aakkosia eikä symboleita, esimerkiksi *#@&
        // kommentoi tämä pois jos haluat ääkköset
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == ' '))) {
            return false;
        }
    }
    // jos kaikki on ok, niin lähetetään true ja ohjelma jatkaa eteenpäin mihin jäi.
    return true; 
}


bool numeroTarkistus(int numero) {

    // muutetaan int stringiksi
    string num = to_string(numero);     
    char c;

    // katsotaan stringin jokainen kohta jos jokin ei ole numero niin returnataan virhe
    for (int i = 0; i < num.length(); i++) {
        if (!(num[i] >= '0' && num[i] <= '9'))
            return false;
    }
    // katsotaan jos jokin on kohta on kirjain, jos on niin returnataan virhe
    for (int i = 0; i < num.length(); i++) {
        c = num.at(i);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == ' ')) {
            return false;
        }
    }
    // muutetaan string takaisin intiksi vielä viimeistä varmistusta varten,
    // ettei voi varata huonetta mikä ei ole olemassa.
    int a = stoi(num);

        if (a > HUONEMAARA ||  a < 1) {
            return false;
        }
    // jos kaikki on ok, niin lähetetään true ja ohjelma jatkaa eteenpäin mihin jäi.
    return true;
}

int tulostus(Asiakas *a, int input) {

    long long int random_varaus = rand() % 99999 + 10000;
    int alennusprosentti_rand = 1; //rand() % (2 - 0 + 1) + 0
    double lopullinen_pros = 0;
    double tulostusale = 0;
    int yhdenhuonenhinta = 100;
    int kahdenhuonenhinta = 150;

    // katsotaan alennusprosentti. lopullinen pros on millä kerrotaan ja tulostusale näyttää käyttäjälle alen.
    if (alennusprosentti_rand == 0) {
        lopullinen_pros = 1;
        tulostusale = 0;
    }
    if (alennusprosentti_rand == 1) {
        lopullinen_pros = 0.9;
        tulostusale = 10; // 100% - 10% = 90% eli 0.9
    }
    if (alennusprosentti_rand == 2) {
        lopullinen_pros = 0.8;
        tulostusale = 20;
    }
    // tulostusta
    cout << endl << "Kiitos huoneen varauksesta!" << endl;
    cout << "Huone " << a[input].huone_numero - 1 << " on nyt varattu nimellä " << a[input].nimi << "." << endl;
    cout << "Varausnumerosi on " << a[input].varaus_numero << "." << endl;
    cout << "Huoneesi ovikoodi on " << a[input].pinkoodi << "." << endl;
    a[input].yo_hinta = lopullinen_pros * (a[input].yo * yhdenhuonenhinta);
    cout << a[input].yo << ". yön vierailu maksaa " << a[input].yo_hinta << " euroa. ";
    // tulostetaan vain alennus jos saa alennusta, "Sait 0% alennusta!" olisi aika surullinen
    if (lopullinen_pros != 1)
    cout << "Sait " << tulostusale << " prosenttia alennusta!";
    cout << endl << endl << endl;
    return 0;
}