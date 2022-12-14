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

#include "assignment_5.h"

using namespace std;


int execute(int key, int row_access_delay, int col_access_delay, int core, int memory_size_per_core, ofstream core_array[])
{
    bool update = true;
    string input, input_mod; 
    vector<string> result;
    int dest = -1; 
    int register1 = -1;
    int register2 = -1;
    string register_name;

    if (key == -1)
    {
        return -2;
    }
        if(!isBusy[core])
        {
            update = true;
        }
        else
        {
            return key;
        }
        map<int, string> temp_map = instr_memory[core];
        input = temp_map[key];
        
        input = input.substr(0, input.find("#", 0));

        boost::trim_left(input);
        boost::trim_right(input);
        
        if (input.find(":") != string::npos)
        {
            key += 4;
            input = temp_map[key];
        
            input = input.substr(0, input.find("#", 0));

            boost::trim_left(input);
            boost::trim_right(input);
        }

        if (input.empty())
        {
            return -3;
        }
        
        
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

            
            bool check = false;

            if(MRM_Ex.core == core)
            {
                if(MRM_Ex.end_cycle >=  clk_cycle_count)
                {
                    if ((MRM_Ex.dest == register1 || MRM_Ex.dest == register2 || MRM_Ex.dest == dest)  && MRM_Ex.type == 0)
                    {
                        check = true;
                        update = false;
                    }
                }
            }
            for (auto it = MRM_vector.begin(); it != MRM_vector.end(); it++)
            {
                
                DramNode element = *it;
                // cout << element.core << " " << clk_cycle_count << " " << core <<   endl;
                // if(element.core == 3)
                // {
                //     cout << element.reg_name << " " << input << endl;
                // }
                // cout << "element.core " << element.core << endl;
                // cout << core << " " << input << endl;
                if(element.core == core)
                {
                    if ((element.dest == register1 || element.dest == register2 || element.dest == dest)  && element.type == 0)
                    {
                        check = true;
                        update = false;
                        isBusy[core] = true;
                        temp_vector.push_back(element);
                        MRM_vector.erase(it);
                        it--;
                    }
                }
                
                
            }

            if (check == true)
            {
                core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " (Queued)" << endl;
                return key;
            }
            
            // for (auto it = temp_vector.begin(); it != temp_vector.end(); it++)
            // {
            //     DramNode element = *it;
            //     MRM_function(element, row_access_delay, col_access_delay, memory_size_per_core, core_array);
            // }

            // Assignment_4
            
        }

        if (result[0] == "sub")
        {
            ::registers[core][dest] = ::registers[core][register1] - ::registers[core][register2];
            inst_exec[core] ++;
            core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[core][dest] << endl;
        }
        else if (result[0] == "mul")
        {
            ::registers[core][dest] = ::registers[core][register1] * ::registers[core][register2];
            inst_exec[core] ++;
            core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[core][dest] << endl;
        }
        else if (result[0] == "add")
        {
            ::registers[core][dest] = ::registers[core][register1] + ::registers[core][register2];
            inst_exec[core] ++;
            core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[core][dest] << endl;
        }
        else if (result[0] == "slt")
        {   
            if (::registers[core][register1] < ::registers[core][register2])
            {
                ::registers[core][dest] = 1;
                inst_exec[core] ++;
                core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[core][dest] << endl;
            }
            else
            {
                ::registers[core][dest] = 0;
                inst_exec[core] ++;
                core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[core][dest] << endl;
            }
        }
        else if (result[0] == "addi")
        {
            if(result[1].substr(0,1) != "$" || result[2].substr(0,1) != "$")
            {
                cout << "Syntax error: $ expected." << endl;
                return -1;
            }
            ::registers[core][dest] = ::registers[core][register1] + stoi(result[3]);
            inst_exec[core] ++;
            core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " => " << register_name << " = " << ::registers[core][dest] << endl;
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

            bool check = false;

            if(MRM_Ex.core == core)
            {
                if(MRM_Ex.end_cycle >=  clk_cycle_count)
                {
                    if ((MRM_Ex.dest == register1 || MRM_Ex.dest == register2 )  && MRM_Ex.type == 0)
                    {
                        check = true;
                        update = false;
                    }
                }
            }

            for (auto it = MRM_vector.begin(); it != MRM_vector.end(); it++)
            {
                
                DramNode element = *it;
                // cout << element.core << " " << clk_cycle_count << " " << core <<   endl;
                // if(element.core == 3)
                // {
                //     cout << element.reg_name << " " << input << endl;
                // }
                // cout << "element.core " << element.core << endl;
                // cout << core << " " << input << endl;
                if(element.core == core)
                {
                    if ((element.dest == register1 || element.dest == register2)  && element.type == 0)
                    {
                        check = true;
                        update = false;
                        isBusy[core] = true;
                        temp_vector.push_back(element);
                        MRM_vector.erase(it);
                        it--;
                    }
                }
                
            }

            if (check == true)
            {
                core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " (Queued)" << endl;
                return key;
            }
            

            bool found = false;

            // Assignment_4

            if (::registers[core][register1] == ::registers[core][register2])
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

                update = false;
            }  
            inst_exec[core] ++;
            core_array[core] << "cycle "<< clk_cycle_count << ": Instruction: " << input << endl;   
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
            
            bool check = false;
            if(MRM_Ex.core == core)
            {
                if(MRM_Ex.end_cycle >=  clk_cycle_count)
                {
                    if ((MRM_Ex.dest == register1 || MRM_Ex.dest == register2 )  && MRM_Ex.type == 0)
                    {
                        check = true;
                        update = false;
                    }
                }
            }
            for (auto it = MRM_vector.begin(); it != MRM_vector.end(); it++)
            {
                
                DramNode element = *it;
                // cout << element.core << " " << clk_cycle_count << " " << core <<   endl;
                // if(element.core == 3)
                // {
                //     cout << element.reg_name << " " << input << endl;
                // }
                // cout << "element.core " << element.core << endl;
                // cout << core << " " << input << endl;
                if(element.core == core)
                {
                    if ((element.dest == register1 || element.dest == register2)  && element.type == 0)
                    {
                        check = true;
                        update = false;
                        isBusy[core] = true;
                        temp_vector.push_back(element);
                        MRM_vector.erase(it);
                        it--;
                    }
                }
                
            }

            if (check == true)
            {
                core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " (Queued)" << endl;
                return key;
            }

            bool found = false;

            // Assignment_4

            if (::registers[core][register1] != ::registers[core][register2])
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

                update = false;
            }  
            inst_exec[core] ++;
            core_array[core] << "cycle "<< clk_cycle_count << ": Instruction: " << input << endl;
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

            update = false;
            inst_exec[core] ++;
            core_array[core] << "cycle "<< clk_cycle_count << ": Instruction: " << input << endl;
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
            address = ::registers[core][register1] + offset + memory_size_per_core*core;
            int row = address / 1024;

            // Assignment_4

            DramNode element;
            element.dest = dest;
            element.reg1 = register1;
            element.type = type;
            element.row = row;
            element.memory_add = address;
            element.reg_name = register_name;
            element.value = ::registers[core][dest];
            element.instr_pos = (key/4);
            element.core = core;

            if (type == 0)
            {
                for (auto it = MRM_vector.begin(); it != MRM_vector.end(); ++it)
                {
                    DramNode element1 = *it;
                    if (element1.reg_name == register_name && element1.type == 0 && element1.core == core)
                    {
                        MRM_vector.erase(it);
                        it--;
                    }
                }
            }
            else if(type == 1)
            {
                for (auto it = MRM_vector.begin(); it != MRM_vector.end(); ++it)
                {
                    DramNode element1 = *it;
                    if (element1.reg_name == register_name && element1.type == 0 && element1.core == core)
                    { 
                        for (auto it = MRM_vector.begin(); it != MRM_vector.end(); ++it)
                        {
                            DramNode element2 = *it;
                            if (element2.memory_add == element1.memory_add && element2.type == 1 && element2.core == core)
                            {
                                element.value = element2.value;
                                break;
                            }
                        }
                        break;
                    }
                }

                for (auto it1 = MRM_vector.begin(); it1 != MRM_vector.end(); it1++)
                {
                    DramNode element0 = *it1;
                    if (element0.memory_add == address && element0.type == 1 && element0.core == core)
                    {
                        bool found = false;
                        for (auto it2 = MRM_vector.begin(); it2 != MRM_vector.end(); it2++)
                        {
                            DramNode element1 = *it2;
                            if (element1.memory_add == element0.memory_add && element1.type == 0 && element1.instr_pos > element0.instr_pos && element0.core == core)
                            {
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            MRM_vector.erase(it1);
                            it1--;
                        }
                    }
                }
            }

            


            MRM_vector.push_back(element);
            if(MRM_vector.size() > MAX_DRAM_SIZE)
            {
                cout << "Error: Memory Request Manager Overflow." << endl;
                return -1;
            }


            // Assignment_4

            core_array[core] << "cycle " << clk_cycle_count << ": Instruction: " << input << " (DRAM request Queued)" << endl;
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
    string filename = argv[1];
    int row_access_delay = atoi(argv[2]);
    int col_access_delay = atoi(argv[3]);

    string line;

    fstream file;
    file.open(filename, ios::in);

    getline(file, line);
    boost::trim_left(line);
    boost::trim_right(line);
    int N = stoi(line);
    if(N > MAX_CORES)
    {
        cout << "Core Limit Exceeded." << endl;
        return -1;
    }
    
    getline(file, line);
    boost::trim_left(line);
    boost::trim_right(line);
    int M = stoi(line);

    int takeInput_return_array[N];

    for (int i = 0; i < N; i++)
    {
        getline(file, line);
        boost::trim_left(line);
        boost::trim_right(line);
        takeInput_return_array[i] = takeInput(line);
        if (takeInput_return_array[i] == -1)
        {
            return -1;
        }
    } 
    
    // initialize memory
    for (int i = 0; i < pow(2,20); i++)
    {
        ::memory[i] = to_string(0);
    }

    int memory_size_per_core = pow(2,20) / N;
    memory_size_per_core -= memory_size_per_core % 4;

    ofstream core_array[N];

    for (int i = 0; i < N; i++)
    {
        string str = "out" + to_string(i+1) + ".txt";
        core_array[i].open(str);
    }
    
    MRM_Ex.core = -1;
    MRM_Ex.end_cycle = -1;    


    int executeInstructions_return;

    for (int j = 1; j < M+1; j++)
    {
        if(init_time == j)
        {
            core_array[MRM_Ex.core] << init_msg << endl;
        }
        if (MRM_BusyTime != 0)
        {
            MRM_BusyTime--; 
        }
        else
        {   
            if(!temp_vector.empty())
            {
                bool found_temp = false;
                for (auto it = temp_vector.begin(); it != temp_vector.end(); it++)
                {
                    DramNode element = *it;
                    if(element.row == activeROW)
                    {
                        MRM_function(element, row_access_delay, col_access_delay, memory_size_per_core, core_array);
                        temp_vector.erase(it);
                        found_temp = true;
                        break;
                    }
                    
                }
                if(!found_temp)
                {
                    MRM_function(temp_vector[0], row_access_delay, col_access_delay, memory_size_per_core, core_array);
                    temp_vector.erase(temp_vector.begin());
                }
            }
            else if (!MRM_vector.empty())
            {
                bool found = false;
                for (auto it = MRM_vector.begin(); it != MRM_vector.end(); it++)
                {
                    DramNode element = *it;
                    if(element.row == activeROW)
                    {
                        MRM_function(element, row_access_delay, col_access_delay, memory_size_per_core, core_array);
                        MRM_vector.erase(it);
                        found = true;
                        break;
                    }
                }

                if (found == false)
                {
                    MRM_function(MRM_vector[0], row_access_delay, col_access_delay, memory_size_per_core, core_array);
                    MRM_vector.erase(MRM_vector.begin());
                }
                
            }
        }
        bool dep_core[N] = {false};
        for (auto it = temp_vector.begin(); it != temp_vector.end(); it++)
        {
            DramNode element = *it;
            dep_core[element.core] = true;
        }
        for (int i = 0; i < N; i++)
        {
            if(isBusy[i])
            {
                if(!dep_core[i])
                {
                    if(MRM_BusyTime == 0)
                    {
                        isBusy[i] = false;
                    }
                }
            }
            clk_cycle_count = j;
            // cout << MRM_Ex.core << " " << i << endl;
            // cout << MRM_Ex.end_cycle << " " << j << endl << endl;

            if(MRM_Ex.core == i && MRM_Ex.end_cycle == j)
            {
                if (MRM_Ex.type == 1)
                {
                    ::bufferROW[MRM_Ex.col] = MRM_Ex.dest;
                    inst_exec[MRM_Ex.core] ++;
                    core_array[MRM_Ex.core] << "cycle " << clk_cycle_count  << ": Column Access => memory address " << MRM_Ex.address << "-" << MRM_Ex.address+3 << " = " << ::bufferROW[MRM_Ex.col] << endl;
                }
                else if (MRM_Ex.type == 0)
                {
                    ::registers[MRM_Ex.core][MRM_Ex.dest] = ::bufferROW[MRM_Ex.col];
                    inst_exec[MRM_Ex.core] ++;
                    core_array[MRM_Ex.core] << "cycle " << clk_cycle_count << ": Column Access => " << MRM_Ex.register_name << " = " << ::registers[MRM_Ex.core][MRM_Ex.dest] << endl;
                }

                MRM_Ex.core = -1;
                continue;
            }
            executeInstructions_return = executeInstructions(takeInput_return_array[i], row_access_delay, col_access_delay, i, memory_size_per_core, core_array);
            if (executeInstructions_return == -1)
            {
                cout << "Execution for core-" << i+1 << " failed." << endl << endl;
                continue;  
            } 
        }
    }
    ofstream stats;
    stats.open("relevant_stats.txt");
    int sum1 = 0;
    for(int k = 0; k<N; k++)
    {
        stats << "Instruction executed in core " << k+1 << ": " << inst_exec[k] << endl;
        sum1 += inst_exec[k];
    }
    stats << "\nTotal buffer update: " << buffer_update_count << endl;
    stats << "\nTotal no. of instruction executed: " << sum1 << endl;

    return 0;
}