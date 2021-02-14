#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

struct Keys
{
    int x;
    int y;
    char z;
    Keys(){};
    Keys() {}
    Keys(int x_, int y_, char z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }
    void print_keys() { cout << "(" << x << "," << y << "," << z; }
};
struct node
{
    /*int *x; //array of x's
    int *y; //array of y's
    char *z;    //array of z's*/
    Keys *keys; // array of x,y,z
    int t;      //degree
    node **c;   //array of child pointers
    bool leaf;
    int n; //current number of keys
    node(int num, bool is_leaf)
    {
    }
    void print_node()
    {
        for (int i = 0; i < n; i++)
        {
            keys[i].print_keys();
        }
        cout << endl;
        if (!this->leaf)
        {
            for (int i = 0; i < n; i++)
            {
                c[i]->print_node();
            }
        }
    }

    void traverse()
    {
    }
    void splitChild(int i, node *arr)
    {
    }
    void insertNotFull(char x, ) {}

    node()
    {
    }

    node(int t, bool is_leaf)
    {
        this->t = t;
        this->leaf = is_leaf;

        keys = new keys[2 * t - 1];
        c = new node[2 * t];
        n = 0;
    }

    node *search(int x = 0, int y = 0, char z = '')
    {
    }
};

class BTree
{
public:
    void insert(Keys &keynode);
    void delete ();
    node *root;
    int t;
    char key;
    void traverse();
    BTree(int degree, char key);
    ~Btree();
    void printTree();
};

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
void BTree::traverse()
{
    if (root != NULL)
        root->traverse();
}

BTree::~BTree()
{
}

void BTree::insert(Keys &keynode)
{
    if (root == NULL)
    {
        root = new node(t, true);
        root->keys[0] = keynode;
        root->n = 1;
    }

    else{
        if(root->n == 2*t - 1){
            node* n= new node(t, flase);

            n->c[0] = root;
            
            n->split(0,root);
            int i = 0;
            if(this->key == 'x')

        }
    }
}

void BTree::delete ()
{
}

int main()
{
    cout << "test";
    int n, degree;
    char key;
    cin >> n >> degree >> key;
    int x, y;
    char z;
    BTree btree(degree, key);
    for (int i = 0; i < n; i++)
    {
        cin >> x >> y >> z;
    }
    return 0;
}
