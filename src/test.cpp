#include "spfa/queue_linked_list.h"
#include <iostream>

int main() {
    size_t size = 5;
    alg_ds::queue<int> q(size);
    q.insert(4);
    q.insert(5);
    q.insert(6);
    q.insert(7);
    q.insert(8);
    q.insert(9);
    q.remove();
    q.remove();

    return 0;
}