#include "BulkProduct.h"
#include <iostream>

BulkProduct::BulkProduct(int id, const std::string& name, double price, int thresholdQty, double discount)
    : Product(id, name, price), threshold(thresholdQty), bulkDiscount(discount) {}

double BulkProduct::calculateFinalPrice(int quantity) const {
    double total = getPrice() * quantity;
    if (quantity >= threshold)
        return total - (total * bulkDiscount);
    return total;
}

void BulkProduct::display() const {
    Product::display();
    std::cout << ", Bulk Discount: " << bulkDiscount * 100
              << "% (Min Qty: " << threshold << ")\n";
}
