#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
using namespace std;

class Employee
{
private:
    int empid;
    char name[30];
    float salary;

public:
    void input()
    {
        cout << "\nEnter employee id, name and salary.\n";
        cin >> empid;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.getline(name, 30);
        cout << "Enter salary: ";
        cin >> salary;
    }

    void display()
    {
        cout << endl;
        cout << empid << " ";
        cout << name << "  ";
        cout << salary << endl;
    }

    void store();
    void printAll();
    void search(int id);
    void edit(int id);
    void del(int id);
};

//to store new employee record
void Employee::store()
{
    ofstream fout("EmployeeFile", ios::binary | ios::app);
    if (!fout)
    {
        cerr << "Error opening file for writing." << endl;
        return;
    }
    fout.write((char *)this, sizeof(*this));
    fout.close();
}

// to print all
void Employee::printAll()
{
    ifstream fin("EmployeeFile", ios::binary | ios::in);
    if (!fin)
    {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    fin.read((char *)this, sizeof(*this));
    while (fin.gcount() > 0)
    {
        display();
        fin.read((char *)this, sizeof(*this));
    }

    fin.close();
}

// to search
void Employee::search(int id)
{
    ifstream fin("EmployeeFile", ios::binary | ios::in);
    if (!fin)
    {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    bool found = false;
    fin.read((char *)this, sizeof(*this));
    while (fin.gcount() > 0)
    {
        if (empid == id)
        {
            display();
            found = true;
            break;
        }
        fin.read((char *)this, sizeof(*this));
    }
    if (!found)
        cout << "Search Failed, record not found." << endl;

    fin.close();
}

// to edit
void Employee::edit(int id)
{
    fstream file("EmployeeFile", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cerr << "Error opening file for editing." << endl;
        return;
    }

    bool found = false;
    file.read((char *)this, sizeof(*this));
    while (file.gcount() > 0)
    {
        if (id == empid)
        {
            cout << "Enter correct Employee name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.getline(name, 30);
            cout << "Enter correct salary: ";
            cin >> salary;
            file.seekp(-static_cast<int>(sizeof(*this)), ios::cur);
            file.write((char *)this, sizeof(*this));
            found = true;
            break;
        }
        file.read((char *)this, sizeof(*this));
    }

    if (!found)
        cout << "Record not found." << endl;

    file.close();
}

// to delete
void Employee::del(int id)
{
    ifstream fin("EmployeeFile", ios::binary);
    if (!fin)
    {
        cerr << "Error opening file for deletion." << endl;
        return;
    }

    ofstream fout("tempfile", ios::binary);
    if (!fout)
    {
        cerr << "Error opening temporary file." << endl;
        return;
    }

    bool found = false;
    fin.read((char *)this, sizeof(*this));
    while (fin.gcount() > 0)
    {
        if (id != empid)
        {
            fout.write((char *)this, sizeof(*this));
        }
        else
        {
            found = true;
        }
        fin.read((char *)this, sizeof(*this));
    }

    fin.close();
    fout.close();

    if (!found)
    {
        cout << "Record not found." << endl;
        remove("tempfile");
    }
    else
    {
        remove("EmployeeFile");
        rename("tempfile", "EmployeeFile");
    }
}

int menu()
{
    int choice;
    cout << "\n1. New Record.";
    cout << "\n2. Edit Record.";
    cout << "\n3. View All Records.";
    cout << "\n4. Search a Record.";
    cout << "\n5. Delete a Record.";
    cout << "\n6. Exit";
    cout << "\n\nEnter your choice: ";
    cin >> choice;
    return choice;
}

int main()
{
    Employee emp;
    int id;
    while (true)
    {
        switch (menu())
        {
        case 1:
            emp.input();
            emp.store();
            break;
        case 2:
            cout << "Enter employee id to edit: ";
            cin >> id;
            emp.edit(id);
            break;
        case 3:
            emp.printAll();
            break;
        case 4:
            cout << "Enter employee id to search: ";
            cin >> id;
            emp.search(id);
            break;
        case 5:
            cout << "Enter employee id to delete: ";
            cin >> id;
            emp.del(id);
            break;
        case 6:
            exit(0);
        default:
            cout << "\nInvalid Choice" << endl;
        }
    }
    return 0;
}
