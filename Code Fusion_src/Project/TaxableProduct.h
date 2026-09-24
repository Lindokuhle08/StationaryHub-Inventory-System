// TaxableProduct.h
#pragma once
#include "Product.h"

class TaxableProduct : public Product {
    double taxRate;
public:
    TaxableProduct(int id, const std::string& name, double price, double tax);
    double calculateFinalPrice(int quantity) const override;
    void display() const override;
};
