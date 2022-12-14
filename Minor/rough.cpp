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


using namespace std;

int main(int argc, char* argv[])
{

  int a[5][5] = {0};
  int b[5] = {1,2,3,4,5};
    for(int i =0; i<5; i++)
    {
      for(int j = 0; j<5; j++)
      {
        cout << a[i][j] << endl; 
      }
      cout <<"b" <<  b[i] <<endl;
    }
    memcpy(a[0], b, sizeof(a[0])); 
    for(int j = 0; j<5; j++)
      {
        cout << "BBa" << a[0][j] << endl;
      }
    
  return 0;
}