// hotellityo.cpp : This file contains the 'main' function. Program execution begins and ends there.
// kristian kähkönen, 21tietoB

// time used: 26 hours

#include <iostream>
#include <string>

using namespace std;

// translate to english

const int ROOMS = 300;

struct Customer {
    string name;
    double roomNum = 0;
    long long int reservationNum = 0;
    int nights = 0;    // öiden määrä
    int nightCost = 0; // vierailun lopullinen hinta
    long int pinCode = 0; // huoneen avauskoodi
    char roomSize = '0'; // 1 on yhden hengen, 2 kahden hengen.
    bool check = false; // katsotaan onko huone varattu. 0 jos ei ole, 1 jos on.
};

Customer customerInfo(Customer *a, int randomNumber);
int search(Customer *a);                               // haetaan varattua huonetta käyttäjän nimellä
int printInfo(Customer *a, int input);                // tulostetaan asiakkaan tiedot
bool nameCheck(string name);                    // tarkistetaan että pelkkiä kirjaimia
bool numberValidation(int number);                   // tarkistetaan että pelkkiä numeroita



// panostin että ohjelma toimii kaikilla käyttöjärjestelmillä, jonka takia en käytä system("cls") -komentoa.
// käytän vain paljon endl. ei ole yhtä hieno, mutta toimii myös linuxilla ja macilla nyt.
// ohjelman olisi voinut modularisoida ehkä enemmän alaohjelmiin, esimerkiksi jotkin syötteet.
// huoneet eivät ole täydellisesti 50-50 vapaana, 40-169 ovat yhden hengen, huoneet 170 - 300 ovat kahden.
// ohjelma sisältää paljon logiikkaa, jonka takia koodi on pitkä.
// 
// lisäosia, jotka itse lisäsin:
// 
// yleisesti tulostuksia hifistelty, esimerkiksi search() funktiossa.
// kun menussa painaa 3, näyttää jokaisen huoneen varajaan nimen. debug tarkoituksiin tottakai,
// aika iso tietoturvapuute jos aidossa varausjärjestelmässä olisi sellainen. :D
// lisäksi ohjelma arvoo pinCoden huoneelle väliltä 2999 - 9999
// 
// olen tyytyväinen projektiin itse, toivon tästä arvosanaa 5. panostin tähän paljon aikaa.



int main()
{
    Customer a[ROOMS];
    setlocale(LC_ALL, "fi_FI");
    srand(time(NULL));

    bool error;
    char input;
    int max = 150; // jaettuna kahdella jotta saadaan parillinen randomNumber
    int min = 20; // jaettuna kahdella jotta saadaan parillinen randomNumber
    int randomReservation = rand() % ROOMS + 1;
    int freeRooms = 2*(rand() % (max - min + 1) + min);
    int reservedRooms = ROOMS - freeRooms;


    // arvotaan randomNumber kappaletta huoneita jotka varataan. jos huone on jo varattu, niin arvotaan uusiksi.
    // tämä osuus käy  vain kerran. ei ole menu loopin sisällä.

    for (int i = 40; i < 170; i++)
    {
        int x = rand() % 299 + 1;
        a[i].roomSize = '1';
    }
    for (int i = 170; i < 300; i++)
    {
        int x = rand() % 299 + 1;
        a[i].roomSize = '2';
    }
    for (int i = 0; i < 40; i++)
        a[i].check = true;

    // täytetään satunnaisia huoneita
    for (int i = 0; i < reservedRooms; i++)
    {
        int x = rand() % 299 + 1;
        if (a[x].check == true && (a[x].roomSize != '1' && a[x].roomSize != '2'))
        {
            i--;
        }
        else {
            a[x].roomNum = x;
            a[x].check = true;
        }
    }


    do {
        cout << "-----------------------------------------------------------" << endl;
        cout << "| Tervetuloa hotellihuoneen varausohjelmaan.              |" << endl;
        cout << "| Mitä haluaisit tehdä? (Syötä number 1-4)                |" << endl;
        cout << "| 1: Varaa huone                                          |" << endl;
        cout << "| 2: Hae varausta nimellä                                 |" << endl;
        cout << "| 3: Näytä varattujen huoneiden nimet                     |" << endl;
        cout << "| 4: Sammuta ohjelma                                      |" << endl;
        cout << "-----------------------------------------------------------" << endl;

        do
        {
            error = false;
            cin >> input;
            // katsotaan että syöte saa olla vain 1 - 4
            if (input != '1' && input != '2' && input !='3' && input != '4'  ) 
            {
                cin.ignore();
                cin.clear();
                cout << "error. Syöte voi olla väliltä 1 - 4." << endl;
                error = true;
            }
        } while (error);

        switch (input)
        {
        case '1': {

            // otetaan asiakaan tiedot ylös, lähetetään sinne taulukko ja vapaat huoneet määrä
            customerInfo(a, freeRooms); 
            break;
        }
        case '2': {
            cout << "Valitsit 2" << endl << endl;
            search(a);
            break;
        }
        case '3': {
            // debug tarkoitukseen, näyttää jokaisen huoneen varaajan nimen
            cout << "Valitsit 3" << endl << endl;
            int laskin = 0;
            for (int i = 0; i < ROOMS; i++) {
                if (a[i].reservationNum != 0)
                {
                    cout << "Huoneessa " << a[i].roomNum << " vierailee Customer " << a[i].name << endl;
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


Customer customerInfo(Customer* a, int randomNumber) {

    long long int randomReservation = rand() % 99999 + 10000;
    char choice;
    int input = 0;
    bool error;
    bool reservationValidation = true;
    int randomRoom = 0;
    int roomPriceSingle = 100;
    int roomPriceTwo = 150;
    long int pinCode = (rand() % (9999 - 2999 + 1) + 2999);


    for (int i = 0; i < ROOMS; i++)
    {
        a[i].roomNum = i + 1;
    }

    cin.ignore();
    
    cout << endl << "Haluatko valita itse huoneen, vai haluatko satunnaisen huoneen?" << endl;
    cout << "Jos haluat päättää itse, syötä 1. Jos haluat satunnaisen, syötä 2." << endl;

    // syötteen varmistus
    do
    {
        error = false;
        cin >> choice;
        if (choice != '1' && choice != '2')
        {
            cin.ignore();
            cin.clear();
            cout << "error. Syöte pitää olla 1 tai 2." << endl;
            error = true;
        }
    } while (error == true);
     

    switch (choice)
    {
    case '1': {
        // lasketaan paljonko huoneita on vapaana ja varattu
        int reservedRoomsSingle = 0;
        int reservedRoomsTwon = 0;

        cout << endl << "Huomioithan, että tällä hetkellä vapaita huoneita on rajattu määrä kysynnän vuoksi." << endl;
        cout << "Vapaana on " << randomNumber << " huonetta, joista puolet ovat yhden hengen huoneita ja toinen puoli kahden hengen huoneita. " << endl;
        cout << "Yhden hengen huoneet, jotka ovat  vapaita:" << endl << endl;
        for (int i = 40; i < ROOMS; i++)
        {
            if (a[i].check == false)
            {
                if (a[i].roomSize == '1')
                {
                    cout << a[i].roomNum - 1 << ", ";
                    reservedRoomsSingle++;
                }
            }
        }
        cout << endl << endl << "Kahden hengen huoneet, jotka ovat  vapaita:" << endl << endl;
        for (int i = 170; i < ROOMS; i++)
        {
            if (a[i].check == false)
            {
                if (a[i].roomSize == '2')
                {
                    cout << a[i].roomNum - 1 << ", ";
                    reservedRoomsTwon++;
                }
            }
        }
        cout << endl << endl << "Vapaita yhden hengen huoneita on " << reservedRoomsSingle;
        cout << endl << "Vapaita kahden hengen huoneita on " << reservedRoomsTwon;
        // käyttäjä valitsee huoneen
        cout << endl << "Minkä huoneen haluat varata? " << endl;
        bool reservationValidation = true;
        while (reservationValidation == true)
        {
            cin >> input;
            while (!(numberValidation(input)))
            {
                cout << "error. Syötä vain numeroita. " << endl;
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
                reservationValidation = false;
            }
        }

        cout << "Syötä nimesi: ";
        cin.ignore();
        getline(cin, a[input].name);

        // katsotaan ettei ole ihme merkkejä syötteessä, pelkkiä aakkosia tai välilyöntejä
        while (!nameCheck(a[input].name))
        {
            cout << "error. Syötä vain kirjaimia. " << endl;
            cin.clear();
            getline(cin, a[input].name);
        }

        a[input].reservationNum = randomReservation;
        a[input].pinCode = pinCode;

        cout << "Montako yötä aiot olla? Tämän hetkinen hinta on " << roomPriceSingle << "e/yö." << endl;
        cin >> a[input].nights;

        // katsotaan että pelkkiä numeroita. käytän aiempaa aliohjelmaa, joten maksimiyömäärä on 300.
        while (!(numberValidation(a[input].nights)))
        {
            cout << "error. Syötä vain numeroita. Voit varata huoneen korkeintaan 300 yöksi. " << endl;
            cin.clear();
            cin.ignore();
            cin >> a[input].nights;
        }

        printInfo(a, input);

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
            cin >> choice;
            error = false;
            // katsotaan että syöte saa olla vain 1 tai 2
            if (choice != '1' && choice != '2')
            {
                cin.ignore();
                cin.clear();
                cout << "error. Syöte voi olla 1 tai 2." << endl;
                error = true;
            }
        } while (error);

        if (choice == '1')
        {
            // yhden hengen satunnainen huone
            while (reservationValidation == true)
            {
                randomRoom = rand() % (ROOMS - 1 + 1) + 1;
                // jos huone on vapaana ja huoneen koko on yhden hengen
                if (a[randomRoom].check == false && a[randomRoom].roomSize == '1') {
                    reservationValidation = false;
                    break;
                }
            }
            cout << "Syötä nimesi: ";
            cin.ignore();
            getline(cin, a[randomRoom].name);

            // katsotaan ettei ole ihme merkkejä syötteessä
            while (!nameCheck(a[randomRoom].name))
            {
                cout << "error. Syötä vain kirjaimia. " << endl;
                cin.clear();
                getline(cin, a[randomRoom].name);
            }

            // annetaan varausnumero ja ovikoodi
            a[randomRoom].reservationNum = randomReservation;
            a[randomRoom].pinCode = pinCode;


            cout << "Montako yötä aiot olla? Tämän hetkinen hinta on " << roomPriceSingle << "e/yö." << endl;
            cin >> a[randomRoom].nights;

            // katsotaan että pelkkiä numeroita. käytän aiempaa aliohjelmaa, joten maksimiyömäärä on 300.
            while (!(numberValidation(a[randomRoom].nights)))
            {
                cout << "error. Syötä vain numeroita. Voit varata huoneen korkeintaan 300 yöksi. " << endl;
                cin.clear();
                cin.ignore();
                cin >> a[randomRoom].nights;
            }
            printInfo(a, randomRoom);

            // merkataan että tämä huone on varattu
            a[randomRoom].check = true;
            // palaamme mainiin tällä tiedolla
            return a[randomRoom];
        }
        // kahden hengen satunnainen huone
        if (choice == '2')
        {
            while (reservationValidation == true)
            {
                randomRoom = rand() % (ROOMS - 1 + 1) + 1;

                if (a[randomRoom].check == false && a[randomRoom].roomSize == '2') {
                    reservationValidation = false;
                    break;
                }
            }

            cout << "Syötä nimesi: ";
            cin.ignore();
            getline(cin, a[randomRoom].name);
            // katsotaan ettei ole ihme merkkejä syötteessä
            while (!nameCheck(a[randomRoom].name))
            {
                cout << "error. Syötä vain kirjaimia. " << endl;
                cin.clear();
                getline(cin, a[randomRoom].name);
            }

            // annetaan varausnumero ja ovikoodi
            a[randomRoom].reservationNum = randomReservation;
            a[randomRoom].pinCode = pinCode;

            cout << "Montako yötä aiot olla? Tämän hetkinen hinta on " << roomPriceTwo << "e/yö." << endl;
            cin >> a[randomRoom].nights;

            // katsotaan että pelkkiä numeroita. käytän aiempaa aliohjelmaa, joten maksimiyömäärä on 300.
            while (!(numberValidation(a[randomRoom].nights)))
            {
                cout << "error. Syötä vain numeroita. Voit varata huoneen korkeintaan 300 yöksi. " << endl;
                cin.clear();
                cin.ignore();
                cin >> a[randomRoom].nights;
            }

            printInfo(a, randomRoom);

            // merkataan että tämä huone on varattu
            a[randomRoom].check = true;
            // palaamme mainiin tällä tiedolla
            return a[randomRoom];
             }
        }
    }
} 

int search(Customer *a)
{
    string varaus_syote;
    bool varmistus = false;

    cout << "Hae varausta nimellä: ";
    cin.ignore();
    getline(cin, varaus_syote);
    cout << endl;

    // syötteen tarkistus
    while (!nameCheck(varaus_syote))
    {
        cout << "error. Syötä vain kirjaimia. " << endl;
        cin.clear();
        getline(cin, varaus_syote);
    }

    for (int i = 0; i < ROOMS; i++)
    {
        if (varaus_syote == a[i].name)
        {
            cout << endl << "Varaus löytyi nimelläsi." << endl;
            cout << "Olet varannut huoneen " << a[i].roomNum - 1 << "." << endl;
            cout << "Huoneen ovikoodi on " << a[i].pinCode << "." << endl;
                // katsotaan huonekoko
                if (a[i].roomSize == '1')
                    cout << "Huone on yhdelle hengelle." << endl;
                
                if (a[i].roomSize == '2') 
                    cout << "Olet varannut kahdelle hengelle." << endl;
                
                if (a[i].roomSize != '1' && a[i].roomSize != '2')
                    cout << "Huoneen koko ei ole tiedossa.";
                
            cout << "Huone on varattu sinulle " << a[i].nights << " yöksi. " << endl;
            cout << "Vierailusi tulee maksamaan " << a[i].nightCost << " euroa." << endl << endl;
            varmistus = true;
        }
        // kun ollaan käyty kaikki huoneet läpi ja ei ole löytynyt yhtään niin 
        if (i == ROOMS - 1 && varmistus == false) 
            cout << "Varausta ei löytynyt." << endl << endl;
    }
    return 0;
} 

bool nameCheck(string name) {
    char c;
    // jos syöte ei ole mitään, palautuu error
    if (name.length() == 0) {
        return false;
    }

    // katsotaan stringin jokainen kohta jos jokin ei ole number niin returnataan error
    for (int i = 0; i < name.length(); i++) {
        if (name[i] >= '0' && name[i] <= '9')
            return false;
    }
    for (int i = 0; i < name.length(); i++) {

        c = name.at(i);

        // tarkistetaan että on vain aakkosia eikä symboleita, esimerkiksi *#@&
        // kommentoi tämä pois jos haluat ääkköset
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == ' '))) {
            return false;
        }
    }
    // jos kaikki on ok, niin lähetetään true ja ohjelma jatkaa eteenpäin mihin jäi.
    return true; 
}


bool numberValidation(int number) {

    // muutetaan int stringiksi
    string num = to_string(number);     
    char c;

    // katsotaan stringin jokainen kohta jos jokin ei ole number niin returnataan error
    for (int i = 0; i < num.length(); i++) {
        if (!(num[i] >= '0' && num[i] <= '9'))
            return false;
    }
    // katsotaan jos jokin on kohta on kirjain, jos on niin returnataan error
    for (int i = 0; i < num.length(); i++) {
        c = num.at(i);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == ' ')) {
            return false;
        }
    }
    // muutetaan string takaisin intiksi vielä viimeistä varmistusta varten,
    // ettei voi varata huonetta mikä ei ole olemassa.
    int a = stoi(num);

        if (a > ROOMS ||  a < 1) {
            return false;
        }
    // jos kaikki on ok, niin lähetetään true ja ohjelma jatkaa eteenpäin mihin jäi.
    return true;
}

int printInfo(Customer *a, int input) {

    long long int randomReservation = rand() % 99999 + 10000;
    int alennusprosentti_rand = 1; //rand() % (2 - 0 + 1) + 0
    double lopullinen_pros = 0;
    double tulostusale = 0;
    int roomPriceSingle = 100;
    int roomPriceTwo = 150;

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
    cout << "Huone " << a[input].roomNum - 1 << " on nyt varattu nimellä " << a[input].name << "." << endl;
    cout << "Varausnumerosi on " << a[input].reservationNum << "." << endl;
    cout << "Huoneesi ovikoodi on " << a[input].pinCode << "." << endl;
    a[input].nightCost = lopullinen_pros * (a[input].nights * roomPriceSingle);
    cout << a[input].nights << ". yön vierailu maksaa " << a[input].nightCost << " euroa. ";
    // tulostetaan vain alennus jos saa alennusta, "Sait 0% alennusta!" olisi aika surullinen
    if (lopullinen_pros != 1)
    cout << "Sait " << tulostusale << " prosenttia alennusta!";
    cout << endl << endl << endl;
    return 0;
}