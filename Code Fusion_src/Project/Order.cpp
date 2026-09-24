#include "Order.h"
#include <iostream>
using namespace std;

Order::Order(int orderID, int warehouseID)
    : orderID_(orderID), warehouseID_(warehouseID) {}

void Order::addItem(const OrderItem& item) { items_.push_back(item); }

int Order::getOrderID() const { return orderID_; }
int Order::getWarehouseID() const { return warehouseID_; }
const vector<OrderItem>& Order::getItems() const { return items_; }

void Order::displayOrder() const {
    cout << "\nOrder ID: " << orderID_
              << " | Warehouse ID: " << warehouseID_ << "\n";
    if (items_.empty()) {
        cout << "No items in this order.\n";
        return;
    }
    for (const auto& item : items_)
        cout << "Product ID: " << item.getProductID()
                  << " | Quantity: " << item.getQuantity() << "\n";
}
