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

#include "Assignment_4.h"

using namespace std;

int execute(int key, int row_access_delay, int col_access_delay)
{
    string input, input_mod; 
    vector<string> result;
    bool update = true;
    
    int dest = -1; 
    int register1 = -1;
    int register2 = -1;
    string register_name;

    
        update = true;
        input = ::memory[key];
        
        input = input.substr(0, input.find("#", 0));

        boost::trim_left(input);
        boost::trim_right(input);
        
        if (input.find(":") != string::npos)
        {
            key += 4;
            return key;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                        cout << "Error: Register doesn't exist." << endl;
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
                        cout << "Error: Register doesn't exist." << endl;
                        return -1;
                    }
                }
            }
            

            // Assignment_4

            vector<DramNode> temp_vector;
            
            bool check = false;

            for (auto it = dram_vector.begin(); it != dram_vector.end(); it++)
            {
                DramNode element = *it;
                if (element.dest == dest && element.dest != register1 && element.dest != register2 && element.type == 0)
                {
                    dram_vector.erase(it);
                }
                else if ((element.dest == register1 || element.dest == register2)  && element.type == 0)
                {
                    check = true;
                    temp_vector.push_back(element);
                    dram_vector.erase(it);
                    it--;
                }
            }

            if (check == true)
            {
                cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " (Queued)" << endl;
            }
            
            for (auto it = temp_vector.begin(); it != temp_vector.end(); it++)
            {
                DramNode element = *it;
                clk_cycle_count--;
                execute_lw_sw(element, row_access_delay, col_access_delay);
                clk_cycle_count++;
            }

            // Assignment_4
            
        }

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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
                    return -1;
                }
            }

            string label = result[3].append(":");

            

            bool found = false;

            // Assignment_4

            for (auto it = dram_vector.begin(); it != dram_vector.end(); ++it)
            {
                DramNode element = *it;
                if ((element.dest == register1 || element.dest == register2) && element.type == 0)
                {
                    execute_lw_sw(element, row_access_delay, col_access_delay);
                    dram_vector.erase(it);
                    --it;
                }
            }

            // Assignment_4

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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
                    return -1;
                }
            }

            string label = result[3].append(":");

            bool found = false;

            // Assignment_4

            for (auto it = dram_vector.begin(); it != dram_vector.end(); ++it)
            {
                DramNode element = *it;
                if ((element.dest == register1 || element.dest == register2) && element.type == 0)
                {
                    execute_lw_sw(element, row_access_delay, col_access_delay);
                    dram_vector.erase(it);
                    --it;
                }
            }

            // Assignment_4

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
        else if (result[0] == "sw" || result[0] == "lw")
        {
            int address = 0;
            int offset = 0;
            int type = 0;

            if (result[0] == "sw")
            {
                type = 1;
            }
            else if (result[0] == "lw")
            {   
                type = 0;
            }

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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
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
                    cout << "Error: Register doesn't exist." << endl;
                    return -1;
                }
            }
            address = ::registers[register1] + offset;
            int row = address / 1024;

            // Assignment_4

            DramNode element;
            element.dest = dest;
            element.reg1 = register1;
            element.type = type;
            element.row = row;
            element.memory_add = address;
            element.reg_name = register_name;
            element.value = ::registers[dest];
            element.instr_pos = (key/4);

            if (type == 0)
            {
                for (auto it = dram_vector.begin(); it != dram_vector.end(); ++it)
                {
                    DramNode element1 = *it;
                    if (element1.reg_name == register_name && element1.type == 0)
                    {
                        dram_vector.erase(it);
                        it--;
                    }
                }
            }
            else if(type == 1)
            {
                for (auto it = dram_vector.begin(); it != dram_vector.end(); ++it)
                {
                    DramNode element1 = *it;
                    if (element1.reg_name == register_name && element1.type == 0)
                    { 
                        for (auto it = dram_vector.begin(); it != dram_vector.end(); ++it)
                        {
                            DramNode element2 = *it;
                            if (element2.memory_add == element1.memory_add && element2.type == 1)
                            {
                                element.value = element2.value;
                                break;
                            }
                        }
                        break;
                    }
                }

                for (auto it1 = dram_vector.begin(); it1 != dram_vector.end(); it1++)
                {
                    DramNode element0 = *it1;
                    if (element0.memory_add == address && element0.type == 1)
                    {
                        bool found = false;
                        for (auto it2 = dram_vector.begin(); it2 != dram_vector.end(); it2++)
                        {
                            DramNode element1 = *it2;
                            if (element1.memory_add == element0.memory_add && element1.type == 0 && element1.instr_pos > element0.instr_pos)
                            {
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            dram_vector.erase(it1);
                            it1--;
                        }
                    }
                }
            }

            


            dram_vector.push_back(element);

            sort(dram_vector.begin(), dram_vector.end(), comparator);

            // Assignment_4

            cout << "cycle " << clk_cycle_count << ": Instruction: " << input << " (DRAM request Queued)" << endl;
        }
        else
        {
            cout << "Command doesn't exist: " << result[0] << endl;
            return -1;
        }

        if (update)
        {
            key += 4;
        }
    
    return key;
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
    
    cout << endl << "Total number of cycles: " << clk_cycle_count << endl;
    cout <<  endl << "Total number of buffer updates: " << buffer_update_count << endl;
    return 0;
}