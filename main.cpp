#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <stdio.h>

using namespace std;

struct Addressee
{
    int id = 0;
    int idUser = 0;
    string name = "", lastName = "", telNumber = "", email = "", address = "";
};

struct User
{
    int id = 0;
    string login = "";
    string password = "";
};

void returnToMenu()
{
    cin.sync();
    cout << endl << "Wcisnij dowolny klawisz, aby wrocic do menu...";
    getch();
}

char getCharacter()
{
    string enteredInput = "";
    char characterFromEnteredInput  = {0};

    while (true){
        getline(cin, enteredInput);

        if (enteredInput.length() == 1){
            characterFromEnteredInput = enteredInput[0];
            break;
        }
        cout << "To nie jest pojedynczy znak. Wpisz ponownie." << endl;
    }

    return characterFromEnteredInput;
}

string convertIntToString(int value)
{
    ostringstream ss;
    ss << value;
    string str = ss.str();

    return str;
}

void saveUsersToFile(string filename, vector<User> vecUser)
{
    fstream usersFile;
    usersFile.open(filename, ios::out | ios::trunc);

    for(vector<User>::iterator iter = vecUser.begin(); iter != vecUser.end(); ++iter)
        usersFile << convertIntToString(iter->id) << "|" << iter->login << "|" << iter->password << endl;

    usersFile.close();
}

int logging(vector<User> vecUser)
{
    string login, password;
    int idUser = 0;
    bool loginCorrect = false;

    cout << "Podaj twoj login: ";
    cin >> login;

    for(vector<User>::iterator iter = vecUser.begin(); iter != vecUser.end(); ++iter){
        if(iter->login == login){
            loginCorrect = true;
            for(int triesToLog = 0; triesToLog < 3; triesToLog++){
                cout << "Podaj haslo (pozostalo prob: " << 3 - triesToLog << "): ";
                cin >> password;
                if(iter->password == password){
                    cout << "Zalogowales sie!" << endl;
                    Sleep(1300);
                    idUser = iter->id;

                    return idUser;
                }
            }
            cout << "Podales 3 razy bledne haslo! Czas oczekiwania: 3 sekundy" << endl;
            Sleep(3000);

            return 0;
        }
    }
    if(!loginCorrect)
        cout << "Nie znaleziono uzytkownika" << endl;
    Sleep(1000);

    return 0;
}

int registration(vector<User>& vecUser, int numberOfUsers)
{
    string login, password;
    User user;

    system("cls");
    cout << "Podaj nazwe uzytkownika (login): ";
    cin >> login;

    for(vector<User>::iterator iter = vecUser.begin(); iter != vecUser.end(); ++iter){
        while(iter->login == login){
            cout << "Podana nazwa uzytkownika juz istnieje, podaj inna: ";
            cin >> login;
        }
    }
    cout << "Podaj haslo: ";
    cin >> password;

    user.login = login;
    user.password = password;
    user.id = numberOfUsers+1;
    vecUser.push_back(user);
    numberOfUsers += 1;
    saveUsersToFile("Uzytkownicy.txt", vecUser);
    cout << "Pomyslnie zarejestrowano uzytkownika" << endl;
    returnToMenu();

    return numberOfUsers;
}

string stringToLower(string wordToLower)
{
    transform(wordToLower.begin(), wordToLower.end(), wordToLower.begin(), ::tolower);

    return wordToLower;
}

void displayMenu(char windowMode)
{
    system("cls");
    if(windowMode == '1'){
        cout << "1. Logowanie" << endl;
        cout << "2. Rejestracja." << endl;
        cout << "3. Zamknij program." << endl;
    }

    else if(windowMode == '2'){
        cout << "KSIAZKA ADRESOWA" << endl;
        cout << "1. Dodaj adresata." << endl;
        cout << "2. Wyszukaj po imieniu." << endl;
        cout << "3. Wyszukaj po nazwisku." << endl;
        cout << "4. Wyswietl wszystkich adresatow." << endl;
        cout << "5. Usun adresata." << endl;
        cout << "6. Edytuj adresata." << endl;
        cout << "7. Zmien haslo." << endl;
        cout << "8. Wyloguj sie." << endl;
        cout << "Twoj wybor: ";
    }
}

void printVectorIteratorContent(vector<Addressee> vecAddressee, vector<Addressee>::iterator iter)
{
    cout << iter->id << ". " << iter->idUser << ". " << iter->name << " " << iter->lastName << " "
         << iter->telNumber << " " << iter->email << " " << iter->address << endl;
}

void displayActualAddresseeList(vector<Addressee> vecAddressee)
{
    system("cls");
    if(!vecAddressee.empty()){
        cout << "Twoja ktualna zawartosc ksiazki adresowej" << endl;
        for(vector<Addressee>::iterator iter = vecAddressee.begin(); iter != vecAddressee.end(); ++iter)
            printVectorIteratorContent(vecAddressee, iter);
    }
    else
        cout << "Lista adresowa jest pusta." << endl;
    returnToMenu();
}

Addressee getDataOfSingleAddressee(string line)
{
    Addressee addressee;
    string singleAddreesseData = "";
    int numberOfSingleAddresseData = 1;
    for (int i = 0; i < line.length(); i++){
        if (line[i] != '|')
            singleAddreesseData += line[i];

        else{
            switch(numberOfSingleAddresseData){
            case 1:
                addressee.id = atoi(singleAddreesseData.c_str());
                break;
            case 2:
                addressee.idUser = atoi(singleAddreesseData.c_str());
                break;
            case 3:
                addressee.name = singleAddreesseData;
                break;
            case 4:
                addressee.lastName = singleAddreesseData;
                break;
            case 5:
                addressee.telNumber = singleAddreesseData;
                break;
            case 6:
                addressee.email = singleAddreesseData;
                break;
            case 7:
                addressee.address = singleAddreesseData;
                break;
            }
            singleAddreesseData = "";
            numberOfSingleAddresseData++;
        }
    }

    return addressee;
}

void saveAddresseeToFile(string fileName, Addressee addressee)
{
    fstream addresseesFile;
    addresseesFile.open(fileName, ios::out | ios::app);

    if(addresseesFile.good() == true){
        addresseesFile << convertIntToString(addressee.id) << '|';
        addresseesFile << convertIntToString(addressee.idUser) << '|';
        addresseesFile << addressee.name << '|';
        addresseesFile << addressee.lastName << '|';
        addresseesFile << addressee.telNumber << '|';
        addresseesFile << addressee.email << '|';
        addresseesFile << addressee.address << '|' << endl;
        addresseesFile.close();
    }
    else{
        cout << "Nie udalo sie otworzyc pliku i zapisac w nim danych." << endl;
        returnToMenu();
    }
}

void saveAddresseeChangesToFile(int idAddresseeToCompare, Addressee editedAddressee)
{
    Addressee addressee;
    string line;
    fstream addresseesFile;
    addresseesFile.open("Adresaci.txt", ios::in);

    if(addresseesFile.good() == true){
        while(getline(addresseesFile, line)){
            addressee = getDataOfSingleAddressee(line);

            if(addressee.id == idAddresseeToCompare){
                if(editedAddressee.id == 0)
                    continue;
                else
                    saveAddresseeToFile("Adresaci_tymczasowi.txt", editedAddressee);
            }
            else
                saveAddresseeToFile("Adresaci_tymczasowi.txt", addressee);
        }
        addresseesFile.close();
        remove("Adresaci.txt");
        rename("Adresaci_tymczasowi.txt", "Adresaci.txt");
        returnToMenu();
    }
    else{
        cout << "Nie udalo sie otworzyc pliku" << endl;
        returnToMenu();
    }
}

int readAddreessesFromFile(string fileName, vector<Addressee>& vecAddressee, int idUser, int lastId)
{
    string line = "";
    fstream addresseesFile;
    addresseesFile.open(fileName.c_str(), ios::in);

    if(!addresseesFile){
        cout << "Nie moglem otworzyc pliku o nazwie: " << fileName << endl;
        returnToMenu();

        return lastId;
    }

    while(getline(addresseesFile, line)){
        Addressee addressee;
        addressee = getDataOfSingleAddressee(line);
        lastId = addressee.id;
        if(addressee.idUser == idUser)
            vecAddressee.push_back(addressee);
    }
    addresseesFile.close();

    if(!vecAddressee.empty()){   //when file exists and if contains any data
        cout << "Ostatnie ID: " << lastId << endl;
        displayActualAddresseeList(vecAddressee);
    }

    return lastId;
}

int readUsersFromFile(string filename, vector<User>& vecUser)
{
    int numberOfUsers = 0;
    string line;
    ifstream usersFile(filename);

    if(!usersFile){
        cout << "Nie moglem otworzyc pliku o nazwie: " << filename << endl;
        Sleep(1500);
        return numberOfUsers;
    }
    while (getline(usersFile, line)){
        User users;
        istringstream iss(line);
        istringstream(line) >> users.id;
        getline(iss, line, '|');
        getline(iss, users.login, '|');
        getline(iss, users.password, '|');
        vecUser.push_back(users);
        numberOfUsers += 1;
    }
    usersFile.close();

    return numberOfUsers;
}

int addAddressee(vector<Addressee>& vecAddressee, int idUser, int lastId)
{
    Addressee addressee;
    string name, lastName, telNumber, email, address;
    system("cls");
    cout << "id, ktoremu uzytkownikowi zostanie przypisane, to: " << lastId+1 << endl;
    cout << "Podaj imie oraz nazwisko: ";
    cin >> name >> lastName;
    cout << endl << "Podaj numer telefonu: ";
    cin.sync();
    getline(cin, telNumber);
    cout << endl << "Podaj email: ";
    cin >> email;
    cout << endl << "Podaj adres: ";
    cin.sync();
    getline(cin, address);

    lastId += 1;
    addressee.id = lastId;
    addressee.idUser = idUser;
    addressee.name = name;
    addressee.lastName = lastName;
    addressee.telNumber = telNumber;
    addressee.email = email;
    addressee.address = address;
    vecAddressee.push_back(addressee);
    saveAddresseeToFile("Adresaci.txt", addressee);
    cout << "Przyjaciel dodany" << endl;
    returnToMenu();

    return lastId;
}

void searchByName(vector<Addressee> vecAddressee)
{
    string name;
    bool nameFound;

    system("cls");
    cout << "Wprowadz imie adresata: ";
    cin >> name;

    for(vector<Addressee>::iterator iter = vecAddressee.begin(); iter != vecAddressee.end(); ++iter){
        if(stringToLower(iter->name) == stringToLower(name)){
            nameFound = true;
            printVectorIteratorContent(vecAddressee, iter);
        }
    }
    if(!nameFound)
        cout << "Nie znaleziono adresata" << endl;

    returnToMenu();
}

void searchByLastName(vector<Addressee> vecAddressee)
{
    string lastName;
    bool lastNameFound = false;

    system("cls");
    cout << "Wprowadz nazwisko adresata: ";
    cin >> lastName;

    for(vector<Addressee>::iterator iter = vecAddressee.begin(); iter != vecAddressee.end(); ++iter){
        if(stringToLower(iter->lastName) == stringToLower(lastName)){
            lastNameFound = true;
            printVectorIteratorContent(vecAddressee, iter);
        }
    }
    if(!lastNameFound)
        cout << "Nie znaleziono adresata" << endl;

    returnToMenu();
}

void deleteAddressee(vector<Addressee>& vecAddressee)
{
    Addressee addressee;
    int addreseeToDelete;
    char deleteChoice;
    bool addresseeFound = false;

    system("cls");
    cout << "Podaj ID adresata, ktorego chcesz usunac: ";
    cin >> addreseeToDelete;

    for(vector<Addressee>::iterator iter = vecAddressee.begin(); iter != vecAddressee.end(); ++iter){
        if(iter->id == addreseeToDelete){
            addresseeFound = true;
            cout << "Znaleziono adresata o ID: " << iter->id << ". " << iter->name << " " << iter->lastName << endl;
            cout << "Czy na pewno chcesz go usunac? Wcisnij t, aby potwiedzic: ";
            cin >> deleteChoice;
            if(deleteChoice == 't'){
                iter = vecAddressee.erase(iter);
                cout << "Usunieto adresata o podanym ID" << endl;
                Sleep(1000);
            }
            else{
                cout << "Adresat nie zostanie usuniety" << endl;
                returnToMenu();
            }
            saveAddresseeChangesToFile(addreseeToDelete, addressee);
            break;
        }
    }
    if(!addresseeFound){
        cout << "Nie znaleziono adresata o podanym id." << endl;
        returnToMenu();
    }
}

void editAddressee(vector<Addressee>& vecAddressee)
{
    int addresseeToEdit, editChoice;
    bool addresseeFound = false;
    string name, lastName, telNumber, email, address;

    system("cls");
    cout << "Podaj ID adresata, ktorego chcesz edytowac: ";
    cin >> addresseeToEdit;

    for(vector<Addressee>::iterator iter = vecAddressee.begin(); iter != vecAddressee.end(); ++iter){
        if(iter->id == addresseeToEdit){
            addresseeFound = true;
            system("cls");
            cout << "Wybierz co chcesz zmienic" << endl;
            cout << "1 - Imie" << endl;
            cout << "2 - Nazwisko" << endl;
            cout << "3 - Numer telefonu" << endl;
            cout << "4 - Email" << endl;
            cout << "5 - Adres" << endl;
            cout << "6 - Powrot do menu" << endl;
            cout << "Twoj wybor: ";
            cin >> editChoice;
            if(editChoice == 1){
                cout << "Wprowadz nowe imie: ";
                cin >> name;
                iter->name = name;
                cout << "Imie zostalo zmienione!" << endl;
            }
            else if(editChoice == 2){
                cout << "Wprowadz nowe nazwisko: ";
                cin >> lastName;
                iter->lastName = lastName;
                cout << "Nazwisko zostalo zmienione!" << endl;
            }
            else if(editChoice == 3){
                cout << "Wprowadz nowy numer telefonu: ";
                cin.sync();
                getline(cin, telNumber);
                iter->telNumber = telNumber;
                cout << "Nr telefonu zostal zmieniony!" << endl;
            }
            else if(editChoice == 4){
                cout << "Wprowadz nowy email: ";
                cin >> email;
                iter->email = email;
                cout << "Email zostal zmieniony!" << endl;
            }
            else if(editChoice == 5){
                cout << "Wprowadz nowy adres: ";
                cin.sync();
                getline(cin, address);
                iter->address = address;
                cout << "Adres zostal zmieniony!" << endl;
            }
            else if(editChoice = 6)
                break;
            saveAddresseeChangesToFile(addresseeToEdit, *iter);
            break;
        }
    }
    if(!addresseeFound){
        cout << "Nie znaleziono osoby o podanym id!";
        returnToMenu();
    }
}

void changePassword(vector<User>& vecUser, int idUser)
{
    string password;

    system("cls");
    cout << "Podaj nowe haslo: ";
    cin >> password;

    for(vector<User>::iterator iter = vecUser.begin(); iter != vecUser.end(); ++iter){
        if(iter->id == idUser){
            iter->password = password;
            cout << "Haslo zostalo zmienione!";
            returnToMenu();
            saveUsersToFile("Uzytkownicy.txt", vecUser);
            break;
        }
    }
}

int main()
{
    vector<Addressee> vecAddressee;
    vector<User> vecUser;
    int numberOfUsers = 0, idUser = 0, lastId = 0;
    char windowMode = '1', menuChoice = '0';

    numberOfUsers = readUsersFromFile("Uzytkownicy.txt", vecUser);

    while(true){
        if(windowMode == '1'){
            displayMenu(windowMode);
            menuChoice = getCharacter();

            if(menuChoice == '1'){
                if(numberOfUsers == 0){
                    cout << "Brak zarejestrowanych uzytkownikow!" << endl;
                    Sleep(1000);
                }
                else{
                    idUser = logging(vecUser);
                    if(idUser > 0){
                        lastId = readAddreessesFromFile("Adresaci.txt", vecAddressee, idUser, lastId);
                        windowMode = '2';
                    }
                }
            }
            else if(menuChoice == '2')
                numberOfUsers = registration(vecUser, numberOfUsers);

            else if(menuChoice == '3')
                exit(0);
        }
        else if(windowMode == '2'){
            displayMenu(windowMode);
            menuChoice = getCharacter();

            if(menuChoice == '1')
                lastId = addAddressee(vecAddressee, idUser, lastId);

            else if(menuChoice == '2')
                searchByName(vecAddressee);

            else if(menuChoice == '3')
                searchByLastName(vecAddressee);

            else if(menuChoice == '4')
                displayActualAddresseeList(vecAddressee);

            else if(menuChoice == '5')
                deleteAddressee(vecAddressee);

            else if(menuChoice == '6')
                editAddressee(vecAddressee);

            else if(menuChoice == '7')
                changePassword(vecUser, idUser);

            else if(menuChoice == '8'){
                cout << endl << "Wylogowano sie!" << endl;
                returnToMenu();
                windowMode = '1';
                vecAddressee.clear();
            }
        }
    }

    return 0;
}


/*
void sortVectorOfAddressees(vector<Addressee>& vecAddressee)
{
    sort(vecAddressee.begin(), vecAddressee.end(), [](const Addressee &left, const Addressee &right)
    {
        return left.id < right.id;
    });
}
*/
