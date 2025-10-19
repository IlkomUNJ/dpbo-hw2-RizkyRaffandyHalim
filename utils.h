#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>

using namespace std;

// --- ENUMERASI GLOBAL ---
enum Status { PAID, COMPLETE, CANCELED };
enum BankTxType { TOPUP, WITHDRAW, TRANSFER_IN, TRANSFER_OUT };

// --- UTILITY TANGGAL ---
time_t getCurrentTime() {
    return time(nullptr);
}

// Konversi time_t ke string format (DD/MM/YYYY)
string timeToString(time_t t) {
    tm* ltm = localtime(&t);
    stringstream ss;
    ss << put_time(ltm, "%d/%m/%Y");
    return ss.str();
}

// Mendapatkan time_t untuk k hari yang lalu
time_t daysAgo(int k) {
    time_t now = getCurrentTime();
    return now - (k * 24 * 60 * 60); // Detik per hari
}

// Cek apakah waktu 't' berada dalam rentang 'k' hari terakhir
bool isWithinLastDays(time_t t, int k) {
    return t >= daysAgo(k);
}

// Cek apakah waktu 't' adalah hari ini
bool isToday(time_t t) {
    time_t now = getCurrentTime();
    tm* ltm_t = localtime(&t);
    tm* ltm_now = localtime(&now);
    
    return ltm_t->tm_year == ltm_now->tm_year &&
           ltm_t->tm_mon == ltm_now->tm_mon &&
           ltm_t->tm_mday == ltm_now->tm_mday;
}

// --- BASE CLASS TRANSAKSI ---

class Transaction {
protected:
    time_t timestamp;
    double amount;

public:
    Transaction(double amount) : amount(amount) {
        timestamp = getCurrentTime();
    }

    time_t getTimestamp() const { return timestamp; }
    double getAmount() const { return amount; }

    virtual void printInfo() const = 0; // Pure virtual
};

#endif // UTILS_H