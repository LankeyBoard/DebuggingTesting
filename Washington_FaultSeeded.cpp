/**
 * File contains classes Student, ClassRoll, and ClassRollUI as well
 * as their implementations. Student contains the data fields specified
 * in the project description as well as the setter and getter functions
 * needed to manage them. ClassRecords contains a vector of Student
 * objects and, as the name implies, it provides functions for managing
 * the records of students within a class. ClassRollUI holds a ClassRecords
 * object and presents the console I/O interface with its functions.
 * 
 * At program start, the file "ClassRecords.csv" is read from (or created,
 * if it does not already exist) and any added records are added to this
 * file before the program ends. The records are stored as comma separated
 * values with fields appearing in the following order:
 * Name, UID, Email, PresentationGrade, EssayGrade, ProjectGrade
 * 
 * This program does not currently utilize any form of input validation for
 * the 6 fields associated with a Student.
*/

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std; 

const string YESNO("(YES/NO)\n");

const string MAIN_PROMPT(
"\n-----------------------------------------------------------------\n"
"SELECT WHICH ACTION TO TAKE.\n"
"Enter  READ    to display student data\n"
"Enter  SEARCH  to search student data based on name, email, or U-ID\n"
"Enter  ADD     to add a student to records\n"
"Enter  EDIT    to edit student data or delete a record\n"
"\nTo quit, enter: QUIT\n"
"-----------------------------------------------------------------\n");

const string EDIT_PROMPT(
"\n-----------------------------------------------------------------\n"
"SELECT WHICH EDIT ACTION TO TAKE.\n"
"Enter  NAME    to change name\n"
"Enter  UID     to change U-ID\n"
"Enter  EMAIL   to change email\n"
"Enter  PRES    to change the presentation grade\n"
"Enter  ESSAY   to change the essay grade\n"
"Enter  PROJ    to change the term project grade\n"
"Enter  DELET   to delete the record (WARNING: THIS CANNOT BE UNDONE)\n"
"\nEnter  DONE    to exit.\n"
"-----------------------------------------------------------------\n");

const string INVALID_PROMPT(
"\n-----------------------------------------------------------------\n"
"INVALID INPUT. Please input one of the following commands.\n"
"Enter  READ    to display student data\n"
"Enter  SEARCH  to search student data based on name, email, or U-ID\n"
"Enter  ADD     to add a student to records\n"
"Enter  EDIT    to edit student data or delete a record\n"
"\nTo quit, enter: QUIT\n"
"-----------------------------------------------------------------\n");

const int MAX_NAME_EMAIL = 40;
const int MAX_UID = 10;
const int GRADE_W = 10;

void toUpperCase(string& string)
{
    for (char& c : string)
        c = toupper(c);
}

void prompt_and_collect_choice(const string& PROMPT, string& choice)
{
    cout << PROMPT;
    // cout << "Selection: ";
    cin >> choice;
    toUpperCase(choice);
}

class Student
{
private:
    string name;
    string uid;
    string email;
    int presGrade = 4;
    int essayGrade = 4;
    int projGrade = 4;
public:
    Student(string name, string uid, string email);
    Student(const string& name, const string& uid, const string& email, int presGr, int essGr, int projGr);
    void displayStudentData() const;
    string getStudentData() const;
    string getName() const {return name;}
    string getUID() const {return uid;}
    string getEmail() const {return email;}
    int getPresGrade() const {return presGrade;}
    int getEssayGrade() const {return essayGrade;}
    int getProjGrade() const {return projGrade;}
    void setName(string str){name = str.substr(0, MAX_NAME_EMAIL);}
    void setUID(string str){uid = str.substr(0, MAX_UID);} //To-Do: data validation
    void setEmail(string str){email = str.substr(0, MAX_NAME_EMAIL);}
    void setPresentationGrade(int gr){presGrade = gr;}
    void setEssayGrade(int gr){essayGrade = gr;}
    void setTermProjGrade(int gr){projGrade = gr;}
};

class ClassRecords
{
private:
    vector<Student> records;
    int currKey = 0;
public:
    ClassRecords();
    void fileInput();
    void fileOutput();
    void displayRecords();
    void displayStudent(int key){records[key].displayStudentData();} 
    int getSize(){return records.size();}
    int searchRecords(string& identifier);
    bool identifierMatch(const string& identifier);
    bool addRecord(string& name, string& uid, string& email);
    bool addRecord(string& name, string& uid, string& email, int pres, int ess, int proj);
    bool editRecord(int key, const string& action, string& update);
    bool editRecord(int key, const string& action, int update);
    bool deleteRecord(int key);
};

class ClassRecordsUI
{
private:
    ClassRecords records;
    void read(){records.displayRecords();}
    void search();
    void add();
    void edit();
public:
    void run();
};

#ifndef DEBUG
int main()
{
    ClassRecordsUI pgm = ClassRecordsUI();
    pgm.run();
    return 0;
}
#endif /* DEBUG */

/******************************************************
 *   Beginning of Student function implementations    *
******************************************************/

Student::Student(string name, string uid, string email)
{
    this->name = name.substr(MAX_NAME_EMAIL);
    this->uid = uid.substr(0, MAX_UID); 
    this->email = email.substr(0, MAX_NAME_EMAIL);
    this->essayGrade = -1;
    this->presGrade = -1;
    this->projGrade = -1;
}

Student::Student(const string& name, const string& uid, const string& email, int presGr, int essGr, int projGr)
{
    this->name = name.substr(0, MAX_NAME_EMAIL);
    this->uid = uid.substr(0, MAX_UID);
    this->email = email.substr(0, MAX_NAME_EMAIL);

    presGrade = presGr;
    essayGrade = essGr;
    projGrade = projGr;
}

void Student::displayStudentData() const
{
    //using std::setw;
    cout << std::left
         << setw(MAX_NAME_EMAIL+2) << name
         << setw(MAX_UID+2) << uid
         << setw(MAX_NAME_EMAIL+2) << email
         << setw(GRADE_W) << presGrade
         << setw(GRADE_W) << essayGrade
         << setw(GRADE_W) << projGrade
         << endl;
}

/******************************************************
 * Beginning of ClassRecords function implementations *
******************************************************/
ClassRecords::ClassRecords()
{
    
}
void ClassRecords::fileInput()
{
    string line, att, name, UID, email;
    fstream fin; 
    fin.open("ClassRecords.csv", ios::in);
    if (fin.fail())
        return;
    while(getline(fin, line))
    {
        stringstream str(line); 
        int j=0;
        int num, presGr=-1, essGr=-1, projGr=-1;
        while (getline(str, att, ','))
        {
            if(j == 0)
                name=att;
            else if(j == 1)
                UID=att;
            else if(j == 2)
                email=att;
            else if(j == 3)
                presGr=stod(att);
            else if(j == 4)
                essGr=stod(att);
            else if(j == 5)
                projGr=stod(att);
            j++;
        }
        if(presGr!=-1.0)
        {
            Student s = Student(name, UID, email, presGr, essGr, projGr);
            records.push_back(s);
        }
        else
        {
            Student s = Student(name, UID, email);
            records.push_back(s);
        }
    }
}

void ClassRecords::fileOutput()
{
    fstream fout; 
    fout.open("ClassRecords.csv", ios::out);
    for(int i=0; i<records.size(); i++)
    {   
        fout << records.at(i).getName() << ","
        << records.at(i).getUID() << ","
        << records.at(i).getEmail()  << ","
        << records.at(i).getPresGrade() << ","
        << records.at(i).getEssayGrade() << ","
        << records.at(i).getProjGrade() << "\n"; 
    } 
}

void ClassRecords::displayRecords()
{
    if (records.size() == 0)
        cout << "No records to display.\n";
    else{
        using std::setw;
        cout << std::left
             << setw(MAX_NAME_EMAIL+2) << "STUDENT_NAME"
             << setw(MAX_UID+2) << "UID"
             << setw(MAX_NAME_EMAIL+2) << "EMAIL"
             << setw(GRADE_W) << "PRES."
             << setw(GRADE_W) << "ESSAY"
             << setw(GRADE_W) << "PROJ."
             << endl;

        for (Student& s : records)
            s.displayStudentData();
    }
}

int ClassRecords::searchRecords(string& identifier)
{
    for (int i = 0; i < records.size(); i++)
    {   
        if (records[i].getName() == identifier
        || records[i].getUID() == identifier
        || records[i].getEmail() == identifier)
        {
            //cout << "MATCH: ";
            //records[i].displayStudentData();
            return i;
        }
    }
    //cout << "NO MATCHING RECORD FOUND.\n";
    return -1;
}


bool ClassRecords::identifierMatch(const string& identifier)
{
    bool match = false;
    if (currKey != -1)
    {
        if (identifier==records[currKey].getName() || identifier==records[currKey].getUID() || identifier==records[currKey].getEmail())
            match = true;
    }
    return match;
}

bool ClassRecords::addRecord(string& name, string& uid, string& email)
{
    records.emplace_back(Student(name, uid, email, 0, 0, 0));
    //Stub: return false?
    return true;
}

bool ClassRecords::addRecord(string& name, string& uid, string& email, int pres, int ess, int proj)
{
    records.emplace_back(name, uid, email, pres, ess, proj);
    //Stub: return false?
    return true;
}

/* Updates either the name, uid, or email fields (as determined by action) of
 * the student in position indicated by key within the vector "records".*/
bool ClassRecords::editRecord(int key, const string& action, string& update)
{
    bool recordUpdated = false;
    
    if ((key >= 0) && (key < records.size())) 
    {
        if (action == "NAME")
        {
            records[key].setName(update);
            recordUpdated = true;
        }
        else if (action == "UID")
        {
            records[key].setUID(update); 
            recordUpdated = true;
        }
        else if (action == "EMAIL")
        {
            records[key].setEmail(update);
            recordUpdated = true;
        }
        else
        {
            //Invalid action: throw exception?
            recordUpdated = false;
        }
    }
    return recordUpdated;
}

/* Updates either the presentation, essay, or project grades (as determined by action)
 * of the student in position indicated by key within the vector "records".*/
bool ClassRecords::editRecord(int key, const string& action, int update)
{
    bool recordUpdated = false;

    if ((key >= 0) && (key < records.size()) ) 
    {
        if (action == "PRES")
        {
            records[key].setPresentationGrade(update);
            recordUpdated = true;
            
        }
        else if (action == "ESSAY")
        {
            records[key].setEssayGrade(update);
            recordUpdated = true;
        }
        else if (action == "PROJ")
        {
            records[key].setTermProjGrade(update);
            recordUpdated = true;
        }
        else
        {
            //Invalid action: throw exception?
            recordUpdated = false;
        }
    }
    return recordUpdated;
}

bool ClassRecords::deleteRecord(int key)
{
    records.erase(records.begin()+key);
    return true;
    //return false?
}

/********************************************************
 * Beginning of ClassRecordsUI function implementations *
********************************************************/

//Searches for a record based on name, uid, or email.
//Uses linear search provided by searchRecords function.
void ClassRecordsUI::search()
{
    string search;
    cout << "Enter the name, UID, or email to be searched: ";
    cin.ignore(MAX_NAME_EMAIL,'\n');
    getline(cin , search, '\n');
    

    int key;
    key = records.searchRecords(search);
    if (key != -1)
    { 
        using std::setw;
        cout << std::left
             << setw(MAX_NAME_EMAIL+2) << "STUDENT_NAME"
             << setw(MAX_UID+2) << "UID"
             << setw(MAX_NAME_EMAIL+2) << "EMAIL"
             << setw(GRADE_W) << "PRES."
             << setw(GRADE_W) << "ESSAY"
             << setw(GRADE_W) << "PROJ."
             << endl;
        records.displayStudent(key);
    }
    else
        cout << "NO MATCHING RECORD FOUND\n";
}

void ClassRecordsUI::add()
{
    string name, uid, email, choice;
    cout << "Enter \"FIRSTNAME LASTNAME\": ";
    cin.ignore();
    getline(cin, name, '\n');
    cout << "Enter UID: ";
    getline(cin, uid, '\n');
    cout << "Enter Email: ";
    getline(cin, email, '\n');

    cout << "Add grades for this student? ";
    prompt_and_collect_choice(YESNO, choice);

    if (choice == "YES")
    {
        int presGrade, essayGrade, projGrade;
        cout << "Enter Presentation Grade (from 0 to 4): ";
        cin >> presGrade;
        cout << "Enter Essay Grade (from 0 to 4): ";
        cin >> essayGrade;
        cout << "Enter Project Grade (from 0 to 4): ";
        cin >> projGrade;
        records.addRecord(name, uid, email, presGrade, essayGrade, projGrade); //Un-used bool
    }
    else
        records.addRecord(name, uid, email); //Un-used bool
}

//Searches for, then allows editing or deleting of a record.
void ClassRecordsUI::edit()
{
    string identifier, update, choice;
    int grade;
    int key;

    getline(cin, identifier);
    cout << "Enter the name, UID, or email of the record you want to edit: ";
    getline(cin, identifier);

    key = records.searchRecords(identifier);
    if (key < 0)
    {
        cout << "NO MATCHING RECORD FOUND.\n";
        return;
    }

    cout << "MATCH: ";
    records.displayStudent(key);
    prompt_and_collect_choice(EDIT_PROMPT, choice);
    while (choice != "DONE")
    {
        getline(cin, update);
        if (choice == "NAME" || choice == "UID" || choice == "EMAIL")
        {
            cout << "Enter updated information: ";
            getline(cin, update);
            records.editRecord(key, choice, update);
        }
        else if (choice == "PRES" || choice == "ESSAY" || choice == "PROJ")
        {
            cout << "Enter updated grade (from 0 to 4): ";
            cin >> grade;
            records.editRecord(key, choice, grade);
        }
        else if (choice == "DELET")
        {
            cout << "Are you sure you want to delete this record? ";
            prompt_and_collect_choice(YESNO, choice);
            if (choice == "YES")
            {
                records.deleteRecord(key);
                cout << "RECORD ERASED.\n";
                return;
            }
        }
        else
        {
            cout << "INVALID CHOICE.\n";
            prompt_and_collect_choice(EDIT_PROMPT, choice);
            continue;
        }
        cout << "Updated record: ";
        records.displayStudent(key);
        prompt_and_collect_choice(EDIT_PROMPT, choice);
    }
}

void ClassRecordsUI::run()
{
    records = ClassRecords();
    string choice;

    records.fileInput();

    prompt_and_collect_choice(MAIN_PROMPT, choice);
    while (choice != "QUIT")
    {
        if (choice == "READ")
            read();
        else if (choice == "SEARCH")
            search();
        else if (choice == "ADD")
            add();
        else if (choice == "EDIT")
            edit();
        else
        {
            prompt_and_collect_choice(INVALID_PROMPT, choice);
            continue;
        }
        prompt_and_collect_choice(MAIN_PROMPT, choice);
    }

    records.fileOutput();
}
