#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#pragma region types

enum color_t
{
    BLACK,
    RED
};

struct node
{
    node *parent;
    node *left;
    node *right;

    color_t color;

    int count;

    int key;

    node() {}

    node(int key)
    {
        this->parent = this->left = this->right = nullptr;
        this->color = RED;

        this->key = key;
        this->count = 1;
    }

    node(color_t color, int count)
    {
        this->parent = this->left = this->right = nullptr;
        this->color = color;
        this->count = count;
    }
};

struct rbtree
{
    node *root;
    node *nil;

    rbtree()
    {
        this->nil = new node(BLACK, 0);
        this->root = this->nil;
    }
};

#pragma endregion

#pragma region helpers

int size(rbtree *t)
{
    return t->root->count;
}

// Set tree's root to the true value.
void update_root(rbtree *tree)
{
    while (tree->root->parent != nullptr)
        tree->root = tree->root->parent;
}

void update_count(node *n)
{
    if (n == nullptr)
        return;

    int new_count = 1 + n->left->count + n->right->count;
    if (n->count != new_count)
        n->count = new_count;
    else
        return;

    if (n->parent != nullptr)
        update_count(n->parent);
}

// Rotate left a subtree of root at 'n' replacing it with its right child.
void leftRotate(rbtree *t, node *n)
{
    // a node to be the new root of subtree (it's assumed to be not NIL)
    node *pivot = n->right;

    // updating counts
    n->count -= pivot->count;
    n->count += pivot->left->count;

    pivot->count -= pivot->left->count;
    pivot->count += n->count;

    // connecting n's parent -> pivot
    pivot->parent = n->parent;
    if (n->parent != nullptr)
    {
        if (n->parent->left == n)
            n->parent->left = pivot;
        else
            n->parent->right = pivot;
    }

    // connecting n -> pivot's left child
    n->right = pivot->left;
    if (pivot->left != t->nil)
        pivot->left->parent = n;

    // connecting pivot -> n
    n->parent = pivot;
    pivot->left = n;

    update_root(t);
}

// Rotate right a subtree of root at 'n' replacing it with its left child.
void rightRotate(rbtree *t, node *n)
{
    // a node to be the new root of subtree (it's assumed to be not NIL)
    node *pivot = n->left;

    // updating counts
    n->count -= pivot->count;
    n->count += pivot->right->count;

    pivot->count -= pivot->right->count;
    pivot->count += n->count;

    // connecting n's parent -> pivot
    pivot->parent = n->parent;
    if (n->parent != nullptr)
    {
        if (n->parent->right == n)
            n->parent->right = pivot;
        else
            n->parent->left = pivot;
    }

    // connecting n -> pivot's left child
    n->left = pivot->right;
    if (pivot->right != t->nil)
        pivot->right->parent = n;

    // connecting pivot -> n
    n->parent = pivot;
    pivot->right = n;

    update_root(t);
}

// Return a pointer to a node of specified key if there's such.
// Return nil if there's no such node.
//
// This function is used when deleting by key.
node *find_by_key(rbtree *tree, int key)
{
    node *p = tree->root;
    while (p != tree->nil && p->key != key)
    {
        if (key > p->key)
            p = p->right;
        else
            p = p->left;
    }

    return p;
}

// Return a node of the min key in subtree of root n.
node *minimum(rbtree *t, node *n)
{
    while (n->left != t->nil)
        n = n->left;

    return n;
}

// Return a node of the max key in subtree of root n.
node *maximum(rbtree *t, node *n)
{
    while (n->right != t->nil)
        n = n->right;

    return n;
}

// Recursive helper function for the preorder function.
void preorderHelper(std::vector<int> &keys, rbtree *t, node *n)
{
    keys.push_back(n->key);

    if (n->left != t->nil)
        preorderHelper(keys, t, n->left);

    if (n->right != t->nil)
        preorderHelper(keys, t, n->right);
}

// Return vector of tree's keys preorder.
std::vector<int> preorder(rbtree *tree)
{
    std::vector<int> keys;

    if (tree->root != tree->nil)
        preorderHelper(keys, tree, tree->root);

    return keys;
}

// Recursive helper function for the inorder function.
void inorderHelper(std::vector<int> &keys, rbtree *t, node *n)
{
    if (n->left != t->nil)
        inorderHelper(keys, t, n->left);

    keys.push_back(n->key);

    if (n->right != t->nil)
        inorderHelper(keys, t, n->right);
}

// Return vector of tree's keys in order.
std::vector<int> inorder(rbtree *tree)
{
    std::vector<int> keys;

    if (tree->root != tree->nil)
        inorderHelper(keys, tree, tree->root);

    return keys;
}

// Recursive helper function for the postorder function.
void postorderHelper(std::vector<int> &keys, rbtree *t, node *n)
{
    if (n->left != t->nil)
        postorderHelper(keys, t, n->left);

    if (n->right != t->nil)
        postorderHelper(keys, t, n->right);

    keys.push_back(n->key);
}

// Return vector of tree's keys postorder.
std::vector<int> postorder(rbtree *tree)
{
    std::vector<int> keys;

    if (tree->root != tree->nil)
        postorderHelper(keys, tree, tree->root);

    return keys;
}

// Connect a subtree of root v in place of a subtree of root u in tree.
// u should not be NIL;
void transplant(rbtree *tree, node *u, node *v)
{
    if (u->parent == nullptr)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
    update_root(tree);
    update_count(v->parent);
}

#pragma endregion

#pragma region insertion

// Source: http://neerc.ifmo.ru/wiki/index.php?title=%D0%9A%D1%80%D0%B0%D1%81%D0%BD%D0%BE-%D1%87%D0%B5%D1%80%D0%BD%D0%BE%D0%B5_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%BE#.D0.92.D1.81.D1.82.D0.B0.D0.B2.D0.BA.D0.B0_.D1.8D.D0.BB.D0.B5.D0.BC.D0.B5.D0.BD.D1.82.D0.B0

// Return a pointer to a node which would be a parent
// of a new node with a specified key.
//
// This function is used when inserting a new key.
node *find_insert_point(rbtree *tree, int key)
{
    node *p = tree->root;
    node *q = tree->nil;

    while (p != tree->nil)
    {
        q = p;
        if (p->key < key)
            p = p->right;
        else
            p = p->left;
    }

    return q;
}

// Insert a new node with specified key into a tree keeping it balanced.
void insert_node(rbtree *tree, int key)
{
    node *new_node = new node(key);
    new_node->left = new_node->right = tree->nil;
    node *insert_point = find_insert_point(tree, key);

    // if the tree is empty
    if (insert_point == tree->nil)
    {
        tree->root = new_node;
        tree->root->color = BLACK;
        return;
    }

    // adding new node (the tree is left-leaning apparently?)
    new_node->parent = insert_point;
    if (insert_point->key < new_node->key)
        insert_point->right = new_node;
    else
        insert_point->left = new_node;

    update_count(insert_point);

    // rebalancing:
    node *t = new_node;

    // while parent is red
    while (t->parent != nullptr && t->parent->color == RED)
    {
        if (t->parent == t->parent->parent->left)
        {
            node *uncle = t->parent->parent->right;
            if (uncle->color == RED) //    [red uncle]
            {
                t->parent->color = BLACK;
                uncle->color = BLACK;
                t->parent->parent->color = RED;
                t = t->parent->parent;
            }
            else //    [black uncle]
            {
                if (t == t->parent->right)
                {
                    t = t->parent;
                    leftRotate(tree, t);
                }
                t->parent->color = BLACK;
                t->parent->parent->color = RED;
                rightRotate(tree, t->parent->parent);
            }
        }
        else // if (parent == grandparent->right)
        {
            node *uncle = t->parent->parent->left;

            if (uncle->color == RED) //    [red uncle]
            {
                t->parent->color = BLACK;
                uncle->color = BLACK;
                t->parent->parent->color = RED;
                t = t->parent->parent;
            }
            else //    [black uncle]
            {
                if (t == t->parent->left)
                {
                    t = t->parent;
                    rightRotate(tree, t);
                }
                t->parent->color = BLACK;
                t->parent->parent->color = RED;
                leftRotate(tree, t->parent->parent);
            }
        }
    }

    // setting root's color to black
    update_root(tree);
    tree->root->color = BLACK;
}

#pragma endregion

#pragma region deletion

// Source: Cormen's Intro to Algorithms.

void delete_fixup(rbtree *t, node *x)
{
    while (x != t->root && x->color == BLACK)
    {
        node *w;
        if (x == x->parent->left)
        {
            w = x->parent->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(t, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(t, w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(t, x->parent);
                x = t->root;
            }
        }
        else
        {
            w = x->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(t, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(t, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(t, x->parent);
                x = t->root;
            }
        }
    }

    x->color = BLACK;
}

// Remove a node with specified key (if there is such) from a tree keeping it balanced.
void delete_node(rbtree *tree, int key)
{
    node *z = find_by_key(tree, key);
    if (z == tree->nil)
        return;

    // z now points to a non-nil node to be deleted

    node *y = z, *x;
    color_t y_original_color = y->color;
    if (z->left == tree->nil)
    {
        x = z->right;
        transplant(tree, z, z->right);
    }
    else if (z->right == tree->nil)
    {
        x = z->left;
        transplant(tree, z, z->left);
    }
    else
    {
        y = minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z)
        {
            x->parent = y;
        }
        else
        {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;

        update_count(y);
    }

    if (y_original_color == BLACK)
        delete_fixup(tree, x);

    delete z;
}

#pragma endregion

#pragma region search by index and iterators

node *next(rbtree *t, node *x)
{
    if (x->right != t->nil)
        return minimum(t, x->right);
    node *y = x->parent;
    while (y != nullptr && x == y->right)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

node *prev(rbtree *t, node *x)
{
    if (x->left != t->nil)
        return maximum(t, x->left);
    node *y = x->parent;
    while (y != nullptr && x == y->left)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

node *index_helper(node *n, int index)
{
    if (index < n->left->count)
        return index_helper(n->left, index);

    if (n->left->count == index)
        return n;

    return index_helper(n->right, index - n->left->count - 1);
}

node *find_by_index(rbtree *t, int index)
{
    if (index < 0)
        index = t->root->count - index;

    return index_helper(t->root, index);
}

#pragma endregion

#pragma region some kind of serialization for webGraphviz tree output

// (Recursive) to return a string initializing red nodes and connections for webGraphviz.
void wgv_reds_and_conns_input(std::string &conn_info, std::string &reds_info, rbtree *tree, node *n, std::map<int, int> &occurrences)
{
    if (n == tree->nil)
        return;

    int n_occs = occurrences[n->key]++;

    if (n->right != tree->nil)
    {
        int nr_occs = occurrences[n->right->key];
        conn_info += ("\"" + std::to_string(n->key) + (n_occs ? "_" + std::to_string(n_occs) : "") + " " + std::to_string(n->count) + "\"" + " -> " + "\"" + std::to_string(n->right->key) + (nr_occs ? "_" + std::to_string(nr_occs) : "") + " " + std::to_string(n->right->count) + "\"" + " [label = \"right\"]; ");
    }

    if (n->left != tree->nil)
    {
        int nl_occs = occurrences[n->left->key];
        conn_info += ("\"" + std::to_string(n->key) + (n_occs ? "_" + std::to_string(n_occs) : "") + " " + std::to_string(n->count) + "\"" + " -> " + "\"" + std::to_string(n->left->key) + (nl_occs ? "_" + std::to_string(nl_occs) : "") + " " + std::to_string(n->left->count) + "\"" + " [label = \"left\"]; ");
    }

    if (n->color == RED)
        reds_info += ("\"" + std::to_string(n->key) + (n_occs ? "_" + std::to_string(n_occs) : "") + " " + std::to_string(n->count) + "\" ");

    wgv_reds_and_conns_input(conn_info, reds_info, tree, n->left, occurrences);
    wgv_reds_and_conns_input(conn_info, reds_info, tree, n->right, occurrences);
}

// Return a string for webGraphviz service to draw the tree.
std::string wgv_input(rbtree *tree)
{
    std::string reds, conns;
    std::map<int, int> m;
    wgv_reds_and_conns_input(conns, reds, tree, tree->root, m);
    std::string output = "node [style = \"filled\", fontcolor = \"white\"];\n\nnode [color = \"red\"]; " + reds + "\n\nnode [color = \"black\"];\n" + conns;

    return output;
}

#pragma endregion
