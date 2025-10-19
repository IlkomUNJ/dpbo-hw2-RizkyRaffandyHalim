#ifndef STORE_H
#define STORE_H

#include "bank.h"
#include "buyer.h"
#include "seller.h"
#include "store_transaction.h"
#include <map>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <sstream> 
#include <string>  

using namespace std;

class Store {
private:
    Bank* _bankService;
    vector<StoreTransaction> logTransaksi;
    int idTransaksiBerikutnya = 10001;

public:
    Store(Bank* b) : _bankService(b) {}
    
    // Fungsi untuk menyimpan transaksi ke file
    void saveTransactions(ofstream& file) const {
        if (!file.is_open()) return;
        for (const auto& tx : logTransaksi) {
            file << tx.toString() << endl; 
        }
    }
    
    // Fungsi untuk memuat transaksi dari file
    void loadTransaction(const string& line) {
        // Cek prefix
        if (line.rfind("[STORE],", 0) != 0) return; 
        
        // Parsing data
        string data = line.substr(7);
        stringstream ss(data);
        string segment;
        vector<string> seglist;
        
        while(getline(ss, segment, ',')) {
            seglist.push_back(segment);
        }
        
        if (seglist.size() == 8) {
            try {
                time_t t = stoll(seglist[0]);
                int txId = stoi(seglist[1]);
                int bId = stoi(seglist[2]);
                int sId = stoi(seglist[3]);
                int iId = stoi(seglist[4]);
                int qty = stoi(seglist[5]);
                double amount = stod(seglist[6]);
                Status stat = (Status)stoi(seglist[7]);

                // Gunakan helper constructor untuk loading
                logTransaksi.emplace_back(t, txId, bId, sId, iId, qty, amount, stat);
                
                if (txId >= idTransaksiBerikutnya) {
                    idTransaksiBerikutnya = txId + 1;
                }
            } catch (...) {
                cerr << "Gagal memproses baris Store: " << line << endl;
            }
        }
    }
    
    // --- Purchase Logic ---
    bool purchaseItem(Buyer* buyer, Seller* seller, Item* item, int qty) {
        if (!buyer || !seller || !item) {
            cout << "Pembelian GAGAL: Entitas tidak valid." << endl;
            return false;
        }
        if (item->getQuantity() < qty) {
            cout << "Pembelian GAGAL: Stok tidak mencukupi." << endl;
            return false;
        }
        
        double totalHarga = item->getPrice() * qty;
        
        // Pembayaran melalui Bank
        if (_bankService->transfer(buyer->getAccount()->getId(), seller->getAccount()->getId(), totalHarga)) {
            
            // Update Stok Barang
            item->reduceStock(qty);
            
            // Catat Transaksi
            logTransaksi.emplace_back(idTransaksiBerikutnya++, 
                                      buyer->getId(), 
                                      seller->getId(), 
                                      item->getId(), 
                                      qty, 
                                      totalHarga, 
                                      PAID);
            
            cout << "Pembelian BERHASIL. ID Transaksi: " << logTransaksi.back().getId() << endl;
            return true;
        }
        return false;
    }
    
    void processTransaction(int transactionId, Status newStatus) {
        for (auto& tx : logTransaksi) {
            if (tx.getId() == transactionId) {
                if (tx.getStatus() == CANCELED) {
                    cout << "Update GAGAL: Transaksi sudah dibatalkan sebelumnya." << endl;
                    return;
                }
                
                if (newStatus == CANCELED && tx.getStatus() != CANCELED) {
                    cout << "Memproses pembatalan Transaksi ID: " << transactionId << endl;
                }
                
                tx.setStatus(newStatus);
                cout << "Status Transaksi ID " << transactionId << " diperbarui menjadi " 
                     << (newStatus == PAID ? "PAID" : (newStatus == COMPLETE ? "COMPLETE" : "CANCELED")) << endl;
                return;
            }
        }
        cout << "Update GAGAL: Transaksi ID " << transactionId << " tidak ditemukan." << endl;
    }
    
    const vector<StoreTransaction>& getTransactions() const { return logTransaksi; }
    
    void listBuyerOrders(int buyerId, const string& filter) const {
        cout << "\n--- Pesanan Buyer ID: " << buyerId << " (Filter: " << filter << ") ---" << endl;
        int count = 0;
        for (const auto& tx : logTransaksi) {
            if (tx.getBuyerId() == buyerId) {
                if (filter == "ALL" || 
                    (filter == "PAID" && tx.getStatus() == PAID) ||
                    (filter == "COMPLETED" && tx.getStatus() == COMPLETE) ||
                    (filter == "CANCELED" && tx.getStatus() == CANCELED)) {
                    tx.printInfo();
                    count++;
                }
            }
        }
        if (count == 0) cout << "Tidak ada pesanan ditemukan." << endl;
    }
    
    void checkBuyerSpending(int buyerId, int kDays) const {
        cout << "\n--- Laporan Pengeluaran Buyer ID: " << buyerId << " (" << kDays << " Hari Terakhir) ---" << endl;
        time_t kHariLalu = daysAgo(kDays);
        double totalBelanja = 0.0;
        
        for (const auto& tx : logTransaksi) {
            if (tx.getBuyerId() == buyerId && tx.getStatus() != CANCELED && tx.getTimestamp() >= kHariLalu) {
                totalBelanja += tx.getAmount();
            }
        }
        
        cout << "Total Belanja: $" << fixed << setprecision(2) << totalBelanja << endl;
        
        if (totalBelanja == 0) cout << "Tidak ada pengeluaran dalam " << kDays << " hari terakhir." << endl;
    }
    
    void reportPopularItems(int n) const {
        cout << "\n--- LAPORAN: Top " << n << " Item Paling Populer (Berdasarkan Jumlah Terjual) ---" << endl;
        map<int, int> hitungQty; 
        
        for (const auto& tx : logTransaksi) {
            if (tx.getStatus() != CANCELED) {
                hitungQty[tx.getItemId()] += tx.getQuantity();
            }
        }
        
        vector<pair<int, int>> itemTerurut;
        for (const auto& pair : hitungQty) {
            itemTerurut.push_back({pair.second, pair.first}); 
        }
        
        sort(itemTerurut.rbegin(), itemTerurut.rend());
        
        int batas = min((int)itemTerurut.size(), n);
        for (int i = 0; i < batas; ++i) {
            cout << i + 1 << ". Item ID: " << itemTerurut[i].second 
                 << " | Terjual: " << itemTerurut[i].first << " unit" << endl;
        }
    }
};

#endif // STORE_H