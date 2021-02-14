#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include <ctime>
#include <iomanip>
#include <limits>
#include <random>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;

//a binary min_heap, will be used to create a primary queue
class Heap
{
private:
public:
    Heap();
    ~Heap();
    double distance;                          //eucludian distance
    double x;                                 //longtitude
    double y;                                 //latitude
    int heap_size;                            //size of the min-heap
    vector<double> ary;                       //the array representing the binary min-heap
    void minHeap_insert(double dist);         //function to insert an element to the min-heap
    double minHeap_extract_min();             //function to remove and return the first (min) element of heap
    void minHeap_decrease(int i, double key); //decrease a value in a min_heap
    void min_heapify(int i);                  //heapify an element
    void build_minHeap();                     //build the min_heap property of a heap
};

Heap::Heap()
{
    distance = 0.0;
    x = 0.0;
    y = 0.0;
    heap_size = 0;
}

Heap::~Heap()
{
}

double Heap::minHeap_extract_min()
{

    if (heap_size < 1)
    {
        cerr << "error extract min" << endl;
        return int(-1); //if there is no taxi to extract (empty heap)
    }

    double min = ary[1];
    ary[1] = ary[heap_size];
    ary.pop_back();
    heap_size = heap_size - 1;
    min_heapify(1);
    return min;
}

void Heap::min_heapify(int i)
{
    int l = 2 * i;                          //child left
    int r = 2 * i + 1;                      //child right
    int smallest = i;                       //smallest temp
    if (l <= heap_size and ary[l] < ary[i]) //if index l is smaller set it to smallest
    {
        smallest = l;
    }
    else
    {
        smallest = i;
    }

    if (r <= heap_size and ary[r] < ary[smallest])
    {
        smallest = r;
    }

    if (smallest != i) //if index is not the smallest swap with its child (put the parent down because it is bigger)and recurse to othe childs
    {
        swap(ary[i], ary[smallest]);
        min_heapify(smallest);
    }
}

void Heap::build_minHeap()
{
    int heapSize = heap_size;
    for (int i = floor(heap_size / 2); i >= 1; i--) //go through almost half of the nodes which are the interior nodes and apply min_heapify on all of them
    {
        min_heapify(i);
    }
}

void Heap::minHeap_decrease(int i, double key)
{
    //key = ary[i]-0.01

    if (key > ary[i])
    {
        cerr << "error new key is bigger than current key: " << endl;
        exit(1);
    }
    ary[i] = key;
    while (i > 1 and ary[i / 2] > ary[i])
    {
        swap(ary[i], ary[i / 2]);
        i = i / 2;
    }
}

void Heap::minHeap_insert(double key)
{
    ary.push_back(double(100000007.0)); //get a very big value so when we insert we can use decrease function.
    heap_size += 1;
    //ary[heap_size] = double(INT_MAX);
    minHeap_decrease(heap_size, key);
}

class PriorityQueue
{
private:
public:
    PriorityQueue();
    ~PriorityQueue();
    Heap heap;                            //the binary min_heap that creates the pq
    void push(double x, double y);        //insert a value to the pq
    double pop();                         //extract the first value (minimum) from the pq
    void decrease(int index, double key); //decrease a key in the pq
    void decreaseRandom(double amount);   //for part 2 of implementation
    double calc_dist(double x, double y); //calculate distance of taxis to the hotel
    int updated;                          //The number of distance updates
    int added;                            //The number of taxi additions
    int removed;                          //The number of removed taxis after 100th operation
};

PriorityQueue::PriorityQueue()
{
    updated = 0;
    added = 0;
}

PriorityQueue::~PriorityQueue()
{
}

double PriorityQueue::calc_dist(double x, double y)
{
    double x_hotel = 33.40819;
    double y_hotel = 39.19001;
    double dist = sqrt(pow((x - x_hotel), 2) + pow((y - y_hotel), 2));
    return dist;
}

void PriorityQueue::push(double x, double y)
{
    heap.minHeap_insert(calc_dist(x, y));
    added++;
}

double PriorityQueue::pop()
{
    //chechk if there is a taxi to remove
    if (heap.heap_size > 1) //if there is a taxi then we can remove it
    {
        removed++;
        // heap.heap_size -= 1;
        return heap.minHeap_extract_min(); //call heap min extract function to remove the smallest element
    }
    else
    {
        return -1;
    }
}

void PriorityQueue::decrease(int index, double key)
{
    //key => gets swapped with array[index] if key < array[index]
    heap.minHeap_decrease(index, key);
}

void PriorityQueue::decreaseRandom(double amount)
{ //decrease the distance value of a random taxi by 0.01
    int selector;
    if (heap.heap_size > 0)
    {
        selector = (rand() % heap.heap_size) + 1; //random taxi between all taxis [1 - heap_size]
        updated++;
    }
    else
    {
        return;
    }
    double key = heap.ary[selector] - amount;
    decrease(selector, key); //decrease the selected key
}

int main(int argc, char *argv[])
{
    int m = atoi(argv[1]);
    double p = stod(argv[2]);
    srand((unsigned)time(NULL));

    ifstream file;
    file.open("locations.txt");

    if (!file)
    {
        cerr << "File cannot be opened!";
        exit(1);
    }

    string dump; //dummy variable to get the first line
    getline(file, dump);

    double x_taxi, y_taxi;

    PriorityQueue pq;
    pq.heap.ary.push_back(-1);

    vector<double> called_distances; //a vector to store the distances of called taxis
    clock_t time;
    time = clock();
    for (int i = 1; i <= m; i++)
    {

        //remove a taxi in every 100 ops
        if (i % 100 == 0)
        {
            double removed = pq.pop();

            if (removed != -1) //if there is taxi to remove
            {
                called_distances.push_back(removed);
                cout << "removed taxi 100 op." << removed << endl;
            }
            continue;
        }
        double random = (double)rand() / RAND_MAX; //a random value between 0 and 1 to determine the operation

        if (random >= 0.0 and random < p) //if random between 0 and p => update
        {
            pq.decreaseRandom(0.01); //decrase a random taxis distance
        }
        else if (random >= p and random <= 1.0) //if random between p and 1 (1-p) => add
        {
            file >> x_taxi >> y_taxi; //read longtitude and latitude from file
            pq.push(x_taxi, y_taxi);  //insert in priority queue
        }
        else
        {
            cerr << "broken and p is:" << p << "random is:  " << random << "1-p is : " << 1.0 - p << endl;
        }
    }
    time = clock() - time;
    long double duration = double(time) / CLOCKS_PER_SEC;
    duration *= 1000.0;
    file.close();

    //writing to file operations
    ofstream fileo;
    string s = "output.txt";
    fileo.open(s); //output file to write the output

    if (!fileo)
    {
        cerr << "File cannot be created to write!";
    }

    //writing the time passed in ms
    fileo << "Time of the process in ms\n";
    fileo << duration << "\n";

    //number of additions and updates
    fileo << "number of taxi additions\tnumber of distance updates\n";
    fileo << pq.added << "\t" << pq.updated << "\n";

    //distances of called taxis
    fileo << "Dstances of called taxis\n";
    for (int i = 0; i < pq.removed; i++)
    {
        fileo << called_distances[i] << "\n";
    }

    fileo.close();
    return 0;
}