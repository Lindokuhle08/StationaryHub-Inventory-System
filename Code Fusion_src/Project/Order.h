#pragma once
#include <vector>
#include "OrderItem.h"
using namespace std;

class Order {
    int orderID_;
    int warehouseID_;
    vector<OrderItem> items_;

public:
    Order(int orderID, int warehouseID);

    void addItem(const OrderItem& item);

    int getOrderID() const;
    int getWarehouseID() const;
    const vector<OrderItem>& getItems() const;

    void displayOrder() const;
};
