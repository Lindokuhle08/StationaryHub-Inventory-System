#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include "Warehouse.h"
#include "Order.h"
using namespace std;

class InventoryManagementSystem {
    vector<shared_ptr<Warehouse>> warehouses_;
    mutable mutex systemMutex_;
    mutable mutex consoleMutex_;

public:
    void addWarehouse(shared_ptr<Warehouse> warehouse);
    void removeWarehouse(int id);
    shared_ptr<Warehouse> findWarehouse(int id);
    void displayAllWarehouses() const;

    void processOrders(const vector<Order>& orders);
};
