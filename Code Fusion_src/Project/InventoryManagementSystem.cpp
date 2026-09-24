#include "InventoryManagementSystem.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <functional>

using namespace std;
void InventoryManagementSystem::addWarehouse(std::shared_ptr<Warehouse> warehouse) {
    lock_guard<mutex> lock(systemMutex_);
    warehouses_.push_back(warehouse);
}

void InventoryManagementSystem::removeWarehouse(int id) {
    lock_guard<std::mutex> lock(systemMutex_);
    auto it = find_if(warehouses_.begin(), warehouses_.end(),
        [id](const shared_ptr<Warehouse>& w){ return w->getID() == id; });
    if (it != warehouses_.end()) {
        cout << "Warehouse [" << (*it)->getLocation() << "] removed.\n";
        warehouses_.erase(it);
    } else {
        cout << "Warehouse ID " << id << " not found.\n";
    }
}

shared_ptr<Warehouse> InventoryManagementSystem::findWarehouse(int id) {
    lock_guard<mutex> lock(systemMutex_);
    for (auto& w : warehouses_)
        if (w->getID() == id) return w;
    return nullptr;
}

void InventoryManagementSystem::displayAllWarehouses() const {
    lock_guard<mutex> lock(systemMutex_);
    cout << "\nAll Warehouses:\n";
    for (const auto& wh : warehouses_) {
        cout << "- " << wh->getLocation() << "\n";
    }
}
void InventoryManagementSystem::processOrders(const std::vector<Order>& orders) {
    vector<thread> threads;

    for (auto& order : orders) {
        threads.emplace_back([this, order]() {
            auto warehouse = findWarehouse(order.getWarehouseID());

            // ── Build the result string first, THEN print atomically ──
            string result = "\nProcessing Order ID: " +
                            to_string(order.getOrderID()) +
                            " | Thread ID: " +
                            to_string(hash<thread::id>{}(this_thread::get_id())) +
                            " at Warehouse ";

            if (!warehouse) {
                lock_guard<mutex> lock(consoleMutex_);
                cerr << "Warehouse " << order.getWarehouseID() << " not found.\n";
                return;
            }

            result += warehouse->getLocation() + "\n";

            for (auto& item : order.getItems()) {
                bool success = warehouse->decreaseStock(
                    item.getProductID(), item.getQuantity());

                if (success)
                    result += "  [SUCCESS] Product ID: " +
                               to_string(item.getProductID()) +
                               " | Qty: " + to_string(item.getQuantity()) + "\n";
                else
                    result += "  [FAILED]  Product ID: " +
                               to_string(item.getProductID()) +
                               " | Qty: " + to_string(item.getQuantity()) +
                               " (Insufficient stock or missing)\n";
            }

            // ── Single locked print of the entire order result ──
            {
                lock_guard<mutex> lock(consoleMutex_);
                cout << result;
            }
        });
    }

    for (auto& t : threads)
        if (t.joinable()) t.join();
}
