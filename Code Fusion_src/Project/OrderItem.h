#pragma once

class OrderItem {
    int productID_;
    int quantity_;
public:
    OrderItem(int productID, int quantity);

    int getProductID() const;
    int getQuantity() const;
    void setQuantity(int quantity);
};
