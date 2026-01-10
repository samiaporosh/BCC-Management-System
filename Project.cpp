#include <iostream>
#include <fstream>
#include <string>
#include <cstdio> // for remove() and rename()

using namespace std;

/* ========= AUTHORIZATION ========= */
class Auth
{
public:
    static bool canAdd(const string &role)
    {
        return (role == "President" || role == "Advisor" ||
                role == "GS" || role == "Executive");
    }

    static bool canDelete(const string &role)
    {
        return (role == "President" || role == "Advisor" || role == "GS");
    }
};

/* ========= MEMBER ========= */
class Member
{
public:
    static void addMember(const string &loggedRole)
    {
        if (!Auth::canAdd(loggedRole))
        {
            cout << "Access Denied! View-only permission.\n";
            return;
        }

        int id;
        string name, role;

        ofstream file("members.txt", ios::app);
        if (!file)
        {
            cout << "File error!\n";
            return;
        }

        cout << "Enter ID: ";
        cin >> id;
        cout << "Enter Name: ";
        cin >> name;
        cout << "Enter Role (President/Advisor/GS/Executive/Normal): ";
        cin >> role;

        file << id << " " << name << " " << role << endl;
        file.close();

        cout << "Member added successfully!\n";
    }

    static void viewMembers()
    {
        ifstream file("members.txt");
        int id;
        string name, role;

        cout << "\n--- MEMBER LIST ---\n";
        while (file >> id >> name >> role)
        {
            cout << "ID: " << id
                 << " | Name: " << name
                 << " | Role: " << role << endl;
        }
        file.close();
    }

    static void deleteMember(const string &loggedRole)
    {
        if (!Auth::canDelete(loggedRole))
        {
            cout << "Access Denied! Delete not allowed.\n";
            return;
        }

        ifstream file("members.txt");
        ofstream temp("temp.txt");

        int delId, id;
        string name, role;
        bool found = false;

        cout << "Enter Member ID to delete: ";
        cin >> delId;

        while (file >> id >> name >> role)
        {
            if (id != delId)
                temp << id << " " << name << " " << role << endl;
            else
                found = true;
        }

        file.close();
        temp.close();

        remove("members.txt");
        rename("temp.txt", "members.txt");

        cout << (found ? "Member deleted successfully!\n"
                       : "Member not found!\n");
    }
};

/* ========= PANEL ========= */
class Panel
{
public:
    static void addPanel(const string &role)
    {
        if (!Auth::canAdd(role))
        {
            cout << "Access Denied!\n";
            return;
        }

        ofstream file("panels.txt", ios::app);
        string panel, m1, m2;

        cout << "Enter Panel Name: ";
        cin >> panel;
        cout << "Enter Panel Member 1: ";
        cin >> m1;
        cout << "Enter Panel Member 2: ";
        cin >> m2;

        file << panel << " " << m1 << " " << m2 << endl;
        file.close();

        cout << "Panel added successfully!\n";
    }

    static void viewPanels()
    {
        ifstream file("panels.txt");
        string panel, m1, m2;

        cout << "\n--- PANEL LIST ---\n";
        while (file >> panel >> m1 >> m2)
        {
            cout << "Panel: " << panel
                 << " | Members: " << m1 << ", " << m2 << endl;
        }
        file.close();
    }

    static void deletePanel(const string &role)
    {
        if (!Auth::canDelete(role))
        {
            cout << "Access Denied!\n";
            return;
        }

        ifstream file("panels.txt");
        ofstream temp("temp.txt");

        string panel, m1, m2, delPanel;
        bool found = false;

        cout << "Enter Panel Name to delete: ";
        cin >> delPanel;

        while (file >> panel >> m1 >> m2)
        {
            if (panel != delPanel)
                temp << panel << " " << m1 << " " << m2 << endl;
            else
                found = true;
        }

        file.close();
        temp.close();

        remove("panels.txt");
        rename("temp.txt", "panels.txt");

        cout << (found ? "Panel deleted successfully!\n"
                       : "Panel not found!\n");
    }
};

/* ========= EVENT ========= */
class Event
{
public:
    static void addEvent(const string &role)
    {
        if (!Auth::canAdd(role))
        {
            cout << "Access Denied!\n";
            return;
        }

        ofstream file("events.txt", ios::app);
        string panel, event;

        cout << "Enter Panel Name: ";
        cin >> panel;
        cout << "Enter Event Name: ";
        cin >> event;

        file << panel << " " << event << endl;
        file.close();

        cout << "Event added successfully!\n";
    }

    static void viewEvents()
    {
        ifstream file("events.txt");
        string panel, event;

        cout << "\n--- EVENT LIST ---\n";
        while (file >> panel >> event)
        {
            cout << "Panel: " << panel
                 << " | Event: " << event << endl;
        }
        file.close();
    }

    static void deleteEvent(const string &role)
    {
        if (!Auth::canDelete(role))
        {
            cout << "Access Denied!\n";
            return;
        }

        ifstream file("events.txt");
        ofstream temp("temp.txt");

        string panel, event, delEvent;
        bool found = false;

        cout << "Enter Event Name to delete: ";
        cin >> delEvent;

        while (file >> panel >> event)
        {
            if (event != delEvent)
                temp << panel << " " << event << endl;
            else
                found = true;
        }

        file.close();
        temp.close();

        remove("events.txt");
        rename("temp.txt", "events.txt");

        cout << (found ? "Event deleted successfully!\n"
                       : "Event not found!\n");
    }
};

/* ========= MAIN ========= */
int main()
{
    string role;
    int choice;

    cout << "Enter your role (President/Advisor/GS/Executive/Normal): ";
    cin >> role;

    cout << "\n====================================\n";
    cout << " Welcome to BAIUST Computer Clubbing Management System\n";
    cout << " Logged in as: " << role << endl;
    cout << "====================================\n";

    do
    {
        cout << "\n1. View Members";
        cout << "\n2. View Panels";
        cout << "\n3. View Events";

        if (Auth::canAdd(role))
        {
            cout << "\n4. Add Member";
            cout << "\n5. Add Panel";
            cout << "\n6. Add Event";
        }

        if (Auth::canDelete(role))
        {
            cout << "\n7. Delete Member";
            cout << "\n8. Delete Panel";
            cout << "\n9. Delete Event";
        }

        cout << "\n10. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            Member::viewMembers();
            break;
        case 2:
            Panel::viewPanels();
            break;
        case 3:
            Event::viewEvents();
            break;
        case 4:
            Member::addMember(role);
            break;
        case 5:
            Panel::addPanel(role);
            break;
        case 6:
            Event::addEvent(role);
            break;
        case 7:
            Member::deleteMember(role);
            break;
        case 8:
            Panel::deletePanel(role);
            break;
        case 9:
            Event::deleteEvent(role);
            break;
        case 10:
            cout << "Thanks for being with us.\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 10);

    return 0;
}
