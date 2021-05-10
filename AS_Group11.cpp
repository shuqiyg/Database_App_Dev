/* Group 11 - Shuqi Yang
   Date: 04-10-2021  */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

struct Employee {
    int employeeNumber;
    char lastName[50];
    char firstName[50];
    char extension[10];
    char email[100];
    char officecode[10];
    int reportsTo;
    char jobTitle[50];
};

int getInt();
int menu(void);
int findEmployee(Connection*, int, struct Employee*);
void displayEmployee(struct Employee);
void displayAllEmployees(Connection*);
void insertEmployee(struct Employee*);
void insertEmployee(Connection*, struct Employee);
void updateEmployee(Connection*, int);
void deleteEmployee(Connection*, int);

int main(void) {
	Environment* env = nullptr;
	Connection* conn = nullptr;
	string user = "dbs211_211j33";
	string pass = "42059066";
	string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

	env = Environment::createEnvironment(Environment::DEFAULT);
	conn = env->createConnection(user, pass, constr);
    int option;
    Employee emp;

    do {
        option = menu();
        switch (option) {
        case 1:
            int empNum;
            cout << "Enter the employee number: ";
            cin >> empNum;
            if (!findEmployee(conn, empNum, &emp)) {
                cout << "Employee " << empNum << " does not exist" << endl;
            }
            else {
                cout << endl << "-------------- Employee Information -------------" << endl;
                displayEmployee(emp);
            }
            cout << endl;
            break;
        case 2:
            displayAllEmployees(conn);
            break;
        case 3:
            insertEmployee(&emp);
            if (findEmployee(conn, emp.employeeNumber, &emp)) {
                cout << "An employee with the same employee number exists" << endl;
            }
            else {
                insertEmployee(conn, emp);
                cout << "The new employee is added successfully." << endl;

            }
            cout << endl;
            
            break;
        case 4:
            int eNum_inPut;          
            cout << "Employee Number: ";
            cin >> eNum_inPut;
            cin.ignore(1000, '\n');
            if (findEmployee(conn, eNum_inPut, &emp)) {
                cout << "Last Name: " << emp.lastName << endl;
                cout << "First Name: " << emp.firstName << endl;
                updateEmployee(conn, eNum_inPut);
                cout << "The employee's extension is updated successfully" << endl << endl;
                
            }
            else {
                cout << "The employee with ID " << eNum_inPut << " does not exist." << endl << endl;
            }
            break;
        case 5:
            cout << "Employee Number: ";
            cin >> eNum_inPut;
            cin.ignore(1000, '\n');
            if (findEmployee(conn, eNum_inPut, &emp)) {
                deleteEmployee(conn, eNum_inPut);
                cout << "The employee with ID " << eNum_inPut << " is deleted successfully." << endl << endl;
            }
            else {
                cout << "The employee with ID " << eNum_inPut << " does not exist." << endl << endl;
            }
            break;
        case 0:
            return 0;
        }
    } while (option != 0);

	env->terminateConnection(conn);
	Environment::terminateEnvironment(env);
    return 0;
}

int getInt() {
    int intInput;
    char NL;
    do {
        cout << "Enter an option (0 to 5): ";
        while (!(cin >> intInput)) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Enter an option (0 to 5): ";
        }
        cin.get(NL);
        if (NL != '\n') {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            //cout << "Enter an option (0 to 5): ";
        }                   
    } while (intInput < 0 || intInput > 5 || NL != '\n' );
    return intInput;
}

int menu() {
	cout << "********************* HR Menu *********************" << endl;
    cout << "1) Find Employee" << endl << "2) Employees Report" << endl << "3) Add Employee" << endl << "4) Update Employee" << endl << "5) Remove Employee" << endl << "0) Exit" << endl;
    return getInt();
}

int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp) {
    emp->reportsTo;
    int find;
    string query = "SELECT * FROM employees WHERE employeenumber=:1";
    try {
        Statement* stmt = nullptr;
        stmt = conn->createStatement(query);
        stmt->setInt(1, employeeNumber);
        ResultSet* rs = nullptr;
        rs = stmt->executeQuery();
        if (!rs->next()) {
            find = 0;
        }
        else {
            emp->employeeNumber = rs->getInt(1);
            strcpy(emp->lastName, (rs->getString(2)).c_str());
            strcpy(emp->firstName, (rs->getString(3)).c_str());
            strcpy(emp->extension, (rs->getString(4)).c_str());
            strcpy(emp->email, (rs->getString(5)).c_str());
            strcpy(emp->officecode, (rs->getString(6)).c_str());
            emp->reportsTo = rs->getInt(7);
            strcpy(emp->jobTitle, (rs->getString(8)).c_str());
            find = 1;
        }
        conn->terminateStatement(stmt);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    return find;
}

void displayEmployee(struct Employee emp) {
    cout << "Employee Number:" << emp.employeeNumber << endl;
    cout << "Last Name: " << emp.lastName << endl;
    cout << "Fisrt Name: " << emp.lastName << endl;
    cout << "Extension: " << emp.extension << endl;
    cout << "Email: " << emp.email << endl;
    cout << "Office Code: " << emp.officecode << endl;
    cout << "Manager ID: " << emp.reportsTo << endl;
    cout << "Job Title: " << emp.jobTitle << endl;
}

void displayAllEmployees(Connection* conn) {
    string query = "SELECT e.employeenumber, e.firstname||' '|| e.lastname, e.email, o.phone, e.extension, m.firstname||' '||  m.lastname FROM employees e JOIN offices o USING(officecode) LEFT JOIN employees m ON e.reportsto=m.employeenumber ORDER BY 1";
    try {
        Statement* stmt = nullptr;
        stmt = conn->createStatement(query);
        ResultSet* rs = nullptr;
        rs = stmt->executeQuery();
        if (!rs->next()) {
            cout << "There is no employees' information to be displayed." << endl;
        }
        else {
            cout << "------   ---------------    --------------------------------  ----------------  ---------  ------------------" << endl;
            cout << "ID       " << "Employee Name      " << "Email                             " << "Phone             " << "Extension  " << "Manager Name     " << endl;
            cout << "-----    ---------------    --------------------------------  ----------------  ---------  ------------------" << endl;
            do {
                int ID = rs->getInt(1);
                string eName = rs->getString(2);
                string email = rs->getString(3);
                string phone = rs-> getString(4);
                string ext = rs->getString(5);
                string mName = rs->getString(6);

                cout.setf(ios::left);
                cout.width(9);
                cout << ID;
                cout.width(19);
                cout << eName;
                cout.width(34);
                cout << email;
                cout.width(18);
                cout << phone;
                cout.width(11);
                cout << ext;
                cout.width(16);
                cout << mName << endl;
            } while (rs->next());
            cout << endl;
        }
        conn->terminateStatement(stmt);
    }
    catch(SQLException& sqlExcp){
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

void insertEmployee(struct Employee* emp) {
    cout << endl << "-------------- New Employee Information -------------" << endl;
    cout << "Employee Number: ";
    cin >> emp->employeeNumber;
    cin.ignore(1000, '\n');
    cout << "Last Name: ";
    cin >> emp->lastName;
    cin.ignore(1000, '\n');
    cout << "First Name: ";
    cin >> emp->firstName;
    cin.ignore(1000, '\n');
    cout << "Extension: ";
    cin >> emp->extension;
    cin.ignore(1000, '\n');
    cout << "Email: ";
    cin >> emp->email;
    cin.ignore(1000, '\n');
    cout << "Office Code: 1\n";
    strcpy(emp->officecode, "1");
    cout << "Manager ID: 1002\n";
    emp->reportsTo = 1002;
    cout << "Job Title: ";
    cin >> emp->jobTitle;
    cin.ignore(1000, '\n');
    cout << endl;
}

void insertEmployee(Connection* conn, struct Employee emp) {
    string query = "INSERT INTO employees VALUES (:1, :2, :3, :4, :5, :6, :7, :8)";
    try {
        Statement* stmt = nullptr;
        stmt = conn->createStatement(query);
        stmt->setInt(1, emp.employeeNumber);
        stmt->setString(2, emp.lastName);
        stmt->setString(3, emp.firstName);
        stmt->setString(4, emp.extension);
        stmt->setString(5, emp.email);
        stmt->setString(6, emp.officecode);
        stmt->setInt(7, emp.reportsTo);
        stmt->setString(8, emp.jobTitle);
        stmt->executeUpdate();
        conn->commit();
        conn->terminateStatement(stmt);
      
    }catch(SQLException& sqlExcp){
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

void updateEmployee(Connection* conn, int employeeNumber) {
    string extInput;
    cout << "Entension: ";
    cin >> extInput;
    cin.ignore(1000, '\n');
    string query = "UPDATE employees SET extension=:1 WHERE employeenumber=:2";
    try {
        Statement* stmt = nullptr;
        stmt = conn->createStatement(query);
        stmt->setString(1, extInput);
        stmt->setInt(2, employeeNumber);
        stmt->executeUpdate();
        conn->commit();
        conn->terminateStatement(stmt);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

void deleteEmployee(Connection* conn, int employeeNumber) {
    string query_1 = "ALTER TABLE employees DISABLE CONSTRAINT EMP_RTEMP_FK";
    string query_2 = "DELETE FROM employees WHERE employeenumber=:1";
    string query_3 = "ALTER TABLE employees ENABLE CONSTRAINT EMP_RTEMP_FK";
    try {
        Statement* stmt = nullptr;
        stmt = conn->createStatement(query_1);
        stmt->executeUpdate();
        stmt->setSQL(query_2);
        stmt->setInt(1, employeeNumber);
        stmt->executeUpdate();
        stmt->setSQL(query_3);
        stmt->executeUpdate();
        conn->commit();
        conn->terminateStatement(stmt);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}