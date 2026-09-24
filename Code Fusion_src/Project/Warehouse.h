#pragma once
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include "Product.h"

struct StockItem {
    std::shared_ptr<Product> product;
    int quantity;
};

class Warehouse {
    int warehouseID;
    std::string location;
    std::map<int, StockItem> inventory;
    mutable std::mutex inventoryMutex;

public:
    Warehouse(int id, const std::string& location);

    int getID() const;
    std::string getLocation() const;

    void addProduct(std::shared_ptr<Product> product, int quantity);
    void removeProduct(int productID);
    bool decreaseStock(int productID, int quantity);
    void increaseStock(int productID, int quantity);
    void displayAllProducts() const;
    void searchProduct(int productID) const;
    void sortAndDisplay(int sortBy) const; //sort by , price:1 quantity:2
};
