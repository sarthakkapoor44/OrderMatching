# Toy Order Matching Engine (C++)

A **single-threaded, price–time priority order matching engine** implemented in modern C++.
This project is designed as a **learning and demonstration system** to explore data structures, invariants, and performance trade-offs commonly found in real-world trading systems.

---

## ✨ Features

* **Price–time priority matching**

  * Best price first
  * FIFO matching within the same price level
* **Immediate matching on order placement**
* **Partial and full fills**
* **Order cancellation by order ID**
* **Efficient best bid / best ask queries**
* Clean separation between:

  * Matching logic
  * Order book representation
  * Data structures

---

## 📐 Design Overview

### Order Book Structure

Each side of the book (BUY / SELL) is implemented as:

* `std::map<price, LinkedList<Order>>`

  * Maintains sorted price levels
  * Enables O(1) access to best bid / ask via `begin()` / `--end()`
* `LinkedList<Order>`

  * Preserves FIFO ordering at each price level
  * Provides O(1) insertion and removal
* `unordered_map<orderId, Node*>`

  * Enables O(1) cancellation by order ID

This design ensures:

* No stale price levels
* Deterministic matching behavior
* Efficient cancellation without scanning the book

---

## ⚙️ Matching Semantics

* Orders are matched **immediately** upon placement.
* A BUY order matches against the lowest available SELL prices.
* A SELL order matches against the highest available BUY prices.
* Matching continues until:

  * The incoming order is fully filled, or
  * No compatible price levels remain.
* Any remaining quantity is inserted into the book.
* Cancellations only affect **remaining live quantity**.

---

## 🧵 Threading Model

The matching engine itself is **single-threaded**.

This is intentional:

* Price–time priority requires strict ordering
* Deterministic behavior is critical
* Locking inside the matching path increases latency and complexity

In real systems, parallelism typically exists *around* the matching engine
(IO, parsing, risk checks), not inside it.

---

## 🛠️ Build & Run

### Compile

```bash
g++ -std=c++17 -O2 main.cpp orderMatching.cpp -o matcher
```

### Run

```bash
./matcher
```

Input is read from `stdin` in the following format:

```
PLACE <orderId> <BUY/SELL> <price> <quantity>
CANCEL <orderId>
QUERY
```

---

## 📌 Example

Input:

```
6
PLACE 1 BUY 100 10
PLACE 2 SELL 100 5
QUERY
CANCEL 2
QUERY
PLACE 3 SELL 99 3
```

Output:

```
100 -1
100 -1
```

---

## 📚 Motivation

This project was built to:

* Deepen understanding of low-latency system design
* Practice writing correctness-focused C++
* Explore how real trading engines reason about performance and determinism

---

## 📄 License

This project is released for educational purposes.
Feel free to explore, fork, or adapt it for learning.
