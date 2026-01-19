#pragma once
#include "linkedList.h"
#include<string>
#include<map>

enum ORDER_TYPE{
    BUY,SELL
};

struct QueryResult{
    int bid,ask;
    QueryResult();
    QueryResult(int,int);
};

struct Order{
    int orderId;
    int price;
    int quantity;
    ORDER_TYPE type;
    Order();
    Order(int,ORDER_TYPE,int,int);
};

class Book{
    public:
        std::map<int,LinkedList<Order> > QueueMap;
        std::unordered_map<int,Node<Order>*> IdToNode;
        void insert(int,Order&);
        void remove(int,Node<Order>*);
        void executeBuyOrder(Order&,Book&);
        void executeSellOrder(Order&,Book&);
        void handleCancellation(int);
        size_t size();
};


class Matching{
    private:
        Book BuyBook,SellBook;
    public:
        void PlaceOrder(int,ORDER_TYPE,int ,int);
        void CancelOrder(int);
        QueryResult Query() ;
};

ORDER_TYPE strToOrderType(std::string);