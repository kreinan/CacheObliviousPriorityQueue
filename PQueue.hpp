//
//  PQueue.hpp
//  CacheObliviousPriorityQueue
//
//  Created by Kaitlyn Reinan on 3/9/17.
//  Copyright © 2017 KaitlynReinan. All rights reserved.
//

#ifndef PQueue_hpp
#define PQueue_hpp

#include <stdio.h>
#include "Level.hpp"
#include <math.h>
#include <new>
#include <iostream>

class PQueue
{
    
public:
    PQueue(int N);
    void insert(int element);
    int deleteMin();
private:
    Level *head;
    Level *tail;
    int size;
    static const int threshold = 3;
    int N0;
    double c;
    void *ptr;
    IDBuffer *insertionBuffer;
    IDBuffer *deletionBuffer;
    double computec(double n0);
    void createLevels(Level *l, Level *loc);
};

#endif /* PQueue_hpp */
