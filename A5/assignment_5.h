#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <map>
#include <bits/stdc++.h> 

using namespace std;


struct MRM_execution 
{
    int dest;
    int col;
    int start_cycle;
    int end_cycle;
    int core;
    int type;
    string register_name;
    int address;
};

struct DramNode
{
    int type;
    int dest;
    int reg1;
    int value;
    int row;
    int memory_add;
    int instr_pos;
    int core;
    string reg_name;
};

int execute(int key, int row_access_delay, int col_access_delay, int core, int memory_size_per_core, ofstream core_array[]);
int execute_lw_sw(DramNode Node, int row_access_delay, int col_access_delay, int memory_size_per_core, ofstream core_array[]);


#define MAX_INSTRUCTIONS 999
#define MAX_CORES 10
#define MAX_DRAM_SIZE 100
#define MRM_DELAY 3;
// Global variables
int inst_exec[MAX_CORES] = {0};
string init_msg;
int init_time;
map<int, string> memory; // hash map represents memory i.e DRAM
vector<map<int, string>> instr_memory; // Vector storing instrustions for multiple cores.
int registers[MAX_CORES][33]; // array representing registers
int bufferROW[1024]; // array for bufferROW
int buffer_update_count = 0;
int clk_cycle_count = 0;
vector<DramNode> dram_vector;
int activeROW = -1;
vector<DramNode> MRM_vector;
vector<DramNode> temp_vector;
int MRM_state;
int MRM_BusyTime = 0;
MRM_execution MRM_Ex;
bool status = false;
int PC[MAX_CORES] = {0};
bool isBusy[MAX_CORES] = {false}; // checks if any dependent intruction is on hold.

// Read the instruction file and store in instr. memory segment
int takeInput(string filename)
{   
    int key = 0; // address of instruction
    fstream file;
    file.open(filename, ios::in); //open a file to perform read operation using file object
    
    //checking whether the file is open
    if (file.is_open())
    {   
        
        map<int, string> input_map;
        string line;
        //read data from file object and put it into string.
        while(getline(file, line))
        { 
            if (key > MAX_INSTRUCTIONS)
            {
                cout << "Error: No. of instructions exceeded max limit." << endl;
                return -1;
            }
            
            boost::trim_left(line);
            boost::trim_right(line);

            if (line.empty())
            {
                continue;
            }
            else
            {
                input_map[key] = line;
                key += 4;
            }
        }

        instr_memory.push_back(input_map);

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

int MRM_function(DramNode Node, int row_access_delay, int col_access_delay, int memory_size_per_core, ofstream core_array[])
{
    if (MRM_BusyTime == 0)
    {
        MRM_state = 0;
        MRM_Ex.core = -1;
    }
    
    if (MRM_state == 0)
    {
        MRM_state = 1;
        MRM_Ex.core = Node.core;
        execute_lw_sw(Node, row_access_delay, col_access_delay, memory_size_per_core, core_array);
    }
    else
    {  
        MRM_vector.push_back(Node);
    }
}


int execute_lw_sw(DramNode Node, int row_access_delay, int col_access_delay, int memory_size_per_core, ofstream core_array[])
{   
    int address = Node.memory_add;
    int dest = Node.dest;
    int reg1 = Node.reg1;
    int row_temp = Node.row; 
    int value = Node.value;
    string register_name = Node.reg_name;
    int type = Node.type;
    int core = Node.core;

    MRM_Ex.type = type;
    MRM_Ex.register_name = register_name;
    MRM_Ex.address = address;

    int address_lower_limit = core*memory_size_per_core;
    int address_upper_limit = ((core+1)*memory_size_per_core);

    if (type == 0)
    {
            
        for (auto it = MRM_vector.begin(); it != MRM_vector.end(); it++)
        {
            DramNode element = *it;
            if (element.memory_add == address && element.type == 1 && element.core == core)
            {
                MRM_state = 0;
                MRM_function(element, row_access_delay, col_access_delay, memory_size_per_core, core_array);
                MRM_vector.erase(it);
                it--;
                temp_vector.insert(temp_vector.begin(), Node);
                return 0;
            }
        }
        

        if (address >= address_lower_limit && address < address_upper_limit)
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
                    ::bufferROW[i] = stoi(::memory.at(row*(1024) + i));
                }
                MRM_Ex.col = col;
                MRM_Ex.dest = dest;
                MRM_Ex.start_cycle = clk_cycle_count + 1;
                MRM_Ex.end_cycle = clk_cycle_count + row_access_delay + col_access_delay-1;
                // ::registers[dest] = ::bufferROW[col];
                // core_array[core] << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + row_access_delay << ": Row-" << row << " Activated" <<  endl;
                // core_array[core] << "cycle " << clk_cycle_count + row_access_delay+1 << "-" << clk_cycle_count + row_access_delay + col_access_delay << ": Column Access => " << register_name << " = " << ::registers[dest] << endl;
                // clk_cycle_count += row_access_delay + col_access_delay;
                MRM_BusyTime = row_access_delay + col_access_delay-1;
            }
            else if (activeROW == row)
            {
                MRM_Ex.col = col;
                MRM_Ex.dest = dest;
                MRM_Ex.start_cycle = clk_cycle_count + 1;
                MRM_Ex.end_cycle = clk_cycle_count + col_access_delay-1;
                // ::registers[dest] = ::bufferROW[col];
                // core_array[core] << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + col_access_delay << ": Column Access => " << register_name << " = " << ::registers[dest] << endl;
                // clk_cycle_count += col_access_delay;
                MRM_BusyTime = col_access_delay-1;
            }
            else
            {
                buffer_update_count++;
                
                for (int i = 0; i < 1024; i++)
                {
                    ::memory[activeROW*1024 + i] = to_string(::bufferROW[i]);
                }
                
                
                for (int i = 0; i < 1024; i++)
                {
                    ::bufferROW[i] = stoi(::memory.at(row*(1024) + i));
                }
                MRM_Ex.col = col;
                MRM_Ex.dest = dest;
                MRM_Ex.start_cycle = clk_cycle_count + 1;
                MRM_Ex.end_cycle = clk_cycle_count + 2*row_access_delay + col_access_delay-1;
                // ::registers[dest] = ::bufferROW[col];
                // core_array[core] << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + 2*row_access_delay  << ": Row-" << activeROW << " Writeback and Row-" << row << " Activated" << endl;
                // core_array[core] << "cycle " << clk_cycle_count + 2*row_access_delay+1 << "-" << clk_cycle_count + 2*row_access_delay + col_access_delay  << ": Column Access => " << register_name << " = " << ::registers[dest] << endl;
                // clk_cycle_count += 2*row_access_delay + col_access_delay;
                activeROW = row;
                MRM_BusyTime = 2*row_access_delay + col_access_delay-1;
            }
        }
        else
        {
            cout << "Error: Memory location not in data memory segment." << endl;
            return -1;
        }
        init_time = clk_cycle_count + MRM_DELAY;
        init_msg = "cycle " +  to_string(init_time) + ":" + " lw " + register_name + " initiated";
    }
    else if (type == 1)
    {                    
        if (address >= address_lower_limit && address < address_upper_limit)
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
                    ::bufferROW[i] = stoi(::memory.at(row*(1024) + i));
                }
                MRM_Ex.col = col;
                MRM_Ex.dest = value;
                MRM_Ex.start_cycle = clk_cycle_count + 1;
                MRM_Ex.end_cycle = clk_cycle_count + row_access_delay + col_access_delay-1;
                // ::bufferROW[col] = value;
                // core_array[core] << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + row_access_delay << ": Row-" << row << " Activated" <<  endl;
                // core_array[core] << "cycle " << clk_cycle_count + row_access_delay+1 << "-" << clk_cycle_count + row_access_delay + col_access_delay << ": Column Access => memory address " << address << "-" << address+3 << " = " << ::bufferROW[col] << endl;
                // clk_cycle_count += row_access_delay + col_access_delay;
                MRM_BusyTime = row_access_delay + col_access_delay-1;
            }
            else if (activeROW == row)
            {
                MRM_Ex.col = col;
                MRM_Ex.dest = value;
                MRM_Ex.start_cycle = clk_cycle_count + 1;
                MRM_Ex.end_cycle = clk_cycle_count + col_access_delay-1;
                // buffer_update_count++;
                // ::bufferROW[col] = value;
                // core_array[core] << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + col_access_delay << ": Column Access => memory address " << address << "-" << address+3 << " = " << ::bufferROW[col] << endl;
                // clk_cycle_count += col_access_delay;
                MRM_BusyTime = col_access_delay-1;
            }
            else
            {
                buffer_update_count += 2;
                
                for (int i = 0; i < 1024; i++)
                {
                    ::memory[activeROW*1024 + i] = to_string(::bufferROW[i]);
                }
                
                for (int i = 0; i < 1024; i++)
                {
                    ::bufferROW[i] = stoi(::memory.at(row*(1024) + i));
                }

                MRM_Ex.col = col;
                MRM_Ex.dest = value;
                MRM_Ex.start_cycle = clk_cycle_count + 1;
                MRM_Ex.end_cycle = clk_cycle_count + 2*row_access_delay + col_access_delay-1;
                // ::bufferROW[col] = value;
                // core_array[core] << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + 2*row_access_delay  << ": Row-" << activeROW << " Writeback and Row-" << row << " Activated" << endl;
                // core_array[core] << "cycle " << clk_cycle_count + 2*row_access_delay+1 << "-" << clk_cycle_count + 2*row_access_delay + col_access_delay << ": Column Access => memory address " << address << "-" << address+3 << " = " << ::bufferROW[col] << endl;
                // clk_cycle_count += 2*row_access_delay + col_access_delay;
                activeROW = row;
                MRM_BusyTime = 2*row_access_delay + col_access_delay -1;
            }
        }
        else
        {
            cout << "Error: Memory location not in data memory segment." << endl;
            return -1;
        }
        init_time = clk_cycle_count + MRM_DELAY;
        init_msg = "cycle " + to_string(init_time) + ":" + " sw " + register_name + " initiated" ;
    }
    MRM_BusyTime += MRM_DELAY;
    MRM_Ex.end_cycle += MRM_DELAY;
    return 0;
}

// function for executing instructions
int executeInstructions(int instr_count, int row_access_delay, int col_access_delay, int core, int memory_size_per_core, ofstream core_array[])
{
    // map<int, string> temp_map1 = instr_memory[core];
    // string input1 = temp_map1[PC[core]];
    // cout << "exInst" << "I: " << core << " " << input1 << endl;
    int key = PC[core];
    if (key < 4*instr_count && key != -3)
    {
        key = execute(key, row_access_delay, col_access_delay, core, memory_size_per_core, core_array);  
        PC[core] = key;
        if (key == -1)
        {
            return -2; 
        }  
    }
    
    
    return 0;
}

