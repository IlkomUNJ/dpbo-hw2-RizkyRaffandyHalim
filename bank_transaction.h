#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H

#include "utils.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class BankTransaction : public Transaction {
private:
    int accountId;
    BankTxType type;

public:
    BankTransaction(int accountId, double amount, BankTxType type)
        : Transaction(amount), accountId(accountId), type(type) {}

    // Helper constructor untuk LOADING dari file
    BankTransaction(time_t t, int accountId, double amount, BankTxType type)
        : Transaction(amount), accountId(accountId), type(type) {
        this->timestamp = t; 
    }

    // Fungsi untuk serialisasi ke baris file
    string toString() const {
        stringstream ss;
        ss << "[BANK]," << timestamp << "," << accountId 
           << "," << fixed << setprecision(2) << amount << "," << (int)type;
        return ss.str();
    }

    int getAccountId() const { return accountId; }
    BankTxType getType() const { return type; }

    void printInfo() const override {
        cout << "| Date: " << timeToString(timestamp) << " | Account ID: " << accountId 
             << " | Type: " << (type == TOPUP ? "TOPUP" : (type == WITHDRAW ? "WITHDRAW" : (type == TRANSFER_IN ? "T-IN" : "T-OUT"))) 
             << " | Amount: $" << fixed << setprecision(2) << amount << endl;
    }
};

#endif // BANK_TRANSACTION_H