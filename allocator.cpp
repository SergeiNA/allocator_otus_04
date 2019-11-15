
//#define DEBUG

#include <iostream>
#include <vector>
#include <map>
#include "utility.h"
#include "easy.h"
#include "hard.h"
#include "container.h"
#include "block_allocator.h"

int main(int, char *[])
{
    const size_t SIZE = 11;
    std::vector<std::pair<size_t, size_t>> FibFact(SIZE);
    formFiFa<SIZE>(FibFact);

    {

        std::map<int, hard, std::less<int>> map_std;
        for (size_t i = 0; i < SIZE; ++i)
            map_std.try_emplace(i, FibFact[i].first, FibFact[i].second);

        // allocations of std::stack<T*> stack_ptr ;
        // libstdc++.so.6->malloc(64)
        // libstdc++.so.6->malloc(512)

        std::map<int, hard, std::less<int>, block_allocator<std::pair<const int, hard>, 10>> map_block;
        for (size_t i = 0; i < SIZE; ++i)
            map_block.try_emplace(i, FibFact[i].first, FibFact[i].second);

        for (const auto &el : map_block)
            std::cout << el.second.getValues() << std::endl;
    }
#if DEBUG

    std::cout << "Custom container custom alloc ----------------\n";
#endif
    {
        CustomContaiter<hard> std_cont_hard;
        for (size_t i = 0; i < SIZE; ++i)
            std_cont_hard.try_push(FibFact[i].first, FibFact[i].second);

        CustomContaiter<hard, block_allocator<Node<hard>, 10>> block_cont_hard;
        for (size_t i = 0; i < SIZE; ++i)
            block_cont_hard.try_push(FibFact[i].first, FibFact[i].second);

        // // if in public    std::cout<<el.value.i<<' '<<el.value.d<<std::endl; works fine
        for (const auto &el : block_cont_hard)
        {
            std::cout << el.value.getValues() << std::endl;
        }
    }

/// copy ctor move ctor same Alloc
#if DEBUG

    std::cout << "CTOR MOVE HARD ------------------------\n";
#endif

    {
        // move for same with hard
        CustomContaiter<hard, block_allocator<Node<hard>, 3>> init_hard;
        init_hard.try_push(1, 2);
        init_hard.try_push(1, 2);
        init_hard.try_push(1, 2);
        CustomContaiter<hard, block_allocator<Node<hard>, 3>> move_hard(std::move(init_hard));
    }
#if DEBUG

    std::cout << " CTOR COPY/MOVE EASY ------------------------\n";
#endif

    {
        // move copy ctor with easy
        std::cout << "---" << std::endl;
        CustomContaiter<easy, block_allocator<Node<easy>, 5>> init_easy;
        init_easy.try_push(1, 2);
        init_easy.try_push(1, 2);
        init_easy.try_push(1, 2);
        CustomContaiter<easy, block_allocator<Node<easy>, 5>> copy_easy(init_easy);
        CustomContaiter<easy, block_allocator<Node<easy>, 5>> move_easy(std::move(init_easy));
    }
#if DEBUG
    std::cout << "DIFFERENT ALLOC MOVE/COPY EASY------------------------\n";
#endif
    // different Alloc - at now I have no idea how to move or copy hard in this case
    {
        CustomContaiter<easy, block_allocator<Node<easy>, 5>> init_easy;
        for (size_t i = 0; i < 3; i++)
        {
            init_easy.try_push(FibFact[i].first, FibFact[i].second);
        }

        CustomContaiter<easy> std_easy(init_easy);
        CustomContaiter<easy, block_allocator<Node<easy>, 2>> move_easy(std_easy);
    }

    return 0;
}
