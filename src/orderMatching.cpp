#include<iostream>
#include<string>
#include "/Users/apple/Desktop/orderMatching/include/orderMatching.h"


// Order
Order::Order():orderId(0),price(0),quantity(0),type(ORDER_TYPE::BUY){}
Order::Order(int orderId,ORDER_TYPE type,int price,int qty):orderId(orderId),price(price),quantity(qty),type(type){}

// QueryResult
QueryResult::QueryResult():bid(-1),ask(-1){}
QueryResult::QueryResult(int b,int a):bid(b),ask(a){}

// Conversion
ORDER_TYPE strToOrderType(std::string str){
    // std::cout<<str<<std::endl;
    if(str == "BUY"){
        return ORDER_TYPE::BUY;
    } 
    return ORDER_TYPE::SELL;
}

// Matching Engine
void Matching::PlaceOrder(int orderId,ORDER_TYPE type,int price,int quantity){
    Order order(orderId,type,price,quantity);
    // std::cout<<order.orderId<<" "<<order.price<<" "<<order.quantity<<" "<<order.type<<std::endl; 
    if(type == ORDER_TYPE::BUY){
        BuyBook.executeBuyOrder(order,SellBook);
    } else{
        SellBook.executeSellOrder(order,BuyBook);
    }
}
void Matching::CancelOrder(int orderId){
    BuyBook.handleCancellation(orderId);
    SellBook.handleCancellation(orderId);
    return;
}

QueryResult Matching::Query(){
    int bid = -1,ask = -1;
    if(BuyBook.size()){
        // std::cout<<"Buy book not empty"<<std::endl;
        bid = (--(BuyBook.QueueMap.end()))->first;
    }
    if(SellBook.size()){
        // std::cout<<"Sell book not empty"<<std::endl;
        ask = ((SellBook.QueueMap.begin()))->first;
    }
    return QueryResult(bid,ask);
}

// Book 
void Book::insert(int key,Order& order){
    QueueMap[key].insertAtEnd(order);
    IdToNode[order.orderId] = QueueMap[key].getTailPtr();
}
void Book::remove(int key,Node<Order>* ptr){
    IdToNode.erase((ptr->val).orderId);
    QueueMap[key].remove(ptr);
    if(QueueMap[key].size() == 0){
        QueueMap.erase(key);
    }
}
void Book::handleCancellation(int orderId){
    auto it = IdToNode.find(orderId);
     if(it!=IdToNode.end()){
        Node<Order>* ptr = it->second;
        if(ptr!=nullptr){
            this->remove((ptr->val).price,ptr);
        }
    }
    return;
}

void Book::executeBuyOrder(Order& order,Book& SellBook) {
    int qtyLeft = order.quantity;
    while(SellBook.size() && qtyLeft){
        auto itr = SellBook.QueueMap.begin();
        if(itr->first > order.price)break;
        Node<Order>* head = (itr->second).getHeadPtr();
        while(head!=nullptr && qtyLeft){
            Node<Order>* curr = head;
            if((curr->val).quantity > qtyLeft){
                (curr->val).quantity -= qtyLeft;
                std::cout<<"Satisfied "<<qtyLeft<<" of ORDER "<<order.orderId<<" using ORDER "<<(curr->val).orderId<<std::endl;
                qtyLeft = 0;
            } else{
                qtyLeft -= (curr->val).quantity;
                std::cout<<"Satisfied "<<(curr->val).quantity<<" of ORDER "<<order.orderId<<" using ORDER "<<(curr->val).orderId<<std::endl;
                SellBook.remove(itr->first,curr);
            }
            // if(!qtyLeft)break;
            head = (itr->second).getHeadPtr();
        }
    }
    if(qtyLeft){
        order.quantity = qtyLeft;
        // only umatched quantity enters the order book
        this->insert(order.price,order);
    }
}

void Book::executeSellOrder(Order& order,Book& BuyBook) {
    int qtyLeft = order.quantity;
    while(BuyBook.size() && qtyLeft){
        auto itr = --(BuyBook.QueueMap.end());
        if(itr->first < order.price)break;
        Node<Order>* head = (itr->second).getHeadPtr();
        while(head!=nullptr && qtyLeft){
            Node<Order>* curr = head;
            if((curr->val).quantity > qtyLeft){
                (curr->val).quantity -= qtyLeft;
                std::cout<<"Satisfied "<<qtyLeft<<" of ORDER "<<order.orderId<<" using ORDER "<<(curr->val).orderId<<std::endl;
                qtyLeft = 0;
            } else{
                qtyLeft -= (curr->val).quantity;
                BuyBook.remove(itr->first,curr);
                std::cout<<"Satisfied "<<(curr->val).quantity<<" of ORDER "<<order.orderId<<" using ORDER "<<(curr->val).orderId<<std::endl;
            }
            // if(!qtyLeft)break;
            head = (itr->second).getHeadPtr();
        }
    }
    if(qtyLeft){
        order.quantity = qtyLeft;
        // only umatched quantity enters the order book
        this->insert(order.price,order);
    }
}

size_t Book::size(){
    return QueueMap.size();
}