#include "Product.h"
#include <iostream>
using namespace std;

void Product::display() const {
    cout << "ID: " << productID
              << " | Name: " << name
              << " | Price: " << basePrice<< "\n";
}
