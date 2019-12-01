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

    T *allocate(std::size_t n) {
        if(!obj_counter || obj_counter == N){
            obj_counter = 0;
        #if DEBUG
            std::cout <<"[START ALLOC] N: "<< N <<std::endl;
            std::cout<< "[ALLOC] SizeOf N*T: "<< N * n * sizeof(T) <<std::endl;
        #endif
            auto p = std::malloc(n*N*sizeof(T));
            if (!p)
                throw std::bad_alloc();
            ++block_counter;
            block_ptr_stack.push(reinterpret_cast<T*>(p));
            #if DEBUG
                std::cout <<"[INIT ALLOC] Return adrr: "<<std::hex<<block_ptr_stack.top()<<std::dec<<std::endl;
            #endif
            auto t_counter = obj_counter;
            obj_counter+=n;
            return block_ptr_stack.top() + t_counter;
        }
        #if DEBUG
            std::cout <<"[CONTIN ALLOC] Return adrr: "<<std::hex<<block_ptr_stack.top()+obj_counter<<std::dec<<std::endl;
        #endif
        auto t_counter = obj_counter;
        obj_counter += n;
        return block_ptr_stack.top() + t_counter;
    }

    void deallocate([[maybe_unused]]T *p, std::size_t n) {
        #if DEBUG
            std::cout <<"[DEALLOC] dealloc adrr: "<<std::hex<<p<<std::dec<<std::endl;
        #endif
        obj_counter-=n;
        if(!obj_counter){
            #if DEBUG
                std::cout<<"[DEALLOC FREE]\n";
                std::cout <<"Init adrr: "<<std::hex<<block_ptr_stack.top()<<"\naddr to free: "<<std::hex<<p<<std::dec<<std::endl;
            #endif
            obj_counter = N;
            --block_counter;
            if(block_counter==0){
                while(!block_ptr_stack.empty()){
                    std::free(block_ptr_stack.top());
                    block_ptr_stack.pop();
                }
            }
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
    size_t obj_counter = 0;
    size_t block_counter = 0;
    std::stack<T*> block_ptr_stack;
};

