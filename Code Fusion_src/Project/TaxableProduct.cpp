// TaxableProduct.cpp
#include "TaxableProduct.h"
#include <iostream>
using namespace std;

TaxableProduct::TaxableProduct(int id, const string& name, double price, double tax)
    : Product(id, name, price), taxRate(tax) {}

double TaxableProduct::calculateFinalPrice(int quantity) const {
    double total = basePrice * quantity;
    return total + (total * taxRate);
}

void TaxableProduct::display() const {
    Product::display();
    cout << ", Tax: " << taxRate * 100 << "%\n";
}
