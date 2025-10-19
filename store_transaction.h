#ifndef STORE_TRANSACTION_H
#define STORE_TRANSACTION_H

#include "utils.h"
#include <string>
#include <sstream> 
#include <iomanip> 

using namespace std;

class StoreTransaction : public Transaction {
private:
    int transactionId;
    int buyerId;
    int sellerId;
    int itemId;
    int quantity;
    Status status;

public:
    StoreTransaction(int id, int bId, int sId, int iId, int qty, double totalAmount, Status stat)
        : Transaction(totalAmount), transactionId(id), buyerId(bId), sellerId(sId), itemId(iId), quantity(qty), status(stat) {}

    // Helper constructor untuk LOADING dari file
    StoreTransaction(time_t t, int id, int bId, int sId, int iId, int qty, double totalAmount, Status stat)
        : Transaction(totalAmount), transactionId(id), buyerId(bId), sellerId(sId), itemId(iId), quantity(qty), status(stat) {
        // [MODIFIED]: Overwrite the timestamp set by Transaction constructor
        this->timestamp = t;
    }
    
    // Fungsi untuk serialisasi ke baris file
    string toString() const {
        stringstream ss;
        ss << "[STORE]," << timestamp << "," << transactionId << "," << buyerId 
           << "," << sellerId << "," << itemId << "," << quantity << "," 
           << fixed << setprecision(2) << amount << "," << (int)status;
        return ss.str();
    }

    int getId() const { return transactionId; }
    int getBuyerId() const { return buyerId; }
    int getSellerId() const { return sellerId; }
    int getItemId() const { return itemId; }
    int getQuantity() const { return quantity; }
    Status getStatus() const { return status; }
    
    void setStatus(Status newStatus) { status = newStatus; }

    void printInfo() const override {
        string statusStr;
        if (status == PAID) statusStr = "PAID";
        else if (status == COMPLETE) statusStr = "COMPLETE";
        else statusStr = "CANCELED";
        
        cout << "| ID: " << transactionId << " | Date: " << timeToString(timestamp) 
             << " | Buyer: " << buyerId << " | Seller: " << sellerId 
             << " | Item: " << itemId << " | Qty: " << quantity 
             << " | Amount: $" << fixed << setprecision(2) << amount 
             << " | Status: " << statusStr << endl;
    }
};

#endif // STORE_TRANSACTION_H