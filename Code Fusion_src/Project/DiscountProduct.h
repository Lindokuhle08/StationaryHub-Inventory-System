#pragma once
#include "Product.h"

class DiscountProduct : public Product {
    double discountRate;

public:
    DiscountProduct(int id, const std::string& name, double price, double discount);
    double calculateFinalPrice(int quantity) const override;
    void display() const override;
};
