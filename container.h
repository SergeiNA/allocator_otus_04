#pragma once

#include <memory>

template <class T, class Alloc = std::allocator<T> >
class CustomContaiter{
    struct Node {
        template<typename... Args>
        Node (Args&&... val):value(std::forward<Args>(val)...), next(nullptr) {}
        T value;
        Node* next;
    };

    using allocator = typename Alloc::template rebind<Node>::other;
    allocator alloc;
private:
    class ListIterator_
    {
    public:
        ListIterator_() : ptr{nullptr} {}
        ListIterator_(Node *ptr_node) : ptr{ptr_node} {}

        ListIterator_ operator++(int){
            if (ptr == nullptr)
                throw std::runtime_error("Ivalid iterator");
            Node *temp = ptr;
            ptr = ptr->next;
            return ListIterator_(temp);
        }

        ListIterator_ operator++(){
            if (ptr == nullptr)
                throw std::runtime_error("Ivalid iterator");
            ptr = ptr->next;
            return ListIterator_(ptr);
        }
        Node &operator*(){
            return *ptr;
        }
        bool operator==(const ListIterator_ &rhs) const{
            return ptr == rhs.ptr;
        }
        bool operator!=(const ListIterator_ &rhs) const{
            return !(*this == rhs);
        }

    private:
        Node *ptr;
    };

public:
    typedef ListIterator_ Iterator;

public:
    CustomContaiter():head{nullptr}, tail{nullptr} {}

    void push(const T& elem){
        if(head == tail && tail == nullptr){
            tail = alloc.allocate(1);
            alloc.construct(tail,elem);
            head = tail;
            return;
        }
        Node* temp = alloc.allocate(1);
        alloc.construct(temp,elem);
        tail->next = temp;
        tail = temp;
    }
    
    template<typename... Args>
    void try_push(Args &&... args){
        if(head == tail && tail == nullptr){
            tail = alloc.allocate(1);
            alloc.construct(tail,std::forward<Args>(args)...);
            head = tail;
            return;
        }
        Node* temp = alloc.allocate(1);
        alloc.construct(temp,std::forward<Args>(args)...);
        tail->next = temp;
        tail = temp;
    }

    CustomContaiter(const CustomContaiter<T,Alloc>& rhs):CustomContaiter() {
        #if DEBUG
            std::cout<<__PRETTY_FUNCTION__ <<std::endl;
        #endif
        Node* curr = rhs.head;
        while(curr!=nullptr){
            push(curr->value);
            curr=curr->next;
        }
    }

    CustomContaiter(CustomContaiter<T,Alloc>&& rhs):CustomContaiter(){
        #if DEBUG
            std::cout<<__PRETTY_FUNCTION__ <<std::endl;
        #endif
        std::swap(alloc,rhs.alloc);
        std::swap(head,rhs.head);
        std::swap(tail,rhs.tail);
    }

    template <class OtherAlloc>
    CustomContaiter(const CustomContaiter<T,OtherAlloc>& rhs):CustomContaiter(){
        #if DEBUG
            std::cout<<__PRETTY_FUNCTION__ <<std::endl;
        #endif
        for(const auto& elem:rhs)
            push(elem.value);
    }

    template <class OtherAlloc>
    CustomContaiter(CustomContaiter<T,OtherAlloc>&& rhs):CustomContaiter(){
        #if DEBUG
            std::cout<<__PRETTY_FUNCTION__ <<std::endl;
        #endif
         for(auto&& elem:rhs)
            push(std::move(elem.value));
    }

    ~CustomContaiter(){
        while(head!=nullptr){
            Node* curr = head;
            head = head->next;
            alloc.destroy(curr);
            alloc.deallocate(curr,1);
        }
    }

    Iterator begin(){
        return Iterator(head);
    }
    Iterator end(){
        return Iterator(tail->next);
    }

    Iterator begin() const{
        return Iterator(head);
    }
    Iterator end() const{
        return Iterator(tail->next);
    }
private:
    Node* head;
    Node* tail;
};