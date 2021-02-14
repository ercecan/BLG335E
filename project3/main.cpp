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

    node(string name, string team, unsigned int rebound, unsigned int assist, unsigned int point) //constructor of player node
    {
        this->name = name;
        this->team = team;
        this->rebound = rebound;
        this->assist = assist;
        this->point = point;

        left = right = parent = NULL;
        color = RED; //default color is always red when inserting
    }

    node *uncle() //to reach the parent->parent->left or parent->parent->right which is uncle (if exists)
    {
        if (parent == NULL || parent->parent == NULL) //if no gparent or dad then no uncle
            return NULL;

        if (parent->parent->left == parent) //if grandpas left is parent then uncle has to be right of gparent
            return parent->parent->right;

        return parent->parent->left; //else it is the left of gparent
    }
};

class RBT //the red-black tree class
{
public:
    node *root;         //the root of the red black tree
    node *max_rebounds; //pointer to hold the player with the max rebounds
    node *max_assists;  //pointer to hold the player with the max assists
    node *max_points;   //pointer to hold the player with the max points
    RBT();
    ~RBT();
    void printSeason(int season_num, node *cur, int height); //prints the red black tree only if it is the first season
    void loadAndPrint(const char *filename);                 //prints the first seasons tree and other seasons maximum stats
    void insert(node *toAdd);                                //inserts a node to red-black tree
    void balance(node *current);                             //balancing the red black tree after insertion
    void leftRotate(node *current);                          //left rotation function in balance
    void rightRotate(node *current);                         //right rotation function in balance
};

RBT::RBT() //constructor of tree assign all pointers to NULL
{
    root = NULL;
    max_assists = NULL;
    max_rebounds = NULL;
    max_points = NULL;
}

RBT::~RBT()
{
}

void RBT::insert(node *toAdd) //iterative BST insert and balancing to make it fit to RBT rules

{
    node **indirect = &root; //get the root to indirect pointer
    if (*indirect == NULL)   //check if root is NULL if it is NULL then all pointers all NULL assign the first node to them
    {
        max_rebounds = toAdd;
        max_assists = toAdd;
        max_points = toAdd;
    }

    while (*indirect != NULL) //if parent is not NULL get in the loop
    {

        toAdd->parent = *indirect; //new comers parent is now root

        if (toAdd->name < (*indirect)->name)
        {
            indirect = &((*indirect)->left);            //if it is smaller then indirect assign it to its left
            if (toAdd->rebound > max_rebounds->rebound) //check if maximum stats change
                max_rebounds = toAdd;
            if (toAdd->assist > max_assists->assist)
                max_assists = toAdd;
            if (toAdd->point > max_points->point)
                max_points = toAdd;
        }

        else if (toAdd->name > (*indirect)->name) //if it is alphabetically bigger then indirect assign it to its right
        {
            indirect = &((*indirect)->right);
            if (toAdd->rebound > max_rebounds->rebound) //check if maximum stats change
                max_rebounds = toAdd;
            if (toAdd->assist > max_assists->assist)
                max_assists = toAdd;
            if (toAdd->point > max_points->point)
                max_points = toAdd;
        }
        else
        { /////////////////////////////////////////if player already exists update his stats
            (*indirect)->point += toAdd->point;
            (*indirect)->assist += toAdd->assist;
            (*indirect)->rebound += toAdd->rebound;
            if ((*indirect)->rebound > max_rebounds->rebound) //check if maximum stats change
                max_rebounds = *indirect;
            if ((*indirect)->assist > max_assists->assist)
                max_assists = *indirect;
            if ((*indirect)->point > max_points->point)
                max_points = *indirect;
            return; //if it is an update no need to call the balance function as it already is balanced
        }
    }

    *indirect = toAdd;
    balance(toAdd);
}

void RBT::leftRotate(node *current)
{
    node *cur_right = current->right; //setting the reight child

    current->right = cur_right->left; //making currents left tree into cur_rights subtree

    if (current->right != NULL)
        current->right->parent = current;

    cur_right->parent = current->parent; //linking currents parent  to currents right

    if (current->parent == NULL)
        this->root = cur_right;

    else if (current == current->parent->left)
        current->parent->left = cur_right;

    else
        current->parent->right = cur_right;

    cur_right->left = current; //putting current to the left of currentright
    current->parent = cur_right;
}

void RBT::rightRotate(node *current)
{
    node *current_left = current->left; //setting the left child

    current->left = current_left->right; //making currents left tree into cur_lefts subtree

    if (current->left != NULL)
        current->left->parent = current;

    current_left->parent = current->parent; //linking current_lefts parent to current

    if (current->parent == NULL)
        this->root = current_left;

    else if (current == current->parent->left)
        current->parent->left = current_left;

    else
        current->parent->right = current_left;

    current_left->right = current; //putting current on curr_lefts right
    current->parent = current_left;
}

void RBT::balance(node *current)
{
    node *cur_gparent = NULL;
    node *cur_parent = NULL;

    while ((current->color != BLACK) and (current != this->root) and (current->parent->color == RED))
    /*1) no need to balance a black node because it is already balanced
     *2) can not do rotations or coloring on root
     *3) no need for balancing if parent is black, check only for red parents
    */
    {

        cur_parent = current->parent;
        cur_gparent = current->parent->parent;

        // if ((current->uncle() != NULL) && (current->uncle()->color == RED)) //if uncle is red color
        // {
        //     cur_gparent->color = RED;
        //     cur_parent->color = BLACK;
        //     current->uncle()->color = BLACK;
        //     current = cur_gparent;
        // }

        if (cur_parent != cur_gparent->left) // if curr->parent is right child  of curr ->gparent
        {
            node *uncle_current = current->uncle();

            if ((uncle_current != NULL) && (uncle_current->color == RED)) //if uncle is red color
            {
                cur_gparent->color = RED;
                cur_parent->color = BLACK;
                uncle_current->color = BLACK;
                current = cur_gparent;
            }
            else
            {
                if (current == cur_parent->left) // LR current is leftchild roate right
                {
                    rightRotate(cur_parent);
                    current = cur_parent;
                    cur_parent = current->parent;
                }

                leftRotate(cur_gparent); //curr is left child of its parent rotate it to left (LL situation)
                bool temp = false;

                temp = cur_parent->color;
                cur_parent->color = cur_gparent->color;
                cur_gparent->color = temp;

                //swap(cur_parent->color, cur_gparent->color);
                current = cur_parent;
            }
        }
        else // if curr->parent is left child  of curr ->gparent
        {
            node *uncle_current = current->uncle();
            if (uncle_current != NULL && uncle_current->color == RED) // if curr->uncle is red just recolor
            {
                cur_gparent->color = RED;
                cur_parent->color = BLACK;
                uncle_current->color = BLACK;
                current = cur_gparent;
            }

            else
            {
                if (current == cur_parent->right) // (RL) curr is right child of its parent rotate it to left
                {
                    leftRotate(cur_parent);
                    current = cur_parent;
                    cur_parent = current->parent;
                }

                rightRotate(cur_gparent); // (RR) curr is the left child of parent rotate right
                bool temp = false;

                temp = cur_parent->color;
                cur_parent->color = cur_gparent->color;
                cur_gparent->color = temp;

                //swap(cur_parent->color, cur_gparent->color);
                current = cur_parent;
            }
        }
    }
    this->root->color = BLACK;
}

void RBT::printSeason(int season_num, node *cur, int height) //height starts from 0 and recurses to the nodes' height
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

void RBT::loadAndPrint(const char *filename)
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
        getline(input, line, ','); //reading season
        string line0 = line;       //season
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
        if (season != line0 and season != "") //if season is not equal to the one just read from file then it is a new season
        {                                     //going through a different season print max and print season tree in the first season
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
        insert(toAdd);  //adding a new node to rbt
        balance(toAdd); //balance the new added node (even if it is an update not an insert it will not get in the balancing loop so complexity is not affected)
    }
    cout << "End of the " << season << " Season" << endl;
    cout << "Max Points: " << max_points->point << " - Player Name: " << max_points->name << endl;
    cout << "Max Assists: " << max_assists->assist << " - Player Name: " << max_assists->name << endl;
    cout << "Max Rebs: " << max_rebounds->rebound << " - Player Name: " << max_rebounds->name << endl;
}

int main(int argc, const char *argv[])
{
    RBT tree;
    tree.loadAndPrint(argv[1]);
    return 0;
}


pos_select(pos, i, node)
        r <- node.left.pos_arr[pos]
        if node.pos = pos //check if it is the position we lookin for if it is increment
            then r+=1
        if i=r and pos = node.pos
            then return node
        elif ( i=r and pos!=node.pos) or i<r // if the index is right but the pos is wrong or index is smaller traverse left
            then return pos_select(node.left, i, pos)
        elif(i>r) //
            then return pos_select(node.right, i-r, pos)

