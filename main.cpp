#include <iostream>
#include <math.h>
#include <vector>
#include "PQueue.hpp"

int main(int argc, const char * argv[]) {
    PQueue *p = new PQueue(20);
    int r;
    srand (time(NULL));
    for(int i = 0; i < 20; i++)
    {
        r = rand() % 100;
        std::cout<< r << std::endl;
        p->insert(r);
    }
    for(int i = 0; i < 20; i++)
    {
        int smallest = p->deleteMin();
        std::cout << "Smallest int is " << smallest << std::endl;
    }
    //Buffer test
    /*void *ptr = malloc(sizeof(Buffer) + 4 * sizeof(int));
    Buffer *b = new (ptr) Buffer(4);
    int inArr[2] = {1,2};
    b->insert(inArr, 2);
    b->insert(3);
    for (int i = 0; i < b->getNumElements(); i++)
    {
        std::cout << b->getStart()[i] << " ";
    }*/
    
    //Level test
    /*void *ptr = malloc(100000);
    Level *l = new (ptr) Level(9);
    int inArr[5] = {9,8,7,6,5};
    int inArr2[5] = {0,8,1,6,2};
    l->push(inArr, 5);
    l->push(inArr2, 5);
    for(int i = 0; i < l->downBufferHead->getNumElements(); i++)
    {
        std::cout << l->downBufferHead->getStart()[i] << " ";
    }*/
    return 0;
}


