#pragma once
#include <utility>

class hard {
    int i;
    double d;
public:
    hard(int i, double d) : i(i), d(d) { 
        #if DEBUG
            std::cout<<  "[CTOR]: " << __PRETTY_FUNCTION__ << std::endl; 
        #endif
        };
        
    hard(const hard &) = delete;

    hard(hard &&) noexcept = delete;

    std::pair<int,double> getValues() const{
        return std::make_pair(i,d);
    }

    ~hard() {
        #if DEBUG
            std::cout<<  "[DESTR]: " << __PRETTY_FUNCTION__ << std::endl; 
        #endif
        };
};

