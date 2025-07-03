#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

#define SIZE 20

class Contact {
public:
    string name;
    string number;
    int id;
    Contact* next;
    Contact* prev;

    Contact(string _name, string _number, int _id) {
        name = _name;
        number = _number;
        id = _id;
        next = nullptr;
        prev = nullptr;
    }
};

class ContactManager {
private:
    Contact* head;
    vector<int> callHistory;
    int dummyId = 1001;

public:
    ContactManager() {
        head = nullptr;
    }

    bool isCallHistoryEmpty() {
        return callHistory.empty();
    }

    bool isCallHistoryFull() {
        return callHistory.size() == SIZE;
    }

    void pushToCallHistory(int id) {
        if (!isCallHistoryFull())
            callHistory.push_back(id);
        else
            cout << "Call history is full.\n";
    }

    void popFromCallHistory() {
        if (!isCallHistoryEmpty()) {
            callHistory.pop_back();
            cout << "LATEST CALL FROM HISTORY DELETED\n";
        } else {
            cout << "Call history is already empty.\n";
        }
    }

    void createContact() {
        string name, number;
        cout << "ENTER THE NAME OF THE CONTACT: ";
        cin >> name;
        cout << "ENTER THE CORRESPONDING CONTACT NUMBER: ";
        cin >> number;

        Contact* newContact = new Contact(name, number, dummyId++);
        if (!head) {
            head = newContact;
        } else {
            Contact* temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newContact;
            newContact->prev = temp;
        }

        ofstream outFile("contact_backend.txt", ios::app);
        outFile << name << "\n" << number << "\n\n";
        outFile.close();
    }

    void printAllContacts() {
        Contact* temp = head;
        while (temp) {
            cout << "\n" << temp->name << "\n" << temp->number << "\n";
            temp = temp->next;
        }
    }

    void displayCallHistory() {
        for (int i = callHistory.size() - 1; i >= 0; --i) {
            int id = callHistory[i];
            Contact* temp = head;
            while (temp) {
                if (temp->id == id) {
                    cout << "called: " << temp->name << endl;
                    break;
                }
                temp = temp->next;
            }
        }
    }

    void callContact() {
        string name;
        cout << "Enter the contact name to call: ";
        cin >> name;
        Contact* temp = head;
        while (temp) {
            if (temp->name == name) {
                pushToCallHistory(temp->id);
                cout << "Calling " << temp->name << "...\n";
                return;
            }
            temp = temp->next;
        }
        cout << "NO MATCHING CONTACTS FOUND\n";
    }

    void searchContact() {
        string key;
        cout << "ENTER THE KEY TO SEARCH FOR CONTACTS: ";
        cin >> key;
        Contact* temp = head;
        bool found = false;
        cout << "CONTACTS MATCHING WITH KEY ARE:\n";
        while (temp) {
            if (temp->name.find(key) == 0) {
                cout << temp->name << "\n" << temp->number << "\n";
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "NO CONTACTS EXISTS WITH THE ENTERED SEARCH KEY!!\n";
        }
    }

    void backupFile() {
        ifstream inFile("contact_backend.txt");
        string line;
        while (getline(inFile, line)) {
            cout << line << "\n";
        }
        inFile.close();
        cout << "END OF LIST OF CONTACTS\n";
    }

    void printContactIDs() {
        Contact* temp = head;
        while (temp) {
            cout << temp->id << "\n";
            temp = temp->next;
        }
    }

    void deleteContact() {
        string name;
        cout << "Enter the contact you want to delete: ";
        cin >> name;
        Contact* temp = head;
        while (temp) {
            if (temp->name == name) {
                if (temp == head) {
                    head = temp->next;
                    if (head) head->prev = nullptr;
                } else {
                    if (temp->prev) temp->prev->next = temp->next;
                    if (temp->next) temp->next->prev = temp->prev;
                }
                delete temp;
                cout << "Contact deleted successfully.\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Contact not found.\n";
    }
};

int main() {
    ContactManager cm;
    int opt;

    cout << "PLEASE SELECT FROM THE BELOW MENU:\n";
    cout << "1. CREATE NEW CONTACT\n";
    cout << "2. SEARCH FOR A CONTACT\n";
    cout << "3. VIEW ALL CONTACTS\n";
    cout << "4. VIEW CALL HISTORY\n";
    cout << "5. DELETE A CONTACT\n";
    cout << "6. CALL A CONTACT\n";
    cout << "7. DELETE LATEST CALL FROM HISTORY\n";
    cout << "8. BACKUP CONTACT FILE\n";
    cout << "9. PRINT CONTACT IDs\n";
    cout << "10. EXIT\n";

    while (true) {
        cout << "\nOPTION: ";
        cin >> opt;
        switch (opt) {
            case 1: cm.createContact(); break;
            case 2: cm.searchContact(); break;
            case 3: cm.printAllContacts(); break;
            case 4: cm.displayCallHistory(); break;
            case 5: cm.deleteContact(); break;
            case 6: cm.callContact(); break;
            case 7: cm.popFromCallHistory(); break;
            case 8: cm.backupFile(); break;
            case 9: cm.printContactIDs(); break;
            case 10: cout << "YOU HAVE EXITED THE PROGRAM!\n"; return 0;
            default: cout << "WRONG OPTION!\n"; break;
        }
    }
}
