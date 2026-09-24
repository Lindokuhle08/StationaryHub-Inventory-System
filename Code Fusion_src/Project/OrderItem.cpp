#include "OrderItem.h"

OrderItem::OrderItem(int productID, int quantity)
    : productID_(productID), quantity_(quantity) {}

int OrderItem::getProductID() const {
    return productID_;
}

int OrderItem::getQuantity() const {
    return quantity_;
}
void OrderItem::setQuantity(int quantity) {
    quantity_ = quantity;
}
