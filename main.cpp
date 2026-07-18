#include "cb.h"
using namespace std;

Contact *contacts[50];
int contactCount = 0;
int nextId = 1;

Group groups[10];
int groupCount = 0;

SearchLog logs[20];
int logCount = 0;

int findIndexById(int id) {
    for (int i = 0; i < contactCount; i++) {
        if (contacts[i]->getId() == id) {
            return i;
        }
    }
    return -1;
}

void addContact() {
    cin.ignore();
    cout << "Type (1-Personal, 2-Business): ";
    int type;
    cin >> type;
    cin.ignore();

    string name, email, phone, city, extra;
    cout << "Name: "; getline(cin, name);
    cout << "Email: "; getline(cin, email);
    cout << "Phone: "; getline(cin, phone);
    cout << "City: "; getline(cin, city);

    Contact *newC;
    if (type == 1) {
        cout << "Relation: "; getline(cin, extra);
        newC = new PersonalContact(nextId, name, email, phone, city, extra);
    } else {
        cout << "Company: "; getline(cin, extra);
        newC = new BusinessContact(nextId, name, email, phone, city, extra);
    }

    for (int i = 0; i < contactCount; i++) {
        if (*contacts[i] == *newC) {
            cout << "Duplicate found with " << contacts[i]->getName() << ". Merging phone number." << endl;
            contacts[i]->mergePhone(newC->getPhone());
            delete newC;
            FileManager::saveContacts(contacts, contactCount);
            return;
        }
    }

    contacts[contactCount] = newC;
    contactCount++;
    nextId++;
    cout << "Contact added successfully." << endl;
    FileManager::saveContacts(contacts, contactCount);
}

void viewAll() {
    if (contactCount == 0) {
        cout << "No contacts found." << endl;
        return;
    }
    for (int i = 0; i < contactCount; i++) {
        contacts[i]->show();
    }
}

void updateContact() {
    cout << "Enter ID to update: ";
    int id;
    cin >> id;
    cin.ignore();

    int idx = findIndexById(id);
    if (idx == -1) {
        cout << "Contact not found." << endl;
        return;
    }

    string name, email, phone;
    cout << "New Name (leave blank to keep same): "; getline(cin, name);
    cout << "New Email (leave blank to keep same): "; getline(cin, email);
    cout << "New Phone (leave blank to keep same): "; getline(cin, phone);

    if (!name.empty()) contacts[idx]->setName(name);
    if (!email.empty()) contacts[idx]->setEmail(email);
    if (!phone.empty()) contacts[idx]->setPhone(phone);

    cout << "Contact updated successfully." << endl;
    FileManager::saveContacts(contacts, contactCount);
}

void deleteContact() {
    cout << "Enter ID to delete: ";
    int id;
    cin >> id;

    int idx = findIndexById(id);
    if (idx == -1) {
        cout << "Contact not found." << endl;
        return;
    }

    delete contacts[idx];
    for (int i = idx; i < contactCount - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contactCount--;
    cout << "Contact deleted successfully." << endl;
    FileManager::saveContacts(contacts, contactCount);
}

void logSearch(string key) {
    for (int i = 0; i < logCount; i++) {
        if (logs[i].getKeyword() == key) {
            logs[i].increment();
            return;
        }
    }
    if (logCount < 20) {
        logs[logCount] = SearchLog(key);
        logCount++;
    }
}

void searchContact() {
    cin.ignore();
    cout << "Enter keyword: ";
    string key;
    getline(cin, key);

    bool found = false;
    for (int i = 0; i < contactCount; i++) {
        if (matchKeyword(contacts[i]->getName(), key) || matchKeyword(contacts[i]->getEmail(), key) || matchKeyword(contacts[i]->getPhone(), key)) {
            contacts[i]->show();
            found = true;
        }
    }
    if (!found) {
        cout << "No matching contact found." << endl;
    }
    logSearch(key);
}

void manageGroups() {
    cout << "1. Create Group  2. Add Contact to Group  3. View Groups\n";
    int ch;
    cin >> ch;
    if (ch == 1) {
        cin.ignore();
        string gname;
        cout << "Group Name: ";
        getline(cin, gname);
        groups[groupCount] = Group(groupCount + 1, gname);
        groupCount++;
        FileManager::saveGroups(groups, groupCount);
        cout << "Group created." << endl;
    } else if (ch == 2) {
        int gid, cid;
        cout << "Group ID: "; cin >> gid;
        cout << "Contact ID: "; cin >> cid;
        for (int i = 0; i < groupCount; i++) {
            if (groups[i].getGroupId() == gid) {
                groups[i].addContact(cid);
                FileManager::saveGroups(groups, groupCount);
                cout << "Added." << endl;
            }
        }
    } else if (ch == 3) {
        for (int i = 0; i < groupCount; i++) {
            groups[i].display();
        }
    }
}

void viewSearchStats() {
    if (logCount == 0) {
        cout << "No searches yet." << endl;
        return;
    }
    int mostFreqIdx = 0;
    for (int i = 0; i < logCount; i++) {
        cout << "Keyword: " << logs[i].getKeyword() << " | Searched " << logs[i].getFrequency();
        cout << " time(s) | Last searched: " << logs[i].getLastSearched() << endl;
        if (logs[i].getFrequency() > logs[mostFreqIdx].getFrequency()) {
            mostFreqIdx = i;
        }
    }
    cout << "\nMost frequently searched keyword: " << logs[mostFreqIdx].getKeyword() << endl;
}

int main() {
    contactCount = FileManager::loadContacts(contacts, nextId);
    groupCount = FileManager::loadGroups(groups);
    cout << "Loaded " << contactCount << " contact(s) and " << groupCount << " group(s) from file.\n";

    int choice = 0;
    while (choice != 7) {
        cout << "\n===== Contact Book Menu =====\n";
        cout << "1. Add Contact\n";
        cout << "2. View All Contacts\n";
        cout << "3. Update Contact\n";
        cout << "4. Delete Contact\n";
        cout << "5. Search Contact\n";
        cout << "6. Manage Groups\n";
        cout << "7. Exit\n";
        cout << "8. View Search Stats\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) addContact();
        else if (choice == 2) viewAll();
        else if (choice == 3) updateContact();
        else if (choice == 4) deleteContact();
        else if (choice == 5) searchContact();
        else if (choice == 6) manageGroups();
        else if (choice == 8) viewSearchStats();
        else if (choice == 7) cout << "Saving and exiting...\n";
        else cout << "Invalid choice.\n";
    }

    for (int i = 0; i < contactCount; i++) {
        delete contacts[i];
    }
    return 0;
}
