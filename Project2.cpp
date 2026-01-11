#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

/* ================= CONSTANT FILES ================= */
const string MEMBER_FILE = "members.txt";
const string PANEL_FILE = "panels.txt";
const string EVENT_FILE = "events.txt";
const string LOG_FILE = "log.txt";

/* ================= LOGGER ================= */
void logAction(const string &role, const string &action)
{
    ofstream log(LOG_FILE, ios::app);
    log << "[" << role << "] " << action << endl;
    log.close();
}

/* ================= ROLES & PASSWORDS ================= */
string roles[] = {"President", "Advisor", "GS", "Executive", "Normal"};
string passwords[] = {"pass123", "adv2026", "gs2026", "exec2026", "normal"};

/* ================= AUTH ================= */
class Auth
{
public:
    static bool canAdd(const string &role)
    {
        return role == "President" || role == "Advisor" ||
               role == "GS" || role == "Executive";
    }

    static bool canDelete(const string &role)
    {
        return role == "President" || role == "Advisor" || role == "GS";
    }
};

/* ================= MEMBER ================= */
class Member
{
public:
    static bool exists(int id)
    {
        ifstream file(MEMBER_FILE);
        string line;
        while (getline(file, line))
        {
            int mid = stoi(line.substr(0, line.find('|')));
            if (mid == id)
                return true;
        }
        return false;
    }

    static void addMember(const string &role)
    {
        if (!Auth::canAdd(role))
        {
            cout << "Access denied.\n";
            return;
        }

        int id;
        string name, mrole;

        cout << "ID: ";
        cin >> id;
        cin.ignore();

        if (exists(id))
        {
            cout << "Member with this ID already exists!\n";
            return;
        }

        cout << "Name: ";
        getline(cin, name);

        cout << "Role: ";
        getline(cin, mrole);

        ofstream file(MEMBER_FILE, ios::app);
        file << id << "|" << name << "|" << mrole << endl;
        file.close();

        logAction(role, "Added member " + name);
        cout << "Member added successfully.\n";
    }

    static void viewMembers()
    {
        ifstream file(MEMBER_FILE);
        if (!file)
        {
            cout << "No members found.\n";
            return;
        }

        cout << "\n--- MEMBER LIST ---\n";
        cout << left << setw(6) << "ID"
             << setw(20) << "Name"
             << setw(15) << "Role" << endl;
        cout << "------------------------------------------\n";

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string id, name, role;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, role, '|');

            cout << setw(6) << id
                 << setw(20) << name
                 << setw(15) << role << endl;
        }
        file.close();
    }

    static void deleteMember(const string &role)
    {
        if (!Auth::canDelete(role))
        {
            cout << "Delete not allowed.\n";
            return;
        }

        int delId;
        cout << "Enter ID to delete: ";
        cin >> delId;

        char confirm;
        cout << "Are you sure? (y/n): ";
        cin >> confirm;
        if (confirm != 'y')
            return;

        ifstream file(MEMBER_FILE);
        ofstream temp("temp.txt");

        string line;
        bool found = false;

        while (getline(file, line))
        {
            int id = stoi(line.substr(0, line.find('|')));
            if (id != delId)
                temp << line << endl;
            else
                found = true;
        }

        file.close();
        temp.close();
        remove(MEMBER_FILE.c_str());
        rename("temp.txt", MEMBER_FILE.c_str());

        if (found)
        {
            logAction(role, "Deleted member ID " + to_string(delId));
            cout << "Member deleted.\n";
        }
        else
        {
            cout << "Member not found.\n";
        }
    }
};

/* ================= PANEL ================= */
class Panel
{
public:
    static bool exists(const string &panelName)
    {
        ifstream file(PANEL_FILE);
        string line;
        while (getline(file, line))
        {
            if (line.substr(0, line.find('|')) == panelName)
                return true;
        }
        return false;
    }

    static void addPanel(const string &role)
    {
        if (!Auth::canAdd(role))
        {
            cout << "Access denied.\n";
            return;
        }

        cin.ignore();
        string panel;
        cout << "Panel Name: ";
        getline(cin, panel);

        if (exists(panel))
        {
            cout << "Panel already exists!\n";
            return;
        }

        string pres, gs, tre, vpT, vpO, vpPR;
        cout << "President: ";
        getline(cin, pres);
        cout << "General Secretary: ";
        getline(cin, gs);
        cout << "Treasurer: ";
        getline(cin, tre);
        cout << "VP (Technical): ";
        getline(cin, vpT);
        cout << "VP (Organizing): ";
        getline(cin, vpO);
        cout << "VP (Public Relations): ";
        getline(cin, vpPR);

        ofstream file(PANEL_FILE, ios::app);
        file << panel << "|"
             << "President:" << pres << "|"
             << "GS:" << gs << "|"
             << "Treasurer:" << tre << "|"
             << "VP-Tech:" << vpT << "|"
             << "VP-Org:" << vpO << "|"
             << "VP-PR:" << vpPR << endl;
        file.close();

        logAction(role, "Added panel " + panel);
        cout << "Panel added successfully.\n";
    }

    static void viewPanel()
    {
        cin.ignore();
        string search;
        cout << "Enter panel name: ";
        getline(cin, search);

        ifstream file(PANEL_FILE);
        string line;
        bool found = false;

        while (getline(file, line))
        {
            if (line.substr(0, line.find('|')) == search)
            {
                stringstream ss(line);
                string part;
                int idx = 0;
                cout << "\n--- PANEL MEMBERS ---\n";
                while (getline(ss, part, '|'))
                {
                    if (idx > 0)
                        cout << part << endl; // skip panel name
                    idx++;
                }
                found = true;
                break;
            }
        }
        if (!found)
            cout << "Panel not found.\n";

        file.close();
    }

    static void deletePanel(const string &role)
    {
        if (!Auth::canDelete(role))
        {
            cout << "Delete not allowed.\n";
            return;
        }

        cin.ignore();
        string panelName;
        cout << "Enter Panel Name to delete: ";
        getline(cin, panelName);

        if (!exists(panelName))
        {
            cout << "Panel not found.\n";
            return;
        }

        char confirm;
        cout << "Are you sure? (y/n): ";
        cin >> confirm;
        if (confirm != 'y')
            return;

        ifstream file(PANEL_FILE);
        ofstream temp("temp.txt");
        string line;

        while (getline(file, line))
        {
            if (line.substr(0, line.find('|')) != panelName)
                temp << line << endl;
        }

        file.close();
        temp.close();
        remove(PANEL_FILE.c_str());
        rename("temp.txt", PANEL_FILE.c_str());

        logAction(role, "Deleted panel " + panelName);
        cout << "Panel deleted successfully.\n";
    }
};

/* ================= EVENT ================= */
class Event
{
public:
    static void addEvent(const string &role)
    {
        if (!Auth::canAdd(role))
        {
            cout << "Access denied.\n";
            return;
        }

        cin.ignore();
        string panel;
        cout << "Panel Name: ";
        getline(cin, panel);

        int n;
        cout << "Number of events (min 5): ";
        cin >> n;
        cin.ignore();
        if (n < 5)
            n = 5;

        ofstream file(EVENT_FILE, ios::app);
        file << panel;

        for (int i = 1; i <= n; i++)
        {
            string ev;
            cout << "Event " << i << ": ";
            getline(cin, ev);
            file << "|Event:" << ev;
        }
        file << endl;
        file.close();

        logAction(role, "Added events for panel " + panel);
        cout << "Events added.\n";
    }

    static void viewEvents()
    {
        cin.ignore();
        string search;
        cout << "Panel Name: ";
        getline(cin, search);

        ifstream file(EVENT_FILE);
        string line;
        bool found = false;

        while (getline(file, line))
        {
            if (line.substr(0, line.find('|')) == search)
            {
                stringstream ss(line);
                string part;
                int ecount = 0;
                getline(ss, part, '|'); // skip panel name
                cout << "\n--- EVENTS ---\n";
                while (getline(ss, part, '|'))
                {
                    ecount++;
                    cout << "Event " << ecount << ": " << part.substr(6) << endl; // remove "Event:" prefix
                }
                found = true;
                break;
            }
        }

        if (!found)
            cout << "No events found for this panel.\n";

        file.close();
    }

    static void deleteEvent(const string &role)
    {
        if (!Auth::canDelete(role))
        {
            cout << "Delete not allowed.\n";
            return;
        }

        cin.ignore();
        string panelName;
        cout << "Enter Panel Name to delete its events: ";
        getline(cin, panelName);

        ifstream file(EVENT_FILE);
        ofstream temp("temp.txt");
        string line;
        bool found = false;

        while (getline(file, line))
        {
            if (line.substr(0, line.find('|')) != panelName)
                temp << line << endl;
            else
                found = true;
        }

        file.close();
        temp.close();
        remove(EVENT_FILE.c_str());
        rename("temp.txt", EVENT_FILE.c_str());

        if (found)
        {
            logAction(role, "Deleted events for panel " + panelName);
            cout << "Events deleted successfully.\n";
        }
        else
            cout << "No events found for this panel.\n";
    }
};

/* ================= ROLE SWITCH ================= */
void switchRole(string &currentRole)
{
    int choice;
    cout << "\n--- SWITCH ROLE ---\n";
    for (int i = 0; i < 5; i++)
    {
        cout << i + 1 << ". " << roles[i] << endl;
    }

    cout << "Select role (1-5): ";
    cin >> choice;

    if (choice < 1 || choice > 5)
    {
        cout << "Invalid selection!\n";
        return;
    }

    string selectedRole = roles[choice - 1];
    string pass;
    cout << "Enter password for " << selectedRole << ": ";
    cin >> pass;

    if (pass == passwords[choice - 1])
    {
        currentRole = selectedRole;
        cout << "Role switched to " << currentRole << endl;
    }
    else
    {
        cout << "Incorrect password! Role switch denied.\n";
    }
}

/* ================= MAIN ================= */
int main()
{
    string role, pass;

    // LOGIN WITH MENU
    do
    {
        cout << "\n--- SELECT ROLE ---\n";
        for (int i = 0; i < 5; i++)
            cout << i + 1 << ". " << roles[i] << endl;

        int choice;
        cout << "Select role (1-5): ";
        cin >> choice;

        if (choice < 1 || choice > 5)
        {
            cout << "Invalid selection! Try again.\n";
            continue;
        }

        role = roles[choice - 1];
        cout << "Enter password for " << role << ": ";
        cin >> pass;

        if (pass != passwords[choice - 1])
        {
            cout << "Incorrect password! Try again.\n";
        }
        else
            break;
    } while (true);

    cout << "\nLogin successful! Role: " << role << endl;

    int choice;
    do
    {
        cout << "\nCurrent Role: " << role << endl;
        cout << "1.Add Member\n2.View Members\n3.Delete Member";
        cout << "\n4.Add Panel\n5.View Panel\n6.Delete Panel";
        cout << "\n7.Add Events\n8.View Events\n9.Delete Events";
        cout << "\n10.Switch Role\n11.Exit\nChoice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            Member::addMember(role);
            break;
        case 2:
            Member::viewMembers();
            break;
        case 3:
            Member::deleteMember(role);
            break;
        case 4:
            Panel::addPanel(role);
            break;
        case 5:
            Panel::viewPanel();
            break;
        case 6:
            Panel::deletePanel(role);
            break;
        case 7:
            Event::addEvent(role);
            break;
        case 8:
            Event::viewEvents();
            break;
        case 9:
            Event::deleteEvent(role);
            break;
        case 10:
            switchRole(role);
            break;
        case 11:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 11);

    return 0;
}
