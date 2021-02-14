#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <limits>
#include <random>
#include <ctime>
#include <cmath>
#include <algorithm>

#define RED true
#define BLACK false

using namespace std;

struct node //the player node in redblack tree
{
    string name; //players name and surname
    string team; //players team
    int rebound; //players total number of rebounds
    int assist;  //players total number of asssits
    int point;   //players total number of points

    node *left;   //left child pointer of player node
    node *right;  //right  child pointer of player node
    node *parent; //parent pointer of player node

    bool color; //color of node it is true if it is RED and false if it is BLACK

    bool operator<(const node &other) const
    {
        return this->name < other.name;
    }
    bool operator>(const node &other) const
    {
        return this->name > other.name;
    }

    node(string name, string team, unsigned int rebound, unsigned int assist, unsigned int point)
    {
        this->name = name;
        this->team = team;
        this->rebound = rebound;
        this->assist = assist;
        this->point = point;

        left = right = parent = NULL;
        color = RED;
    }

    node *uncle()
    {
        if (parent == NULL || parent->parent == NULL)
            return NULL;

        if (parent->parent->left == parent)
            return parent->parent->right;

        return parent->parent->left;
    }
};

class RBTree
{
public:
    node *root;
    node *max_rebounds;
    node *max_assists;
    node *max_points;
    RBTree();
    ~RBTree();
    // void remove(node *current);
    void printSeason(int season_num, node *cur, int height);
    void loadAndPrint(const char *filename);
    node *insert(node *root, node *toAdd);
    //void insert(node *toAdd);
    //void insert(node *root, node *toAdd);
    void balance(node *&root, node *&pt);
    void leftRotate(node *&root, node *&pt);
    void rightRotate(node *&root, node *&pt);
    bool is_new;
};

RBTree::RBTree()
{
    root = NULL;
    max_assists = NULL;
    max_rebounds = NULL;
    max_points = NULL;
}

RBTree::~RBTree()
{
}

// void RBTree::leftRotate(node *current)
// {

//     node *right = current->right;

//     if (current->parent == NULL)
//         root = right;
//     else if (current->parent->right == current)
//         current->parent->right = right;
//     else
//         current->parent->left = right;

//     if (right->left)
//         right->left->parent = current;
//     current->right = right->left;

//     right->left = current;
//     right->parent = current->parent;
//     current->parent = right;
// }

// void RBTree::rightRotate(node *current)
// {
//     node *left = current->left;

//     if (current->parent == NULL)
//         root = left;
//     else if (current->parent->right == current)
//         current->parent->right = left;
//     else
//         current->parent->left = left;

//     if (left->right)
//         left->right->parent = current;
//     current->left = left->right;

//     left->right = current;
//     left->parent = current->parent;
//     current->parent = left;
// }

void RBTree::leftRotate(node *&root, node *&pt)
{
    node *pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != NULL)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

void RBTree::rightRotate(node *&root, node *&pt)
{
    node *pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != NULL)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

void RBTree::balance(node *&root, node *&pt)
{
    node *parent_pt = NULL;
    node *grand_parent_pt = NULL;

    while ((pt != root) && (pt->color != BLACK) &&
           (pt->parent->color == RED))
    {

        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A 
            Parent of pt is left child  
            of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left)
        {

            node *uncle_pt = grand_parent_pt->right;

            /* Case : 1 
               The uncle of pt is also red 
               Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color ==
                                        RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }

            else
            {
                /* Case : 2 
                   pt is right child of its parent 
                   Left-rotation required */
                if (pt == parent_pt->right)
                {
                    leftRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3 
                   pt is left child of its parent 
                   Right-rotation required */
                rightRotate(root, grand_parent_pt);
                swap(parent_pt->color,
                     grand_parent_pt->color);
                pt = parent_pt;
            }
        }

        /* Case : B 
           Parent of pt is right child  
           of Grand-parent of pt */
        else
        {
            node *uncle_pt = grand_parent_pt->left;

            /*  Case : 1 
                The uncle of pt is also red 
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color ==
                                       RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else
            {
                /* Case : 2 
                   pt is left child of its parent 
                   Right-rotation required */
                if (pt == parent_pt->left)
                {
                    rightRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3 
                   pt is right child of its parent 
                   Left-rotation required */
                leftRotate(root, grand_parent_pt);
                swap(parent_pt->color,
                     grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    root->color = BLACK;
}

// void RBTree::balance(node *current)
// {
//     if (current == root) // node is root
//         root->color = BLACK;

//     else if (current->parent->color == BLACK) // parent is black
//         return;

//     else if (current->uncle() && current->uncle()->color == RED)
//     { // uncle is RED
//         current->parent->color = BLACK;
//         current->uncle()->color = BLACK;
//         current->parent->parent->color = RED;

//         balance(current->parent->parent);
//     }

//     else
//     {                                           // uncle is BLACK
//         if (current->parent->left == current && // LEFT-LEFT
//             current->parent->parent->left == current->parent)
//         {

//             current->parent->color = BLACK;
//             current->parent->parent->color = RED;
//             rightRotate(current->parent->parent);
//         }

//         else if (current->parent->right == current && // LEFT-RIGHT
//                  current->parent->parent->left == current->parent)
//         {

//             leftRotate(current->parent);
//             //balance(current->left);
//         }

//         else if (current->parent->right == current && // RIGHT-RIGHT
//                  current->parent->parent->right == current->parent)
//         {

//             current->parent->color = BLACK;
//             current->parent->parent->color = RED;
//             leftRotate(current->parent->parent);
//         }

//         else if (current->parent->left == current && // RIGHT-LEFT
//                  current->parent->parent->right == current->parent)
//         {

//             rightRotate(current->parent);
//             //balance(current->right);
//         }
//     }
// }

node *RBTree::insert(node *root, node *toAdd)
{
    if (root == NULL)
    {
        if (max_rebounds == NULL)
        {
            max_points = toAdd;
            max_assists = toAdd;
            max_rebounds = toAdd;
            is_new = true;
        }
        return toAdd;
    }
    if (root->name > toAdd->name)
    {
        root->right = insert(root->right, toAdd);
        root->right->parent = root;
        is_new = true;
    }
    else if (root->name < toAdd->name)
    {
        root->left = insert(root->left, toAdd);
        root->left->parent = root;
        is_new = true;
    }
    else if (root->name == toAdd->name)
    {
        is_new = false;
        root->rebound += toAdd->rebound;
        root->assist += toAdd->assist;
        root->point += toAdd->point;

        if (root->rebound > max_rebounds->rebound)
            max_rebounds = root;
        if (root->assist > max_assists->assist)
            max_assists = root;
        if (root->point > max_points->point)
            max_points = root;
    }
    return root;
}

// void RBTree::insert(node *toAdd)

// {
//     node *indirect = root;
//     if (indirect == NULL)
//     {
//         max_rebounds = toAdd;
//         max_assists = toAdd;
//         max_points = toAdd;
//     }

//     while (indirect != NULL)
//     {

//         toAdd->parent = indirect;

//         if (*toAdd < *indirect)
//         {
//             indirect = &(*indirect)->left);
//             if (toAdd->rebound > max_rebounds->rebound)
//                 max_rebounds = toAdd;
//             if (toAdd->assist > max_assists->assist)
//                 max_assists = toAdd;
//             if (toAdd->point > max_points->point)
//                 max_points = toAdd;
//         }

//         else if (*toAdd > **indirect)
//         {
//             indirect = &((*indirect)->right);
//             if (toAdd->rebound > max_rebounds->rebound)
//                 max_rebounds = toAdd;
//             if (toAdd->assist > max_assists->assist)
//                 max_assists = toAdd;
//             if (toAdd->point > max_points->point)
//                 max_points = toAdd;
//         }
//         else
//         {
//             &(**indirect).point += toAdd->point;
//             &(**indirect).assist += toAdd->assist;
//             root->rebound += toAdd->rebound;

//             if (root->rebound > max_rebounds->rebound)
//                 max_rebounds = root;
//             if (root->assist > max_assists->assist)
//                 max_assists = root;
//             if (root->point > max_points->point)
//                 max_points = root;
//             //*indirect = toAdd; // BU LAZIM MI ?
//             // if (root->rebound > max_rebounds->rebound)
//             //     max_rebounds = root;
//             // if (root->assist > max_assists->assist)
//             //     max_assists = root;
//             // if (root->point > max_points->point)
//             //     max_points = root;
//             return;
//         }
//     }

//     *indirect = toAdd;
//     balance(this->root, toAdd);
// }

// void RBTree::insert(node *root, node *toAdd)
// { //bst insetti yap color red sonra balance çalıştır
//     if (this->root == NULL)
//     {
//         this->root = toAdd;
//         if (max_assists == NULL) //if one of the maximums is null all of them is null
//         {
//             this->max_assists = toAdd;
//             this->max_points = toAdd;
//             this->max_rebounds = toAdd;
//         }
//     }

//     else
//     {
//         if (toAdd->name > root->name)
//         {
//             if (root->right == NULL)
//             {
//                 root->right = toAdd;
//                 toAdd->parent = root;
//                 if (toAdd->rebound > max_rebounds->rebound)
//                     max_rebounds = toAdd;
//                 if (toAdd->assist > max_assists->assist)
//                     max_assists = toAdd;
//                 if (toAdd->point > max_points->point)
//                     max_points = toAdd;
//                 return;
//             }
//             insert(root->right, toAdd);
//         }
//         else if (toAdd->name < root->name)
//         {
//             if (root->left == NULL)
//             {
//                 root->left = toAdd;
//                 toAdd->parent = root;
//                 if (toAdd->rebound > max_rebounds->rebound)
//                     max_rebounds = toAdd;
//                 if (toAdd->assist > max_assists->assist)
//                     max_assists = toAdd;
//                 if (toAdd->point > max_points->point)
//                     max_rebounds = toAdd;
//                 return;
//             }
//             insert(root->left, toAdd);
//         }
//         else if (root->name == toAdd->name)
//         {
//             root->point += toAdd->point;
//             root->assist += toAdd->assist;
//             root->rebound += toAdd->rebound;
//             return;
//         }
//     }
//     //root = toAdd;
//     //balance(root, toAdd);
// }

// void RBTree::remove(node *current)
// {
//     if (current->left)
//         remove(current->left);

//     if (current->right)
//         remove(current->right);

//     delete current;
// }

void RBTree::printSeason(int season_num, node *cur, int height)
{
    if (season_num == 0)
    {
        if (!cur)
            return;
        for (int i = 0; i < height; i++)
            cout << "-";
        printf("(%s) %s\n", cur->color == RED ? "RED" : "BLACK", cur->name.c_str());
        printSeason(season_num, cur->left, height + 1);
        printSeason(season_num, cur->right, height + 1);
    }
}

void RBTree::loadAndPrint(const char *filename)
{ //fix file read
    ifstream input(filename);
    //file.open(filename);
    string dump;          //dummy variable to read header and dump
    getline(input, dump); //get header and dump
    node *toAdd = NULL;
    int season_num = 0;
    string line; //too keep the info of each column
    string season = "";
    unsigned int age;

    while (!input.eof())
    {
        getline(input, line, ',');
        string line0 = line;
        getline(input, line, ',');
        string name = line;
        getline(input, line, ',');
        string team = line;
        getline(input, line, ',');
        int rebounds = stoi(line);
        getline(input, line, ',');
        int assists = stoi(line);
        getline(input, line);
        int points = stoi(line);
        if (season != line0 and season != "")
        { //going through a different season print max and print season tree in the first season
            cout << "End of the " << season << " Season" << endl;
            cout << "Max Points: " << max_points->point << " - Player Name: " << max_points->name << endl;
            cout << "Max Assists: " << max_assists->assist << " - Player Name: " << max_assists->name << endl;
            cout << "Max Rebs: " << max_rebounds->rebound << " - Player Name: " << max_rebounds->name << endl;
            if (season_num == 0)
                printSeason(season_num, this->root, 0);
            season_num++;
        }
        season = line0;
        toAdd = new node(name, team, rebounds, assists, points);
        root = insert(root, toAdd);
        if (toAdd->point > max_points->point)
        {
            max_points = toAdd;
        }
        if (toAdd->assist > max_assists->assist)
        {
            max_assists = toAdd;
        }
        if (toAdd->rebound > max_rebounds->rebound)
        {
            max_rebounds = toAdd;
        }
        if (is_new)
        {
            balance(root, toAdd);
        }
        else if (!is_new)
        {
            delete toAdd;
        }
        is_new = true;
        //insert(root, toAdd);
    }
    cout << "End of the " << season << " Season" << endl;
    cout << "Max Points: " << max_points->point << " - Player Name: " << max_points->name << endl;
    cout << "Max Assists: " << max_assists->assist << " - Player Name: " << max_assists->name << endl;
    cout << "Max Rebs: " << max_rebounds->rebound << " - Player Name: " << max_rebounds->name << endl;

    // if (season_num == 0)
    // {
    //     cout << "End of the " << season << " Season" << endl;
    //     cout << "Max Points: " << max_points->point << " - Player Name: " << max_points->name << endl;
    //     cout << "Max Assists: " << max_assists->assist << " - Player Name: " << max_assists->name << endl;
    //     cout << "Max Rebs: " << max_rebounds->rebound << " - Player Name: " << max_rebounds->name << endl;
    //     printSeason(season_num, this->root, 0);
    // }
}

int main(int argc, const char *argv[])
{
    RBTree tree;
    tree.loadAndPrint("euroleague.csv");
    return 0;
}