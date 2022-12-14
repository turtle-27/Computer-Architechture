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
            front = rear = temp;
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


int main(int argc, char* argv[])
{
    
}