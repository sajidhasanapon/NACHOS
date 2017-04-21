// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create several threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustrate the inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.
//
// Parts from Copyright (c) 2007-2009 Universidad de Las Palmas de Gran Canaria
//

#include "copyright.h"
#include "system.h"

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 10 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"name" points to a string with a thread name, just for
//      debugging purposes.
//----------------------------------------------------------------------




//----------------------------------------------------------------------

#include "producer.h"
#include "consumer.h"
#include "sharedBuffer.h"

//----------------------------------------------------------------------


void
SimpleThread(void* name)
{
    // Reinterpret arg "name" as a string
    char* threadName = (char*)name;

    // If the lines dealing with interrupts are commented,
    // the code will behave incorrectly, because
    // printf execution may cause race conditions.
    for (int num = 0; num < 10; num++) {
        //IntStatus oldLevel = interrupt->SetLevel(IntOff);
	printf("*** thread %s looped %d times\n", threadName, num);
	//interrupt->SetLevel(oldLevel);
        //currentThread->Yield();
    }
    //IntStatus oldLevel = interrupt->SetLevel(IntOff);
    printf(">>> Thread %s has finished\n", threadName);
    //interrupt->SetLevel(oldLevel);
}


void StartProducing(void* argument)
{
    Producer* p = (Producer*) argument;
    p->StartProducing();
}


void StartConsuming(void* argument)
{
    Consumer* c = (Consumer*) argument;
    c->StartConsuming();
}


















//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between several threads, by launching
//	ten threads which call SimpleThread, and finally calling
//	SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
    int PRODUCER_CNT = 3;
    int CONSUMER_CNT = 5;
    int BUFFER_SIZE = 10;

    DEBUG('t', "Entering SimpleTest");

    Lock* tableLock = new Lock("Table lock");

    Lock* productionLock = new Lock("Production lock");
    Lock* consumptionLock = new Lock("Consumption lock");

    Condition* productionCondition = new Condition("Production Condition", tableLock);
    Condition* consumptionCondition = new Condition("Consumption Condition", tableLock);

    SharedBuffer* foodTable = new SharedBuffer(BUFFER_SIZE);


    for (int i = 1; i <= PRODUCER_CNT; i++)
    {
      char* producerName = new char[50];
      sprintf(producerName, "Producer %d", i);
      Producer* producer = new Producer(producerName, tableLock, productionCondition, consumptionCondition, foodTable);
      Thread* newThread = new Thread (producerName);
      newThread->Fork (StartProducing, (void*)producer);
    }

    for (int i = 1; i <= CONSUMER_CNT; i++)
    {
      char* consumerName = new char[50];
      sprintf(consumerName, "Consumer %d", i);
      Consumer* consumer = new Consumer(consumerName, tableLock, productionCondition, consumptionCondition, foodTable);
      Thread* newThread = new Thread (consumerName);
      newThread->Fork (StartConsuming, (void*)consumer);
    }



//    for ( int k=1; k<=10; k++) {
//      char* threadname = new char[100];
//      sprintf(threadname, "Hilo %d", k);
//      Thread* newThread = new Thread (threadname);
//      newThread->Fork (SimpleThread, (void*)threadname);
//    }
//
//   SimpleThread( (void*)"Hilo 0");
}

