#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <map>
#include <bits/stdc++.h> 

using namespace std;

int execute(int key, int row_access_delay, int col_access_delay);

struct DramNode
{
    int type;
    int dest;
    int reg1;
    int value;
    int row;
    int memory_add;
    int instr_pos;
    string reg_name;
};

struct DramNode dramnode;

// Global variables 
map<int, string> memory; // hash map represents memory
int registers[33]; // array representing registers
int bufferROW[1024]; // array for bufferROW
int buffer_update_count = 0;
int clk_cycle_count = 0;
vector<DramNode> dram_vector;
int activeROW = -1;

// Read the instruction file and store in instr. memory segment
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
            if (key > 999)
            {
                cout << "Error: Cannot access data memory segment." << endl;
                return -1;
            }
            
            boost::trim_left(line);
            boost::trim_right(line);

            if (line.empty())
            {
                continue;
            }

            memory[key] = line;
            key += 4;
        }

        file.close(); //close the file object.
        
        return int(key/4);
    }
    else
    {
        cout << "Unable to open file." << endl;
        return -1;
    }
}


bool comparator(const DramNode &a, const DramNode &b)
{
    return a.row < b.row;
}

int execute_lw_sw(DramNode Node, int row_access_delay, int col_access_delay)
{   
    int address = Node.memory_add;
    int dest = Node.dest;
    int reg1 = Node.reg1;
    int row_temp = Node.row; 
    int value = Node.value;
    string register_name = Node.reg_name;
    int type = Node.type;

    if (type == 0)
    {
            
        for (auto it = dram_vector.begin(); it != dram_vector.end(); it++)
        {
            DramNode element = *it;
            if (element.memory_add == address && element.type == 1)
            {
                execute_lw_sw(element, row_access_delay, col_access_delay);
                dram_vector.erase(it);
                it--;
            }
        }
        

        if (address >= 1000 && address < pow(2,20))
        {
            if (address % 4 != 0)
            {
                cout << "Error: Memory addresses which are not multiples 4 of cannot be accessed." << endl;
                return -1;
            }
            
            int row = address / 1024;
            int col = address % 1024;
            
            
            if (activeROW == -1)
            {
                activeROW = row;
                buffer_update_count++;
                for (int i = 0; i < 1024; i++)
                {
                    if ((row == 0 && i < 1000 ) || i % 4 != 0)
                    {
                        ::bufferROW[i] = 0;
                    }
                    else
                    {
                        ::bufferROW[i] = stoi(::memory.at(row*(1024) + i));
                    }
                }
                ::registers[dest] = ::bufferROW[col];
                cout << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + row_access_delay << ": Row-" << row << " Activated" <<  endl;
                cout << "cycle " << clk_cycle_count + row_access_delay+1 << "-" << clk_cycle_count + row_access_delay + col_access_delay << ": Column Access => " << register_name << " = " << ::registers[dest] << endl;
                clk_cycle_count += row_access_delay + col_access_delay;
            }
            else if (activeROW == row)
            {
                ::registers[dest] = ::bufferROW[col];
                cout << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + col_access_delay << ": Column Access => " << register_name << " = " << ::registers[dest] << endl;
                clk_cycle_count += col_access_delay;
            }
            else
            {
                buffer_update_count++;
                
                for (int i = 0; i < 1024; i++)
                {
                    if (activeROW == 0 && i < 1000)
                    {
                        continue;
                    }
                    else
                    {
                        ::memory[activeROW*1024 + i] = to_string(::bufferROW[i]);
                    } 
                }
                
                
                for (int i = 0; i < 1024; i++)
                {
                    if (row == 0 && i < 1000)
                    {
                        ::bufferROW[i] = 0;
                    }
                    else
                    {
                        ::bufferROW[i] = stoi(::memory.at(row*(1024) + i));
                    }
                }
                ::registers[dest] = ::bufferROW[col];
                cout << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + 2*row_access_delay  << ": Row-" << activeROW << " Writeback and Row-" << row << " Activated" << endl;
                cout << "cycle " << clk_cycle_count + 2*row_access_delay+1 << "-" << clk_cycle_count + 2*row_access_delay + col_access_delay  << ": Column Access => " << register_name << " = " << ::registers[dest] << endl;
                clk_cycle_count += 2*row_access_delay + col_access_delay;
                activeROW = row;
            }
            
        }
        else
        {
            cout << "Error: Memory location not in data memory segment." << endl;
            return -1;
        }
    }
    else if (type == 1)
    {                    
        if (address >= 1000 && address < pow(2,20))
        {
            if (address % 4 != 0)
            {
                cout << "Error: Memory addresses which are not multiples 4 of cannot be accessed." << endl;
                return -1;
            }
            int row = address / 1024;
            int col = address % 1024;
            



            if (activeROW == -1)
            {
                activeROW = row;
                buffer_update_count += 2;
                for (int i = 0; i < 1024; i++)
                {
                    if ((row == 0 && i < 1000 ) || i % 4 != 0)
                    {
                        ::bufferROW[i] = 0;
                    }
                    else
                    {
                        ::bufferROW[i] = stoi(::memory.at(row*(1024) + i));
                    }
                }
                ::bufferROW[col] = value;
                cout << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + row_access_delay << ": Row-" << row << " Activated" <<  endl;
                cout << "cycle " << clk_cycle_count + row_access_delay+1 << "-" << clk_cycle_count + row_access_delay + col_access_delay << ": Column Access => memory address " << address << "-" << address+3 << " = " << ::bufferROW[col] << endl;
                clk_cycle_count += row_access_delay + col_access_delay;
            }
            else if (activeROW == row)
            {
                buffer_update_count++;
                ::bufferROW[col] = value;
                cout << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + col_access_delay << ": Column Access => memory address " << address << "-" << address+3 << " = " << ::bufferROW[col] << endl;
                clk_cycle_count += col_access_delay;
            }
            else
            {
                buffer_update_count += 2;
                
                for (int i = 0; i < 1024; i++)
                {
                    if (activeROW == 0 && i < 1000)
                    {
                        continue;
                    }
                    else
                    {
                        ::memory[activeROW*1024 + i] = to_string(::bufferROW[i]);
                    } 
                }
                
                for (int i = 0; i < 1024; i++)
                {
                    if (row == 0 && i < 1000)
                    {
                        ::bufferROW[i] = 0;
                    }
                    else
                    {
                        ::bufferROW[i] = stoi(::memory.at(row*(1024) + i));
                    }
                }
                ::bufferROW[col] = value;
                cout << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + 2*row_access_delay  << ": Row-" << activeROW << " Writeback and Row-" << row << " Activated" << endl;
                cout << "cycle " << clk_cycle_count + 2*row_access_delay+1 << "-" << clk_cycle_count + 2*row_access_delay + col_access_delay << ": Column Access => memory address " << address << "-" << address+3 << " = " << ::bufferROW[col] << endl;
                clk_cycle_count += 2*row_access_delay + col_access_delay;
                activeROW = row;
            }
        }
        else
        {
            cout << "Error: Memory location not in data memory segment." << endl;
            return -1;
        }
    }
    
    return 0;
}

// function for executing instructions
int executeInstructions(int instr_count, int row_access_delay, int col_access_delay)
{
    int key = 0;
    while (key < 4*instr_count)
    {
        key = execute(key, row_access_delay, col_access_delay);  
        
        if (key == -1)
        {
            return -1; 
        }  
    }

    for (auto it = dram_vector.begin(); it != dram_vector.end(); it++)
    {
        DramNode element = *it;
        execute_lw_sw(element, row_access_delay, col_access_delay);
        dram_vector.erase(it);
        it--;
    }
    
    return 0;
}

