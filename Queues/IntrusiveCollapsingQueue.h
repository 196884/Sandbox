template < typename T, T* T::*NEXT >
class IntrusiveCollapsingQueueT
{
public:
    typedef uint32_t Size;

private:
    T *   m_head;
    T *   m_tail;
    Size  m_size;

public:
    IntrusiveCollapsingQueueT()
    : m_head( NULL )
    , m_tail( NULL )
    , m_size( 0 )
    {}

    void push( T * t )
    {
        if( NULL == t->*NEXT )
        {
            // t isn't in the queue yet
            if( NULL != m_tail )
            {
                m_tail->*NEXT = t;
                m_tail        = t;
            }
            else
            {
                m_head = t;
                m_tail = t;
            }
            m_tail->*NEXT = m_tail;
            ++m_size;
        }
    }

    Size size() const { return m_size; }

    bool empty() const { return NULL == m_head; }

    T * front() { return m_head; }

    T * pop()
    {
        T * result = m_head;
        if( result )
        {
            m_head = result->*NEXT;
            result->*NEXT = NULL;
            if( m_head == result )
            {
                m_head = NULL;
                m_tail = NULL;
            }
            --size;
        }
        return result;
    }
}; // class IntrusiveCollapsingQueueT
