#pragma once

#include <stack>

template<typename T, size_t N>
struct block_allocator {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = block_allocator<U,N>;
    };

    T *allocate(std::size_t n) const {
        if(!pos || pos == N){
            pos = 0;
        #if DEBUG
            std::cout <<"[START ALLOC] N: "<< N <<std::endl;
            std::cout<< "[ALLOC] SizeOf N*T: "<< N * n * sizeof(T) <<std::endl;
        #endif
            auto p = std::malloc(N*n * sizeof(T));
            if (!p)
                throw std::bad_alloc();
            stack_ptr.push(reinterpret_cast<T *>(p));
            #if DEBUG
                std::cout <<"[INIT ALLOC] Return adrr: "<<std::hex<<stack_ptr.top()<<std::dec<<std::endl;
            #endif
            return stack_ptr.top()+pos++;
        }
        #if DEBUG
            std::cout <<"[CONTIN ALLOC] Return adrr: "<<std::hex<<stack_ptr.top()+pos<<std::dec<<std::endl;
        #endif
        return stack_ptr.top()+pos++;
    }

    void deallocate([[maybe_unused]]T *p, [[maybe_unused]]std::size_t n) const {
        #if DEBUG
            std::cout <<"[DEALLOC] dealloc adrr: "<<std::hex<<p<<std::dec<<std::endl;
        #endif
        --pos;
        if(!pos){
            #if DEBUG
                std::cout<<"[DEALLOC FREE]\n";
                std::cout <<"Init adrr: "<<std::hex<<stack_ptr.top()<<"\naddr to free: "<<std::hex<<p<<std::dec<<std::endl;
            #endif
            pos = N;
            std::free(stack_ptr.top());
            stack_ptr.pop();
        }
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) const {
        #if DEBUG
            std::cout <<"[CONSTR] constr by adrr: "<<std::hex<<p<<std::dec<<std::endl;
        #endif
        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(T *p) const {
        #if DEBUG
            std::cout <<"[DESRT] destr by adrr: "<<std::hex<<p<<std::dec<<std::endl;
        #endif
        p->~T();
    }
private:
    mutable size_t pos = 0;
    mutable std::stack<T*> stack_ptr;
};

