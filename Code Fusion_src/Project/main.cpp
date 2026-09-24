#include <iostream>
#include <memory>
#include <limits>
#include <vector>
#include "InventoryManagementSystem.h"
#include "DiscountProduct.h"
#include "TaxableProduct.h"
#include "BulkProduct.h"
#include "OrderGenerator.h"

using namespace std;

static void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static void printMenu()
{
    cout << "\n***************************************\n";
    cout << "*   INVENTORY MANAGEMENT SYSTEM MENU  *\n";
    cout << "*****************************************\n";
    cout << "*  --- Warehouse Operations ---       *\n";
    cout << "*  1.  Add warehouse                  *\n";
    cout << "*  2.  Remove warehouse               *\n";
    cout << "*  3.  Display all warehouses         *\n";
    cout << "*  --- Product Operations ---         *\n";
    cout << "*  4.  Add product to warehouse       *\n";
    cout << "*  5.  Remove product from warehouse  *\n";
    cout << "*  6.  Search product in warehouse    *\n";
    cout << "*  7.  Process Orders (Multi-thread)  *\n";
    cout << "*  8.  Display all products           *\n";
    cout << "*  9.  Sort products by price/qty     *\n";
    cout << "*  0.  Exit                           *\n";
    cout << "*****************************************\n";
    cout << "Choice: ";
}

// Warehouse/Product Handlers
static void handleAddWarehouse(InventoryManagementSystem& system)
{
    int id; string location;
    cout << "Warehouse ID: "; cin >> id; clearInput();
    cout << "Location: "; getline(cin, location);
    system.addWarehouse(make_shared<Warehouse>(id, location));
    cout << "Warehouse [" << location << "] added.\n";
}

static void handleRemoveWarehouse(InventoryManagementSystem& system)
{
    int id; cout << "Warehouse ID to remove: "; cin >> id;
    system.removeWarehouse(id);
}

static void handleAddProduct(InventoryManagementSystem& system)
{
    int warehouseID;
    cout << "Warehouse ID: "; cin >> warehouseID;
    auto wh = system.findWarehouse(warehouseID);
    if (!wh) {
        cout << "Warehouse not found.\n";
        return;
    }

    cout << "Product type:\n 1.Taxable 2.Discount 3.Bulk\nChoice: ";
    int type; cin >> type;

    int pid, qty; double price, rate; string name;
    cout << "Product ID: "; cin >> pid; clearInput();
    cout << "Name: "; getline(cin, name);
    cout << "Base Price: "; cin >> price;
    cout << "Stock Qty: "; cin >> qty;

    shared_ptr<Product> product;
    if (type == 1) {
        cout << "Tax Rate: "; cin >> rate;
        product = make_shared<TaxableProduct>(pid,name,price,rate);
    }
    else if (type == 2) {
            cout << "Discount Rate: ";
            cin >> rate;
            product = make_shared<DiscountProduct>(pid,name,price,rate);
    }
    else if (type == 3) {
            int threshold;
            cout << "Bulk Threshold: ";
            cin >> threshold;
            cout << "Bulk Rate: ";
            cin >> rate; product = make_shared<BulkProduct>(pid,name,price,threshold,rate);
    }
    else { cout << "Invalid type.\n"; return; }

    wh->addProduct(product, qty);
    cout << "Product '" << name << "' added.\n";
}

static void handleRemoveProduct(InventoryManagementSystem& system)
{
    int warehouseID, productID;
    cout << "Warehouse ID: "; cin >> warehouseID;
    auto wh = system.findWarehouse(warehouseID);
    if (!wh) { cout << "Warehouse not found.\n"; return; }

    cout << "Product ID: "; cin >> productID;
    wh->removeProduct(productID);
    cout << "Product " << productID << " removed.\n";
}

static void handleSearchProduct(InventoryManagementSystem& system)
{
    int warehouseID, productID;
    cout << "Warehouse ID: "; cin >> warehouseID;
    auto wh = system.findWarehouse(warehouseID);
    if (!wh) { cout << "Warehouse not found.\n"; return; }

    cout << "Product ID: "; cin >> productID;
    wh->searchProduct(productID);
}

static void handleProcessOrders(InventoryManagementSystem& system)
{
    string orderFile = "orders.txt";

    cout << "Processing orders from file: " << orderFile << "\n";

    vector<Order> orders = OrderGenerator::loadOrders(orderFile);
    if (orders.empty()) {
        cout << "No orders to process or file not found.\n";
        return;
    }

    system.processOrders(orders);
    cout << "Orders processed successfully!\n";
}

static void handleDisplayProducts(InventoryManagementSystem& system)
{
    int warehouseID;
    cout << "Warehouse ID: "; cin >> warehouseID;
    auto wh = system.findWarehouse(warehouseID);
    if (!wh) { cout << "Warehouse not found.\n"; return; }
    wh->displayAllProducts();
}

static void handleSortProducts(InventoryManagementSystem& system)
{
    int warehouseID;
    cout << "Warehouse ID: "; cin >> warehouseID;
    auto wh = system.findWarehouse(warehouseID);
    if (!wh) { cout << "Warehouse not found.\n"; return; }

    cout << "Sort by:\n 1. Price\n 2. Quantity\nChoice: ";
    int sortBy; cin >> sortBy;
    if (sortBy != 1 && sortBy != 2) { cout << "Invalid choice.\n"; return; }
    wh->sortAndDisplay(sortBy);
}

// main
int main()
{
    cout << "****STATIONARYHUB**********\n";

    InventoryManagementSystem system;

    // Add some initial warehouses
    vector<shared_ptr<Warehouse>> warehouses = {
       make_shared<Warehouse>(1,"Gauteng"),
        make_shared<Warehouse>(2,"Western Cape"),
        make_shared<Warehouse>(3,"KwaZulu-Natal"),
        make_shared<Warehouse>(4,"Eastern Cape"),
        make_shared<Warehouse>(5,"Free State"),
        make_shared<Warehouse>(6,"Limpopo"),
        make_shared<Warehouse>(7,"Mpumalanga"),
        make_shared<Warehouse>(8,"North West"),
        make_shared<Warehouse>(9,"Northern Cape")
    };
    for(auto& w: warehouses) system.addWarehouse(w);

    // Warehouse 1 - Gauteng
    warehouses[0]->addProduct(make_shared<TaxableProduct>(101,"Laptop",10000,0.15),50);
    warehouses[0]->addProduct(make_shared<BulkProduct>(103,"Mouse",200,10,0.20),150);
    warehouses[0]->addProduct(make_shared<DiscountProduct>(102,"Headphones",1500,0.10),80);

    // Warehouse 2 - Western Cape
    warehouses[1]->addProduct(make_shared<TaxableProduct>(104,"Keyboard",500,0.15),60);
    warehouses[1]->addProduct(make_shared<DiscountProduct>(105,"Monitor",3000,0.08),40);
    warehouses[1]->addProduct(make_shared<BulkProduct>(106,"USB Cable",50,20,0.15),200);

    // Warehouse 3 - KwaZulu-Natal
    warehouses[2]->addProduct(make_shared<TaxableProduct>(107,"Tablet",7000,0.12),25);
    warehouses[2]->addProduct(make_shared<DiscountProduct>(108,"Webcam",800,0.10),55);
    warehouses[2]->addProduct(make_shared<BulkProduct>(109,"Notebook",30,50,0.25),300);

    // Warehouse 4 - Eastern Cape
    warehouses[3]->addProduct(make_shared<TaxableProduct>(110,"Printer",4000,0.18),20);
    warehouses[3]->addProduct(make_shared<DiscountProduct>(111,"Desk Lamp",350,0.12),70);

    // Warehouse 5 - Free State
    warehouses[4]->addProduct(make_shared<BulkProduct>(112,"Pen",5,100,0.30),1000);
    warehouses[4]->addProduct(make_shared<TaxableProduct>(113,"Calculator",250,0.10),90);

    // Warehouse 6 - Limpopo
    warehouses[5]->addProduct(make_shared<DiscountProduct>(114,"Stapler",120,0.05),45);

    // Warehouse 7 - Mpumalanga
    warehouses[6]->addProduct(make_shared<BulkProduct>(115,"Folder",25,30,0.20),400);

    // Warehouse 8 - North
    warehouses[7]->addProduct(make_shared<BulkProduct>(103,"Mouse",200,10,0.20),100);
    warehouses[7]->addProduct(make_shared<TaxableProduct>(101,"Laptop",10000,0.15),30);
    warehouses[7]->addProduct(make_shared<DiscountProduct>(102,"Headphones",1500,0.10),50);

    // Warehouse 9 - Northern Cape (restock)
    warehouses[8]->addProduct(make_shared<TaxableProduct>(107,"Tablet",7000,0.12),15);
    warehouses[8]->addProduct(make_shared<BulkProduct>(109,"Notebook",30,50,0.25),150);
    warehouses[8]->addProduct(make_shared<DiscountProduct>(105,"Monitor",3000,0.08),20);

    int choice = -1;
    while(choice != 0)
    {
        printMenu();
        cin >> choice;
        switch(choice)
        {
            case 1: handleAddWarehouse(system); break;
            case 2: handleRemoveWarehouse(system); break;
            case 3: system.displayAllWarehouses(); break;
            case 4: handleAddProduct(system); break;
            case 5: handleRemoveProduct(system); break;
            case 6: handleSearchProduct(system); break;
            case 7: handleProcessOrders(system); break;
            case 8: handleDisplayProducts(system); break;
            case 9: handleSortProducts(system); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n"; break;
        }
    }

    return 0;
}
