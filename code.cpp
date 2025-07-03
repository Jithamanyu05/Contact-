
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

#define SIZE 20

class Contact {
private:
    string name;
    string number;
    int id;

public:
    Contact* next;
    Contact* prev;

    Contact(string _name, string _number, int _id)
        : name(_name), number(_number), id(_id), next(nullptr), prev(nullptr) {}

    string getName() const { return name; }
    string getNumber() const { return number; }
    int getId() const { return id; }

    void setName(const string& newName) { name = newName; }
    void setNumber(const string& newNumber) { number = newNumber; }

    void display() const {
        cout << "Name: " << name << "\nNumber: " << number << "\nID: " << id << "\n";
    }
};

class CallHistory {
private:
    vector<int> stack;

public:
    void push(int id) {
        if (stack.size() < SIZE) {
            stack.push_back(id);
        } else {
            cout << "Call history is full. Oldest entry will be removed.\n";
            stack.erase(stack.begin());
            stack.push_back(id);
        }
    }

    void pop() {
        if (!stack.empty()) {
            stack.pop_back();
            cout << "LATEST CALL FROM HISTORY DELETED\n";
        } else {
            cout << "Call history is already empty.\n";
        }
    }

    vector<int> getHistory() const {
        return stack;
    }
};

class ContactManager {
private:
    Contact* head;
    int dummyId;
    CallHistory callHistory;

public:
    ContactManager() : head(nullptr), dummyId(1001) {}

    ~ContactManager() {
        while (head) {
            Contact* temp = head;
            head = head->next;
            delete temp;
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
        cout << "Contact saved successfully!\n";
    }

    void displayAllContacts() const {
        if (!head) {
            cout << "No contacts available.\n";
            return;
        }
        Contact* temp = head;
        while (temp) {
            temp->display();
            cout << "------------------\n";
            temp = temp->next;
        }
    }

    void searchContact() const {
        string key;
        cout << "ENTER THE KEY TO SEARCH FOR CONTACTS: ";
        cin >> key;
        Contact* temp = head;
        bool found = false;
        while (temp) {
            if (temp->getName().find(key) == 0) {
                temp->display();
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "NO CONTACTS EXISTS WITH THE ENTERED SEARCH KEY!!\n";
        }
    }

    void deleteContact() {
        string name;
        cout << "Enter the contact you want to delete: ";
        cin >> name;
        Contact* temp = head;
        while (temp) {
            if (temp->getName() == name) {
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

    void callContact() {
        string name;
        cout << "Enter the contact name to call: ";
        cin >> name;
        Contact* temp = head;
        while (temp) {
            if (temp->getName() == name) {
                callHistory.push(temp->getId());
                cout << "Calling " << name << "...\n";
                return;
            }
            temp = temp->next;
        }
        cout << "NO MATCHING CONTACTS FOUND\n";
    }

    void displayCallHistory() const {
        vector<int> history = callHistory.getHistory();
        if (history.empty()) {
            cout << "No call history found.\n";
            return;
        }

        for (int i = history.size() - 1; i >= 0; --i) {
            Contact* temp = head;
            while (temp) {
                if (temp->getId() == history[i]) {
                    cout << "Called: " << temp->getName() << "\n";
                    break;
                }
                temp = temp->next;
            }
        }
    }

    void deleteLatestCall() {
        callHistory.pop();
    }

    void backupContacts() const {
        ifstream inFile("contact_backend.txt");
        string line;
        while (getline(inFile, line)) {
            cout << line << "\n";
        }
        inFile.close();
        cout << "END OF LIST OF CONTACTS\n";
    }

    void showMenu() {
        cout << "\n=========== DigiDex Pro – Smart Contact Manager ===========\n";
        cout << "1. Create New Contact\n";
        cout << "2. Search for a Contact\n";
        cout << "3. View All Contacts\n";
        cout << "4. View Call History\n";
        cout << "5. Delete a Contact\n";
        cout << "6. Call a Contact\n";
        cout << "7. Delete Latest Call from History\n";
        cout << "8. Backup Contact File\n";
        cout << "9. Exit\n";
    }

    void run() {
        int choice;
        while (true) {
            showMenu();
            cout << "Choose an option: ";
            cin >> choice;
            switch (choice) {
                case 1: createContact(); break;
                case 2: searchContact(); break;
                case 3: displayAllContacts(); break;
                case 4: displayCallHistory(); break;
                case 5: deleteContact(); break;
                case 6: callContact(); break;
                case 7: deleteLatestCall(); break;
                case 8: backupContacts(); break;
                case 9: cout << "Exiting DigiDex Pro. Thank you!\n"; return;
                default: cout << "Invalid option! Try again.\n"; break;
            }
        }
    }
};

int main() {
    ContactManager manager;
    manager.run();
    return 0;
}

