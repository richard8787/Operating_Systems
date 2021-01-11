#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
using namespace std;
const long long partition_num = 700000;
const long long buffer_size = 7000000;
long long key;
string value;
string instr;
string new_value;
string temp_value;
long long temp_key;
long long from, tail;
long long choose = 0;
vector<string> temp_f;
string g_name;
string input_f;
string output_f;
map<long long, string> input_buffer[partition_num];
map<long long, string>::iterator iter;
vector<string> output_buffer;
void release_input_buffer();
void release_output_buffer();
long long input_buffer_counter = 0;

int main(int argc, char *argv[])
{
    time_t t_start, t_end;
    t_start = time(NULL);
    int get = mkdir("storage", S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    input_f = argv[1];

    //partition the file name
    for (long long i = 0; i < partition_num; i++)
    {
        g_name = "storage/" + to_string(i) + ".temp";
        temp_f.push_back(g_name);
    }

    //set output file name
    int getsite = -1;
    int getdot = -1;
    for (int i = 0; i < input_f.size(); i++)
    {
        if (input_f[i] == '/')
            getsite = i;

        if (input_f[i] == '.')
            getdot = i;
    }
    output_f = input_f.substr(getsite + 1, getdot - getsite - 1) + ".output";

    //start input
    ifstream input_file(input_f);
    while (input_file >> instr)
    {
        //PUT start
        if (instr == "PUT")
        {
            if (input_buffer_counter >= buffer_size)
                release_input_buffer();
            input_file >> key >> value;
            choose = key % partition_num;
            input_buffer[choose][key] = value;
            input_buffer_counter++;
        }
        //PUT end

        //GET start
        if (instr == "GET")
        {
            if (output_buffer.size() >= buffer_size)
                release_output_buffer();
            input_file >> key;
            choose = key % partition_num;
            new_value = "";
            iter = input_buffer[choose].find(key);
            if (iter != input_buffer[choose].end())
                new_value = iter->second;

            if (new_value == "")
            {
                choose = key % partition_num;
                ifstream rtemp_file(temp_f[choose]);
                while (rtemp_file >> temp_key >> temp_value)
                {
                    if (temp_key == key)
                        new_value = temp_value;
                }
                rtemp_file.close();
            }
            if (new_value != "")
                output_buffer.push_back(new_value);
            else
                output_buffer.push_back("EMPTY");
        }
        //GET end

        //SCAN start
        if (instr == "SCAN")
        {
            input_file >> from >> tail;
            for (long long i = from; i <= tail; i++)
            {
                if (output_buffer.size() >= buffer_size)
                    release_output_buffer();
                key = i;
                choose = key % partition_num;
                new_value = "";
                iter = input_buffer[choose].find(key);
                if (iter != input_buffer[choose].end())
                    new_value = iter->second;

                if (new_value == "")
                {
                    choose = key % partition_num;
                    ifstream rtemp_file(temp_f[choose]);
                    while (rtemp_file >> temp_key >> temp_value)
                    {
                        if (temp_key == key)
                            new_value = temp_value;
                    }
                    rtemp_file.close();
                }
                if (new_value != "")
                    output_buffer.push_back(new_value);
                else
                    output_buffer.push_back("EMPTY");
            }
        }
        //SCAN end
    }
    input_file.close();
    release_input_buffer();
    if (!output_buffer.empty())
        release_output_buffer();
    t_end = time(NULL);
    cout << "It costs : " << t_end - t_start << " secs" << endl;
}
void release_input_buffer()
{
    //
    //cout << "start to clear input buffer" << endl;
    //
    for (long long i = 0; i < partition_num; i++)
    {
        if (input_buffer[i].empty())
            continue;
        ofstream wtemp_file(temp_f[i], ios::app);
        for (iter = input_buffer[i].begin(); iter != input_buffer[i].end(); iter++)
            wtemp_file << iter->first << " " << iter->second << endl;
        wtemp_file.close();
    }
    for (long long i = 0; i < partition_num; i++)
        input_buffer[i].clear();
    input_buffer_counter = 0;
    //
    //cout << "clear input buffer finish" << endl;
    //
}
void release_output_buffer()
{
    //
    //cout << "start to clear output buffer" << endl;
    //
    ofstream output_file(output_f, ios::app);
    for (long long i = 0; i < output_buffer.size(); i++)
        output_file << output_buffer[i] << endl;
    output_file.close();
    output_buffer.clear();
    output_buffer.shrink_to_fit();
    //
    //cout << "clear output buffer finish" << endl;
    //
}