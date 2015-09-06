// This is from Herb Sutter's 'Writing a Generalized Concurrent Queue', on Dr Dobb's Journal (20081029)
// Very straightforward implementation

template < typename T >
struct LowLockQueue
{

private:
    struct Node
    {
        Node( T * val )
        : value( val )
        , next( nullptr )
        {}

        T *                    value;
        std::atomic< Node * >  next;   // protected by 'atomic' as it's shared
        // The padding is there to ensure that two different 'Node' objects cannot share the same
        // cache line (it's conservative)
        char                   pad[ CACHE_LINE_SIZE - sizeof( T* ) - sizeof( std::atomic< Node * > ) ];
    };

public:
    LowLockQueue()
    {
        m_first = m_last = new Node( nullptr );
        m_producerLock = false;
        m_consumerLock = false;
    }

    ~LowLockQueue()
    {
        while( m_first != nullptr )
        {
            Node * tmp = m_first;
            m_first = tmp -> next;
            delete tmp -> value;
            delete tmp;
        }
    }

    void produce( const T & t )
    {
        Node * tmp = new Node( new T( t ) );
        // std::atomic::exchange does an atomic 'Read-Modify-Write', and returns
        // the value before the write, so the loop below waits until it sees
        // m_producerLock == false, and sets it to true.
        while( m_producerLock.exchange( true ) )
        {}
        m_last -> next = tmp;
        m_last         = tmp;
        m_producerLock = false;
    }

    // Returns true iff the queue wasn't empty (in which case result is set)
    bool consume( T & result )
    {
        while( m_consumerLock.exchange( true ) )
        {}

        Node * oldFirst = m_first;
        Node * oldNext  = m_first -> next;
        if( oldNext != nullptr )
        {
            T * val          = oldNext -> value;
            oldNext -> value = nullptr;
            m_first          = oldNext;
            m_consumerLock   = false;
            // copy and cleanup
            result           = *val;
            delete val;
            delete oldFirst;
            return true;
        }
        m_consumerLock = false;
        return false;
    }

private:
    char pad0[ CACHE_LINE_SIZE ];

    // for one consumer at a time
    Node *               m_first;
    char pad1[ CACHE_LINE_SIZE - sizeof( std::atomic< bool > ) ];

    // shared among consumers
    std::atomic< bool >  m_consumerLock;
    char pad2[ CACHE_LINE_SIZE - sizeof( std::atomic< bool > ) ];

    // for one producer at a time
    Node *               m_last;
    char pad3[ CACHE_LINE_SIZE - sizeof( std::atomic< bool > ) ];

    // shared among producers
    std::atomic< bool >  m_producerLock;
    char pad4[ CACHE_LINE_SIZE - sizeof( std::atomic< bool > ) ];
}; // struct LowLockQueue
