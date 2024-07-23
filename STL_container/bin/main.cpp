#include "lib/bst.h"

int main() {
    // BST<int, pre_order_tag> copy;
    // BST<int, pre_order_tag> work;

    // work.insert(1);

    // int arr[9] = {8, 3, 1, 6, 4, 7, 10, 14, 13};

    // for (int i = 0; i < 9; i++)
    // {
    //     copy.insert(arr[i]);
    // }

    // work.merge(copy);
    // work.print();

    // std::cout << work.contains(143);

    // work.print();

    BST<int, in_order_tag> bst;

    int arr[9] = {8, 3, 1, 6, 4, 7, 10, 14, 13};

    for (int i = 0; i < 9; i++)
    {
        bst.insert(arr[i]);
    }
    
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        std::cout << "Yra ";
    }

    // return 0;
}