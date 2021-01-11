#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;
void data_processing(vector<int[21]> &data, int num_of_thread, int order);
vector<string> read;
int count = 0;
string srod = "|";
char crod = '|';
int main()
{
    int num_of_thread;
    time_t start, input_end, data_processing_end, output_end;
    cout << "please enter the number of thread 1~6 :";
    cin >> num_of_thread;
    cout << endl;
    if (num_of_thread < 1 || num_of_thread > 6)
    {
        cout << "error, please enter the number of thread 1~6" << endl;
        exit(-1);
    }

    //input
    start = time(NULL);
    cout << "start to do input" << endl;
    ifstream input_file("input.csv");
    string temp;
    while (input_file >> temp)
    {
        temp = srod + to_string(count) + srod + temp + srod;
        read.push_back(temp);
        count++;
    }
    input_file.close();
    input_end = time(NULL);
    cout << "input is over, input costs " << input_end - start << " secs" << endl;
    //input end

    //data processing
    cout << "start to do data processing" << endl;
    vector<int[21]> data(count);
    if (num_of_thread == 1)
        data_processing(data, 1, 0);
    else if (num_of_thread == 2)
    {
        thread t1(data_processing, ref(data), num_of_thread, 0);
        t1.join();
    }
    else if (num_of_thread == 3)
    {
        thread t1(data_processing, ref(data), num_of_thread, 0);
        thread t2(data_processing, ref(data), num_of_thread, 1);
        t1.join();
        t2.join();
    }
    else if (num_of_thread == 4)
    {
        thread t1(data_processing, ref(data), num_of_thread, 0);
        thread t2(data_processing, ref(data), num_of_thread, 1);
        thread t3(data_processing, ref(data), num_of_thread, 2);
        t1.join();
        t2.join();
        t3.join();
    }
    else if (num_of_thread == 5)
    {
        thread t1(data_processing, ref(data), num_of_thread, 0);
        thread t2(data_processing, ref(data), num_of_thread, 1);
        thread t3(data_processing, ref(data), num_of_thread, 2);
        thread t4(data_processing, ref(data), num_of_thread, 3);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
    else if (num_of_thread == 6)
    {
        thread t1(data_processing, ref(data), num_of_thread, 0);
        thread t2(data_processing, ref(data), num_of_thread, 1);
        thread t3(data_processing, ref(data), num_of_thread, 2);
        thread t4(data_processing, ref(data), num_of_thread, 3);
        thread t5(data_processing, ref(data), num_of_thread, 4);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
    }
    data_processing_end = time(NULL);
    cout << "data processing is over, data processing costs " << data_processing_end - input_end << " secs" << endl;
    //data processing end

    //output
    cout << "start to do output" << endl;
    ofstream output_file("output.json");
    output_file << "[" << endl;
    for (int i = 0; i < count; i++)
    {
        output_file << "    {" << endl;
        for (int j = 1; j < 21; j++)
        {
            output_file << "        \"col_" << j << "\":";
            if (j != 20)
                output_file << data[i][j] << "," << endl;
            else
                output_file << data[i][j] << endl;
        }
        if (i != count - 1)
            output_file << "    }," << endl;
        else
            output_file << "    }" << endl;
    }
    output_file << "]" << endl;
    output_file.close();
    output_end = time(NULL);
    cout << "output is over, output costs " << output_end - data_processing_end << " secs" << endl;
    //output end

    cout << "program is over, total costs " << output_end - start << " secs" << endl;
}
void data_processing(vector<int[21]> &data, int num_of_thread, int order)
{
    string temp;
    int push_in;
    int c = 0;
    int first = 0;
    int second = 0;
    if (num_of_thread != 1)
        num_of_thread -= 1;
    for (int i = count * order / num_of_thread; i < count * (order + 1) / num_of_thread; i++)
    {
        first = 0;
        second = 0;
        c = 0;
        while (c < 21)
        {
            if (read[i][second] == crod)
            {
                if (first != second)
                {
                    temp = read[i].substr(first + 1, second - first - 1);
                    push_in = stoi(temp);
                    data[i][c] = push_in;
                    c++;
                }

                first = second;
                second++;
            }
            else
                second++;
        }
    }
}