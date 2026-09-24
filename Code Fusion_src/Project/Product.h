#pragma once
#include <string>
using namespace std;

class Product {
protected:
    int productID;
    string name;
    double basePrice;
public:
    Product(int id, const string& name, double price)
        : productID(id), name(name), basePrice(price) {}
    virtual ~Product() = default;

    int getID() const { return productID; }
    string getName() const { return name; }
    double getPrice() const { return basePrice; }

    virtual double calculateFinalPrice(int quantity) const = 0;
    virtual void display() const;
};
