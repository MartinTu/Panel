/*
 * BlockingQueue.h
 *
 *  Created on: 16.04.2013
 *      Author: martin
 */

#ifndef BLOCKINGQUEUE_H_
#define BLOCKINGQUEUE_H_

#include <list>
#include <pthread.h>
#include <iostream>

template<typename T>
class BlockingQueue
{
    public:
        BlockingQueue();
        virtual ~BlockingQueue();

        int setSize(size_t size);
        unsigned int getSize();
        void put(T object);
        T take();

    protected:
        std::list<T> q;
        pthread_mutex_t readMutex;
        pthread_mutex_t writeMutex;
        pthread_mutex_t accessMutex;
        size_t maxSize;
};

#include "BlockingQueue.hpp"

#endif /* BLOCKINGQUEUE_H_ */
