#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <fstream> 
#include <sstream> 

#include "utils.h"
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "item.h"
#include "bank.h"
#include "bank_transaction.h"
#include "store_transaction.h"
#include "store.h"

using namespace std;

// --- STRUKTUR DATA GLOBAL ---
vector<Buyer> allBuyers;
vector<Seller> allSellers;
Bank digitalBank("Digital Market Bank");
Store onlineStore(&digitalBank);

// --- CONSTANT ---
const string TRANSACTION_FILE = "riwayat_transaksi.txt";

// --- HELPER FUNCTIONS ---
int getNextBuyerId() { return allBuyers.empty() ? 101 : allBuyers.back().getId() + 1; }
int getNextSellerId() { return allSellers.empty() ? 2001 : allSellers.back().getId() + 1; }
int getNextBankAccountId() { return digitalBank.getAllCustomers().empty() ? 1 : digitalBank.getAllCustomers().back().getId() + 1; }

Buyer* findBuyer(int id) {
    auto it = find_if(allBuyers.begin(), allBuyers.end(), [id](const Buyer& b) { return b.getId() == id; });
    return (it != allBuyers.end()) ? &(*it) : nullptr;
}

Seller* findSeller(int id) {
    auto it = find_if(allSellers.begin(), allSellers.end(), [id](const Seller& s) { return s.getId() == id; });
    return (it != allSellers.end()) ? &(*it) : nullptr;
}

Item* findItem(int sellerId, int itemId) {
    Seller* s = findSeller(sellerId);
    return s ? s->getItemById(itemId) : nullptr;
}

// Memperbarui semua pointer BankCustomer
void refreshAllPointers() {
    // Perbarui semua Buyer
    for (Buyer& b : allBuyers) {
        BankCustomer* updatedAcc = digitalBank.getCustomerById(b.getAccount()->getId());
        if (updatedAcc) {
            b.setAccount(updatedAcc); 
        }
    }
    // Perbarui semua Seller
    for (Seller& s : allSellers) {
        BankCustomer* updatedAcc = digitalBank.getCustomerById(s.getAccount()->getId());
        if (updatedAcc) {
            s.setAccount(updatedAcc);
        }
    }
}

// FUNGSI PERSISTENCE
void loadAllTransactions() {
    ifstream file(TRANSACTION_FILE);
    if (!file.is_open()) {
        cout << "Info: File riwayat transaksi tidak ditemukan atau gagal dibuka. Memulai dengan riwayat kosong." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.rfind("[BANK],", 0) == 0) {
            digitalBank.loadTransaction(line);
        } else if (line.rfind("[STORE],", 0) == 0) {
            onlineStore.loadTransaction(line);
        }
    }

    file.close();
    cout << "Info: Riwayat transaksi berhasil dimuat dari " << TRANSACTION_FILE << "." << endl;
}

void saveAllTransactions() {
    ofstream file(TRANSACTION_FILE, ios::trunc); 
    if (!file.is_open()) {
        cerr << "ERROR: Gagal membuka/membuat file riwayat transaksi untuk penyimpanan!" << endl;
        return;
    }

    digitalBank.saveTransactions(file);
    onlineStore.saveTransactions(file);

    file.close();
    cout << "Info: Riwayat transaksi berhasil disimpan ke " << TRANSACTION_FILE << "." << endl;
}

// --- FUNGSI DATA DUMMY ---
void setupDummyData() {
    // 1. Akun Bank
    digitalBank.addCustomer(BankCustomer(1, "Alice Akun", 5000.0));
    digitalBank.addCustomer(BankCustomer(2, "Bob Akun", 1500.0));
    digitalBank.addCustomer(BankCustomer(3, "Charlie Akun", 200.0));
    digitalBank.addCustomer(BankCustomer(4, "Diana Akun", 3000.0));
    
    // 2. Buyer & Seller
    BankCustomer* ptrAcc1 = digitalBank.getCustomerById(1);
    BankCustomer* ptrAcc3 = digitalBank.getCustomerById(3);
    BankCustomer* ptrAcc2 = digitalBank.getCustomerById(2);
    BankCustomer* ptrAcc4 = digitalBank.getCustomerById(4);


    if (ptrAcc1 && ptrAcc2 && ptrAcc3 && ptrAcc4) {
        allBuyers.emplace_back(101, "Alice Buyer", ptrAcc1); // ID 101, Bank 1
        allBuyers.emplace_back(102, "Charlie Buyer", ptrAcc3); // ID 102, Bank 3
        allSellers.emplace_back(2001, "Bob Seller", ptrAcc2); // ID 2001, Bank 2
        allSellers.emplace_back(2002, "Diana Seller", ptrAcc4); // ID 2002, Bank 4
    }
    
    // 3. Item & Stock
    Seller* bobSeller = findSeller(2001);
    Seller* dianaSeller = findSeller(2002);
    
    if (bobSeller) {
        // Seller 1 (Bob)
        bobSeller->addItem(Item(1, "Laptop Gaming", 5, 1200.0));
        bobSeller->addItem(Item(2, "Mouse Wireless", 50, 25.0));
    }
    
    if (dianaSeller) {
        // Seller 2 (Diana)
        dianaSeller->addItem(Item(1, "Keyboard Mekanik", 10, 80.0));
        dianaSeller->addItem(Item(2, "Printer", 3, 150.0));
    }
    
    // 4. Dummy Transactions
    if (digitalBank.getTransactions().empty() && onlineStore.getTransactions().empty()) {
        cout << "Info: Menambahkan data transaksi dummy awal." << endl;
        
        Buyer* aliceBuyer = findBuyer(101);
        
        if (aliceBuyer && bobSeller && dianaSeller) {
            digitalBank.topUp(1, 200.00);
            digitalBank.withdraw(3, 10.00);

            onlineStore.purchaseItem(aliceBuyer, bobSeller, bobSeller->getItemById(2), 2); 
            onlineStore.purchaseItem(aliceBuyer, dianaSeller, dianaSeller->getItemById(1), 1); 
            
            onlineStore.processTransaction(10001, COMPLETE);
        }
    }
}

// --- MENU UTAMA ---
void main_menu() {
    cout << "\n========================================" << endl;
    cout << digitalBank.getName() << " | Rizen Online Shop" << endl;
    cout << "========================================" << endl;
    cout << "1. Login sebagai Buyer" << endl;
    cout << "2. Login sebagai Seller" << endl;
    cout << "3. Registrasi (Buyer/Seller)" << endl;
    cout << "4. Exit" << endl;
    cout << "5. Admin Login" << endl;
    cout << "Pilih menu (1-5): ";
}

// --- FUNGSI MENU ---
void adminMenu() {
    int choice;
    do {
 
        cout << "\n--- Admin Menu ---" << endl;
        cout << "1. Laporan Pengguna Bank Tidak Aktif (>30 Hari)" << endl;
        cout << "2. Laporan Top N Pengguna Bank Paling Aktif Hari Ini" << endl;
        cout << "3. Laporan Item Paling Populer" << endl;
        cout << "4. Lihat Semua Transaksi Bank" << endl; 
        cout << "5. Lihat Semua Transaksi Store" << endl; 
        cout << "6. Back to Main Menu" << endl;
        cout << "Pilih menu (1-6): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        if (choice == 1) {
            digitalBank.reportInactiveUsers();
        } else if (choice == 2) {
            int n;
            cout << "Masukkan jumlah pengguna teratas (N): ";
            cin >> n;
            digitalBank.reportTopUsersToday(n);
        } else if (choice == 3) {
            int n;
            cout << "Masukkan jumlah item teratas (N): ";
            cin >> n;
            onlineStore.reportPopularItems(n);
        } else if (choice == 4) {
            cout << "\n--- Semua Transaksi Bank ---" << endl;
            for (const auto& tx : digitalBank.getTransactions()) {
                tx.printInfo();
            }
        } else if (choice == 5) {
            cout << "\n--- Semua Transaksi Store ---" << endl;
            for (const auto& tx : onlineStore.getTransactions()) {
                tx.printInfo();
            }
        }

    } while (choice != 6);
}

void buyerMenu(Buyer* buyer) {
    if (!buyer) return;
    int choice;
    do {

        cout << "\n==========================================" << endl;
        cout << "Buyer Menu | " << buyer->getName() << " (ID: " << buyer->getId() << ")" << endl;
        cout << "Saldo Bank: $" << fixed << setprecision(2) << buyer->getAccount()->getBalance() << endl;
        cout << "==========================================" << endl;
        cout << "1. Beli Item" << endl;
        cout << "2. Top Up Bank Account" << endl;
        cout << "3. Lihat Riwayat Pesanan (Orders)" << endl;
        cout << "4. Cek Pengeluaran Terakhir (K Hari)" << endl;
        cout << "5. Kembali ke Main Menu" << endl;
        cout << "Pilih menu (1-5): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        if (choice == 1) {
            int sellerId, itemId, qty;
            cout << "Masukkan ID Seller: "; cin >> sellerId;
            cout << "Masukkan ID Item: "; cin >> itemId;
            cout << "Masukkan Kuantitas: "; cin >> qty;
            
            Seller* seller = findSeller(sellerId);
            Item* item = findItem(sellerId, itemId);
            
            if (!item || !item->getVisibility()) {
                cout << "Item ID " << itemId << " dari Seller " << sellerId << " tidak ditemukan atau tidak terlihat." << endl;
                continue;
            }
            
            onlineStore.purchaseItem(buyer, seller, item, qty);
            
        } else if (choice == 2) {
            double amount;
            cout << "Masukkan jumlah Top Up: $"; cin >> amount;
            if (amount > 0) {
                digitalBank.topUp(buyer->getAccount()->getId(), amount);
            } else {
                cout << "Jumlah tidak valid." << endl;
            }
        } else if (choice == 3) {
            cout << "Filter (ALL/PAID/COMPLETED/CANCELED): ";
            string filter;
            cin >> filter;
            for_each(filter.begin(), filter.end(), [](char & c){ c = toupper(c); });
            onlineStore.listBuyerOrders(buyer->getId(), filter);
        } else if (choice == 4) {
            int kDays;
            cout << "Cek pengeluaran dalam K hari terakhir. Masukkan K: ";
            cin >> kDays;
            onlineStore.checkBuyerSpending(buyer->getId(), kDays);
        }

    } while (choice != 5);
}

void sellerMenu(Seller* seller) {
    if (!seller) return;
    int choice;
    do {
        cout << "\n==========================================" << endl;
        cout << "Seller Menu | " << seller->getName() << " (ID: " << seller->getId() << ")" << endl;
        cout << "Saldo Bank: $" << fixed << setprecision(2) << seller->getAccount()->getBalance() << endl;
        cout << "==========================================" << endl;
        cout << "1. Tambah Item Baru" << endl;
        cout << "2. Lihat/Kelola Item Stock" << endl;
        cout << "3. Tarik Dana (Withdraw)" << endl;
        cout << "4. Update Status Transaksi" << endl;
        cout << "5. Kembali ke Main Menu" << endl;
        cout << "Pilih menu (1-5): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        if (choice == 1) {
            string name;
            int qty;
            double price;
            cout << "Masukkan Nama Item: "; cin.ignore(); getline(cin, name);
            cout << "Masukkan Harga Item: $"; cin >> price;
            cout << "Masukkan Stok Awal: "; cin >> qty;
            
            int nextItemId = seller->getAllItems().empty() ? 1 : seller->getAllItems().back().getId() + 1;
            seller->addItem(Item(nextItemId, name, qty, price));
            cout << "Item '" << name << "' berhasil ditambahkan. ID Item: " << nextItemId << endl;
            
        } else if (choice == 2) {
            cout << "\n--- Stok Item Anda ---" << endl;
            for (const auto& item : seller->getAllItems()) {
                item.printInfo();
            }
            int itemId, addQty;
            cout << "Masukkan ID Item untuk menambah stok (0 untuk batal): "; cin >> itemId;
            if (itemId == 0) continue;
            
            Item* item = seller->getItemById(itemId);
            if (item) {
                cout << "Tambahkan Stok: "; cin >> addQty;
                if (addQty > 0) {
                    item->addStock(addQty);
                    cout << "Stok Item ID " << itemId << " berhasil diperbarui. Stok baru: " << item->getQuantity() << endl;
                }
            } else {
                cout << "Item ID " << itemId << " tidak ditemukan." << endl;
            }
            
        } else if (choice == 3) {
            double amount;
            cout << "Masukkan jumlah penarikan: $"; cin >> amount;
            if (amount > 0) {
                digitalBank.withdraw(seller->getAccount()->getId(), amount);
            } else {
                cout << "Jumlah tidak valid." << endl;
            }
        } else if (choice == 4) {
            int txId;
            int statusInt;
            cout << "Masukkan ID Transaksi yang akan di-update: "; cin >> txId;
            
            // VERIFIKASI SELLER
            StoreTransaction* targetTx = nullptr;
            for (auto& tx : onlineStore.getTransactions()) {
                if (tx.getId() == txId) {
                    targetTx = const_cast<StoreTransaction*>(&tx); // Perlu const_cast karena getTransactions() const
                    break;
                }
            }
            
            if (!targetTx) {
                cout << "Update GAGAL: Transaksi ID " << txId << " tidak ditemukan." << endl;
            } else if (targetTx->getSellerId() != seller->getId()) {
                cout << "Update GAGAL: Transaksi ID " << txId << " BUKAN milik Anda." << endl;
            } else {
                cout << "Masukkan Status Baru (1=COMPLETE, 2=CANCELED): "; cin >> statusInt;
                
                if (statusInt == 1) {
                    onlineStore.processTransaction(txId, COMPLETE);
                } else if (statusInt == 2) {
                    onlineStore.processTransaction(txId, CANCELED);
                } else {
                    cout << "Status tidak valid." << endl;
                }
            }
        }

    } while (choice != 5);
}

// Fungsi main()
int main() {
    setupDummyData();
    loadAllTransactions();
    refreshAllPointers();

    // Tampilkan informasi ID untuk kemudahan penggunaan
    cout << "\n========================================" << endl;
    cout << "  INFO DUMMY AKUN (Gunakan untuk Login) " << endl;
    cout << "========================================" << endl;
    cout << "Buyer ID:\n- Alice Buyer: 101 (Bank ID: 1)\n- Charlie Buyer: 102 (Bank ID: 3)" << endl;
    cout << "Seller ID:\n- Bob Seller: 2001 (Bank ID: 2)\n- Diana Seller: 2002 (Bank ID: 4)" << endl;
    cout << "Admin Login: root/toor" << endl;
  
    int choice;
    do {
        main_menu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        switch (choice) {
            case 1: { // Login Buyer
                int id;
                cout << "Masukkan ID Buyer: "; cin >> id;
                Buyer* buyer = findBuyer(id);
                if (buyer) {

                    buyer->setAccount(digitalBank.getCustomerById(buyer->getAccount()->getId())); 
                    buyerMenu(buyer);
                } else {
                    cout << "ID Buyer tidak ditemukan." << endl;
                }
                break;
            }
            case 2: { // Login Seller
                int id;
                cout << "Masukkan ID Seller: "; cin >> id;
                Seller* seller = findSeller(id);
                if (seller) {
                    
                    seller->setAccount(digitalBank.getCustomerById(seller->getAccount()->getId())); 
                    sellerMenu(seller);
                } else {
                    cout << "ID Seller tidak ditemukan." << endl;
                }
                break;
            }
            case 3: { // Registrasi
                int regChoice;
                string name;
                cout << "Registrasi sebagai (1=Buyer, 2=Seller): "; cin >> regChoice;
                if (regChoice != 1 && regChoice != 2) {
                    cout << "Pilihan tidak valid." << endl;
                    break;
                }
                
                cout << "Masukkan Nama Anda: "; cin.ignore(); getline(cin, name);
                
                int newBankId = getNextBankAccountId();
                
                // Saldo awal 500.0
                digitalBank.addCustomer(BankCustomer(newBankId, name + " Bank Account", 500.0));
                
                BankCustomer* ptrNewAcc = digitalBank.getCustomerById(newBankId); 

                if (ptrNewAcc) {
                    if (regChoice == 1) { 
                        allBuyers.emplace_back(getNextBuyerId(), name, ptrNewAcc);
                        cout << "Buyer " << name << " berhasil didaftarkan. ID: " << allBuyers.back().getId() << ". Saldo awal $500.00" << endl;
                    } else if (regChoice == 2) { 
                        allSellers.emplace_back(getNextSellerId(), name, ptrNewAcc);
                        cout << "Seller " << name << " berhasil didaftarkan. ID: " << allSellers.back().getId() << ". Saldo awal $500.00" << endl;
                    }
                } else {
                    cout << "Gagal membuat Akun Bank." << endl;
                }
                
                // Panggil refreshAllPointers setelah penambahan data Bank baru
                refreshAllPointers();
                
                break;
            }
            case 4: { // Exit
                cout << "Sistem Toko Online ditutup. Sampai jumpa!" << endl;
                saveAllTransactions(); 
                break;
            }
            case 5: { // Admin Login
                cout << "Username (root): "; string user; cin >> user;
                cout << "Password (toor): "; string pass; cin >> pass;
                if (user == "root" && pass == "toor") {
                    adminMenu();
                } else {
                    cout << "Login Gagal." << endl;
                }
                break;
            }
            default: {
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                break;
            }
        }
    } while (choice != 4);
    
    return 0;
}