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
#include <cassert>

template <typename T>
class Buffer
{
public:
    Buffer(int *_start, int elements);
    int getCapacity();
    int* getStart();
    void setNumElements(int elements);
    int getNumElements();
    void insert(int* elements, int nElements);
    void insert(int element);
    T *getLastElement();
    void empty();
    void *getEnd();
    Buffer* getNext();
    Buffer* getPrev();
    void setNext(Buffer *n);
    void setPrev(Buffer *p);
    bool isFree();
    void setFree();
    void fill();
    void setPivot(int *newPivot);
    int getPivot();
    void split(Buffer *newBuff);

protected:
    Buffer();
    int numElements;
    int *start;
    int capacity;
    Buffer *next;
    Buffer *prev;
    int *pivot;
    bool free;    
};

template <typename T>
class IDBuffer : public Buffer<T>
{
public:
    IDBuffer(int elements);
    void setNext(Buffer<T>* n) = delete;
    void setPrev(Buffer<T>* p) = delete;
};

#endif /* Buffer_hpp */
