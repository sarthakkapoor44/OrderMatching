#include<iostream>
#include "/Users/apple/Desktop/orderMatching/include/orderMatching.h"

int main(){
    int queries;
    Matching engine;
    std::cin>>queries;
    while(queries--){
        std::string s ;
        std::cin>>s;
        if(s == "QUERY"){
            auto [bid,ask] = engine.Query();
            std::cout<<bid<<" "<<ask<<std::endl;
        }else if (s == "PLACE"){
            std::string orderType;
            int orderId,price,qty;
            std::cin>>orderId>>orderType>>price>>qty;
            engine.PlaceOrder(orderId,strToOrderType(orderType),price,qty);
        } else if (s == "CANCEL"){
            int orderId;
            std::cin>>orderId;
            engine.CancelOrder(orderId);
        }
    }
}