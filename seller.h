#ifndef SELLER_H
#define SELLER_H

#include <vector>
#include "buyer.h" // Inheritance atau Composition (saya pilih Composition dengan Buyer sbg parent)
#include "item.h" 

using namespace std;

// Class Seller sebagai turunan dari Buyer (untuk memudahkan akses ID/Name/Account)
class Seller : public Buyer {
private:
    vector<Item> stockItems;

public:
    // Constructor memanggil constructor Buyer
    Seller(int id, const string& name, BankCustomer* account)
        : Buyer(id, name, account) {}

    // --- Item Management ---
    void addItem(const Item& newItem) {
        stockItems.push_back(newItem);
    }
    
    Item* getItemById(int itemId) {
        for (auto& item : stockItems) {
            if (item.getId() == itemId) {
                return &item;
            }
        }
        return nullptr;
    }

    const vector<Item>& getAllItems() const {
        return stockItems;
    }
};

#endif // SELLER_H