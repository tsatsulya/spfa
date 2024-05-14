#pragma once
#include <iostream>
#include <cassert>

namespace alg_ds {

template<typename data_t>
class queue {

    size_t size;
    size_t current_nodes_amount;
    struct node {
        data_t data;
        node *previous = nullptr;
        node *next = nullptr;
        node(data_t data_) : data(data_) {}
        node(data_t data_, node *previous_, node *next_) : data(data_), previous(previous_), next(next_) {}
        node() {}
    };
    node *head = nullptr;
    node *tail;

public:
    queue(size_t size_) : size(size_) {
        assert(size_);
        current_nodes_amount = 0;
    };
    ~queue() {
        node *sub_node = head;
        while (sub_node) {
            if (sub_node == tail) {
                delete sub_node; break;
            }
            sub_node = sub_node->next;
            delete sub_node->previous;
        }
    }

    void insert(data_t new_data) {

        node *new_node = new node(new_data, nullptr, head);
        node *previous_head = head;
        ++current_nodes_amount;

        if (!head) {
            head = new_node;
            tail = head;
            return;
        }
        head->previous = new_node;
        head = new_node;
        if (current_nodes_amount > size) {
            tail = tail->previous;
            delete tail->next;
            tail->next = nullptr;
            current_nodes_amount = size;
        }
        return;
    }

    data_t *peek() {
        if (!current_nodes_amount) return nullptr;
        return &(tail->data);
    }

    data_t remove() {
        assert(current_nodes_amount);

        --current_nodes_amount;
        data_t front_data = tail->data;

        tail = tail->previous;
        delete tail->next;
        tail->next = nullptr;
        return front_data;
    }

    bool is_empty() {
        if (current_nodes_amount) return false;
        return true;
    }

    bool is_full() {
        if (current_nodes_amount < size) return false;
        return true;
    }

    size_t get_nodes_amount() {
        return current_nodes_amount;
    }
};
}