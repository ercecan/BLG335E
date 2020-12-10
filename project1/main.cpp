#include <iostream>
#include <fstream>
#include <string>
//#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>


using namespace std;
//using namespace std::chrono;

struct Sale
{
	string country;
	string item_type;
	string order_id;
	int units_sold;
	long double total_profit;

	/*Sale()
	{
		country = "";
		item_type = "";
		order_id = "";
		units_sold = -1;
		total_profit = -1.0;
	}*/

	Sale(string line[5])
	{
		country = line[0];
		item_type = line[1];
		order_id = line[2];
		units_sold = stoi(line[3]);	   //string to int
		total_profit = stold(line[4]); //string to float
	}

	bool operator<(const Sale &other)
	{
		//overload '<' operator for easier comparing

		if (country.compare(other.country) != 0)
		{
			return country.compare(other.country) < 0;
		}

		//used '>' operator here since it will be descending order in this case
		if (country.compare(other.country) == 0)
			return total_profit > other.total_profit;

		return total_profit > other.total_profit;
	}
};

//sorting by country
class Products
{
public:
	Sale **sales; //holds the pointers of Sales
	Sale **backup;

	int size; //number of products

	Products(string filenmae, int limit);
	~Products();
	void rewind();

	long double sort();
	void QuickSort(Sale **array, int length);

	void write_file(string filename);
};

Products::Products(string filename, int limit)
{
	this->size = 0;

	////// beginning of file operations
	ifstream file;
	file.open(filename);

	if (!file)
	{
		cerr << "File cannot be opened!";
		exit(1);
	}

	string line[5]; //buffer to read valuable inputs into
	string dump;	//buffer to read dump inputs into

	getline(file, dump); //this is the header line

	//will be used to populate
	this->sales = new Sale *[limit];
	this->backup = new Sale *[limit];
	//cerr << 1 << endl;

	for (int i = 0; i < limit; i++)
	{
		//getline read until delimitation char ('\t') found. If found, it is extracted and discarded.
		getline(file, line[0], '\t'); //country (string)
		getline(file, line[1], '\t'); //item type (string)
		getline(file, line[2], '\t'); //order id (string)
		file >> line[3];			  //units sold (integer)
		file >> line[4];			  //total profit (float)
		getline(file, dump, '\n');	  //this is for reading the \n character into dummy variable.

		Sale *temp = new Sale(line); //A temporary Sale pointer to make assignments without dangling pointers
		//cerr << 2 << endl;

		this->sales[i] = temp;	//sales filled with default assignment operation
		this->backup[i] = temp; //backup filled with default assignment operation
		//cerr << 3 << endl;

		this->size++; //incrementing the number of products to keep track

		if (limit <= this->size)
		{
			break;
		}
	}
	////// end of file operations

	file.close();
}

//destructor of products class
Products::~Products()
{
	//deallocation operations
	for (int i = 0; i < this->size; i++)
	{
		delete this->sales[i];
	}
	delete[] this->sales;
	delete[] this->backup;
}

long double Products::sort()
{
	//sort the sales products and return the elpased time during the operation
	clock_t time;
	//auto start = high_resolution_clock::now();
	time = clock();
	this->QuickSort(this->sales, this->size);
	time = clock() - time;
	//auto stop = high_resolution_clock::now();

	// Get duration of the quicksort algorithm
	//auto duration = duration_cast<microseconds>(stop - start);
	long double duration = double(time) / CLOCKS_PER_SEC;

	return duration;
}

void Products::QuickSort(Sale **sales, int size)
{
	if (size <= 1)
	{
		return;
	}
	//pivot is the first element of the array

	//the first element which could be swapped with pivot
	//it is 1 because pivot is always 0
	int after_piv = 1;

	//partition
	for (int i = 1; i < size; i++)
	{
		if (*(sales[i]) < *(sales[0]))
			swap(sales[after_piv++], sales[i]);
	}

	swap(sales[0], sales[after_piv - 1]);
	QuickSort(sales, after_piv - 1);
	QuickSort(sales + after_piv, size - after_piv);
}

void Products::rewind()
{
	//rewinding back to the old state for sorting more than once and calculating the exec. time
	for (int i = 0; i < this->size; i++)
	{
		this->sales[i] = this->backup[i];
	}
}

void Products::write_file(string filename)
{
	ofstream file;
	file.open(filename);

	if (!file)
	{
		cerr << "File cannot be created to write!";
	}

	//writing header
	file << "Country\tItem Type\tOrder ID\tUnits Sold\tTotal Profit\n";

	//writing into file
	for (int i = 0; i < this->size; i++)
	{

		file << this->sales[i]->country + "\t" +
					this->sales[i]->item_type + "\t" +
					this->sales[i]->order_id + "\t" +
					to_string(this->sales[i]->units_sold) + "\t"
			 // << this->sales[i]->total_profit << "\n";
			 << fixed << setprecision(2) << this->sales[i]->total_profit << "\n";
	}

	file.close();
}

int main(int argc, char *argv[])
{

	int N = atoi(argv[1]);			//The value of N read from command line
	long double total_duration = 0; // total duration of sorts
	int test_count = 1;				//number of test
	Products P("sales.txt", N);
	//Products P("sorted.txt", N);
	for (int i = 0; i < test_count; i++)
	{
		P.rewind(); //bring it back to the original state for re-sorting
		long double duration = P.sort();
		total_duration += duration;
		printf("test: [%d/%d] => %Lf seconds elapsed in %d. sort\n", i + 1, test_count, duration, i + 1);
	}
	cout << endl
		 << "the average duration in " << N << " input size and" << test_count << "iterations is: " << fixed << total_duration / double(test_count) << " seconds" << endl;
	//P.write_file("sorted2.txt");
	P.write_file("sorted.txt");

	return 0;
}
