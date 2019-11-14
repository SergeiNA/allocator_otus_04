#pragma once
#include <utility>
#include <ostream>

template <class T, class U>
std::ostream& operator << (std::ostream& os, std::pair<T,U> p){
    return os<<p.first<<' '<<p.second;
}