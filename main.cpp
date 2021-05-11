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

#pragma region speed test

    // generating 1_000_000 random ints:
    srand(time(NULL));
    std::vector<int> random_nums;
    for (int i = 0; i < 1000000; i++)
        random_nums.push_back(rand());

    // 1_000_000 insertions:

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int a : random_nums)
        insert_node(t, a);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Tree size: " << size(t) << "\n";
    std::cout << "Insertion time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "\n";

    // 1_000_000 deletions:

    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(random_nums), std::end(random_nums), rng);

    begin = std::chrono::steady_clock::now();

    for (int a : random_nums)
        delete_node(t, a);

    end = std::chrono::steady_clock::now();

    std::cout << "Tree size: " << size(t) << "\n";
    std::cout << "Deletion time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

    std::cout << "\n\n\n\n\n" + std::string(50, '*') + "\n\n\n\n\n";

#pragma endregion

#pragma region regular work check

    // generating 200 random ints:
    for (int i = 0; i < 200; i++)
        random_nums.push_back(rand());

    // 200 insertions:

    for (int a : random_nums)
        insert_node(t, a);

    // 100 deletions:

    rng = std::default_random_engine{};
    std::shuffle(std::begin(random_nums), std::end(random_nums), rng);
    random_nums.erase(random_nums.end() - 100, random_nums.end());

    for (int a : random_nums)
        delete_node(t, a);

    std::cout << wgv_input(t);

    std::cout << "\n\n\n\n\n" + std::string(50, '*') + "\n\n\n\n\n";

#pragma endregion

#pragma region testing iterators and indexing

    node *x = minimum(t, t->root);
    while (x != nullptr)
    {
        std::cout << x->key << " ";
        x = next(t, x);
    }

    std::cout << "\n";

    for (int i = 0; i < size(t); i++)
        std::cout << find_by_index(t, i)->key << " ";

#pragma endregion

    return 0;
}