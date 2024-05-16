#pragma once

#include <unordered_map>
#include <vector>

namespace alg_ds {

// template<typename tag_t>
// struct node {
//     tag_t tag;
//     node *parent, *child;
//     node *left, *right;
//     int degree = 0;
//     bool mark = false;
//     node(tag_t tag_) : tag(tag_) {}
//     node(tag_t tag_, node *left_, node *right_) :
//         tag(tag_), left(left_), right(right_) {}
// };

template<typename tag_t>
class fibonacci_heap {
public:
    struct node {
        tag_t tag;
        node *parent, *child;
        node *left, *right;
        int degree = 0;
        bool mark = false;
        node(tag_t tag_) : tag(tag_) {}
        node(tag_t tag_, node *left_, node *right_) :
            tag(tag_), left(left_), right(right_) {}
        node(node *other) {
            tag = other->tag;
            parent = other->parent;
            child = other->child;
            left = other->left;
            right = other->right;
            degree = other->degree;
            mark = other->mark;
        }
    };

    fibonacci_heap() {}
    void insert(tag_t);
    fibonacci_heap *join(fibonacci_heap*);
    node *extract_min();
    node *get_minimum() { return min_node; }
    node *cut_node(node*);
    node *cascading_cut_node(node*);
    void decrease_tag(node*, tag_t);

private:
    int (*cmp)(tag_t, tag_t);

    int amount_node = 0;
    node *min_node = nullptr;
    node *root_list = nullptr;

    int max_degree = 0;
    void update_max_degree(int new_degree) {
        if (new_degree > max_degree) max_degree = new_degree;}

    void merge_node_with_list(node*,node*);
    node* extract_node_from_list(node*,node*);
    node* merge_lists(node*, node*);

    void tie_child(node*, node*);
    void tie_nodes(node *first, node *second) {
        first->right = second;
        second->left = first;
    }
    bool extract_child(node*);
    void consolidate();
};


template<typename tag_t>
fibonacci_heap<tag_t>::node *fibonacci_heap<tag_t>::merge_lists(node *list1, node *list2) {
    tie_nodes(list1->left, list2);
    tie_nodes(list1, list2->left);
}

template<typename tag_t>
void fibonacci_heap<tag_t>::merge_node_with_list(node *list, node *new_node) {
    tie_nodes(list->l, new_node);
    tie_nodes(new_node, list);
}

template<typename tag_t>
void fibonacci_heap<tag_t>::insert(tag_t new_tag) {
    node new_node(new_tag);
    ++amount_node;

    if (amount_node == 1)
        root_list = &new_node;

    merge_node_with_list(root_list, &new_node);

    if (cmp(min_node->tag, new_node->tag) >= 0)
        min_node = new_node;
}

template<typename tag_t>
fibonacci_heap<tag_t> *fibonacci_heap<tag_t>::join(fibonacci_heap<tag_t> *other_heap) {
    fibonacci_heap *new_heap;

    node *first_cur = root_list;
    node *last_cur = root_list->l;
    node *first_other = other_heap->root_list;
    node *last_other = other_heap->root_list->left;

    tie_nodes(last_cur, first_other);
    tie_nodes(first_cur, last_other);

    new_heap->root_list = first_cur;
    if (cmp(min_node, other_heap->min_node) > 0)
        new_heap->min_node = other_heap->min_node;
    else
        new_heap->min_node = min_node;
    new_heap->amount_node = amount_node + other_heap->amount_node;
    new_heap->max_degree = (max_degree > other_heap->max_degree) ? max_degree : other_heap->max_degree;
    return new_heap;
}

template<typename tag_t>
fibonacci_heap<tag_t>::node *fibonacci_heap<tag_t>::extract_min() {
    if (!root_list) return;
    amount_node--;

    node *curr_min_node = min_node;
    merge_lists(root_list, curr_min_node->child);

    node *new_root = extract_node_from_list(root_list, curr_min_node);
    if (!new_root)
        return;
    min_node = new_root;

    consolidate();
    return curr_min_node->tag;
}

template<typename tag_t>
void fibonacci_heap<tag_t>::tie_child(node *root, node *new_child) {
    root->degree = 1;
    update_max_degree(root->degree);

    if (!root->child) {
        root->child = new_child;
        new_child->parent = root;
        return;
    }

    node* brother = root->child;
    tie_nodes(brother->left, new_child);
    tie_nodes(new_child, brother);
}

template<typename tag_t>
bool fibonacci_heap<tag_t>::extract_child(node *picked) {
    if (!picked->parent) return;

    bool previous_parent_mark = picked->parent->mark;
    picked->parent->degree--;
    picked->parent->mark = true;

    node *right = picked->right;
    node *left = picked->left;

    if (picked->parent->child)
        parent_child->child = extract_node_from_list(picked->parent->child, picked);
    picked->parent = nullptr;
    return previous_parent_mark;
}

template<typename tag_t>
fibonacci_heap<tag_t>::node *fibonacci_heap<tag_t>::extract_node_from_list(node *list, node *picked) {

    if (!list) return nullptr;

    node *right = picked->right;
    node *left = picked->left;

    if (left) left->right = right;
    if (right) right->left = left;

    picked->left = nullptr;
    picked->right = nullptr;

    if (right->tag != picked->tag) {
        return right;
    }
    return nullptr;
}

template<typename tag_t>
void fibonacci_heap<tag_t>::consolidate() {
    node *sub_root = root_list->right;
    std::vector<node*> degrees(max_degree + 1, nullptr);

    while (true) {
        if (!degrees[sub_root->degree]) {
            degrees[sub_root->degree] = sub_root;
            sub_root = sub_root->right;
            continue;
        }
        node *conflicted = degrees[sub_root->degree];
        degrees[sub_root->degree] = nullptr;

        if (cmp(sub_root->tag, conflicted->tag) < 0) {
            extract_node_from_list(root_list, conflicted);
            tie_child(sub_root, conflicted);
        }
        else {
            extract_node_from_list(root_list, sub_root);
            tie_child(conflicted, sub_root);
            sub_root = conflicted;
        }

        if (cmp(min_node->tag, sub_root) < 0)
            min_node = sub_root;
    }
}


template<typename tag_t>
fibonacci_heap<tag_t>::node *fibonacci_heap<tag_t>::cut_node(node *picked) {
    bool previous_parent_mark =
    extract_child(picked);
    amount_node--;
    picked->mark = false;
    merge_node_with_list(root_list, picked);
    return picked;
}

template<typename tag_t>
fibonacci_heap<tag_t>::node *fibonacci_heap<tag_t>::cascading_cut_node(node *picked) {
    if (!picked) return;

    while (picked->mark) {
        cut_node(picked);
        picked = picked->parent;
    }
    picked->mark = true;
}

template<typename tag_t>
void fibonacci_heap<tag_t>::decrease_tag(node *picked, tag_t new_tag) {
    if (cmp(new_tag, picked->tag0) > 0) {
        picked->tag = new_tag;
        return;
    }
    node *parent = picked->parent;
    cut_node(picked);
    cascading_cut_node(parent);
}
}