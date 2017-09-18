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

template <typename T>
class Level
{
public:
    Level(double s, Buffer<T> *buffer, T *item)
    {
        size = s;
        numDown = (int)ceil(pow(size,1.0/3.0)) + 1;
        //downBuffers = new (this + 1) DownBuffer*[numDown];
        upBuffer = buffer;
        //makeDownBuffers(addBytes<DownBuffer,Buffer>(upBuffer,upBuffer->getCapacity() * sizeof(int) + sizeof(Buffer)));
        downBufferHead = buffer + 1;
        //nextLevel = nullptr;
        //prevLevel = nullptr;
    }

    double getSize() { return size; }
    int getNumDown() { return numDown; }
    //Level* getNext();
    //Level* getPrev();
    //void setNext(Level *l);
    //void setPrev(Level *l);
    int getMemSize();
    void push(int *elements, int numElements);
    void pull(int *elements, int numElements);
    Buffer<T> *getLast();
    void insertUp(int *elements, int numElements);
    
private:
    //void makeDownBuffers(Buffer *loc);
    Buffer<T> *freeBuffer();
    Buffer<T> *downBufferHead;
    //Level *nextLevel;
    //Level *prevLevel;
    Buffer<T> *upBuffer;
    //Buffer **downBuffers;
    double size;
    int numDown;
};

#endif /* Level_hpp */
