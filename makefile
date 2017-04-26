CC = clang++
CCFLAGS = -std=c++11

priorityqueue: main.o pqueue.o level.o buffer.o
	$(CC) $(CCFLAGS) -o priorityqueue main.o pqueue.o level.o buffer.o

main.o: main.cpp
	$(CC) $(CCFLAGS) -c main.cpp

pqueue.o: PQueue.cpp PQueue.hpp
	$(CC) $(CCFLAGS) -c PQueue.cpp

level.o: Level.cpp Level.hpp
	$(CC) $(CCFLAGS) -c Level.cpp

buffer.o: Buffer.cpp Buffer.hpp
	$(CC) $(CCFLAGS) -c Buffer.cpp
