// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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

// globals
int activeROW = -1;
int clk_cycle_count = 0;

// structs 
struct QueueNode {
    string data;
    QueueNode* next;
    QueueNode(string d)
    {
        data = d;
        next = NULL;
    }
};
  
struct Queue {
    QueueNode *front, *rear;
    Queue()
    {
        front = rear = NULL;
    }
  
    void enQueue(string x)
    {
  
        // Create a new LL node
        QueueNode* temp = new QueueNode(x);
  
        // If queue is empty, then
        // new node is front and rear both
        if (rear == NULL) {
            front = temp;
            front->next = rear;
            return;
        }
  
        // Add the new node at
        // the end of queue and change rear
        rear->next = temp;
        rear = temp;
    }
  
    // Function to remove
    // a key from given queue q
    void deQueue()
    {
        // If queue is empty, return NULL.
        if (front == NULL)
            return;
  
        // Store previous front and
        // move front one node ahead
        QueueNode* temp = front;
        front = front->next;
  
        // If front becomes NULL, then
        // change rear also as NULL
        if (front == NULL)
            rear = NULL;
  
        delete (temp);
    }
};



// Global variables 
map<int, string> memory; // hash map represents memory
int registers[33]; // array representing registers
int bufferROW[1024]; // array for bufferROW
int buffer_update_count = 0;
Queue queue;

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

            ::memory[key] = line;
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

// ---- temporary changes start---- //
bool check(string result, int dest, int register1, int register2, int address)
{
        
    QueueNode temp = queue.front;
    vector<string> list;
    if(result == "add" or "sub" or "mul" or "addi" or "bne" or "beq" or "slt" or "sw")
    {
        
        while(temp != NULL)
        {
            boost::split(list, temp.data, boost::is_any_of(" "));
            if(list[0] == "lw")
            {
                if(stoi(list[1]) == dest || register1 || register2)
                {
                    return false
                }
            }
            temp = temp->next
        }
    }
    else if(result == "lw")
    {
        while(temp != NULL)
        {
            boost::split(list, temp, boost::is_any_of(" "));
            if(list[0] == "sw")
            {
                if(stoi(list[2]) == address)
                {
                    return false
                }
            }
            temp = temp.next
        }
    }
    return true
}

// ---- temporary chages closed ---- //
// function for executing instructions
int executeInstructions(int key, int instr_count, int row_access_delay, int col_access_delay)
{
    string input, input_mod; 
    vector<string> result;
    int key = 0; // PC
    bool update = true;
    bool check = false;
    int dest = -1;
    int register1 = -1;
    int register2 = -1;
    string register_name;

    while(key < instr_count*4)
    {
        update = true;
        input = ::memory[key];
        
        input = input.substr(0, input.find("#", 0));

        boost::trim_left(input);
        boost::trim_right(input);
        
        if (input.find(":") != string::npos)
        {
            key += 4;
            continue;
        }
        
        clk_cycle_count++;
        input_mod = input;
        boost::replace_all(input_mod, ",", "");
        boost::split(result, input_mod, boost::is_any_of(" "));

        if (result[0] == "sub" || result[0] == "mul" || result[0] == "add" || result[0] == "slt" || result[0] == "addi")
        {
            if (result.size() != 4)
            {
                cout << "Error: Invalid Instruction." << endl;
                return -1;
            }

            if(result[1] == "$zero")
            {
                cout << "Error: Cannot change zero register's content." << endl;
                return -1;
            }
            
            if(result[1].substr(0,1) != "$" || result[2].substr(0,1) != "$" || result[3].substr(0,1) != "$" && result[0] != "addi")
            {
                cout << "Syntax error: $ expected." << endl;
                return -1;
            }
            
            if(result[1].substr(1,1) == "t")
            {   
                dest = stoi(result[1].substr(2,1));
                register_name = "$t" + to_string(dest);

                if(dest == 8 || dest == 9)
                {
                    dest += 16;
                }
                else if (dest <= 7 && dest >= 0)
                {
                    dest += 8;
                }
                else
                {
                    cout << "Syntax error: Register doesn't exist." << endl;
                    return -1;
                }
            }
            else if (result[1].substr(1,1) == "s")
            {
                dest = stoi(result[1].substr(2,1));
                register_name = "$s" + to_string(dest);

                if(dest == 8)
                {
                    dest = 30;
                }
                else if (dest <= 7 && dest >= 0)
                {
                    dest += 16;
                }
                else
                {
                    cout << "Syntax error: Register doesn't exist." << endl;
                    return -1;
                }
            }

            if (result[2] == "$zero")
            {
                register1 = 32;
            }
            else if (result[2].substr(1,1) == "t")
            {
                register1 = stoi(result[2].substr(2,1));

                if(register1 == 8 || register1 == 9)
                {
                    register1 += 16;
                }
                else if (register1 <= 7 && register1 >= 0)
                {
                    register1 += 8;
                }
                else
                {
                    cout << "Syntax error: Register doesn't exist." << endl;
                    return -1;
                }
            }
            else if (result[2].substr(1,1) == "s")
            {
                register1 = stoi(result[2].substr(2,1));

                if(register1 == 8)
                {
                    register1 = 30;
                }
                else if (register1 <= 7 && register1 >= 0)
                {
                    register1 += 16;
                }
                else
                {
                    cout << "Syntax error: Register doesn't exist." << endl;
                    return -1;
                }
            }

            if(result[0] != "addi")
            {
                if (result[3] == "$zero")
                {
                    register2 = 32;
                }
                else if (result[3].substr(1,1) == "t")
                {
                    register2 = stoi(result[3].substr(2,1));

                    if(register2 == 8 || register2 == 9)
                    {
                        register2 += 16;
                    }
                    else if (register2 <= 7 && register2 >= 0)
                    {
                        register2 += 8;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
                else if (result[3].substr(1,1) == "s")
                {
                    register2 = stoi(result[3].substr(2,1));

                    if(register2 == 8)
                    {
                        register2 = 30;
                    }
                    else if (register2 <= 7 && register2 >= 0)
                    {
                        register2 += 16;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
            
            }
            
        }
        check = check(result[0], dest, register1, register2, register2);
        if(result[0] == "lw" || result[0] == "sw" || check)
        {
            if (result[0] == "sub")
            {
                ::registers[dest] = ::registers[register1] - ::registers[register2];
                cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[dest] << endl;
            }
            else if (result[0] == "mul")
            {
                ::registers[dest] = ::registers[register1] * ::registers[register2];
                cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[dest] << endl;
            }
            else if (result[0] == "add")
            {
                ::registers[dest] = ::registers[register1] + ::registers[register2];
                cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[dest] << endl;
            }
            else if (result[0] == "slt")
            {   
                if (::registers[register1] < ::registers[register2])
                {
                    ::registers[dest] = 1;
                    cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[dest] << endl;
                }
                else
                {
                    ::registers[dest] = 0;
                    cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[dest] << endl;
                }
            }
            else if (result[0] == "addi")
            {
                if(result[1].substr(0,1) != "$" || result[2].substr(0,1) != "$")
                {
                    cout << "Syntax error: $ expected." << endl;
                    return -1;
                }
                ::registers[dest] = ::registers[register1] + stoi(result[3]);
                cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[dest] << endl;
            }
            else if (result[0] == "beq")
            {
                if (result.size() != 4)
                {
                    cout << "Error: Invalid Instruction." << endl;
                    return -1;
                }

                if(result[1].substr(0,1) != "$" || result[2].substr(0,1) != "$")
                {
                    cout << "Syntax error: $ expected." << endl;
                    return -1;
                }

                if (result[1] == "$zero")
                {
                    register1 = 32;
                }
                else if(result[1].substr(1,1) == "t")
                {   
                    register1 = stoi(result[1].substr(2,1));

                    if(register1 == 8 || register1 == 9)
                    {
                        register1 += 16;
                    }
                    else if (register1 <= 7 && register1 >= 0)
                    {
                        register1 += 8;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
                else if (result[1].substr(1,1) == "s")
                {
                    register1 = stoi(result[1].substr(2,1));

                    if(register1 == 8)
                    {
                        register1 = 30;
                    }
                    else if (register1 <= 7 && register1 >= 0)
                    {
                        register1 += 16;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }

                if (result[2] == "$zero")
                {
                    register2 = 32;
                }
                else if (result[2].substr(1,1) == "t")
                {
                    register2 = stoi(result[2].substr(2,1));

                    if(register2 == 8 || register2 == 9)
                    {
                        register2 += 16;
                    }
                    else if (register2 <= 7 && register2 >= 0)
                    {
                        register2 += 8;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
                else if (result[2].substr(1,1) == "s")
                {
                    register2 = stoi(result[2].substr(2,1));

                    if(register2 == 8)
                    {
                        register2 = 30;
                    }
                    else if (register2 <= 7 && register2 >= 0)
                    {
                        register2 += 16;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }

                string label = result[3].append(":");

                bool found = false;

                if (::registers[register1] == ::registers[register2])
                {
                    for (auto& it : memory) 
                    {
                        if (it.second == label) 
                        {
                            key = it.first;
                            found = true;
                            break;
                        }
                    }
                    
                    if (found == false)
                    {
                        cout << "Error: Undefined label." << endl;
                        return -1;
                    }

                    if (key > 999) 
                    {
                        cout << "Error: Memory location out of instructions memory segment." << endl;
                        return -1;
                    }
                    update = false;
                }  
                cout << "cycle "<< clk_cycle_count << ": Instruction: " << input << endl;   
            }
            else if (result[0] == "bne")
            {
                if (result.size() != 4)
                {
                    cout << "Error: Invalid Instruction." << endl;
                    return -1;
                }

                if(result[1].substr(0,1) != "$" || result[2].substr(0,1) != "$")
                {
                    cout << "Syntax error: $ expected." << endl;
                    return -1;
                }

                if (result[1] == "$zero")
                {
                    register1 = 32;
                }
                else if(result[1].substr(1,1) == "t")
                {   
                    register1 = stoi(result[1].substr(2,1));

                    if(register1 == 8 || register1 == 9)
                    {
                        register1 += 16;
                    }
                    else if (register1 <= 7 && register1 >= 0)
                    {
                        register1 += 8;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
                else if (result[1].substr(1,1) == "s")
                {
                    register1 = stoi(result[1].substr(2,1));

                    if(register1 == 8)
                    {
                        register1 = 30;
                    }
                    else if (register1 <= 7 && register1 >= 0)
                    {
                        register1 += 16;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }

                if (result[2] == "$zero")
                {
                    register2 = 32;
                }
                else if (result[2].substr(1,1) == "t")
                {
                    register2 = stoi(result[2].substr(2,1));

                    if(register2 == 8 || register2 == 9)
                    {
                        register2 += 16;
                    }
                    else if (register2 <= 7 && register2 >= 0)
                    {
                        register2 += 8;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
                else if (result[2].substr(1,1) == "s")
                {
                    register2 = stoi(result[2].substr(2,1));

                    if(register2 == 8)
                    {
                        register2 = 30;
                    }
                    else if (register2 <= 7 && register2 >= 0)
                    {
                        register2 += 16;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }

                string label = result[3].append(":");

                bool found = false;

                if (::registers[register1] != ::registers[register2])
                {
                    for (auto& it : memory) 
                    {
                        if (it.second == label) 
                        {
                            key = it.first;
                            found = true;
                            break;
                        }
                    }
                    
                    if (found == false)
                    {
                        cout << "Error: Undefined label." << endl;
                        return -1;
                    }

                    if (key > 999) 
                    {
                        cout << "Error: Memory location out of instructions memory segment." << endl;
                        return -1;
                    }
                    update = false;
                }  
                cout << "cycle "<< clk_cycle_count << ": Instruction: " << input << endl;
            }
            else if (result[0] == "j")
            {
                if (result.size() != 2)
                {
                    cout << "Error: Invalid Instruction." << endl;
                    return -1;
                }
                
                string label = result[1].append(":");

                bool found = false;
    
                for (auto& it : memory) 
                {
                    if (it.second == label) 
                    {
                        key = it.first;
                        found = true;
                        break;
                    }
                }

                if (found == false)
                {
                    cout << "Error: Undefined label." << endl;
                    return -1;
                }

                if (key > 999) 
                {
                    cout << "Error: Memory location out of instructions memory segment." << endl;
                    return -1;
                }
                update = false;
                cout << "cycle "<< clk_cycle_count << ": Instruction: " << input << endl;
            }
            else if (result[0] == "lw")
            {
                int address = 0;
                int offset = 0;
                
                if (result.size() != 3)
                {
                    cout << "Error: Invalid Instruction." << endl;
                    return -1;
                }

                if (result[1] == "$zero")
                {
                    cout << "Error: Cannot change zero register's content." << endl;
                    return -1;
                }

                if(result[1].substr(0,1) != "$")
                {
                    cout << "Syntax error: $ expected." << endl;
                    return -1;
                }

                if(result[1].substr(1,1) == "t")
                {   
                    dest = stoi(result[1].substr(2,1));
                    register_name = "$t" + to_string(dest);

                    if(dest == 8 || dest == 9)
                    {
                        dest += 16;
                    }
                    else if (dest <= 7 && dest >= 0)
                    {
                        dest += 8;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
                else if (result[1].substr(1,1) == "s")
                {
                    dest = stoi(result[1].substr(2,1));
                    register_name = "$s" + to_string(dest);

                    if(dest == 8)
                    {
                        dest = 30;
                    }
                    else if (dest <= 7 && dest >= 0)
                    {
                        dest += 16;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }


                vector<string> result_address;    
            
                boost::split(result_address, result[2], boost::is_any_of("("));
                
                if (result_address.size() != 2)
                {
                    cout << "Syntax Error: '(' expected." << endl;
                    return -1;
                }

                if (result_address[0].empty())
                {
                    result_address[0] = "0";    
                }

                offset = stoi(result_address[0]);

                if(result_address[1].substr(0,1) != "$")
                {
                    cout << "Syntax error: $ expected." << endl;
                    return -1;
                }
                
                if(result_address[1].substr(1,1) == "s")
                {
                    register1 = stoi(result_address[1].substr(2,1));

                    if(register1 == 8)
                    {
                        register1 = 30;
                    }
                    else if (register1 <= 7 && register1 >= 0)
                    {
                        register1 += 16;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
                else if (result_address[1].substr(1,1) == "t")
                {
                    register1 = stoi(result_address[1].substr(2,1));

                    if(register1 == 8 || register1 == 9)
                    {
                        register1 += 16;
                    }
                    else if (register1 <= 7 && register1 >= 0)
                    {
                        register1 += 8;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }

                address = ::registers[register1] + offset;

                if (address >= 1000 && address < pow(2,20))
                {
                    if (address % 4 != 0)
                    {
                        cout << "Error: Memory addresses which are not multiples 4 of cannot be accessed." << endl;
                        return -1;
                    }
                    
                    int row = address / 1024;
                    int col = address % 1024;
                    
                    
                    check = check(result[0],dest,register1,register2,address);
                    if(check)
                    {
                        cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " (DRAM request issued)" << endl;
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
                            if(check)
                            {
                                ::registers[dest] = ::bufferROW[col];
                                cout << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + col_access_delay << ": Column Access => " << register_name << " = " << ::registers[dest] << endl;
                                clk_cycle_count += col_access_delay;
                            }
                            else
                            {
                                execQueue();
                                //first execute queued instrctions until currInstruction becomes independent
                            }
                        }
                        else
                        {
                            enqueue();
                        }
                    }
                    else
                    {
                        execQueue();
                    }
                }
                else
                {
                    cout << "Error: Memory location not in data memory segment." << endl;
                    return -1;
                }
            }
            else if (result[0] == "sw")
            {
                int address = 0;
                int offset = 0;
                
                if (result.size() != 3)
                {
                    cout << "Error: Invalid Instruction." << endl;
                    return -1;
                }

                if(result[1].substr(0,1) != "$")
                {
                    cout << "Syntax error: $ expected." << endl;
                    return -1;
                }

                if(result[1].substr(1,1) == "t")
                {   
                    dest = stoi(result[1].substr(2,1));
                    register_name = "$t" + to_string(dest);
                    
                    if(dest == 8 || dest == 9)
                    {
                        dest += 16;
                    }
                    else if (dest <= 7 && dest >= 0)
                    {
                        dest += 8;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
                else if (result[1].substr(1,1) == "s")
                {
                    dest = stoi(result[1].substr(2,1));
                    register_name = "$s" + to_string(dest);
                    
                    if(dest == 8)
                    {
                        dest = 30;
                    }
                    else if (dest <= 7 && dest >= 0)
                    {
                        dest += 16;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }

                
                vector<string> result_address;    
            
                boost::split(result_address, result[2], boost::is_any_of("("));
                
                if (result_address.size() != 2)
                {
                    cout << "Syntax Error: '(' expected." << endl;
                    return -1;
                }

                if (result_address[0].empty())
                {
                    result_address[0] = "0";    
                }
                
                offset = stoi(result_address[0]);
                
                if(result_address[1].substr(0,1) != "$")
                {
                    cout << "Syntax error: $ expected." << endl;
                    return -1;
                }
        
                if(result_address[1].substr(1,1) == "s")
                {
                    register1 = stoi(result_address[1].substr(2,1));

                    if(register1 == 8)
                    {
                        register1 = 30;
                    }
                    else if (register1 <= 7 && register1 >= 0)
                    {
                        register1 += 16;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
                else if (result_address[1].substr(1,1) == "t")
                {
                    register1 = stoi(result_address[1].substr(2,1));

                    if(register1 == 8 || register1 == 9)
                    {
                        register1 += 16;
                    }
                    else if (register1 <= 7 && register1 >= 0)
                    {
                        register1 += 8;
                    }
                    else
                    {
                        cout << "Syntax error: Register doesn't exist." << endl;
                        return -1;
                    }
                }

                address = ::registers[register1] + offset;            

                if (address >= 1000 && address < pow(2,20))
                {
                    if (address % 4 != 0)
                    {
                        cout << "Error: Memory addresses which are not multiples 4 of cannot be accessed." << endl;
                        return -1;
                    }
                    check = check(result[0],dest,register1,register2,address);
                    int row = address / 1024;
                    int col = address % 1024;
                    if(check)
                    {
                        cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " (DRAM request issued)" << endl;
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

                            ::bufferROW[col] = ::registers[dest];
                            cout << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + row_access_delay << ": Row-" << row << " Activated" <<  endl;
                            cout << "cycle " << clk_cycle_count + row_access_delay+1 << "-" << clk_cycle_count + row_access_delay + col_access_delay << ": Column Access => memory address " << address << "-" << address+3 << " = " << ::bufferROW[col] << endl;
                            clk_cycle_count += row_access_delay + col_access_delay;
                        }
                        else if (activeROW == row)
                        {
                            buffer_update_count++;
                            ::bufferROW[col] = ::registers[dest];
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

                            ::bufferROW[col] = ::registers[dest];
                            cout << "cycle " << clk_cycle_count+1 << "-" << clk_cycle_count + 2*row_access_delay  << ": Row-" << activeROW << " Writeback and Row-" << row << " Activated" << endl;
                            cout << "cycle " << clk_cycle_count + 2*row_access_delay+1 << "-" << clk_cycle_count + 2*row_access_delay + col_access_delay << ": Column Access => memory address " << address << "-" << address+3 << " = " << ::bufferROW[col] << endl;
                            clk_cycle_count += 2*row_access_delay + col_access_delay;
                            activeROW = row;
                        }

                    }
                    else
                    {
                        enqueue()
                    }
                    
                }
                else
                {
                    cout << "Error: Memory location not in data memory segment." << endl;
                    return -1;
                }
            }
            else
            {
                cout << "Command doesn't exist: " << result[0] << endl;
                return -1;
            }
        }
        else
        {
            first execute queued instrctions until currInstruction becomes independent
        }
        if (update)
        {
            key += 4;
        }
    }
    return key;
}

void execQueue(int col,int row,int row_access_delay,int col_access_delay)
{
    string temp =  queue.first.data;
    vector<string> tempList;
    boost::split(tempList, temp, boost::is_any_of(" "));
    int dest = stoi(tempList[1]);
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
int main(int argc, char* argv[])
{
    // command line validation
    if (argc != 4)
    {
        cout << "Input format: ./out filename.txt ROW_ACCESS_DELAY COL_ACCESS_DELAY" << endl;
        return -1;
    }
    
    int row_access_delay = atoi(argv[2]);
    int col_access_delay = atoi(argv[3]);
 
    // initialize memory
    for (int i = 0; i < pow(2,20); i++)
    {
        ::memory[i] = to_string(0);
    }

    int takeInput_return = takeInput(argv[1]); // argv[1] is the filename

    if (takeInput_return == -1)
    {
        return -1;
    }
    
    int executeInstructions_return = executeInstructions(takeInput_return, row_access_delay, col_access_delay);

    if (executeInstructions_return == -1)
    {
        return -1;  
    } 
    
    cout << endl << "Total number of cycles: " << executeInstructions_return << endl;
    cout <<  endl << "Total number of buffer updates: " << buffer_update_count << endl;
    return 0;
}