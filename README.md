# StationaryHub: Multi-Threaded Real-Time Inventory & Order Processing System

**Academic Group Project**
**Tools:** C++, STL (Standard Template Library), Multithreading (`std::thread`, `std::mutex`)

## Project Overview

StationaryHub is a high-performance, multi-warehouse inventory management system developed in C++ to coordinate stock across **nine regional warehouses representing South Africa's nine provinces**. The system manages stationery products (calculators, pens, pencils, exercise books) and processes high-volume customer orders using a concurrent execution model, while ensuring inventory data remains accurate and consistent under simultaneous access.

The project's three primary objectives were:
1. Apply advanced Object-Oriented Programming principles through a polymorphic product hierarchy that handles complex pricing logic (tax, discounts, bulk rates).
2. Leverage STL containers and algorithms for safe, efficient inventory management.
3. Implement multithreading to process large volumes of simultaneous customer orders without data corruption or race conditions.

The system also demonstrates modern C++ memory management by using **smart pointers exclusively**, eliminating the risks of manual memory handling.

## System Architecture

StationaryHub is structured into six layers, with communication flowing downward — the CLI calls the manager, which delegates to the warehouses, which own the products.

1. **User Interface (CLI / `main.cpp`)** -A menu-driven command-line interface through which staff manage warehouses, products, and order processing, displaying thread IDs, order results, and stock levels in a structured format.
2. **Inventory Management System (IMS)** -The central controller, managing all nine warehouse objects, routing requests to the correct warehouse, and ensuring thread-safe operations throughout.
3. **Warehouse** -Represents a physical storage unit in a specific province. Each warehouse maintains its own product collection (`std::map<int, shared_ptr<Product>>`) and is protected by its own `std::mutex`.
4. **Product Classes** -A polymorphic hierarchy where `Product` is an abstract base class, with `TaxableProduct`, `DiscountProduct`, and `BulkProduct` each overriding pricing logic.
5. **Order Processor** -The concurrency layer, where each order runs in its own `std::thread`, validating stock, deducting quantities, and recording SUCCESS/FAILED outcomes.
6. **Shared Data Protection** -Two mutexes (`inventoryMutex`, `consoleMutex`) prevent race conditions on stock data and garbled console output respectively.

### Data Flow

1. A staff member interacts with the CLI, which forwards input to the IMS.
2. The IMS validates input and routes it: warehouse operations, the relevant `Warehouse` object; product operations → the warehouse's product map; order processing → the concurrency layer.
3. Each `Warehouse` searches its `std::map` using `find()` to locate products by ID, locking `inventoryMutex` before any stock modification.
4. For order processing, the system spawns a separate `std::thread` per order (up to 20 concurrent orders), each independently locking the target warehouse's mutex, validating stock, and recording the result.
5. `inventoryMutex` protects stock during concurrent updates; `consoleMutex` prevents interleaved console output across threads.
6. Once all threads complete, results are aggregated and displayed via the CLI, showing thread ID, product details, warehouse location, and order status.

## Class Design & OOP Structure

- **`Product` (Abstract Base Class):** Defines shared attributes (`productID`, `name`, `basePrice`) and declares `calculateFinalPrice()` as pure virtual.
- **`TaxableProduct`:** Overrides pricing to apply `price × quantity × (1 + taxRate)`.
- **`DiscountProduct`:** Overrides pricing to subtract a discount percentage.
- **`BulkProduct`:** Applies a bulk discount only when quantity meets or exceeds a defined threshold.
- **`Warehouse`:** Manages localized inventory for a specific province, with thread-safe access via its own mutex.
- **`InventoryManagementSystem` (IMS):** Central orchestrator coordinating all warehouses.
- **`Order` / `OrderItem`:** Represent a customer order and its individual product line items, processed concurrently by worker threads.

**Why virtual functions matter here:** without them, the compiler would resolve `calculateFinalPrice()` at compile time based on the pointer type (`shared_ptr<Product>`) rather than the actual object type — meaning every product would incorrectly use the base class's pricing logic regardless of its real type. Virtual functions let a single loop iterate through a `std::map` of mixed product types and automatically invoke each one's correct, specialized pricing behaviour — this also makes the system easily extensible, since new product types can be added later without changing existing code.

## Data Structures & STL Usage

- **`std::vector<shared_ptr<Warehouse>>`**- stores the fixed collection of 9 warehouses, offering simple indexing and sequential access.
- **`std::map<int, shared_ptr<Product>>`** -stores products within each warehouse, keyed by unique product ID, enabling O(log n) lookups and guaranteeing no duplicate IDs.
- **`std::find_if`** - used to locate a warehouse by ID.
- **`std::map::find`** -used to locate a product within a warehouse's map, far faster than scanning linearly.
- **`std::sort`** -used to sort products by price or quantity for organized display.

**Safety & performance benefits:** STL containers automatically manage memory and prevent unsafe pointer arithmetic; `shared_ptr` enforces proper ownership so objects are destroyed only when the last reference is released; no manual `new`/`delete` means no memory leaks or double-free bugs; `std::map` provides O(log n) lookups, far superior to linear search for large inventories.

## Concurrency Design

- **Threading model:** When "Process All Orders" is selected, the system spawns a separate thread per order and calls `join()` on each, allowing all orders to run simultaneously rather than sequentially — significantly reducing total processing time.
- **Synchronization:** Each warehouse's inventory is protected by its own `inventoryMutex`, locked via `std::lock_guard` before any read/write to stock — this automatically locks and unlocks even if the function exits early or throws an exception.
- **Preventing race conditions:** Stock existence checks, availability checks, and stock reduction all happen inside a single locked section, so no other thread can interleave and corrupt the operation — this guarantees stock never goes negative and always stays accurate.
- **Console output protection:** A separate `consoleMutex` ensures each thread's full output message prints atomically, preventing garbled, interleaved text from multiple threads printing at once.

## Memory Management Strategy

The system uses **smart pointers exclusively** — there are no raw `new`/`delete` statements anywhere in the codebase.

- **`std::shared_ptr<Product>`** -used since products are accessed from multiple parts of the system (warehouse inventory, order processing); the object persists as long as it's in use.
- **`std::shared_ptr<Warehouse>`** - required because warehouses contain a `std::mutex`, which cannot be copied; `shared_ptr` allows safe storage and access without copy issues.
- **`std::shared_ptr<Order>`** - keeps orders valid in memory while their threads are still running.

This design eliminates memory leaks and use-after-free bugs entirely, since objects are automatically destroyed only when the last reference to them goes out of scope.

## User Interface

A menu-driven Command-Line Interface (CLI) supports:
- **Warehouse Operations:** add/remove a warehouse, display all warehouses and their inventories
- **Product Operations:** add a product (Taxable/Discount/Bulk), remove/search by ID, sort by price or quantity
- **Order Processing:** process all orders concurrently (up to 20 threads), display SUCCESS/FAILED outcomes per order

## Performance & Limitations

**Concurrency benefits:** total processing time approximates the time of the single slowest order, rather than the sum of all orders' processing times — since orders run in parallel, and warehouse-level mutexes allow simultaneous access to *different* warehouses without blocking each other.

**Known limitations:**
- Fixed at 9 warehouses -adding more requires modifying `main()` and recompiling
- No database persistence - inventory loads from a text file at startup, but changes made during a session aren't saved
- Synchronous menu - the admin must wait for all orders in a batch to complete before returning to the menu
- No thread pooling - a new thread is created for every order rather than reusing a pool of threads

## Challenges & Solutions

**Challenge 1 - Race conditions in inventory:** Initial attempts to update stock without proper synchronization allowed two threads to read the same stock value simultaneously and both process orders against it, reducing stock only once instead of twice.
**Solution:** Enclosed all stock validation and modification within a single `std::lock_guard` critical section, ensuring only one thread can execute the full validate-then-update sequence at a time.

**Challenge 2 - Interleaved console output:** Multiple threads printing simultaneously produced garbled output, with text from different orders mixed together.
**Solution:** Implemented `consoleMutex` to serialize all output, so each thread's complete message prints atomically before the next thread can print.

**Challenge 3 - Memory management:** Early attempts using raw pointers led to memory leaks and use-after-free bugs when objects were deleted prematurely.
**Solution:** Replaced all raw pointers with smart pointers (`shared_ptr`), ensuring automatic, safe cleanup without manual intervention.

## Files in this Repository

- `Proposal and Report/` - original project proposal and final written report
- `Code Fusion_src/` - full C++ source code (`main.cpp`, `Product.cpp`, `Warehouse.cpp`, `Order.cpp`, `InventoryManagementSystem.cpp`, etc.)
- `README.md` - this project summary
