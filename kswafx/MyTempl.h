#ifndef _MYTEMPL_H_
#define _MYTEMPL_H_

#include <stdlib.h>

#define INCREMENT_COUNT 4

template<class T> class MyList
{
private:
    T **m_Items ;
    int m_ItemsCount;
    int m_ArraySize;

public:
    MyList()
    {
        m_ItemsCount = 0;
        m_ArraySize  = 0;
    }
    ~MyList()
    {
        Clear();
    }

// Operations ---------------------------------------------------
public:
    int Add(T *Item)
    {
        int lError = 0;
        // ---Allocating memory  for first time -------
        if (m_ArraySize == 0)
        {
            m_Items = new T*[INCREMENT_COUNT] ;
            if (m_Items == NULL)
            {
                lError = -1 ;
            }
            else
            {
                m_ArraySize = INCREMENT_COUNT ;
                m_Items[m_ItemsCount] = Item ;
                m_ItemsCount++;
            }
        }
        else if (m_ItemsCount < m_ArraySize)
        {
            // ---Add Items till it reach the allocated array size ---
            m_Items[m_ItemsCount] = Item ;
            m_ItemsCount++;
        }
        else
        {
            // ---Reallocate the memory -----------
            m_Items = (T **)realloc(m_Items ,
                    (INCREMENT_COUNT * sizeof(T *)) +
                               (m_ItemsCount * sizeof(T *)));
            if (m_Items == NULL)
            {
                lError = -1 ;
            }
            else
            {
                m_ArraySize += INCREMENT_COUNT ;
                m_Items[m_ItemsCount] = Item ;
                m_ItemsCount++;
            }
        }
        return(lError);
    }
    // end add

    T* Items(int pIndex)
    {
        if ( (m_ItemsCount == 0)||(pIndex > m_ItemsCount)||(pIndex < 0) )
            return NULL;
        else
            return (m_Items[pIndex]);
    }

    void Clear(void)
    {
        if (m_ArraySize > 0)
        {
            delete[]m_Items;
            m_ArraySize = 0;
            m_ItemsCount = 0;
        }
    }

    void DeleteObjects(void)
    {
        if (m_ArraySize > 0)
        {
            for (int lCount = 0;lCount < m_ItemsCount;lCount++)
                delete Items(lCount);

            delete[]m_Items;
            m_ArraySize = 0;
            m_ItemsCount = 0;
        }
    }

    int Count(void)
    {
        return (m_ItemsCount);
    }

    int Insert(int pIndex ,T* pItem)
    {
        T **lTempItems;
        int lRetValue = -1 ;
        if ((pIndex >= 0)&&(pIndex < m_ItemsCount))
        {
            lTempItems=(T **)malloc((m_ArraySize + 1) * sizeof(T *));
            if (lTempItems != NULL)
            {
                memcpy(lTempItems , m_Items , pIndex * (sizeof(T *)));
                lTempItems[pIndex] = pItem ;
                memcpy(&lTempItems[pIndex + 1],&m_Items[pIndex],
                      (m_ArraySize - pIndex )*(sizeof(T *)));
                delete[] m_Items;
                m_Items = lTempItems;
                m_ItemsCount +=1;
                m_ArraySize +=1;
                lRetValue = 0 ;
            }
        }
        else if (pIndex == m_ItemsCount)
            lRetValue = Add(pItem) ;

        return(lRetValue);
    }

    int Remove(int pIndex)
    {
        T **lTempItems;
        int lRetValue = -1 ;
        if ( (pIndex >= 0)&&(pIndex < m_ItemsCount) )
        {
            if (m_ArraySize == 1)
            {
                delete[] m_Items;
                m_ItemsCount -=1;
                m_ArraySize -=1;
                lRetValue = 0 ;
            }
            else
            {
                lTempItems=(T **)malloc((m_ArraySize -1) * sizeof(T *));
                if (lTempItems != NULL)
                {
                    memcpy(lTempItems , m_Items , (pIndex ) * (sizeof(T *)));
                    memcpy(&lTempItems[pIndex],&m_Items[pIndex+1],
                            (m_ArraySize - pIndex - 1)*(sizeof(T *)));
                    delete[] m_Items;
                    m_Items = lTempItems;
                    m_ItemsCount -=1;
                    m_ArraySize -=1;
                    lRetValue = 0 ;
                }
            }
        }
        return(lRetValue);
    }

};

#endif
