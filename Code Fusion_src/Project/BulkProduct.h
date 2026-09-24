#pragma once
#include "Product.h"

class BulkProduct : public Product {
    int threshold;
    double bulkDiscount;

public:
    BulkProduct(int id, const std::string& name, double price, int thresholdQty, double discount);
    double calculateFinalPrice(int quantity) const override;
    void display() const override;
};
