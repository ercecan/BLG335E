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
    /*int *x; //array of xs
    int *y; //array of ys
    char *z;    //array of zs*/
    char key;        //holds the key type either 'x', 'y' or 'z'
    Keys *keys;      // array of x,y,z
    int t;           //degree
    node **children; //array of child pointers
    bool leaf;       //if it is a leaf set it to true
    int n;           //current number of keys

    int findKeynode(Keys &keynode, char key) //returns the index of the key
    {
        int i = 0;
        if (key == 'x')
            while (i < n && keys[i].x < keynode.x)
                ++i;
        if (key == 'y')
            while (i < n && keys[i].y < keynode.y)
                ++i;
        if (key == 'z')
            while (i < n && keys[i].z < keynode.z)
                ++i;
        return i;
    }
    void deleteOp(Keys &keynode, char key)
    {
        int i = findKeynode(keynode, key);

        if (i < n and (keys[i].x == keynode.x) and (keys[i].y == keynode.y) and (keys[i].z == keynode.z))
        {
            if (!leaf)
                rmvFromNonL(i);
            else
            {
                for (int j = i + 1; j < n; ++j)
                    keys[j - 1] = keys[j];

                n--;
            }
        }
        else
        {
            if (leaf)
            {
                cout << "the key (" << keynode.x << ", " << keynode.y << ", " << keynode.z << ") does not exist" << endl;
                return;
            }
            int k = 0; // flag checking if the key to be removed is at the last child or not
            if (i != n)
                k = 0;
            else
                k = 1;
            if (children[i]->n < t)
            {
                fill(i);
            }

            if (k and i > n)
            {
                children[i - 1]->deleteOp(keynode, key);
            }
            else
            {
                children[i]->deleteOp(keynode, key);
            }
        }
        return;
    }

    void rmvFromNonL(int i)
    {
        Keys key = keys[i];

        if (this->children[i]->n >= t)
        {
            node *tmp = children[i];
            while (!tmp->leaf)
                tmp = tmp->children[tmp->n];
            Keys pred = tmp->keys[tmp->n - 1];
            keys[i] = pred;
            children[i]->deleteOp(pred, this->key);
        }
        else if (children[i + 1]->n >= t)
        {
            node *tmp = children[i + 1];
            while (!tmp->leaf)
                tmp = tmp->children[0];
            Keys succ = tmp->keys[0];
            keys[i] = succ;
            children[i + 1]->deleteOp(succ, this->key);
        }
        else
        {
            merge(i);
            children[i]->deleteOp(key, this->key);
        }
    }

    void fill(int i)
    {
        if (i != n and children[i + 1]->n >= t)
            takeNext(i);
        else if (i and (children[i - 1]->n >= t))
            takePrev(i);
        else
        {
            if (i == n)
            {
                merge(i - 1);
            }
            else
            {
                merge(i);
            }
        }
    }

    void takePrev(int i)
    {

        node *s = children[i - 1]; //sibling node

        node *c = children[i]; //child node

        for (int j = c->n; j >= 0; --j)
        {
            c->keys[j + 1] = c->keys[j];
        }
        if (!c->leaf)
        {
            for (int j = c->n; j >= 0; --j)
                c->children[j + 1] = c->children[j];
        }

        c->keys[0] = keys[i - 1];

        if (!c->leaf)
        {
            c->children[0] = s->children[s->n];
        }

        keys[i - 1] = s->keys[s->n - 1];

        c->n += 1;
        s->n -= 1;
    }
    void takeNext(int i)
    {
        node *s = children[i + 1]; //sibling node

        node *c = children[i]; //child node

        c->keys[(c->n)] = keys[i];
        if (!(c->leaf))
            c->children[(c->n) + 1] = s->children[0];

        keys[i] = s->keys[0];

        for (int j = 1; j < s->n; ++j)
            s->keys[j - 1] = s->keys[j];

        if (!s->leaf)
        {
            for (int j = 1; j <= s->n; ++j)
                s->children[j - 1] = s->children[j];
        }

        c->n += 1;
        s->n -= 1;
    }
    void merge(int i)
    {
        node *s = children[i + 1]; //sibling node

        node *c = children[i]; //child node
        c->keys[t - 1] = keys[i];
        for (int j = 0; j < s->n; ++j)
        {
            c->keys[t + j] = s->keys[j];
        }

        if (!c->leaf)
        {
            for (int j = 0; j <= s->n; ++j)
                c->children[j + t] = s->children[j];
        }
        for (int j = i + 1; j < n; ++j)
        {
            keys[j - 1] = keys[j];
        }

        for (int j = i + 2; j <= n; ++j)
        {
            children[j - 1] = children[j];
        }
        c->n += s->n + 1;
        n--;
        delete (s);
        return;
    }

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

    void childSplit(int x, node *splitNode)
    {
        node *newnode = new node(splitNode->t, splitNode->leaf, this->key);
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

        if (this->leaf)
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
        else
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
    }

    node() {}

    node(int t, bool is_leaf, char key)
    {
        this->t = t;
        this->leaf = is_leaf;
        this->key = key;
        keys = new Keys[2 * t - 1];
        children = new node *[2 * t];
        n = 0;
    }

    Keys searchX(int x) //keys* or just keys
    {

        int i = 0;
        while (i < n and x > keys[i].x)
            i++;

        if (keys[i].x == x)
            return this->keys[i];

        if (leaf)
            return this->keys[i]; //bugggg fixx normalde NULL returnlemesi lazim

        // Go to the appropriate child
        return children[i]->searchX(x);
    }
    Keys searchY(int y) //keys* or just keys
    {

        int i = 0;
        while (i < n and y > keys[i].y)
            i++;

        if (keys[i].y == y)
            return this->keys[i];

        if (leaf)
            return this->keys[i]; //buggggggg

        return children[i]->searchY(y);
    }
    Keys searchZ(char z) //keys* or just keys
    {

        int i = 0;
        while (i < n and z > keys[i].z)
            i++;

        if (keys[i].z == z)
            return this->keys[i];

        if (leaf)
            return this->keys[i]; //buggggggg

        return children[i]->searchZ(z);
    }
};

class BTree
{
public:
    void insert(Keys &keynode);
    void del(Keys &keynode);
    node *root;
    int t;
    char key;
    BTree(int degree, char key);
    void printTree();
};

void BTree::del(Keys &keynode)
{
    if (!root)
    {
        cout << "tree is empty!" << endl;
        return;
    }

    root->deleteOp(keynode, this->key);

    if (root->n == 0)
    {
        node *tmp = root;
        if (root->leaf)
        {
            root = NULL;
        }
        else
        {
            root = root->children[0];
        }
        delete tmp;
    }
    return;
}

void BTree::printTree()
{
    if (root != NULL)
    {
        root->print_node();
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
    if (root == NULL)
    {
        root = new node(t, true, key);
        root->keys[0] = keynode;
        root->n = 1;
    }

    else
    {
        if (root->n == 2 * t - 1)
        {
            node *newnode = new node(t, false, key);

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
    if (key == 'x')
    {
        int x1 = 0;
        cin >> x1;
        Keys newkey;
        newkey = btree.root->searchX(x1);
        btree.del(newkey);
    }
    else if (key == 'y')
    {
        int y1 = 0;
        cin >> y1;
        Keys newkey;
        newkey = btree.root->searchY(y1);
        btree.del(newkey);
    }
    else
    {
        char z1 = 0;
        cin >> z1;
        Keys newkey;
        newkey = btree.root->searchZ(z1);
        btree.del(newkey);
    }
    btree.printTree();
    return 0;
}
