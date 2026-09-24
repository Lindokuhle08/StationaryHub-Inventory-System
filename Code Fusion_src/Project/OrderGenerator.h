#pragma once
#include <vector>
#include <string>
#include "Order.h"
using namespace std;

class OrderGenerator {
public:
    static vector<Order> loadOrders(const string& filename);
};
