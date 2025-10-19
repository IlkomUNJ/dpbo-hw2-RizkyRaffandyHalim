#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

using namespace std;

class Item {
private:
    int id;
    string name;
    int quantity;
    double price;
    bool isVisible;

public:
    Item(int id, const string& name, int quantity, double price)
        : id(id), name(name), quantity(quantity), price(price), isVisible(true) {}

    // Getters
    int getId() const { return id; }
    const string& getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    bool getVisibility() const { return isVisible; }

    // Mutators/Actions
    void setVisibility(bool visible) { isVisible = visible; }
    
    void addStock(int count) {
        quantity += count;
    }
    
    bool reduceStock(int count) {
        if (quantity < count) return false;
        quantity -= count;
        return true;
    }
    
    void printInfo() const {
        cout << "| ID: " << id << " | Name: " << name << " | Price: $" << fixed << setprecision(2) << price 
             << " | Stock: " << quantity << " | Visible: " << (isVisible ? "Yes" : "No") << endl;
    }
};

#endif // ITEM_H