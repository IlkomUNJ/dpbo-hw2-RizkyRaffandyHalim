#ifndef BANK_H
#define BANK_H

#include "bank_customer.h"
#include "bank_transaction.h"
#include "utils.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream> 
#include <sstream> 
#include <string>  

using namespace std;

class Bank {
private:
    string namaBank;
    vector<BankCustomer> daftarAkun; 
    vector<BankTransaction> riwayatTransaksi; 

public:
    Bank(const string& nama) : namaBank(nama) {}
    
    // Getter untuk namaBank
    const string& getName() const { return namaBank; }

    // ---Getter Customer (Dua Versi)---
    BankCustomer* getCustomerById(int id) {
        for (auto& pelanggan : daftarAkun) {
            if (pelanggan.getId() == id) {
                return &pelanggan;
            }
        }
        return nullptr;
    }
    
    // 2. CONST version: Untuk laporan/akses read-only
    const BankCustomer* getCustomerById(int id) const {
        for (const auto& pelanggan : daftarAkun) {
            if (pelanggan.getId() == id) {
                return &pelanggan;
            }
        }
        return nullptr;
    }

    void addCustomer(const BankCustomer& customer) {
        if (getCustomerById(customer.getId())) {
            cout << "Customer ID " << customer.getId() << " sudah terdaftar." << endl;
            return;
        }
        daftarAkun.push_back(customer);
    }

    const vector<BankCustomer>& getAllCustomers() const { return daftarAkun; }
    const vector<BankTransaction>& getTransactions() const { return riwayatTransaksi; }
    
    void saveTransactions(ofstream& file) const {
        if (!file.is_open()) return;
        for (const auto& tx : riwayatTransaksi) {
            file << tx.toString() << endl; 
        }
    }
    
    void loadTransaction(const string& line) {
        // Cek prefix
        if (line.rfind("[BANK],", 0) != 0) return; 
        
        // Parsing data
        string data = line.substr(6);
        stringstream ss(data);
        string segment;
        vector<string> seglist;
        
        while(getline(ss, segment, ',')) {
            seglist.push_back(segment);
        }
        
        if (seglist.size() == 4) {
            try {
                time_t t = stoll(seglist[0]);
                int accountId = stoi(seglist[1]);
                double amount = stod(seglist[2]);
                BankTxType type = (BankTxType)stoi(seglist[3]);

                // Gunakan helper constructor untuk loading
                riwayatTransaksi.emplace_back(t, accountId, amount, type);
            } catch (...) {
                cerr << "Gagal memproses baris Bank: " << line << endl;
            }
        }
    }

    bool topUp(int accountId, double amount) {
        BankCustomer* acc = getCustomerById(accountId);
        if (!acc) {
            cout << "Top Up GAGAL: Akun Bank ID " << accountId << " tidak ditemukan." << endl;
            return false;
        }
        acc->addBalance(amount);
        riwayatTransaksi.emplace_back(accountId, amount, TOPUP);
        cout << "Top Up BERHASIL. Saldo baru: $" << fixed << setprecision(2) << acc->getBalance() << endl;
        return true;
    }

    bool withdraw(int accountId, double amount) {
        BankCustomer* acc = getCustomerById(accountId);
        if (!acc) {
            cout << "Penarikan GAGAL: Akun Bank ID " << accountId << " tidak ditemukan." << endl;
            return false;
        }
        if (acc->withdrawBalance(amount)) {
            riwayatTransaksi.emplace_back(accountId, amount, WITHDRAW);
            cout << "Penarikan BERHASIL. Saldo baru: $" << fixed << setprecision(2) << acc->getBalance() << endl;
            return true;
        }
        return false;
    }
    
    bool transfer(int fromId, int toId, double amount) {
        BankCustomer* fromAcc = getCustomerById(fromId);
        BankCustomer* toAcc = getCustomerById(toId);
        
        if (!fromAcc || !toAcc) {
            cout << "Transfer GAGAL: Salah satu Akun tidak valid." << endl;
            return false;
        }
        
        if (fromAcc->withdrawBalance(amount)) {
            toAcc->addBalance(amount);
            
            // Catat 2 Transaksi: Keluar (OUT) dan Masuk (IN)
            riwayatTransaksi.emplace_back(fromId, amount, TRANSFER_OUT);
            riwayatTransaksi.emplace_back(toId, amount, TRANSFER_IN);
            
            cout << "Transfer BERHASIL. Saldo " << fromAcc->getName() << ": $" << fixed << setprecision(2) << fromAcc->getBalance() << endl;
            return true;
        }
        return false;
    }

    // --- Laporan ---
    void reportInactiveUsers() const { 
        cout << "\n--- LAPORAN: Akun Bank Tidak Aktif (>30 hari) ---" << endl;
        time_t sebulanLalu = daysAgo(30);
        bool found = false;
        
        for (const auto& acc : daftarAkun) {
            bool aktif = false;
            for (const auto& tx : riwayatTransaksi) {
                if (tx.getAccountId() == acc.getId() && tx.getTimestamp() >= sebulanLalu) {
                    aktif = true;
                    break;
                }
            }
            if (!aktif) {
                acc.printInfo();
                found = true;
            }
        }
        if (!found) {
            cout << "Semua akun aktif dalam 30 hari terakhir." << endl;
        }
    }
    
    void reportTopUsersToday(int n) const {
        cout << "\n--- LAPORAN: Top " << n << " Pengguna Bank Paling Aktif Hari Ini ---\r\n" << endl;
        map<int, int> hitungTx; 
        
        for (const auto& tx : riwayatTransaksi) {
            if (isToday(tx.getTimestamp())) {
                hitungTx[tx.getAccountId()]++;
            }
        }
        
        vector<pair<int, int>> penggunaTerurut;
        for (const auto& pair : hitungTx) {
            penggunaTerurut.push_back({pair.second, pair.first}); 
        }
        
        sort(penggunaTerurut.rbegin(), penggunaTerurut.rend());
        
        int batas = min((int)penggunaTerurut.size(), n);
        for (int i = 0; i < batas; ++i) {
            const BankCustomer* acc = getCustomerById(penggunaTerurut[i].second);
            if (acc) {
                cout << i + 1 << ". ID: " << acc->getId() << " | Name: " << acc->getName() 
                     << " | Transaksi: " << penggunaTerurut[i].first << " kali" << endl;
            }
        }
    }
};

#endif // BANK_H