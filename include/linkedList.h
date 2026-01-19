#pragma once
#include<cstddef>

template<typename T>
struct Node {
    Node<T>* next;
    Node<T>* prev;
    T val;
    Node():next(nullptr),prev(nullptr){}
    Node(Node<T>* next,Node<T>* prev,const T& value):next(next),prev(prev),val(value){}
};

template<typename T>
class LinkedList
{
    private: 
        Node<T>* head;
        Node<T>* tail; 
        size_t sz ;
    public:
        LinkedList():head(nullptr),tail(nullptr),sz(0){}

        void clear(){
            while(tail != nullptr){
                Node<T>* ptr2 = tail;
                tail = tail->prev;
                sz--;
                delete ptr2;
            }
            head = nullptr;
            tail = nullptr;
            sz =0 ;
        }

        LinkedList(const LinkedList& other):head(nullptr),tail(nullptr),sz(0){
            Node<T>* current = other.head;
            while(current != nullptr){
                if(head == nullptr){
                    head = new Node<T>(nullptr,nullptr,current->val);
                    tail = head;
                } else{
                    Node<T>* newNode = new Node<T>(nullptr,tail,current->val);
                    tail->next = newNode;
                    tail= newNode;
                }
                sz++;
                current = current->next;
            }
        }

        LinkedList& operator=(const LinkedList& other){
            if(this != &other){
                // delete this first
                this->clear();
                Node<T>* current = other.head;
                while(current != nullptr){
                    if(head == nullptr){
                        head = new Node<T>(nullptr,nullptr,current->val);
                        tail = head;
                    } else{
                        Node<T>* newNode = new Node<T>(nullptr,tail,current->val);
                        tail->next = newNode;
                        tail= newNode;
                    }
                    sz++;
                    current = current->next;
                }
            }
            return *this;
        }

        Node<T>* getHeadPtr() const{
            return head;
        }

        Node<T>* getTailPtr() const{
            return tail;
        }

        void insertAtEnd(const T& value){
            Node<T>* NewNode = new Node<T>(nullptr,tail,value);
            if(sz){
                tail->next = NewNode;
                tail = NewNode;
            } else{
               head = NewNode;
               tail = head;
            }
            sz++;
        }
        void remove(Node<T>* ptr){
            if(ptr!=nullptr){
                Node<T>* prev = ptr->prev;
                Node<T>* next = ptr->next;
                
                if(prev != nullptr){
                    prev->next = next;
                }
                else{
                    head = next;
                }

                if(next != nullptr){
                    next->prev = prev;
                } else{
                    tail = prev;
                }

                delete ptr;
                sz--;
            }
        }

        size_t size() const{
            return sz;
        }

        ~LinkedList(){
            this->clear();
        }     
};