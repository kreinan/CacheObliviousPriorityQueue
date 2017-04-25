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
    return 0;
}


