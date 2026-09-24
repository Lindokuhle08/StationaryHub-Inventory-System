#include "DiscountProduct.h"
#include <iostream>

DiscountProduct::DiscountProduct(int id, const std::string& name, double price, double discount)
    : Product(id, name, price), discountRate(discount) {}

double DiscountProduct::calculateFinalPrice(int quantity) const {
    double total = getPrice() * quantity;
    return total - (total * discountRate);
}

void DiscountProduct::display() const {
    Product::display();
    std::cout << ", Discount: " << discountRate * 100 << "%\n";
}
