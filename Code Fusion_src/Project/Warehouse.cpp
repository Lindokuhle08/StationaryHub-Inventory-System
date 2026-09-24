#include "Warehouse.h"
#include <iostream>
#include <algorithm>
#include <vector>

Warehouse::Warehouse(int id, const std::string& loc)
    : warehouseID(id), location(loc) {}

int Warehouse::getID() const { return warehouseID; }
std::string Warehouse::getLocation() const { return location; }

void Warehouse::addProduct(std::shared_ptr<Product> product, int quantity) {
    std::lock_guard<std::mutex> lock(inventoryMutex);
    int pid = product->getID();
    if (inventory.find(pid) != inventory.end())
        inventory[pid].quantity += quantity;
    else
        inventory[pid] = { product, quantity };
}

void Warehouse::removeProduct(int productID) {
    std::lock_guard<std::mutex> lock(inventoryMutex);
    inventory.erase(productID);
}

//decrease stock
bool Warehouse::decreaseStock(int productID, int quantity) {
    std::lock_guard<std::mutex> lock(inventoryMutex);
    auto it = inventory.find(productID);
    if (it == inventory.end() || it->second.quantity < quantity) return false;
    it->second.quantity -= quantity;
    return true;
}

//increase stock
void Warehouse::increaseStock(int productID, int quantity) {
    std::lock_guard<std::mutex> lock(inventoryMutex);
    auto it = inventory.find(productID);
    if (it != inventory.end())
        it->second.quantity += quantity;
}

//display all products
void Warehouse::displayAllProducts() const {
    std::lock_guard<std::mutex> lock(inventoryMutex);
    std::cout << "\nWarehouse: " << location << "\n";
    for (const auto& pair : inventory)
    {
        pair.second.product->display();
        std::cout << " | Stock: " << pair.second.quantity << "\n";
    }
}
//search product
void Warehouse::searchProduct(int productID) const {
    std::lock_guard<std::mutex> lock(inventoryMutex);
    auto it = inventory.find(productID);
    if (it != inventory.end()) {
        it->second.product->display();
        std::cout << " | Stock: " << it->second.quantity << "\n";
    } else {
        std::cout << "Product not found.\n";
    }
}

//sorting items
void Warehouse::sortAndDisplay(int sortBy) const {
    lock_guard<mutex> lock(inventoryMutex);

    // Copy inventory into a vector
    vector<pair<int, StockItem>> items(inventory.begin(), inventory.end());

    if (sortBy == 1) {
        // Sort by base price ascending
        sort(items.begin(), items.end(),
            [](const pair<int, StockItem>& a, const pair<int, StockItem>& b) {
                return a.second.product->getPrice() < b.second.product->getPrice();
            });
        cout << "\nProducts in [" << location << "] sorted by price:\n";
    } else {
        // Sort by quantity ascending
        sort(items.begin(), items.end(),
            [](const pair<int, StockItem>& a, const pair<int, StockItem>& b) {
                return a.second.quantity < b.second.quantity;
            });
        cout << "\nProducts in [" << location << "] sorted by quantity:\n";
    }

    for (const auto& entry : items) {
        entry.second.product->display();
        cout << "   Stock: " << entry.second.quantity << "\n";
    }
}
