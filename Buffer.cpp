#include <iostream>
#include "Buffer.hpp"

Buffer::Buffer(int elements)
{
    capacity = elements;
    start = new (this + 1) int[elements];
    numElements = 0;
}

Buffer::Buffer()
{
    capacity = 0;
    start = nullptr;
    numElements = 0;
}

int Buffer::getCapacity()
{
    return capacity;
}


int* Buffer::getStart()
{
    return start;
}

void Buffer::setNumElements(int elements)
{
    numElements = elements;
    assert(numElements <= capacity);
}

int Buffer::getNumElements()
{
    assert(numElements <= capacity);
    return numElements;
}

void Buffer::insert(int* elements, int nElements)
{
    if(capacity - numElements >= nElements)
    {
        std::copy(elements, elements + nElements, getLastElement());
        numElements += nElements;
        assert(numElements <= capacity);
    }
    else{
        std::cout << "not enough space" << std::endl;
    }
}

void Buffer::insert(int element)
{
    //std::cout << getLastElement() << std::endl;
    start[numElements] = element;
    numElements++;
}

int* Buffer::getLastElement()
{
    return start + numElements;
}

void Buffer::empty()
{
    numElements = 0;
}

void* Buffer::getEnd()
{
    return (void *)(start + capacity);
}

IDBuffer::IDBuffer(int elements) : Buffer()
{
    capacity = elements;
    start = new int[elements];
}

DownBuffer::DownBuffer(int elements) : Buffer()
{
    //prev = nullptr;
    capacity = elements;
    start = new (this + 1) int[elements];
    free = true;
    next = nullptr;
    prev = nullptr;
    pivot = nullptr;
}

DownBuffer* DownBuffer::getNext()
{
    return next;
}

DownBuffer* DownBuffer::getPrev()
{
    return prev;
}

void DownBuffer::setNext(DownBuffer *n)
{
    next = n;
}

void DownBuffer::setPrev(DownBuffer *p)
{
    prev = p;
}

bool DownBuffer::isFree()
{
    return free;
}

void DownBuffer::setFree()
{
    free = true;
    prev = nullptr;
    next = nullptr;
    pivot = nullptr;
}

void DownBuffer::fill()
{
    free = false;
}

void DownBuffer::setPivot(int *newPivot)
{
    pivot = newPivot;
}

int DownBuffer::getPivot()
{
    return *pivot;
}

void DownBuffer::split(DownBuffer *newBuff)
{
    std::sort(start, getLastElement());
    int thisElements = (int)(numElements/2);
    int otherElements = numElements - thisElements;
    int *mid = start + thisElements;
    std::copy(mid, mid + otherElements, newBuff->getStart());
    newBuff->fill();
    this->setNumElements(thisElements);
    newBuff->setNumElements(otherElements);
    newBuff->setPivot(newBuff->getStart() + otherElements - 1);
    newBuff->setNext(this->getNext());
    this->setNext(newBuff);
    newBuff->setPrev(this);
    pivot = mid - 1;
}
