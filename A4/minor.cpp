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
map<int, int> changedMemory; // memory content changed
int DRAM[1024][1024] = {0}; //intialized DRAM to 0.
int rowBuffer[1024] = {0}; // intialized buffer to 0.
int bufferIndex = 0; // maintain index of buffer.
int clk_cycle_count = 0;
int temp_data = 0;
bool flag = true;
int row_buffer_update = 0; // no. of row buffer updates
int ROW_ACCESS_DELAY = 0;
int COL_ACCESS_DELAY = 0;
int temp_clk_cycle = -1;
int part = 0; // Normal Memory access or Non-Blocking Memory accesss 
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
            if (key >= 1000)
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
int nonBlocking(int key, int instr_count, int busyDest,int type);
// createQueue 
//Queue.head -> Gives the head of queue
void Execute()
{
    bool check = check(currInstruction)
    if(check)
    {
        execute
    }
    else
    {
        if(currInstruction == bne or beq)
        {
            first execute queued instrctions
        }
        else
        {
            Queue.add(currInstruction)
        }
        
    }
}
bool check(currInstruction)
{
    if(currInstruction == bne or beq)
    {
        return false;
    }
    else
    {
        if(currInstruction == add)
        {
            dest = dest reg. of currInstruction
            r1 = ......
            r2 = ......
            for elem in queue
            {
                if(elem == add)
                {
                    if(elem.dest == dest or r1 or r2)
                    {
                        return false
                    }
                }
                else if(elem == sub)
                {
                    same as add
                }
                else if(elem == mul)
                {
                    same as mul
                }
                else if(elem == addi)
                {
                    similar as add
                }
                else if(elem == slt)
                {
                    similar as add
                }
                else if(elem = lw)
                {
                    similar as add
                }
                else if(elem == sw)
                {
                    return True
                }
            }
        }
        else if(currInstruction == sub)
        {
            same as add
        }
        else if(currInstructon == mul)
        {
            same as a64l
        }
        else if(currInstruction == addi)
        {
            same as add
        }
        else if(currInstruction == lw)
        {
            for elem in queue
            {
                if(elem == sw)
                {
                    if(lw.add == sw.add)
                    {
                        return false
                    }
                }
            }
        }    
        else if(currInstruction == slt)
        {
            same as add
        }
        else if(currInstruction == sw)
        {
            for elem in queue
            {
                if(elem == sw)
                {
                    if(sw.add == lw.add)
                    {
                        return false
                    }
                }
            }
        }
    }
    return true
}


int executeInstructions(int instr_count, int part,int key)
{
    string input; 
    vector<string> result;
    int intial = 0; // tracking start and end if clock cycle for performing an instruction.
    int final = 0; 
    int memAddress = 0; // memory address given in lw and sw instruction
    int row = 0; // row no. of DRAM
    int col = 0; // col no. of DRAM
    bool update = true;
    int dest = -1; 
    while(key < instr_count*4)
    {
        update = true;
        input = ::memory[key];
        //cout << "Instruction: " << input << endl;
        boost::split(result, input, boost::is_any_of(","));
        if (result[0].substr(0,3) == "sub")
        {
            clk_cycle_count++;
            ::NOI[1] += 1;
            dest = stoi(result[0].substr(6));
            if(dest == 31)
            {
                cout << "Error: Cannot change zero register's content." << endl;
                return -1;
            }
            ::registers[dest] = ::registers[stoi(result[1].substr(2))] - ::registers[stoi(result[2].substr(2))];
            cout <<  "cycle " << to_string(clk_cycle_count) << ": $t" << to_string(dest) << "=" << to_string(::registers[dest]) << endl;
        }
        else if (result[0].substr(0,3) == "mul")
        {
            clk_cycle_count++;
            ::NOI[2] += 1;
            dest = stoi(result[0].substr(6));
            if(dest == 31)
            {
                cout << "Error: Cannot change zero register's content." << endl;
                return -1;
            }
            ::registers[dest] = ::registers[stoi(result[1].substr(2))] * ::registers[stoi(result[2].substr(2))];
             cout <<  "cycle " << to_string(clk_cycle_count) << ": $t" << to_string(dest) << "=" << to_string(::registers[dest]) << endl;
        }
        else if (result[0].substr(0,4) == "addi")
        {
            clk_cycle_count++;
            ::NOI[9] += 1;
            dest = stoi(result[0].substr(7));
            if(dest == 31)
            {
                cout << "Error: Cannot change zero register's content." << endl;
                return -1;
            }
            ::registers[dest] = ::registers[stoi(result[1].substr(2))] + stoi(result[2].substr(0));
             cout <<  "cycle " << to_string(clk_cycle_count) << ": $t" << to_string(dest) << "=" << to_string(::registers[dest]) << endl;
        }
        else if (result[0].substr(0,3) == "add")
        {
            clk_cycle_count++;
            ::NOI[0] += 1;
            dest = stoi(result[0].substr(6));
            if(dest == 31)
            {
                cout << "Error: Cannot change zero register's content." << endl;
                return -1;
            }
            ::registers[dest] = ::registers[stoi(result[1].substr(2))] + ::registers[stoi(result[2].substr(2))];
             cout <<  "cycle " << to_string(clk_cycle_count) << ": $t" << to_string(dest) << "=" << to_string(::registers[dest]) << endl;
        }
        else if (result[0].substr(0,3) == "beq")
        {
            clk_cycle_count++;
            ::NOI[3] += 1;
            if (::registers[stoi(result[0].substr(6))] == ::registers[stoi(result[1].substr(2))])
            {
                int line_no = stoi(result[2].substr(0));
                key = 4*(line_no - 1);
                update = false;
            }
             cout <<  "cycle " << to_string(clk_cycle_count) << ": beq jumps to " << to_string(key) << endl;  
        }
        else if (result[0].substr(0,3) == "bne")
        {
            clk_cycle_count++;
            ::NOI[4] += 1;
            if (::registers[stoi(result[0].substr(6))] != ::registers[stoi(result[1].substr(2))])
            {
                int line_no = stoi(result[2].substr(0));
                key = 4*(line_no - 1);
                update = false;
            }
            cout <<  "cycle " << to_string(clk_cycle_count) << ": bne jumps to " << to_string(key) << endl;  
        }
        else if (result[0].substr(0,3) == "slt")
        {
            clk_cycle_count++;
            ::NOI[5] += 1;
            if (::registers[stoi(result[1].substr(2))] < ::registers[stoi(result[2].substr(2))])
            {
                ::registers[stoi(result[0].substr(6))] = 1;
            }
            else
            {
                ::registers[stoi(result[0].substr(6))] = 0;
            }
            cout <<  "cycle " << to_string(clk_cycle_count) << ": slt jumps to " << to_string(key) << endl;
        }
        else if (result[0].substr(0,1) == "j")
        {
            clk_cycle_count++;
            ::NOI[6] += 1;
            int line_no = stoi(result[0].substr(2));
            key = 4*(line_no - 1);
            if (key > instr_count)
            {
                cout << "Error: Memory location out of instructions memory segment." << endl;
                return -1;
            }
            update = false;
            cout <<  "cycle " << to_string(clk_cycle_count) << ": j jumps to " << to_string(key) << endl;
        }
        else if (result[0].substr(0,2) == "lw")
        {   
            if(part == 1)
            {
                clk_cycle_count++;
                cout << "cycle " << to_string(clk_cycle_count) << ": DRAM request issued" << endl;
                
                //update clock cycle
                intial = clk_cycle_count;
                ::NOI[7] += 1;
                dest = stoi(result[0].substr(5));
                if (stoi(result[1].substr(0)) >= 1000)
                {
                   
                    if(dest == 31)
                    {
                        cout << "Error: Cannot change zero register's content." << endl;
                        return -1;
                    }
                    memAddress = stoi(result[1]);
                    if(memAddress%4 != 0)
                    {
                        cout << "Memory Address should be a multiple of 4.";
                        return -1;
                    }
                    row = memAddress/1024;
                    col = memAddress - 1024*row;
                    if(flag)
                    {
                        memcpy(rowBuffer, DRAM[row], sizeof(rowBuffer));
                        row_buffer_update++;
                        bufferIndex = row;
                        clk_cycle_count += ROW_ACCESS_DELAY;
                        ::registers[dest] = rowBuffer[col];
                        clk_cycle_count += COL_ACCESS_DELAY;
                        flag = false;
                    }
                    else
                    {
                        if(bufferIndex == row)
                        {
                            ::registers[dest] = rowBuffer[col];
                            clk_cycle_count += COL_ACCESS_DELAY;
                            flag = false;
                        }
                        else
                        {
                            memcpy(DRAM[bufferIndex],rowBuffer, sizeof(DRAM[bufferIndex]));
                            clk_cycle_count += ROW_ACCESS_DELAY;
                            memcpy(rowBuffer, DRAM[row], sizeof(rowBuffer));
                            row_buffer_update++;
                            bufferIndex = row;
                            clk_cycle_count += ROW_ACCESS_DELAY;
                            ::registers[dest] = rowBuffer[col];
                            clk_cycle_count += COL_ACCESS_DELAY;
                        }
                    }
                    //::registers[dest] = stoi(::memory.at(stoi(result[1])));
                    final = clk_cycle_count;
                }
                else
                {
                    return -1;
                }
                cout << "cycle " << to_string(intial+1) << "-" << to_string(final) << " : $t" << to_string(dest) << "= " << to_string(rowBuffer[col]) << endl;  
            }
            else if(part == 2)
            {
                //Non-blocking Memory Access
                    clk_cycle_count++;
                    cout << "cycle " << to_string(clk_cycle_count) << ": DRAM request issued" << endl;
                    //update clock cycle
                    temp_clk_cycle = clk_cycle_count;
                    ::NOI[7] += 1;
                    dest = stoi(result[0].substr(5));
                    if (stoi(result[1].substr(0)) >= 1000)
                    {
                    
                        if(dest == 31)
                        {
                            cout << "Error: Cannot change zero register's content." << endl;
                            return -1;
                        }
                        memAddress = stoi(result[1]);
                        if(memAddress%4 != 0)
                        {
                            cout << "Memory Address should be a multiple of 4.";
                            return -1;
                        }
                        row = memAddress/1024;
                        col = memAddress - 1024*row;
                        if(flag)
                        {
                            memcpy(rowBuffer, DRAM[row], sizeof(rowBuffer));
                            row_buffer_update++;
                            bufferIndex = row;
                            temp_clk_cycle += ROW_ACCESS_DELAY;
                            ::registers[dest] = rowBuffer[col];
                            temp_clk_cycle += COL_ACCESS_DELAY;
                            flag = false;
                        }
                        else
                        {
                            if(bufferIndex == row)
                            {
                                ::registers[dest] = rowBuffer[col];
                                temp_clk_cycle += COL_ACCESS_DELAY;
                                flag = false;
                            }
                            else
                            {
                                memcpy(DRAM[bufferIndex],rowBuffer, sizeof(DRAM[bufferIndex]));
                                temp_clk_cycle += ROW_ACCESS_DELAY;
                                memcpy(rowBuffer, DRAM[row], sizeof(rowBuffer));
                                row_buffer_update++;
                                bufferIndex = row;
                                temp_clk_cycle += ROW_ACCESS_DELAY;
                                ::registers[dest] = rowBuffer[col];
                                temp_clk_cycle += COL_ACCESS_DELAY;
                            }
                        }
                        //::registers[dest] = stoi(::memory.at(stoi(result[1])));
                    }
                    else
                    {
                        return -1;
                    }
                //cout << "cycle " << to_string(intial+1) << "-" << to_string(final) << " : $t" << to_string(dest) << "= " << to_string(rowBuffer[col]) << endl;
                key += 4;
                update = false;
                key = nonBlocking(key, instr_count, dest,0);
                if(clk_cycle_count == temp_clk_cycle)
                {
                    cout << "cycle " << to_string(clk_cycle_count) << "-" << to_string(temp_clk_cycle) << " : $t" << to_string(dest) << "= " << to_string(rowBuffer[col]) << endl;
                }
                else
                {
                    cout << "cycle " << to_string(clk_cycle_count+1) << "-" << to_string(temp_clk_cycle) << " : $t" << to_string(dest) << "= " << to_string(rowBuffer[col]) << endl;
                    clk_cycle_count = temp_clk_cycle;
                }
                
            }
        }
        else if (result[0].substr(0,2) == "sw")
        {
            if(part == 1)
            {
                clk_cycle_count++;
                cout << "cycle " << to_string(clk_cycle_count) << ": DRAM request issued" << endl;
                intial = clk_cycle_count;
                ::NOI[8] += 1;
                if (stoi(result[1].substr(0)) >= 1000)
                {
                    memAddress = stoi(result[1].substr(0));
                    if(memAddress%4 != 0)
                    {
                        cout << "Memory Address should be a multiple of 4.";
                        return -1;
                    }
                    row = memAddress/1024;
                    col = memAddress - 1024*row;
                    if(flag)
                    {
                        memcpy(rowBuffer, DRAM[row], sizeof(rowBuffer));
                        row_buffer_update++;
                        bufferIndex = row;
                        clk_cycle_count += ROW_ACCESS_DELAY;
                        rowBuffer[col] = ::registers[stoi(result[0].substr(5))];
                        clk_cycle_count += COL_ACCESS_DELAY;
                        flag = false;
                    }
                    else
                    {
                        if(bufferIndex == row)
                        {
                            rowBuffer[col] = ::registers[stoi(result[0].substr(5))];
                            clk_cycle_count += COL_ACCESS_DELAY;
                        }
                        else
                        {
                            memcpy(DRAM[bufferIndex],rowBuffer, sizeof(DRAM[bufferIndex]));
                            clk_cycle_count += ROW_ACCESS_DELAY;
                            memcpy(rowBuffer, DRAM[row], sizeof(rowBuffer));
                            row_buffer_update++;
                            bufferIndex = row;
                            clk_cycle_count += ROW_ACCESS_DELAY;
                            rowBuffer[col] = ::registers[stoi(result[0].substr(5))];
                            clk_cycle_count += COL_ACCESS_DELAY;
                        }
                    }
                    ::changedMemory[memAddress] = rowBuffer[col];
                    final = clk_cycle_count;
                    //::memory[stoi(result[1].substr(0))] = to_string(::registers[stoi(result[0].substr(5))]);
                }
                else
                {
                    cout << "Error: Cannot access instructions memory segment." << endl; 
                    return -1;
                }
                cout << "cycle " << to_string(intial+1) << "-" << to_string(final) << ": memory address " << to_string(memAddress) << "-" << to_string(memAddress+3) << "= " << rowBuffer[col] << endl;
            }
            else if(part = 2)
            {
                //Non-blocking Memory Access
                clk_cycle_count++;
                cout << "cycle " << to_string(clk_cycle_count) << ": DRAM request issued" << endl;
                temp_clk_cycle = clk_cycle_count;
                dest = stoi(result[0].substr(5));
                ::NOI[8] += 1;
                if (stoi(result[1].substr(0)) >= 1000)
                {
                    memAddress = stoi(result[1].substr(0));
                    if(memAddress%4 != 0)
                    {
                        cout << "Memory Address should be a multiple of 4.";
                        return -1;
                    }
                    row = memAddress/1024;
                    col = memAddress - 1024*row;
                    if(flag)
                    {
                        memcpy(rowBuffer, DRAM[row], sizeof(rowBuffer));
                        row_buffer_update++;
                        bufferIndex = row;
                        temp_clk_cycle += ROW_ACCESS_DELAY;
                        rowBuffer[col] = ::registers[stoi(result[0].substr(5))];
                        temp_clk_cycle += COL_ACCESS_DELAY;
                        flag = false;
                    }
                    else
                    {
                        if(bufferIndex == row)
                        {
                            rowBuffer[col] = ::registers[stoi(result[0].substr(5))];
                            temp_clk_cycle += COL_ACCESS_DELAY;
                        }
                        else
                        {
                            memcpy(DRAM[bufferIndex],rowBuffer, sizeof(DRAM[bufferIndex]));
                            temp_clk_cycle += ROW_ACCESS_DELAY;
                            memcpy(rowBuffer, DRAM[row], sizeof(rowBuffer));
                            row_buffer_update++;
                            bufferIndex = row;
                            temp_clk_cycle += ROW_ACCESS_DELAY;
                            rowBuffer[col] = ::registers[stoi(result[0].substr(5))];
                            temp_clk_cycle += COL_ACCESS_DELAY;
                        }
                    }
                    ::changedMemory[memAddress] = rowBuffer[col];
                    //::memory[stoi(result[1].substr(0))] = to_string(::registers[stoi(result[0].substr(5))]);
                }
                else
                {
                    cout << "Error: Cannot access instructions memory segment." << endl; 
                    return -1;
                }
                key += 4;
                update = false;
                key = nonBlocking(key, instr_count, dest,1);
                if(clk_cycle_count == temp_clk_cycle)
                {
                    cout << "cycle " << to_string(clk_cycle_count) << "-" << to_string(temp_clk_cycle) << ": memory address " << to_string(memAddress) << "-" << to_string(memAddress+3) << "= " << rowBuffer[col] << endl;
                }
                else
                {
                    cout << "cycle " << to_string(clk_cycle_count+1) << "-" << to_string(temp_clk_cycle) << ": memory address " << to_string(memAddress) << "-" << to_string(memAddress+3) << "= " << rowBuffer[col] << endl;
                    clk_cycle_count = temp_clk_cycle;
                }
                //cout << "cycle " << to_string(intial+1) << "-" << to_string(final) << ": memory address " << to_string(memAddress) << "-" << to_string(memAddress+3) << "= " << rowBuffer[col] << endl;
            }
            
        }
        
        if (update)
        {
            key += 4;
        }
    }
    memcpy(DRAM[bufferIndex],rowBuffer, sizeof(DRAM[bufferIndex]));         
    return key;
}
int nonBlocking(int key, int instr_count, int busyDest, int type)
{
    string input; 
    vector<string> result;
    bool update = true;
    int dest1 = -1;
    int dest2 = -1;
    int dest3 = -1;
    while(key < instr_count*4)
    {
        if(clk_cycle_count == temp_clk_cycle)
        {
            return key;
        }
        update = true;
        input = ::memory[key];
        //cout << "Instruction: " << input << endl;
        boost::split(result, input, boost::is_any_of(","));
        if (result[0].substr(0,3) == "sub")
        {
            dest1 = stoi(result[0].substr(6));
            dest2 = stoi(result[1].substr(2));
            dest3 = stoi(result[2].substr(2));
            if(type == 0)
            {
                if (dest1 == busyDest || dest2 == busyDest || dest3 == busyDest)
                {
                    return key;
                }
            }
            else
            {
                if (dest1 == busyDest)
                {
                    return key;
                }
            }
            clk_cycle_count++;
            ::NOI[1] += 1;
            if(dest1 == 31)
            {
                cout << "Error: Cannot change zero register's content." << endl;
                return -1;
            }
            ::registers[dest1] = ::registers[stoi(result[1].substr(2))] - ::registers[stoi(result[2].substr(2))];
            cout <<  "cycle " << to_string(clk_cycle_count) << ": $t" << to_string(dest1) << "=" << to_string(::registers[dest1]) << endl;
        }
        else if (result[0].substr(0,3) == "mul")
        {
            
            dest1 = stoi(result[0].substr(6));
            dest2 = stoi(result[1].substr(2));
            dest3 = stoi(result[2].substr(2));
            if(type == 0)
            {
                if (dest1 == busyDest || dest2 == busyDest || dest3 == busyDest)
                {
                    return key;
                }
            }
            else
            {
                if (dest1 == busyDest)
                {
                    return key;
                }
            }
            ::NOI[2] += 1;
            clk_cycle_count++;
            if(dest1 == 31)
            {
                cout << "Error: Cannot change zero register's content." << endl;
                return -1;
            }
            ::registers[dest1] = ::registers[stoi(result[1].substr(2))] * ::registers[stoi(result[2].substr(2))];
             cout <<  "cycle " << to_string(clk_cycle_count) << ": $t" << to_string(dest1) << "=" << to_string(::registers[dest1]) << endl;
        }
        else if (result[0].substr(0,4) == "addi")
        {
            dest1 = stoi(result[0].substr(7));
            dest2 = stoi(result[1].substr(2));
            if(type == 0)
            {
                if (dest1 == busyDest || dest2 == busyDest)
                {
                    return key;
                }
            }
            else
            {
                if (dest1 == busyDest)
                {
                    return key;
                }
            }
            ::NOI[9] += 1;
            clk_cycle_count++;
            if(dest1 == 31)
            {
                cout << "Error: Cannot change zero register's content." << endl;
                return -1;
            }
            ::registers[dest1] = ::registers[stoi(result[1].substr(2))] + stoi(result[2].substr(0));
             cout <<  "cycle " << to_string(clk_cycle_count) << ": $t" << to_string(dest1) << "=" << to_string(::registers[dest1]) << endl;
        }
        else if (result[0].substr(0,3) == "add")
        {
            dest1 = stoi(result[0].substr(6));
            dest2 = stoi(result[1].substr(2));
            dest3 = stoi(result[2].substr(2));
            if(type == 0)
            {
                if (dest1 == busyDest || dest2 == busyDest || dest3 == busyDest)
                {
                    return key;
                }
            }
            else
            {
                if (dest1 == busyDest)
                {
                    return key;
                }
            }
            ::NOI[0] += 1;
            clk_cycle_count++;
            if(dest1 == 31)
            {
                cout << "Error: Cannot change zero register's content." << endl;
                return -1;
            }
            ::registers[dest1] = ::registers[stoi(result[1].substr(2))] + ::registers[stoi(result[2].substr(2))];
             cout <<  "cycle " << to_string(clk_cycle_count) << ": $t" << to_string(dest1) << "=" << to_string(::registers[dest1]) << endl;
        }
        else if (result[0].substr(0,3) == "beq")
        {
            ::NOI[3] += 1;
            dest1 = stoi(result[0].substr(6));
            dest2 = stoi(result[1].substr(2));
            if(type == 0)
            {
                if (dest1 == busyDest || dest2 == busyDest)
                {
                    return key;
                }
            }
            clk_cycle_count++;
            if (::registers[dest1] == ::registers[dest2])
            {
                int line_no = stoi(result[2].substr(0));
                key = 4*(line_no - 1);
                update = false;
            }
             cout <<  "cycle " << to_string(clk_cycle_count) << ": beq jumps to " << to_string(key) << endl;  
        }
        else if (result[0].substr(0,3) == "bne")
        {
            ::NOI[4] += 1;
            dest1 = stoi(result[0].substr(6));
            dest2 = stoi(result[1].substr(2));
            if(type == 0)
            {
                if (dest1 == busyDest || dest2 == busyDest)
                {
                    return key;
                }
            }
            clk_cycle_count++;
            if (::registers[dest1] != ::registers[dest2])
            {
                int line_no = stoi(result[2].substr(0));
                key = 4*(line_no - 1);
                update = false;
            }
            cout <<  "cycle " << to_string(clk_cycle_count) << ": bne jumps to " << to_string(key) << endl;  
        }
        else if (result[0].substr(0,3) == "slt")
        {
            ::NOI[5] += 1;
            dest1 = stoi(result[1].substr(2));
            dest2 = stoi(result[2].substr(2));
            dest3 = stoi(result[0].substr(6));
            if(type == 0)
            {
                if (dest1 == busyDest || dest2 == busyDest || dest3 == busyDest)
                {
                    return key;
                }
            }
            else
            {
                if (dest3 == busyDest)
                {
                    return key;
                }
            }
            clk_cycle_count++;
            if (::registers[dest1] < ::registers[dest2])
            {
                ::registers[dest3] = 1;
            }
            else
            {
                ::registers[dest3] = 0;
            }
            cout <<  "cycle " << to_string(clk_cycle_count) << ": slt jumps to " << to_string(key) << endl;
        }
        else if (result[0].substr(0,1) == "j")
        {
            clk_cycle_count++;
            ::NOI[6] += 1;
            int line_no = stoi(result[0].substr(2));
            key = 4*(line_no - 1);
            if (key > instr_count)
            {
                cout << "Error: Memory location out of instructions memory segment." << endl;
                return -1;
            }
            update = false;
            cout <<  "cycle " << to_string(clk_cycle_count) << ": j jumps to " << to_string(key) << endl;
        }
        else if (result[0].substr(0,2) == "lw")
        {
            return key;
        }
        else if (result[0].substr(0,2) == "sw")
        {
            return key;
        }
        if (update)
        {
            key += 4;
        }        
    }
    return key;
}
int main(int argc, char* argv[])
{
    int takeInput_return = takeInput(argv[1]);
    cout << "Enter Part (1 for Normal execution and 2 for Non-blocking memory access): ";
    cin >> part;
    cout << "ROW_ACCESS_DELAY: ";
    cin >> ROW_ACCESS_DELAY;
    cout << "COL_ACCESS_DELAY: " ;
    cin >> COL_ACCESS_DELAY;
    if (takeInput_return == -1)
    {
        cout << "Error: Memory address out of bounds. Memory size is 2^19 bytes." << endl;
        return -1;
    }
    cout << "\nEvery cycle description: " << endl;
    int executeInstructions_return = executeInstructions(takeInput_return, part,0);
    if (executeInstructions_return == -1)
    { 
        return -1;  
    } 
    cout << "\nTotal number of cycles: " << clk_cycle_count << endl;
    cout << "\nMemory content at the end of the execution: " << endl;
    for(map<int,int>::const_iterator it = changedMemory.begin();it != changedMemory.end(); ++it)
        {
            std::cout <<  it->first << "-" << it->first+3 << ": " << it->second << " " << "\n";
        }
    cout << "\nNumber of row buffer updates: " << row_buffer_update << endl;    
    return 0;
}