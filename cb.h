#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

// Contact base class - sab contacts ke common fields yahan hain
class Contact {
protected:
    int id;
    string name;
    string email;
    string phone;
    string city;        // ab ye seedha Contact ka apna variable hai
public:
    Contact();
    Contact(int i, string n, string e, string ph, string c);
    virtual void show();               // virtual isliye derived class apna version chala sake
    virtual string report();
    virtual string toFileLine();       // file mein save karne ke liye ek line banata hai
    bool operator==(Contact &c);       // duplicate check ke liye operator overload
    int getId();
    string getName();
    string getEmail();
    string getPhone();
    void setName(string n);
    void setEmail(string e);
    void setPhone(string ph);
    void mergePhone(string extraPhone);
    virtual ~Contact();
};

// PersonalContact - Contact se inherit karti hai (is-a relationship)
class PersonalContact : public Contact {
private:
    string relation;   // jaise Cousin, Friend
public:
    PersonalContact();
    PersonalContact(int i, string n, string e, string ph, string c, string rel);
    void show();
    string report();
    string toFileLine();
};

// BusinessContact - ye bhi Contact se inherit karti hai
class BusinessContact : public Contact {
private:
    string company;
public:
    BusinessContact();
    BusinessContact(int i, string n, string e, string ph, string c, string comp);
    void show();
    string report();
    string toFileLine();
};

// Group class - many-to-many relationship ke liye, sirf IDs store karti hai
class Group {
private:
    int groupId;
    string groupName;
    int contactIds[20];
    int memberCount;
public:
    Group();
    Group(int gid, string gname);
    void addContact(int cid);
    void display();
    int getGroupId();
    string getGroupName();
    int getMemberCount();
    int getMemberAt(int index);
};

// SearchLog - keyword, kitni baar search hua, aur aakhri baar kab search hua
class SearchLog {
private:
    string keyword;
    int frequency;
    string lastSearched;
public:
    SearchLog();
    SearchLog(string k);
    void increment();
    string getKeyword();
    int getFrequency();
    string getLastSearched();
};

// FileManager - poore program ki file reading/writing yahan centralize hai
class FileManager {
public:
    static void saveContacts(Contact *arr[], int n);
    static int loadContacts(Contact *arr[], int &nextId);
    static void saveGroups(Group arr[], int n);
    static int loadGroups(Group arr[]);
};

// helper functions
bool matchKeyword(string field, string key);
string currentTimestamp();
