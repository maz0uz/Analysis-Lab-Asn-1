#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <queue>

using namespace std;
using namespace std::filesystem;

class Compare { // custom comparator for priority queue
public:
    bool operator()(vector<int> a, vector<int> b)
    {   // Defining priority on the basis of size
        return a.size() > b.size();
    }
};
vector<int> read(string filename) // read function takes a filename and returns a vector containing all the integers stored in the file
{
    ifstream inputFile(filename);
    vector<int> contents;

    if (!inputFile.is_open())
    {
        cout << "Error opening file" << endl;
        return contents;
    }

    string temp;

    while (getline(inputFile, temp)) // each number much be stored on a separate line.
    {
        contents.push_back(stoi(temp)); // stoi converts the string read to an integer to allow more efficient comparisons
    }
    inputFile.close();
    return contents;
}

vector<int> merge(vector<int> a, vector<int> b) // merge function takes two integer vectors and merges them
{
    vector<int> result;
    vector<int>::iterator itera = a.begin();
    vector<int>::iterator iterb = b.begin();
    while(itera != a.end() && iterb != b.end())
    {
        if(*itera > *iterb) // if b is smaller than a, then be is added to the result vector and its iterator is incremented
        {
            result.push_back(*iterb);
            iterb++;
        }else // the same as the other condition however for when a is smaller than b
        {
            result.push_back(*itera);
            itera++;
        }
    }
    return result;
}
void save(vector<int> list, string filename) // save function stores the result into a file. the name of the file is passed as a parameter
{
    vector<int>::iterator iter = list.begin();
    ofstream out(filename);
    for(iter; iter<list.end(); iter++) // outputs the numbers 1 by 1 into the file
    {
        out<<(*iter)<<endl;
    }
    out.close();
}

int main()
{
    vector<string> files;
    cout << "Enter input folder path:"<<endl;
    string input;
    getline(cin, input, '\n');
    cout << "Enter output file path:"<<endl;
    string output;
    getline(cin, output, '\n');

    string temp;

    for (const auto& entry : directory_iterator(input)) // for loop utilises the filesystem directory to get a list of all the files which are stored into a vector
        files.push_back(entry.path().generic_string());
    vector<string>::iterator iter = files.begin();

    priority_queue<vector<int>,vector<vector<int>>, Compare> pq; // a priority queue is made which allows the shortest file to be selected in O(1) complexity
    for(iter;iter<files.end();iter++)
    {
        pq.push(read(*iter)); // the files are passed to the read function and the vector containing the numbers in the file is added to the priority queue
    }

    vector<int> temp1,temp2;
    while(pq.size() > 1)
    {
        temp1 = pq.top();
        pq.pop();
        temp2 = pq.top();
        pq.pop();
        pq.push(merge(temp1,temp2)); // the top 2 elements in the prioirity queue are merged as they are the shortest and then pushed back onto the queue. This terminates when only 1 vector is left.
    }
    save(pq.top(), output); // the final vector is saved to the file.
}