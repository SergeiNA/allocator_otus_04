#pragma once

#include <utility>

class easy {
    int i;
    double d;
public:
    easy(int i, double d) : i(i), d(d) { 
        #if DEBUG
            std::cout<<  "[CTOR]: " << __PRETTY_FUNCTION__ << std::endl; 
        #endif
        };
        
    easy(const easy &) = default;

    easy(easy &&) noexcept = default;

    std::pair<int,double> getValues() const{
        return std::make_pair(i,d);
    }

    ~easy() {
        #if DEBUG
            std::cout<<  "[DESTR]: " << __PRETTY_FUNCTION__ << std::endl; 
        #endif
        };
};

