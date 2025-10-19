#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <string>
#include <iostream>

using namespace std;

class BankCustomer {
private:
    int id;
    string name;
    double balance;

public:
    BankCustomer(int id, const string& name, double balance) 
        : id(id), name(name), balance(balance) 
    {}

    // Getters dengan const
    int getId() const { return id; }
    string getName() const { return name; }
    double getBalance() const { return balance; }

    // Mutator
    void setName(const string& newName) { name = newName; }
    
    void addBalance(double amount) {
        balance += amount;
    }

    bool withdrawBalance(double amount) {
        if (amount > balance) {
            cout << "Rejected: Insufficient funds!" << endl;
            return false;
        }
        balance -= amount;
        return true;
    }
    
    void printInfo() const {
        cout << "| ID: " << id << " | Name: " << name << " | Balance: $" << fixed << setprecision(2) << balance << endl;
    }
};

#endif // BANK_CUSTOMER_H