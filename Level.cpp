#include "Level.hpp"
#include <math.h>
#include <iostream>

Level::Level(double s)
{
    size = s;
    memSize = (ceil(pow(s,1.0/3.0)) + 1) * (2 * floor(pow(s,2.0/3.0)) * sizeof(int) + sizeof(DownBuffer)) + floor(s) * sizeof(int) + sizeof(Buffer) + sizeof(Level);
    upBuffer = new (this + sizeof(Level)) Buffer(floor(size));
    downBuffers = new DownBuffer*[(int)ceil(pow(size,1.0/3.0))];
    makeDownBuffers((DownBuffer *)(upBuffer + upBuffer->getCapacity() * sizeof(int) + sizeof(Buffer)));
    downBufferHead = downBuffers[0];
}

double Level::getSize()
{
    return size;
}

Level* Level::getNext()
{
    return nextLevel;
}

Level* Level::getPrev()
{
    return nextLevel;
}

void Level::setNext(Level *l)
{
    nextLevel = l;
}

void Level::setPrev(Level *l)
{
    prevLevel = l;
}

int Level::getMemSize()
{
    return memSize;
}

void Level::push(int *elements, int numElements)
{
    std::sort(elements, elements + numElements);
    for(int i = 0; i < numElements; i++)
    {
        std::cout << "elements[" << i << "]: " << elements[i] << std::endl;
    }
    DownBuffer *buff = downBufferHead;
    while(numElements > 0 && buff){
        if(buff->isFree())
        {
            buff->fill();
            buff->setNumElements(numElements);
            std::copy(elements, elements + numElements, buff->getStart());
            buff->setPivot(buff->getLastElement() - sizeof(int));
            numElements = 0;
        }
        else
        {
            int elementsToInsert = 0;
            int pivot = buff->getPivot();
            for(int i = 0; i < numElements; i++)
            {
                std::cout << "pivot: " << pivot << std::endl;
                if(elements[i] > pivot)
                {
                    break;
                }
                elementsToInsert++;
            }
            while(buff->getCapacity() - buff->getNumElements() < elementsToInsert)
            {
                DownBuffer *slot = freeBuffer();
                buff->split(slot);
                elementsToInsert = 0;
                int pivot = buff->getPivot();
                for(int i = 0; i < numElements; i++)
                {
                    if(elements[i] > pivot)
                    {
                        break;
                    }
                    elementsToInsert++;
                }
            }
            buff->insert(elements, elementsToInsert);
            buff = buff->getNext();
            elements += elementsToInsert * sizeof(int);
            numElements -= elementsToInsert;
        }
        
        if(numElements > 0)
        {
            std::cout << "numElements: " << numElements << std::endl;
            insertUp(elements, numElements);
        }
    }
}

void Level::pull(int *elements, int numElements)
{
    if(downBufferHead->getNumElements() >= numElements)
    {
        for (int i = 0; i < downBufferHead->getNumElements(); i++)
        {
            std::cout << "downbufferhead[" << i << "]: " << downBufferHead->getStart()[i] << std::endl;
        }
        std::sort(downBufferHead->getStart(), downBufferHead->getLastElement());
        std::copy(downBufferHead->getStart(), downBufferHead->getStart() + numElements, elements);
        downBufferHead->setNumElements(downBufferHead->getNumElements() - numElements);
        //downBufferHead->setLast(downBufferHead->getStart() + downBufferHead->getNumElements() * sizeof(int));
        if(downBufferHead->getNumElements() > numElements)
        {
            int temp[downBufferHead->getNumElements()];
            std::copy(&downBufferHead->getStart()[numElements], downBufferHead->getStart() + downBufferHead->getNumElements() - numElements, temp);
            std::copy(temp, temp + downBufferHead->getNumElements(), downBufferHead->getStart());
            //downBufferHead->setLast(downBufferHead->getStart() + downBufferHead->getNumElements() * sizeof(int));
        }
    }
    else
    {
        std::copy(downBufferHead->getStart(), downBufferHead->getStart() + downBufferHead->getNumElements(), elements);
        int copied = downBufferHead->getNumElements();
        int remaining = numElements - copied;
        if(downBufferHead->getNext()){
            DownBuffer *temp = downBufferHead;
            downBufferHead->getNext()->setPrev(nullptr);
            downBufferHead = downBufferHead->getNext();
            temp->setFree();
            std::cout << "remaining: " << remaining << " elements in down buffer head: " << downBufferHead->getNumElements() << std::endl;
            std::sort(downBufferHead->getStart(), downBufferHead->getLastElement());
            std::copy(downBufferHead->getStart(), downBufferHead->getStart() + remaining, elements + copied * sizeof(int));
            downBufferHead->setNumElements(downBufferHead->getNumElements() - remaining);
            int temp2[downBufferHead->getNumElements()];
            std::copy(downBufferHead->getStart() + remaining * sizeof(int), downBufferHead->getStart() + downBufferHead->getNumElements() - remaining, temp2);
            std::copy(temp2, temp2 + remaining, downBufferHead->getStart());
        }
        else{
            int sortSize = (int)floor(size) + upBuffer->getNumElements();
            int pulledElements[sortSize];
            nextLevel->pull(pulledElements, floor(size));
            std::copy(upBuffer->getStart(), upBuffer->getStart() + upBuffer->getNumElements(), &pulledElements[(int)floor(size)]);
            std::sort(pulledElements, &pulledElements[sortSize]);
            std::copy(&pulledElements[(int)floor(size)], pulledElements + sortSize, upBuffer->getStart());
            std::copy(pulledElements, pulledElements + numElements, elements + copied * sizeof(int));
            int elementsToDistribute = sortSize - numElements - upBuffer->getNumElements();
            int *distElements = &pulledElements[numElements];
            DownBuffer *buff = downBufferHead;
            int maxSize = floor(pow(size, 2.0/3.0));
            while(elementsToDistribute > 0)
            {
                int els;
                if(elementsToDistribute > maxSize)
                {
                    els = maxSize;
                    DownBuffer *newBuff = freeBuffer();
                    buff->setNext(newBuff);
                    newBuff->setPrev(buff);
                }
                else{
                    els = elementsToDistribute;
                }
                std::copy(distElements, distElements + els, buff->getStart());
                //buff->setLast(buff->getStart() + els * sizeof(int));
                buff->setNumElements(els);
                elementsToDistribute -= els;
                distElements += els * sizeof(int);
                buff = buff->getNext();
            }
        }
    }
}

DownBuffer* Level::getLast()
{
    DownBuffer *cur = downBufferHead;
    while(cur->getNext())
    {
        cur = cur->getNext();
    }
    return cur;
}

void Level::insertUp(int *elements, int numElements)
{
    int elementsUp = std::min(numElements, upBuffer->getCapacity() - upBuffer->getNumElements());
    upBuffer->insert(elements, elementsUp);
    int remaining = numElements - elementsUp;
    if(remaining > 0)
    {
        nextLevel->push(upBuffer->getStart(), upBuffer->getNumElements());
        //upBuffer->setLast(upBuffer->getStart());
        upBuffer->setNumElements(0);
        upBuffer->insert(elements + elementsUp * sizeof(int), remaining);
    }
    
}

void Level::makeDownBuffers(DownBuffer *loc)
{
    int downSize = 2 * floor(pow(size,2.0/3.0));
    for(int i = 0; i <= ceil(pow(size,1.0/3.0)); i++)
    {
        DownBuffer *newBuff = new (loc) DownBuffer(downSize);
        downBuffers[i] = newBuff;
        loc += newBuff->getCapacity() * sizeof(int) + sizeof(DownBuffer);
    }
}

DownBuffer* Level::freeBuffer()
{
    DownBuffer *slot = nullptr;
    for(int j = 0; j <= ceil(pow(size,1.0/3.0)); j++){
        if(downBuffers[j]->isFree())
        {
            slot = downBuffers[j];
            break;
        }
    }
    if(!slot)
    {
        DownBuffer *last = getLast();
        insertUp(last->getStart(), last->getNumElements());
        last->getPrev()->setNext(nullptr);
        last->setFree();
        slot = last;
    }
    return slot;
}
