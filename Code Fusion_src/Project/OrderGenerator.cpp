#include "OrderGenerator.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

vector<Order> OrderGenerator::loadOrders(const string& filename) {
    vector<Order> orders;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open orders file: " << filename << "\n";
        return orders;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        int orderID, warehouseID;
        iss >> orderID >> warehouseID;

        Order order(orderID, warehouseID);
        int productID, quantity;
        while (iss >> productID >> quantity) {
            order.addItem(OrderItem(productID, quantity));
        }
        orders.push_back(order);
    }

    return orders;
}
