//
//  Buffer.hpp
//  CacheObliviousPriorityQueue
//
//  Created by Kaitlyn Reinan on 3/9/17.
//  Copyright © 2017 KaitlynReinan. All rights reserved.
//

#ifndef Buffer_hpp
#define Buffer_hpp

#include <stdio.h>

class Buffer
{
public:
    Buffer(int elements);
    int getCapacity();
    int* getStart();
    void setNumElements(int elements);
    int getNumElements();
    void insert(int* elements, int nElements);
    void insert(int element);
    int *getLastElement();
    void empty();
protected:
    int *start;
    int numElements;
    
private:
    int capacity;
    
};

class IDBuffer : public Buffer
{
public:
    IDBuffer(int elements);
};

class DownBuffer : public Buffer
{
public:
    DownBuffer(int elements);
    DownBuffer* getNext();
    DownBuffer* getPrev();
    void setNext(DownBuffer *n);
    void setPrev(DownBuffer *p);
    bool isFree();
    void setFree();
    void fill();
    void setPivot(int *newPivot);
    int getPivot();
    void split(DownBuffer *newBuff);

private:
    
    DownBuffer *next;
    DownBuffer *prev;
    bool free;
    int *pivot;
};

#endif /* Buffer_hpp */
