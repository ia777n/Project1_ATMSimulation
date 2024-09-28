#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <limits>
#include <conio.h>
#include <thread>
#include <chrono>
using namespace std;

bool isNumeric(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

class ATMAccount {
public:
    int accountNumber;
    string accountName;
    string birthday;
    string contactNumber;
    double balance;
    int pinCode;

    ATMAccount() {
        accountNumber = 0;
        balance = 0.0;
        pinCode = 0;
    }

    ATMAccount(int accNumber, string accName, string bday, string contact, double initialDeposit, int pin) {
        accountNumber = accNumber;
        accountName = accName;
        birthday = bday;
        contactNumber = contact;
        balance = initialDeposit;
        pinCode = pin;
    }
};

class ATMSystem {
private:
    static const int MAX_ACCOUNTS = 100;
    ATMAccount accounts[MAX_ACCOUNTS];
    int accountCount = 0;

public:
    ATMSystem() {

    loadAccountDataFromFile();
    system("pause");

    }

bool isValidAccountNumber(int accNumber){
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
      }
    return accNumber>= 10000 && accNumber <= 99999;
    }
    bool isValidPinCode(int pin) {
        return pin >= 1000 && pin <= 999999;
    }

void enrollNewAccount() {
    if (accountCount < MAX_ACCOUNTS) {
        int accNumber, pin;
        string accName, bday, contact;
        double initialDeposit;

// Input for Account Number
        do{
        cout << "\n\n\n\n\n\n\n\t\t\t\t\tEnter Account Number (5 digits): ";
        cin >> accNumber;

        if  (!isValidAccountNumber(accNumber)){
            cout << "\t\t\t\t\tInvalid account number. Please enter a 5-digit number." <<endl;
            }
        } while (!isValidAccountNumber(accNumber));
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\t\t\t\t\tEnter Account Name: ";
        getline(cin, accName);


//Input for birthday
bool validDate = false;
    while (!validDate) {
        cout << "\t\t\t\t\tEnter Birthday (MM/DD/YYYY): ";
        getline(cin, bday);
        int month, day, year;
        if (sscanf(bday.c_str(), "%d/%d/%d", &month, &day, &year) != 3) {
            cout << "\t\t\t\t\tInvalid date format. Please use MM/DD/YYYY." << endl;
            continue;
        }

    if (month >= 1 && month <= 12 && day >= 1 && day <= 31 && year >= 1900 && year <= 2023) {
        validDate = true;
    } else {
        cout << "\t\t\t\t\tInvalid date. Please enter a valid date within the specified range." << endl;
    }
}

// Input for contact number
    do {
        cout << "\t\t\t\t\tEnter Contact Number: ";
        cin >> contact;

        if (contact.length() != 11 || !isNumeric(contact)) {
            cout << "\t\t\t\t\tInvalid contact number. Please enter an 11-digit numeric number. PH Based only!" << endl;
        }
    } while (contact.length() != 11 || !isNumeric(contact));

//Input for deposit
    do {
        cout << "\t\t\t\t\tEnter Initial Deposit (Min. 5000): ";
        cin >> initialDeposit;

        if (initialDeposit < 5000) {
            cout << "\t\t\t\t\tInitial deposit must be at least 5000. Please enter a valid amount." << endl;
        }
        } while (initialDeposit < 5000);


// Input for PIN Code
    do {
        cout << "\t\t\t\t\tEnter PIN Code (4 to 6 digits): ";
        cin >> pin;

        if (!isValidPinCode(pin)) {
            cout << "\t\t\t\t\tInvalid PIN code. Please enter a 4 to 6 digit numeric PIN." << endl;
        }
    } while (!isValidPinCode(pin));

        ATMAccount newAccount(accNumber, accName, bday, contact, initialDeposit, pin);
        accounts[accountCount] = newAccount;
        accountCount++;
        cout << "\t\t\t\t\tAccount successfully enrolled." << endl;
        saveAccountDataToFile();
        system("pause");
    } else {
        cout << "\t\t\t\t\tMaximum number of accounts reached." << endl;
    }
}

    ATMAccount* findAccount(int accNumber) {
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].accountNumber == accNumber) {
                return &accounts[i];
            }
        }
        return nullptr;
    }

void balanceInquiry(ATMAccount* account) {
    if (account) {
        cout << "\n\n\n\n\n\n\n\t\t\t\t\tAccount Balance: " << account->balance << endl;
        system("pause");
    } else {
        cout << "Account not found." << endl;
        system("pause");
    }
}

void withdraw(ATMAccount* account, double amount) {
    if (account) {
        if (account->balance >= amount) {
            account->balance -= amount;
            cout << "\t\t\t\t\tWithdrawal successful. New balance: " << account->balance << endl;
            saveAccountDataToFile();
            system("pause");
        } else {
            cout << "\t\t\t\t\tInsufficient balance." << endl;
            system("pause");
        }
    } else {
        cout << "Account not found." << endl;
        system("pause");
    }
}

void deposit(ATMAccount* account, double amount) {
    if (account) {
        account->balance += amount;
        cout << "\t\t\t\t\tDeposit successful. New balance: " << account->balance << endl;
        saveAccountDataToFile();
        system("pause");
    } else {
        cout << "Account not found." << endl;
        system("pause");
    }
}

void fundTransfer(ATMAccount* sender) {
    int receiverAccNumber;
    double transferAmount;

    cout << "\n\n\n\n\n\n\n\t\t\t\t\tEnter Receiver Account Number: ";
    cin >> receiverAccNumber;

    ATMAccount* receiver = findAccount(receiverAccNumber);

    if (receiver) {
        cout << "\t\t\t\t\tEnter Transfer Amount: ";
        cin >> transferAmount;

        if (sender->balance >= transferAmount) {
            sender->balance -= transferAmount;
            receiver->balance += transferAmount;
            cout << "\t\t\t\t\tFund transfer successful." << endl;
            cout << "\t\t\t\t\tNew balance for sender: " << sender->balance << endl;
            cout << "\t\t\t\t\tNew balance for receiver: " << receiver->balance << endl;
            saveAccountDataToFile();
            system("pause");
        } else {
            cout << "\t\t\t\t\tInsufficient balance for the sender." << endl;
            system("pause");
        }
    } else {
        cout << "\t\t\t\t\tReceiver account not found." << endl;
        system("pause");
    }
}

void changePinCode(ATMAccount* account) {
    if (account) {
        int currentPin;
        int newPin;
        int newPinConfirmation;

        cout << "\n\n\n\n\n\n\n\t\t\t\t\tEnter Current PIN: ";
        cin >> currentPin;

        if (currentPin == account->pinCode) {
            cout << "\t\t\t\t\tEnter New PIN (4-6 digits): ";
            while (true) {
                cin >> newPin;
                if (cin.fail() || newPin < 1000 || newPin > 999999) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\t\t\t\t\tInvalid PIN. Please enter a 4-6 digit PIN: ";
                } else {
                    cout << "\t\t\t\t\tRe-enter New PIN: ";
                    cin >> newPinConfirmation;
                    if (newPin == newPinConfirmation) {
                        account->pinCode = newPin;
                        cout << "\t\t\t\t\tPIN code changed successfully." << endl;
                        saveAccountDataToFile();
                        system("pause");
                        break;
                    } else {
                        cout << "\t\t\t\t\tPIN confirmation does not match. Please try again." << endl;
                    }
                }
            }
        } else {
            cout << "\t\t\t\t\tCurrent PIN is incorrect. PIN change failed." << endl;
            system("pause");
        }
    } else {
        cout << "Account not found." << endl;
        system("pause");
    }
}

    void saveAccountDataToFile() {
        ofstream file("E:\\pin.code.txt");

        if (file.is_open()) {
            for (int i = 0; i < accountCount; i++) {
            file << accounts[i].accountNumber << " ";
            file << accounts[i].accountName << " ";
            file << accounts[i].birthday << " ";
            file << accounts[i].contactNumber << " ";
            file << accounts[i].balance << " ";
            file << accounts[i].pinCode << endl;
            }
            file.close();
        } else {
            cout << "\n\n\n\n\n\n\n\t\t\t\t\tUnable to open the file for writing." << endl;
        }
    }

void loadAccountDataFromFile() {
    ifstream file("E:\\pin.code.txt");

        if (file.is_open()) {
            accountCount = 0;
            while (file >> accounts[accountCount].accountNumber) {
            file >> accounts[accountCount].accountName;
            file >> accounts[accountCount].birthday;
            file >> accounts[accountCount].contactNumber;
            file >> accounts[accountCount].balance;
            file >> accounts[accountCount].pinCode;
            accountCount++;
            }
            file.close();
            cout << "\n\n\n\n\n\n\n\t\t\t\t\tATM successfully inserted.\n\n\n\n\n\n\n\n" << endl;
        } else {
            cout << "\n\n\n\n\n\n\n\t\t\t\t\tUnable to open the file for reading." << endl;
        }
    }

    ATMAccount* findAccount(int accNumber, int pin) {
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].accountNumber == accNumber && accounts[i].pinCode == pin) {
                return &accounts[i];
            }
        }
        return nullptr;
    }
};

int main() {

bool flashDriveInserted = false;

    ATMSystem atmSystem;
    ATMAccount* loggedInAccount = nullptr;

    while (true) {
        ifstream flashDriveFile("E:\\pin.code.txt");
        if (flashDriveFile.is_open()) {
            flashDriveInserted = true;
            flashDriveFile.close();
            break;
        } else {
        flashDriveFile.close();
        system("cls");
        cout << "\n\n\n\n\n\n\n\t\t\t\t\tPlease insert the ATM flash drive" << endl;
        system("pause");
        }
    }

    if (flashDriveInserted) {
        int choice;

        while (true) {
        system("cls");
        cout << "\n\n\n\n\n\n\n\t\t\t\t============================================" << endl;
        cout << "\n\t\t\t\t          Welcome to the ATM System         " << endl;
        cout << "\n\t\t\t\t============================================\n\n\n\n\n\n" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        system("cls");
        cout << "\n\n\n\n\n\n\n\t\t\t\t\t1. Create Account" << endl;
        cout << "\n\t\t\t\t\t2. Log In" << endl;
        cout << "\n\t\t\t\t\t3. Exit" << endl;
        cout << "\n\t\t\t\t\tEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            system("cls");
            atmSystem.enrollNewAccount();
        } else if (choice == 2) {
            system("cls");
            int enteredAccountNumber;
            cout << "\n\n\n\n\n\n\n\t\t\t\t\tEnter your account number: ";
            cin >> enteredAccountNumber;
            system("cls");
                string enteredPin = "";
                char ch;
                cout << "\n\n\n\n\n\n\n\t\t\t\t\tEnter your PIN: ";
                while (true) {
                    ch = _getch();
                    if (ch == 13) {
                        break;
                    } else if (ch == 8 && !enteredPin.empty()) {
                        enteredPin.pop_back();
                        cout << "\b \b";
                    } else if (isdigit(ch)) {
                        enteredPin.push_back(ch);
                        cout << '*';
                    }
                }

                int pin = stoi(enteredPin);

            ATMAccount* account = atmSystem.findAccount(enteredAccountNumber, pin);

            if (account) {
                loggedInAccount = account;
                cout << "\n\t\t\t\t\tLogin successful. Welcome, " << loggedInAccount->accountName << "!" << endl;
                this_thread::sleep_for(chrono::seconds(1));
                int mainMenuChoice;

                do {
                    system("cls");
                    cout << "\n\n\n\n\n\n\n\t\t\t\t\tATM System Menu:" << endl;
                    cout << "\t\t\t\t\t1. Balance Inquiry" << endl;
                    cout << "\t\t\t\t\t2. Withdraw" << endl;
                    cout << "\t\t\t\t\t3. Deposit" << endl;
                    cout << "\t\t\t\t\t4. Fund Transfer" << endl;
                    cout << "\t\t\t\t\t5. Change PIN Code" << endl;
                    cout << "\t\t\t\t\t6. Exit" << endl;
                    cout << "\t\t\t\t\tEnter your choice: ";
                    cin >> mainMenuChoice;

                    switch (mainMenuChoice) {
                        case 1:
                            system("cls");
                            atmSystem.balanceInquiry(loggedInAccount);
                            break;
                        case 2:
                            system("cls");
                            double amount;
                            cout << "\n\n\n\n\n\n\n\t\t\t\t\tEnter Withdrawal Amount: ";
                            cin >> amount;
                            atmSystem.withdraw(loggedInAccount, amount);
                            break;
                        case 3:
                            system("cls");
                            double depositAmount;
                            cout << "\n\n\n\n\n\n\n\t\t\t\t\tEnter Deposit Amount: ";
                            cin >> depositAmount;
                            atmSystem.deposit(loggedInAccount, depositAmount);
                            break;
                        case 4:
                            system("cls");
                            atmSystem.fundTransfer(loggedInAccount);
                            break;
                        case 5:
                            system("cls");
                            atmSystem.changePinCode(loggedInAccount);
                            break;
                        case 6:
                            cout << "\t\t\t\t\tExiting ATM System." << endl;
                            return 0;
                            break;
                        default:
                            cout << "\t\t\t\t\tInvalid choice. Please try again." << endl;
                            system("pause");
                    }
                } while (mainMenuChoice != 6);
            } else {
                cout << "\n\t\t\t\t\tInvalid account number or PIN. Login failed." << endl;
                system("pause");
            }
        } else if (choice == 3) {
            cout << "\n\t\t\t\t\tExiting ATM System." << endl;
            return 0;
        } else {
            cout << "\n\t\t\t\t\tInvalid choice. Please try again." << endl;
            system("pause");
        }
    }
}
    return 0;
}










