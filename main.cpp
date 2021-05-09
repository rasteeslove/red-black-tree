#include <time.h>
#include <algorithm>
#include <vector>
#include <random>
#include <stdlib.h>
#include <chrono>
#include "rbtree.h"

int main()
{
    rbtree *t = new rbtree;

    // generating 1_000_000 random ints:
    srand(time(NULL));
    std::vector<int> random_nums;
    for (int i = 0; i < 1000000; i++)
        random_nums.push_back(rand());

    // 1_000_000 insertions:

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int a : random_nums)
    {
        insert_node(t, a);
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Insertion time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

    // 1_000_000 deletions:

    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(random_nums), std::end(random_nums), rng);

    begin = std::chrono::steady_clock::now();

    for (int a : random_nums)
    {
        delete_node(t, a);
    }

    end = std::chrono::steady_clock::now();

    std::cout << "Deletion time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

    return 0;
}