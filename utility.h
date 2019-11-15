#pragma once
#include <utility>
#include <ostream>
#include <vector>

template <class T, class U>
std::ostream& operator << (std::ostream& os, std::pair<T,U> p){
    return os<<p.first<<' '<<p.second;
}

template<size_t N>
size_t fib(){
    return fib<N-2>()+fib<N-1>();
}

template<>
size_t fib<0>(){return 1;}

template<>
size_t fib<1>(){return 1;}

template<size_t N>
size_t fact(){
    return N*fact<N-1>();
}

template<>
size_t fact<0>(){
    return 1;
}

template <size_t N>
void formFiFa(std::vector<std::pair<size_t,size_t>>& arr){
    arr[N-1].first = fib<N-1>();
    arr[N-1].second = fact<N-1>();
    formFiFa<N-1>(arr);
}

template<>
void formFiFa<0>(std::vector<std::pair<size_t,size_t>>& arr){
    arr[0].first = fib<0>();
    arr[0].second = fact<0>();
}


