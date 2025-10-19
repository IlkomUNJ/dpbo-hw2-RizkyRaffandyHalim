// File: buyer.h
#ifndef BUYER_H
#define BUYER_H

#include <string>
#include "bank_customer.h" 

using namespace std;

class Buyer {
private:
    int id;
    string nama;
    BankCustomer *_akunBank; 

public:
    // Constructor
    Buyer(int idBeli, const string& namaBeli, BankCustomer *akun)
        : id(idBeli), nama(namaBeli), _akunBank(akun) {} 

    // --- Getters ---
    int getId() const { return id; }
    string getName() const { return nama; }
    
    // Untuk modifikasi saldo / akses pointer
    BankCustomer* getAccount() { return _akunBank; } 

    // Untuk akses read-only
    const BankCustomer* getAccount() const { return _akunBank; } 

    // --- Setters / Mutators ---
    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { nama = newName; }
    
    // Fungsi Setter baru untuk memperbarui pointer BankCustomer
    void setAccount(BankCustomer* newAccount) {
        _akunBank = newAccount;
    }
};

#endif // BUYER_H