/*
 * BlockingQueue.cpp
 *
 *  Created on: 16.04.2013
 *      Author: martin
 */

#include "BlockingQueue.h"

template<typename T>
BlockingQueue<T>::BlockingQueue()
{
    this->maxSize = 100;
    pthread_mutex_init(&readMutex, NULL);
    pthread_mutex_init(&writeMutex, NULL);
    pthread_mutex_init(&accessMutex, NULL);

    pthread_mutex_lock (&readMutex);
}

template<typename T>
BlockingQueue<T>::~BlockingQueue()
{
    pthread_mutex_destroy (&readMutex);
    pthread_mutex_destroy (&writeMutex);
    pthread_mutex_destroy (&accessMutex);
}

template<typename T>
int BlockingQueue<T>::setSize(size_t size)
{
    int rt = 0;
    if (size > this->maxSize)
    {
        this->maxSize = size;
    }
    else
    {
        pthread_mutex_lock(&this->accessMutex);
        if (this->q.size() < size)
            this->maxSize = size;
        else
            rt = -1; /* queue is larger than size */
        pthread_mutex_unlock(&this->accessMutex);
    }
    return rt;
}

template<typename T>
unsigned int BlockingQueue<T>::getSize()
{
    unsigned int rt;
    pthread_mutex_lock(&this->accessMutex);
    rt = this->q.size();
    pthread_mutex_unlock(&this->accessMutex);
    return rt;
}

template<typename T>
void BlockingQueue<T>::put(T object)
{

    /* block until the queue is ready for writing access */
    pthread_mutex_lock (&writeMutex);
    pthread_mutex_unlock(&writeMutex);

    pthread_mutex_lock (&accessMutex);
    this->q.push_back(object);

    /* block the queue if the maximum size is reached */
    if (this->q.size() == this->maxSize)
    {
        pthread_mutex_lock(&writeMutex);
    }
    pthread_mutex_unlock(&accessMutex);

    /* unblock the queue for reading access */
    pthread_mutex_unlock (&readMutex);
}

template<typename T>
T BlockingQueue<T>::take()
{
    T object;
    pthread_mutex_lock (&readMutex);
    pthread_mutex_unlock(&readMutex);

    pthread_mutex_lock (&accessMutex);
    object = this->q.front();
    this->q.pop_front();

    if (this->q.empty())
    {
        pthread_mutex_lock(&readMutex);
    }
    pthread_mutex_unlock(&accessMutex);
    pthread_mutex_unlock (&writeMutex);
    return object;
}

