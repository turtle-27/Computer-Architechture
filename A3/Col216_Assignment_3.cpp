// INCLUDES
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

using namespace std;

// Global variables 
map<int, string> memory; // hash map represents memory
int NOI[10]; // no. of each instruction
int registers[32]; // array representing registers
string inst[10] = {"add: ", "sub: ", "mul: ", "beq: ", "bne: ", "slt: ", "j: ", "lw: ", "sw: ", "addi: "};
int takeInput(string filename)
{   
    int key = 0; // address of instruction
    fstream file;
    file.open(filename, ios::in); //open a file to perform read operation using file object
    //checking whether the file is open
    if (file.is_open())
    {   
        string line;
        //read data from file object and put it into string.
        while(getline(file, line))
        { 
            if (key > pow(2,19))
            {
                return -1;
            }
            ::memory[key] = line;
            key += 4;
        }
        file.close(); //close the file object.
    }
    return int(key/4);
}

int executeInstructions(int instr_count)
{
    string input; 
    vector<string> result;
    int clk_cycle_count = 0;
    int key = 0; // PC
    bool update = true;
    int dest = -1; 
    while(key < instr_count*4)
    {
        update = true;
        clk_cycle_count++;
        input = ::memory[key];
        cout << "Instruction: " << input << endl;
        boost::split(result, input, boost::is_any_of(","));
        if (result[0].substr(0,3) == "sub")
        {
            ::NOI[1] += 1;
            dest = stoi(result[0].substr(6));
            if(dest == 31)
            {
                return -2;
            }
            ::registers[dest] = ::registers[stoi(result[1].substr(2))] - ::registers[stoi(result[2].substr(2))];
        }
        else if (result[0].substr(0,3) == "mul")
        {
            ::NOI[2] += 1;
            dest = stoi(result[0].substr(6));
            if(dest == 31)
            {
                return -2;
            }
            ::registers[dest] = ::registers[stoi(result[1].substr(2))] * ::registers[stoi(result[2].substr(2))];
        }
        else if (result[0].substr(0,4) == "addi")
        {
            ::NOI[9] += 1;
            dest = stoi(result[0].substr(7));
            if(dest == 31)
            {
                return -2;
            }
            ::registers[dest] = ::registers[stoi(result[1].substr(2))] + stoi(result[2].substr(0));
        }
        else if (result[0].substr(0,3) == "add")
        {
            ::NOI[0] += 1;
            dest = stoi(result[0].substr(6));
            if(dest == 31)
            {
                return -2;
            }
            ::registers[dest] = ::registers[stoi(result[1].substr(2))] + ::registers[stoi(result[2].substr(2))];
        }
        else if (result[0].substr(0,3) == "beq")
        {
            ::NOI[3] += 1;
            if (::registers[stoi(result[0].substr(6))] == ::registers[stoi(result[1].substr(2))])
            {
                int line_no = stoi(result[2].substr(0));
                key = 4*(line_no - 1);
                update = false;
            }  
        }
        else if (result[0].substr(0,3) == "bne")
        {
            ::NOI[4] += 1;
            if (::registers[stoi(result[0].substr(6))] != ::registers[stoi(result[1].substr(2))])
            {
                int line_no = stoi(result[2].substr(0));
                key = 4*(line_no - 1);
                update = false;
            }  
        }
        else if (result[0].substr(0,3) == "slt")
        {
            ::NOI[5] += 1;
            if (::registers[stoi(result[1].substr(2))] < ::registers[stoi(result[2].substr(2))])
            {
                ::registers[stoi(result[0].substr(6))] = 1;
            }
            else
            {
                ::registers[stoi(result[0].substr(6))] = 0;
            }
        }
        else if (result[0].substr(0,1) == "j")
        {
            ::NOI[6] += 1;
            int line_no = stoi(result[0].substr(2));
            key = 4*(line_no - 1);
            if (key > instr_count)
            {
                return -3;
            }
            update = false;
        }
        else if (result[0].substr(0,2) == "lw")
        {
            ::NOI[7] += 1;
            if (stoi(result[1]) >= pow(2,19))
            {
                dest = stoi(result[0].substr(5));
                if(dest == 31)
                {
                    return -2;
                }
                ::registers[dest] = stoi(::memory.at(stoi(result[1])));
            }
            else
            {
                return -1;
            }
        }
        else if (result[0].substr(0,2) == "sw")
        {
            ::NOI[8] += 1;
            if (stoi(result[1].substr(0)) >= pow(2,19))
            {
                ::memory[stoi(result[1].substr(0))] = to_string(::registers[stoi(result[0].substr(5))]);
            }
            else
            {
                return -1;
            }
        }
        
        if (update)
        {
            key += 4;
        }
        
        for (int i = 0; i < 32; i++) 
        {
        cout << "R" << to_string(i) << ": " << registers[i] << endl;
        } 
    }
    return clk_cycle_count;
}

int main(int argc, char* argv[])
{
    int takeInput_return = takeInput(argv[1]);
    if (takeInput_return == -1)
    {
        cout << "Error: Memory address out of bounds. Memory size is 2^19 bytes." << endl;
        return -1;
    }
    
    int executeInstructions_return = executeInstructions(takeInput_return);
    if (executeInstructions_return == -1)
    {
        cout << "Error: Cannot access instructions memory segment." << endl;  
        return -1;  
    } 
    else if (executeInstructions_return == -2)
    {
        cout << "Error: Cannot change zero register's content." << endl;
        return -1;
    }
    else if (executeInstructions_return == -3)
    {
        cout << "Error: Memory location out of instructions memory segment." << endl;
        return -1;
    }
    cout << "Clock Cycle Count: " << executeInstructions_return << endl;
    for(int i =0; i<10; i++)
    {
        cout << inst[i] << NOI[i] << endl; 
    }
    return 0;
}