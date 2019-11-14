/*
if N =1  DATA CORRUPTED
CustomContaiter<hard,block_allocator<Node<hard>,N>> name;

*/
#define DEBUG 0

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
    {
        //    std::map<int, hard, std::less<int>> map_std;
        //    for (size_t i = 0; i < 11; ++i)
        //        map_std.try_emplace(i, i, i);

        // allocations of std::stack<T*> stack_ptr ;
        // libstdc++.so.6->malloc(64)
        // libstdc++.so.6->malloc(512)

        std::map<int, hard, std::less<int>, block_allocator<std::pair<const int, hard>, 10>> map_block;
        for (size_t i = 0; i < 11; ++i)
            map_block.try_emplace(i, i, i);

        for (const auto &el : map_block)
            std::cout << el.second.getValues() << std::endl;
    }

    std::cout << "------------------------\n";
    {
        // CustomContaiter<hard> std_cont_hard;
        // for (size_t i = 0; i < 11; ++i)
        //     std_cont_hard.try_push(i,i);

        CustomContaiter<hard, block_allocator<Node<hard>, 10>> block_cont_hard;
        for (size_t i = 0; i < 11; ++i)
            block_cont_hard.try_push(i, i);

        // if in public    std::cout<<el.value.i<<' '<<el.value.d<<std::endl; works fine
        for (const auto &el : block_cont_hard)
        {
            std::cout << el.value.getValues() << std::endl;
        }
    }

    /// copy ctor move ctor same Alloc
    std::cout << "CTOR MOVE HARD ------------------------\n";

    {
        // move for same with hard
        std::cout << "---" << std::endl;
        CustomContaiter<hard, block_allocator<Node<hard>, 3>> init_hard;
        init_hard.try_push(1, 2);
        init_hard.try_push(1, 2);
        init_hard.try_push(1, 2);
        std::cout << "---" << std::endl;
        CustomContaiter<hard, block_allocator<Node<hard>, 3>> move_hard(std::move(init_hard));
    }
    std::cout << " CTOR COPY/MOVE EASY ------------------------\n";

    {
        // move copy ctor with easy
        std::cout << "---" << std::endl;
        CustomContaiter<easy, block_allocator<Node<easy>, 5>> init_easy;
        init_easy.try_push(1, 2);
        init_easy.try_push(1, 2);
        init_easy.try_push(1, 2);
        std::cout << "---" << std::endl;
        CustomContaiter<easy, block_allocator<Node<easy>, 5>> copy_easy(init_easy);
        std::cout << "---" << std::endl;
        CustomContaiter<easy, block_allocator<Node<easy>, 5>> move_easy(std::move(init_easy));
    }
    std::cout << " DIFFERENT ALLOC MOVE/COPY EASY------------------------\n";

    // different Alloc - at now I have no idea how to move or copy hard in this case
    {
        std::cout << "---" << std::endl;
        CustomContaiter<easy, block_allocator<Node<easy>, 5>> init_easy;
        init_easy.try_push(1, 2);
        init_easy.try_push(1, 2);
        init_easy.try_push(1, 2);
        init_easy.try_push(1, 2);
        std::cout << "---" << std::endl;
        CustomContaiter<easy> std_easy(init_easy);
        std::cout << "---" << std::endl;
        CustomContaiter<easy, block_allocator<Node<easy>, 2>> move_easy(std_easy);
    }

    return 0;
}

/*
h.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(i, i));
*/
