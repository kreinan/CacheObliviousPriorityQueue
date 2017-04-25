#include "PQueue.hpp"

PQueue::PQueue(int N)
{
    size = 0;
    N0 = 2 * N;
    c = computec(N0);
    //std::cout << size << std::endl;
    ptr = new int[size];
    head = new (ptr)Level(c);
    createLevels(head, ptr + head->getMemSize() * sizeof(int));
    insertionBuffer = new IDBuffer(floor(pow(c, 2.0/3.0)));
    deletionBuffer = new IDBuffer(floor(pow(c, 2.0/3.0)));
}

void PQueue::insert(int element)
{
    //deletionBuffer->printElements();
    if(deletionBuffer->getNumElements() < deletionBuffer->getCapacity())
    {
        deletionBuffer->insert(element);
        //std::cout << *&element <<std::endl;
        /**deletionBuffer->getLastElement() = element;
         deletionBuffer->setLast(deletionBuffer->getLastElement() + sizeof(int));
         deletionBuffer->setNumElements(deletionBuffer->getNumElements() + 1);*/
    }
    else{
        int *max = deletionBuffer->getStart();
        for(int i = 1; i < deletionBuffer->getNumElements(); i++)
        {
            if(deletionBuffer->getStart()[i] > *max)
            {
                max = &deletionBuffer->getStart()[i];
            }
        }
        int elementToInsert;
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
            head->push(insertionBuffer->getStart(), insertionBuffer->getNumElements());
            insertionBuffer->empty();
        }
        insertionBuffer->getStart()[insertionBuffer->getNumElements()] = elementToInsert;
        //insertionBuffer->setLast(insertionBuffer->getLastElement() + sizeof(int));
        insertionBuffer->setNumElements(insertionBuffer->getNumElements() + 1);
    }
}

int PQueue::deleteMin()
{
    int *min = deletionBuffer->getStart();
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
    
    //*min = *deletionBuffer->getLastElement() - sizeof(int);
    //deletionBuffer->setLast(deletionBuffer->getLastElement() - sizeof(int));
    deletionBuffer->setNumElements(deletionBuffer->getNumElements() - 1);
    if(deletionBuffer->getNumElements() == 0)
    {
        int tempSize = (int)floor(pow(c,2.0/3.0)) + insertionBuffer->getNumElements();
        int temp[tempSize];
        head->pull(temp, (int)floor(pow(c,2.0/3.0)));
        std::copy(insertionBuffer->getStart(), insertionBuffer->getCapacity() * sizeof(int) + insertionBuffer->getStart(), &temp[(int)floor(pow(c,2.0/3.0))]);
        std::sort(&temp[0], &temp[tempSize]);
        std::copy(&temp[0], &temp[(int)floor(pow(c,2.0/3.0))], deletionBuffer->getStart());
        //deletionBuffer->setLast(deletionBuffer->getStart() + deletionBuffer->getCapacity() * sizeof(int));
        deletionBuffer->setNumElements((int)floor(pow(c,2.0/3.0)));
        std::copy(&temp[(int)floor(pow(c,2.0/3.0))], &temp[tempSize], insertionBuffer->getStart());
    } else if(minPos < deletionBuffer->getCapacity() - 1) {
        int copySize = deletionBuffer->getCapacity() - minPos - 1;
        int temp[copySize];
        std::copy(&deletionBuffer->getStart()[minPos + 1], deletionBuffer->getStart() + deletionBuffer->getCapacity(), temp);
        std::copy(temp, temp + copySize, &deletionBuffer->getStart()[minPos]);
    }
    
    return ret;
}

double PQueue::computec(double n0)
{
    double c = n0;
    if(n0 >= threshold)
    {
        c = computec(pow(n0,2.0/3.0));
    }
    size += (ceil(pow(n0,1.0/3.0)) + 1) * (2 * floor(pow(n0,2.0/3.0)) * sizeof(int) + sizeof(DownBuffer)) + floor(n0) * sizeof(int) + sizeof(Buffer) + sizeof(Level);
    return c;
}

void PQueue::createLevels(Level *l, int *loc){
    if(l->getSize() < N0)
    {
        Level *n = new (loc) Level(pow(l->getSize(),3.0/2.0));
        l->setNext(n);
        n->setPrev(l);
        createLevels(n, loc + n->getMemSize());
    }
}

