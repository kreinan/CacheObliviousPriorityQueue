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
//#include <new>
#include <iostream>

template <typename T>
class PQueue
{
    
public:
    PQueue(int N)
    {
        size = 0;
        numLevels = 0;
        numBuffers = 0;
        N0 = 2 * N;
        c = computec(N0);
        elements = new T[size];
        levels = new Level<T>[numLevels];
        buffers = new Buffer<T>[numBuffers];
        insertionBuffer = new IDBuffer<T>(floor(pow(c, 2.0/3.0)));
        deletionBuffer = new IDBuffer<T>(floor(pow(c, 2.0/3.0)));
        
        Buffer<T> *currBuffer = buffers[0];
        T *currItem = elements[0];
        double x = c;
        for(int i = 0; i < numLevels; i++)
        {
            levels[i] = new Level<T>(x, currBuffer, currItem);
            x = pow(x, 3.0/2.0);
            currBuffer += levels[i].getNumDown() + 1;
            currItem += levels[i].getMemSize();
        }
    }
    ~PQueue()
    {
        delete[] elements;
        delete[] levels;
        delete[] buffers;
    }
    PQueue(PQueue &p) = delete;
    PQueue &operator=(PQueue p) = delete;
    void insert(T element)
    {
        if(deletionBuffer->getNumElements() < deletionBuffer->getCapacity())
        {
            deletionBuffer->insert(element);
        }
        else{
            T *max = deletionBuffer->getStart();
            for(int i = 1; i < deletionBuffer->getNumElements(); i++)
            {
                if(deletionBuffer->getStart()[i] > *max)
                {
                    max = &deletionBuffer->getStart()[i];
                }
            }
            T elementToInsert;
            if(element > *max)
            {
                elementToInsert = element;
            }
            else{
                elementToInsert = *max;
                *max = element;
            }
            if(insertionBuffer->getNumElements() >= insertionBuffer->getCapacity())
            {
                levels[0].push(insertionBuffer->getStart(), insertionBuffer->getNumElements());
                //insertionBuffer->empty(); // Do this in push
            }
            *insertionBuffer->getLastElement() = elementToInsert;
            insertionBuffer->setNumElements(insertionBuffer->getNumElements() + 1);
        }
    }
    T deleteMin()
    {
        T *min = deletionBuffer->getStart();
        int minPos = 0;
        for(int i = 1; i < deletionBuffer->getNumElements(); i++)
        {
            if(deletionBuffer->getStart()[i] < *min)
            {
                min = &deletionBuffer->getStart()[i];
                minPos = i;
            }
        }
        
        int ret = *min;
        
        deletionBuffer->setNumElements(deletionBuffer->getNumElements() - 1);
        if(deletionBuffer->getNumElements() == 0)
        {
            int tempSize = (int)floor(pow(c,2.0/3.0)) + insertionBuffer->getNumElements();
            T temp[tempSize];
            levels[0]->pull(temp, (int)floor(pow(c,2.0/3.0)));
            std::copy(insertionBuffer->getStart(), insertionBuffer->getStart() + insertionBuffer->getCapacity(), &temp[(int)floor(pow(c,2.0/3.0))]);
            std::sort(&temp[0], &temp[tempSize]);
            std::copy(&temp[0], &temp[(int)floor(pow(c,2.0/3.0))], deletionBuffer->getStart());
            deletionBuffer->setNumElements((int)floor(pow(c,2.0/3.0)));
            std::copy(&temp[(int)floor(pow(c,2.0/3.0))], &temp[tempSize], insertionBuffer->getStart());
        } else if(minPos < deletionBuffer->getCapacity() - 1) {
            int copySize = deletionBuffer->getCapacity() - minPos - 1;
            T temp[copySize];
            std::copy(&deletionBuffer->getStart()[minPos + 1], deletionBuffer->getStart() + deletionBuffer->getCapacity(), temp);
            std::copy(temp, temp + copySize, &deletionBuffer->getStart()[minPos]);
        }
        
        return ret;
    }
private:
    int size;
    int numLevels;
    int numBuffers;
    static const int threshold = 3;
    int N0;
    double c;
    T *elements;
    Level<T> *levels;
    Buffer<T> *buffers;
    IDBuffer<T> *insertionBuffer;
    IDBuffer<T> *deletionBuffer;
    double computec(double n0)
    {
        double c = n0;
        if(n0 >= threshold)
        {
            c = computec(pow(n0,2.0/3.0));
        }
        size += (ceil(pow(n0,1.0/3.0)) + 1) * (2 * floor(pow(n0,2.0/3.0))) + floor(n0);
        numBuffers += ceil(pow(n0,1.0/3.0)) + 2;
        numLevels += 1;
        return c;
    }
};

#endif /* PQueue_hpp */
