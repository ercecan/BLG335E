#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
using namespace std;

struct Keys
{
    int x;
    int y;
    char z;
    Keys(){};
    Keys(int x_, int y_, char z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }
    void printKeys() { printf("(%d,%d,%c)", x, y, z); }
};
struct node
{
    /*int *x; //array of x's
    int *y; //array of y's
    char *z;    //array of z's*/
    Keys *keys;      // array of x,y,z
    int t;           //degree
    node **children; //array of child pointers
    bool leaf;
    int n; //current number of keys

    void print_node()
    {
        for (int i = 0; i < n; i++)
            keys[i].printKeys();
        cout << endl;
        if (!this->leaf)
            for (int i = 0; i < n + 1; i++)
            {
                children[i]->print_node();
            }
    }

    void childSplit(int x, node *splitNode) //x shows which child of parent it is
    {
        node *newnode = new node(splitNode->t, splitNode->leaf);
        newnode->n = t - 1;

        for (int i = 0; i < t - 1; i++)
            newnode->keys[i] = splitNode->keys[i + t];
        if (!splitNode->leaf)
            for (int i = 0; i < t - 1; i++) //t-1 or t//////////////////////////
                newnode->children[i] = splitNode->children[i + t];

        splitNode->n = t - 1;

        for (int j = n; j >= x + 1; j--)
            children[j + 1] = children[j];

        children[x + 1] = newnode;

        for (int j = n - 1; j >= x; j--) /////////n-1 or n
            keys[j + 1] = keys[j];

        keys[x] = splitNode->keys[t - 1];

        n++;
    }

    void insertNotFull(Keys &newnode, char key)
    {
        int tmp = n - 1;

        if (!this->leaf)
        {
            if (key == 'x')
            {
                while (tmp >= 0 and keys[tmp].x > newnode.x)
                    tmp--;
            }

            else if (key == 'y')
            {
                while (tmp >= 0 and keys[tmp].y > newnode.y)
                    tmp--;
            }
            else if (key == 'z')
            {
                while (tmp >= 0 and keys[tmp].z > newnode.z)
                    tmp--;
            }
            if (children[tmp + 1]->n == 2 * t - 1) //if after insertion the node is full, split again
            {
                childSplit(tmp + 1, children[tmp + 1]);

                if (key == 'x')
                    if (keys[tmp + 1].x < newnode.x)
                        tmp++;
                if (key == 'y')
                    if (keys[tmp + 1].y < newnode.y)
                        tmp++;
                if (key == 'z')
                    if (keys[tmp + 1].z < newnode.z)
                        tmp++;
            }
            children[tmp + 1]->insertNotFull(newnode, key);
        }
        else
        {
            if (key == 'x')
                while (tmp >= 0 and keys[tmp].x > newnode.x)
                    keys[tmp + 1] = keys[tmp], tmp--;
            else if (key == 'y')
                while (tmp >= 0 and keys[tmp].y > newnode.y)
                    keys[tmp + 1] = keys[tmp], tmp--;
            else if (key == 'z')
                while (tmp >= 0 and keys[tmp].z > newnode.z)
                    keys[tmp + 1] = keys[tmp], tmp--;
            keys[tmp + 1] = newnode;
            this->n++;
        }
    }

    node() {}

    node(int t, bool is_leaf)
    {
        this->t = t;
        this->leaf = is_leaf;

        keys = new Keys[2 * t - 1];
        children = new node *[2 * t];
        n = 0;
    }
};

class BTree
{
public:
    void insert(Keys &keynode);
    void remove(Keys &leynode);
    node *root;
    int t;
    char key;
    BTree(int degree, char key);
    void printTree();
};

void BTree::printTree()
{
    if (root != NULL)
    {
        this->root->print_node();
    }
}
BTree::BTree(int degree, char key)
{
    this->root = NULL;
    this->t = degree;
    this->key = key;
}

void BTree::insert(Keys &keynode)
{
    int max_val = 2 * t - 1;
    if (root != NULL)
    {
        if (root->n == max_val)
        {
            node *newnode = new node(t, false);

            newnode->children[0] = root;
            newnode->childSplit(0, root);

            int i = 0;
            if (this->key == 'x')
            {
                if (newnode->keys[i].x < keynode.x)
                    i++;
            }

            else if (this->key == 'y')
            {
                if (newnode->keys[i].y < keynode.y)
                    i++;
            }
            else
            {
                if (newnode->keys[i].z < keynode.z)
                    i++;
            }
            newnode->children[i]->insertNotFull(keynode, this->key);
            root = newnode;
        }
        else //if the number of keys is lower than 2*t-1
        {
            root->insertNotFull(keynode, this->key);
        }
    }

    else if (root == NULL)
    {
        root = new node(t, true);
        root->keys[0] = keynode;
        root->n = 1;
    }
}

int main()
{
    int n, degree;
    char key;
    cin >> n >> degree >> key;
    int x, y;
    char z;
    Keys *newnode;
    BTree btree(degree, key);
    for (int i = 0; i < n; i++)
    {
        cin >> x >> y >> z;
        newnode = new Keys(x, y, z);
        btree.insert(*newnode);
    }
    btree.printTree();
    return 0;
}
