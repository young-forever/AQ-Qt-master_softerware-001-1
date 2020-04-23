//#include "circle_queue.h"
//#include <iostream>
//using namespace std;

//template <class T>
//Circle_Queue<T>::Circle_Queue(int queueCapacity)
//{

//    m_iQueueCapacity = queueCapacity;
//    m_pQueue = new T[m_iQueueCapacity];
//    ClearQueue();

//}

//template <class T>
//Circle_Queue<T>::~Circle_Queue()
//{
//    delete[] m_pQueue;
//    m_pQueue = NULL;
//}

//template <class T>
//void Circle_Queue<T>::ClearQueue()
//{
//    m_iHead = 0;
//    m_iTail = 0;
//    m_iQueueLen = 0;
//}


//template <class T>
//bool Circle_Queue<T>::QueueEmpty()
//{
//    return m_iQueueLen == 0 ?  true : false;
//}


//template <class T>
//bool Circle_Queue<T>::QueueFull()
//{
//    return m_iQueueLen == m_iQueueCapacity ? true : false;
//}



//template <class T>
//bool Circle_Queue<T>::EnQueue(T element)
//{
//    if (QueueFull())
//    {
//        int mr;
//        DeQueue(mr);

//        m_pQueue[m_iTail] = element;
//        m_iTail++;
//        m_iTail = m_iTail % m_iQueueCapacity;
//        m_iQueueLen++;
//        return false;
//    }
//    else {
//        m_pQueue[m_iTail] = element;
//        m_iTail++;
//        m_iTail = m_iTail % m_iQueueCapacity;
//        m_iQueueLen++;
//        return true;
//    }
//}


//template <class T>
//bool Circle_Queue<T>::DeQueue(T &element) //传入引用是为了可以直接修改实参的值，
//{
//    if (QueueEmpty())
//    {
//        return false;
//    }
//    else {
//        element = m_pQueue[m_iHead] ;
//        m_iHead++;
//        m_iHead = m_iHead % m_iQueueCapacity;
//        m_iQueueLen--;
//        return true;
//    }
//}

//template <class T>
//QList<T> Circle_Queue<T>::QueueTraverse(int cnt)
//{
//    int head;
//    if(cnt>=m_iQueueLen)
//    {
//        head = m_iHead;
//        cnt = m_iQueueLen;
//    }
//    else
//    {
//        head = (m_iHead + m_iQueueLen - cnt)%m_iQueueCapacity;
//    }

//    data.clear();
//    for (int i = head; i < head + cnt; i++)
//    {
////        cout << m_pQueue[i%m_iQueueCapacity] << endl;
//        data.append(m_pQueue[i%m_iQueueCapacity]);
//    }

//    return data;
//}


//template <class T>
//int Circle_Queue<T>::QueueLength() const
//{
//    return m_iQueueLen;
//}

