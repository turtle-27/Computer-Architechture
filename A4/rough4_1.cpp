#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <iterator>
#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <algorithm>

using namespace std;

map<int, string> memory;

int main(int argc, char* argv[])
{
    fstream file;
    file.open(argv[1], ios::in);
    
    int key = 0;
    string line;
    vector<string> result;

    while(getline(file, line))
    { 
        boost::trim_left(line);
        boost::trim_right(line);

        if (line.empty() || line.substr(0,1) == "#")
        {
            continue;
        }
        
        ::memory[key] = line;
        key += 4;
    }
    
    file.close();

    int key1 = 0;
    string input;
    while(key1 < key)
    {
        input = ::memory[key1];
        input = input.substr(0, input.find("#", 0));
        cout << input << endl;
        
        boost::replace_all(input, ",", "");

        boost::split(result, input, boost::is_any_of("("));

        for (int i = 0; i < result.size(); i++)
        {
            cout << result[i] << endl;
        }
        cout << endl;

        key1 += 4;
    }
    
    return 0;
}