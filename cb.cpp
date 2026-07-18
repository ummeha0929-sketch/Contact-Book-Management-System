#include "cb.h"

// ---------- helper functions ----------

bool matchKeyword(string field, string key) {
    return field.find(key) != string::npos;
}

string currentTimestamp() {
    time_t now = time(0);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&now));
    return string(buf);
}

// ---------- Contact base class ----------

Contact::Contact() {
    id = 0; name = ""; email = ""; phone = ""; city = "";
}

Contact::Contact(int i, string n, string e, string ph, string c) {
    id = i; name = n; email = e; phone = ph; city = c;
}

void Contact::show() {
    cout << "ID: " << id << " | Name: " << name << " | Email: " << email;
    cout << " | Phone: " << phone << " | City: " << city << endl;
}

string Contact::report() {
    return "Contact Report: " + name;
}

string Contact::toFileLine() {
    return "C," + to_string(id) + "," + name + "," + email + "," + phone + "," + city + ",";
}

bool Contact::operator==(Contact &c) {
    return email == c.email || phone == c.phone;
}

int Contact::getId() { return id; }
string Contact::getName() { return name; }
string Contact::getEmail() { return email; }
string Contact::getPhone() { return phone; }

void Contact::setName(string n) { name = n; }
void Contact::setEmail(string e) { email = e; }
void Contact::setPhone(string ph) { phone = ph; }

void Contact::mergePhone(string extraPhone) {
    if (phone != extraPhone) {
        phone = phone + ", " + extraPhone;
    }
}

Contact::~Contact() {
}

// ---------- PersonalContact ----------

PersonalContact::PersonalContact() : Contact() {
    relation = "";
}

PersonalContact::PersonalContact(int i, string n, string e, string ph, string c, string rel) : Contact(i, n, e, ph, c) {
    relation = rel;
}

void PersonalContact::show() {
    cout << "ID: " << id << " | Personal Contact -> Name: " << name << " | Relation: " << relation;
    cout << " | Phone: " << phone << " | Email: " << email << " | City: " << city << endl;
}

string PersonalContact::report() {
    return "Personal Contact: " + name + " Relation: " + relation;
}

string PersonalContact::toFileLine() {
    return "P," + to_string(id) + "," + name + "," + email + "," + phone + "," + city + "," + relation;
}

// ---------- BusinessContact ----------

BusinessContact::BusinessContact() : Contact() {
    company = "";
}

BusinessContact::BusinessContact(int i, string n, string e, string ph, string c, string comp) : Contact(i, n, e, ph, c) {
    company = comp;
}

void BusinessContact::show() {
    cout << "ID: " << id << " | Business Contact -> Name: " << name << " | Company: " << company;
    cout << " | Phone: " << phone << " | Email: " << email << " | City: " << city << endl;
}

string BusinessContact::report() {
    return "Business Contact: " + name + " Company: " + company;
}

string BusinessContact::toFileLine() {
    return "B," + to_string(id) + "," + name + "," + email + "," + phone + "," + city + "," + company;
}

// ---------- Group ----------

Group::Group() {
    groupId = 0; groupName = ""; memberCount = 0;
}

Group::Group(int gid, string gname) {
    groupId = gid; groupName = gname; memberCount = 0;
}

void Group::addContact(int cid) {
    if (memberCount < 20) {
        contactIds[memberCount] = cid;
        memberCount++;
    }
}

void Group::display() {
    cout << "Group -> ID: " << groupId << " Name: " << groupName << " Members: " << memberCount << " -> ";
    for (int i = 0; i < memberCount; i++) {
        cout << contactIds[i] << " ";
    }
    cout << endl;
}

int Group::getGroupId() { return groupId; }
string Group::getGroupName() { return groupName; }
int Group::getMemberCount() { return memberCount; }
int Group::getMemberAt(int index) { return contactIds[index]; }

// ---------- SearchLog ----------

SearchLog::SearchLog() {
    keyword = ""; frequency = 0; lastSearched = "";
}

SearchLog::SearchLog(string k) {
    keyword = k; frequency = 1; lastSearched = currentTimestamp();
}

void SearchLog::increment() {
    frequency++;
    lastSearched = currentTimestamp();
}

string SearchLog::getKeyword() { return keyword; }
int SearchLog::getFrequency() { return frequency; }
string SearchLog::getLastSearched() { return lastSearched; }

// ---------- FileManager (saara file I/O yahan centralize hai) ----------

void FileManager::saveContacts(Contact *arr[], int n) {
    ofstream f("contacts.txt");
    for (int i = 0; i < n; i++) {
        f << arr[i]->toFileLine() << endl;
    }
    f.close();
}

int FileManager::loadContacts(Contact *arr[], int &nextId) {
    ifstream f("contacts.txt");
    int count = 0;
    if (!f.is_open()) {
        return 0;
    }
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string type, idStr, name, email, phone, city, extra;
        getline(ss, type, ',');
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, phone, ',');
        getline(ss, city, ',');
        getline(ss, extra, ',');
        int id = stoi(idStr);

        if (type == "P") {
            arr[count] = new PersonalContact(id, name, email, phone, city, extra);
        } else if (type == "B") {
            arr[count] = new BusinessContact(id, name, email, phone, city, extra);
        } else {
            arr[count] = new Contact(id, name, email, phone, city);
        }
        count++;
        if (id >= nextId) {
            nextId = id + 1;
        }
    }
    f.close();
    return count;
}

void FileManager::saveGroups(Group arr[], int n) {
    ofstream f("groups.txt");
    for (int i = 0; i < n; i++) {
        f << arr[i].getGroupId() << "," << arr[i].getGroupName();
        for (int j = 0; j < arr[i].getMemberCount(); j++) {
            f << "," << arr[i].getMemberAt(j);
        }
        f << endl;
    }
    f.close();
}

int FileManager::loadGroups(Group arr[]) {
    ifstream f("groups.txt");
    int count = 0;
    if (!f.is_open()) {
        return 0;
    }
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr, gname;
        getline(ss, idStr, ',');
        getline(ss, gname, ',');
        arr[count] = Group(stoi(idStr), gname);

        string memberStr;
        while (getline(ss, memberStr, ',')) {
            arr[count].addContact(stoi(memberStr));
        }
        count++;
    }
    f.close();
    return count;
}
