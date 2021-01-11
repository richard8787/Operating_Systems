#include <algorithm>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
using namespace std;

const int mem_size = 1024 * 1024 * 1024;
const int file_size = mem_size / sizeof(int);

struct Compare
{
    //use for heap
    bool operator()(pair<int, int> the1, pair<int, int> the2)
    {
        return the1.first >= the2.first;
    }
};

void partition(int *const values, const int n_of_partition, const int partition_size)
{
    string temp = ("partition" + to_string(n_of_partition) + ".txt");
    ofstream partition_file(temp.c_str());

    //put data to partition
    for (int i = 0; i < partition_size; i++)
        partition_file << values[i] << '\n';
    partition_file.close();
}

int main()
{
    clock_t start, partition_end, merge_end; //to check time
    start = clock();
    ifstream input_txt("input.txt");
    cout << "start to partition" << endl;

    int *file_ptr = new int[file_size]; //to check the site
    int n_of_partition = 1;
    int temp_read = 0;
    int partition_size = 0;
    int is_over = 0;

    //start to read file
    while (input_txt >> temp_read)
    {
        is_over = 0;
        file_ptr[partition_size] = temp_read;
        partition_size++;
        if (partition_size == file_size)
        {
            sort(file_ptr, file_ptr + partition_size);           //sort the file
            partition(file_ptr, n_of_partition, partition_size); //to do partition
            n_of_partition++;
            partition_size = 0;
            is_over = 1;
        }
    }

    //to check is over or not
    if (is_over == 0)
    {
        sort(file_ptr, file_ptr + partition_size);
        partition(file_ptr, n_of_partition, partition_size);
    }
    else
        n_of_partition--;

    //read input over
    input_txt.close();
    delete[] file_ptr;

    partition_end = clock();
    cout << "partition is over!" << endl;
    cout << "partition costs " << double(partition_end - start) / CLOCKS_PER_SEC << " secs" << endl;
    cout << "start to merge" << endl;

    //start to merge
    const string temp{"output.txt"};
    ofstream output_txt(temp.c_str());
    priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> heap; //build the heap
    ifstream *temp_ptr = new ifstream[n_of_partition];                    //to checkt the site

    //push into heap
    for (int i = 1; i <= n_of_partition; i++)
    {
        int temp_value = 0;
        string the_file_name = ("partition" + to_string(i) + ".txt");
        temp_ptr[i - 1].open(the_file_name.c_str());

        if (temp_ptr[i - 1].is_open())
        {
            temp_ptr[i - 1] >> temp_value;
            pair<int, int> top(temp_value, (i - 1));
            heap.push(top); //push the top
        }
    }

    while (heap.size() > 0) //do the whole heap
    {
        int the_next = 0;
        pair<int, int> thepair = heap.top();
        heap.pop();                          //pop the heap
        output_txt << thepair.first << '\n'; //put to output
        flush(output_txt);

        if (temp_ptr[thepair.second] >> the_next)
        {
            pair<int, int> p1(the_next, thepair.second);
            heap.push(p1);
        }
    }

    //close and release mem
    for (int i = 1; i <= n_of_partition; i++)
    {
        temp_ptr[i - 1].close();
    }
    output_txt.close();
    delete[] temp_ptr;
    //merged end

    merge_end = clock();
    cout << "merge is over!" << endl;
    cout << "merge costs " << double(merge_end - partition_end) / CLOCKS_PER_SEC << " secs" << endl;
    cout << "total costs " << double(merge_end - start) / CLOCKS_PER_SEC << " secs" << endl;
    cout << "external sort is over!" << endl;
}