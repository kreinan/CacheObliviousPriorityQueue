//
//  Level.hpp
//  CacheObliviousPriorityQueue
//
//  Created by Kaitlyn Reinan on 3/9/17.
//  Copyright © 2017 KaitlynReinan. All rights reserved.
//

#ifndef Level_hpp
#define Level_hpp

#include <stdio.h>
#include "Buffer.hpp"

class Level
{
public:
    Level(double s);
    double getSize();
    Level* getNext();
    Level* getPrev();
    void setNext(Level *l);
    void setPrev(Level *l);
    int getMemSize();
    void push(int *elements, int numElements);
    void pull(int *elements, int numElements);
    DownBuffer *getLast();
    void insertUp(int *elements, int numElements);
    
private:
    DownBuffer *downBufferHead;
    Level *nextLevel;
    Level *prevLevel;
    double size;
    int memSize;
    Buffer *upBuffer;
    DownBuffer **downBuffers;
    void makeDownBuffers(DownBuffer *loc);
    DownBuffer *freeBuffer();
};

#endif /* Level_hpp */
