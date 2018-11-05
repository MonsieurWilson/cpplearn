#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>


namespace lxy
{

template <typename ElementType>
class Queue {
    public:
        using size_type = unsigned int;

    private:
        class LinkNode {
            public:
                ElementType data;
                LinkNode *  next;
                explicit LinkNode(const ElementType &val, LinkNode *n=nullptr)
                    : data(val), next(n) {}
        };

        using NodePtr = LinkNode *;

    public:
        /*
         * Name: 
         *  Queue::Queue()
         * 
         * Description:
         *  The default constructors of a Queue based on linked list.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  None.
         *
         * Note:
         *  None.
         */
        Queue();

        /*
         * Name: 
         *  Queue::Queue(const Queue &)
         * 
         * Description:
         *  The copy constructors of a Queue based on linked list.
         *
         * Parameters:
         *  original - [IN] const reference of a Queue object.
         *
         * Returns:
         *  None.
         *
         * Note:
         *  None.
         */
        explicit Queue(const Queue &original);

        /*
         * Name: 
         *  Queue::operator=(const Queue &)
         * 
         * Description:
         *  The copy assignment operators of a Queue based on linked list.
         *
         * Parameters:
         *  rightHandSide - [IN] const reference of a Queue object.
         *
         * Returns:
         *  Const reference of this object.
         *
         * Note:
         *  The calling of `lhs = rhs` equals `lhs.operator=(rhs)`.
         */
        const Queue & operator=(const Queue &rightHandSide);

        /*
         * Name: 
         *  Queue::~Queue
         * 
         * Description:
         *  The destructors of a Queue based on linked list.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  None.
         *
         * Note:
         *  None.
         */
        ~Queue() noexcept;

        /*
         * Name: 
         *  Queue::push(const ElementType &)
         * 
         * Description:
         *  The enqueue operation for Queue.
         *
         * Parameters:
         *  elem - [IN] element to be enqueued.
         *
         * Returns:
         *  None.
         *
         * Note:
         *  Queue has no limit for the number of elements that it can contain.
         */
        void push(const ElementType &elem);

        /*
         * Name: 
         *  Queue::pop()
         * 
         * Description:
         *  The dequeue operation for Queue.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  true  - success.
         *  false - failure.
         *
         * Note:
         *  The caller should be aware whether Queue is empty. Pop on a 
         *  empty queue will cause an undefined result. Taking such reason
         *  into account, Queue will return a boolean value to let caller
         *  know what happened.
         */
        bool pop();

        /*
         * Name: 
         *  Queue::front()
         * 
         * Description:
         *  To get the first element of Queue.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  The first element.
         *
         * Note:
         *  The caller should be aware whether Queue is empty. It's an 
         *  undefined operation to operate `front` on a empty queue.
         */
        ElementType & front();

        /*
         * Name: 
         *  Queue::back()
         * 
         * Description:
         *  To get the lastest element of Queue.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  The lastest element.
         *
         * Note:
         *  The caller should be aware whether Queue is empty. It's an 
         *  undefined operation to operate `back` on a empty queue.
         */
        ElementType & back();

        /*
         * Constant member function for `front` and `back`.
         */
        const ElementType & front() const;
        const ElementType & back() const;


        /*
         * Name: 
         *  Queue::size()
         * 
         * Description:
         *  To get the number of elements in Queue.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  Number of elements.
         *
         * Note:
         *  None.
         */
        size_type size() const;

        /*
         * Name: 
         *  Queue::empty()
         * 
         * Description:
         *  Check whether Queue is empty or not.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  true  - empty.
         *  false - nonempty.
         *
         * Note:
         *  None.
         */
        bool empty() const;

        /*
         * Name:
         *  Queue::traverse(std::ostream &)
         *
         * Description:
         *  Traverse Queue and redirect outputs to ostrearm "out"
         *
         * Parameters:
         *  out - [IN] std::ostream object.
         *
         * Returns:
         *  None.
         *
         * Note:
         *  None.
         */
        void traverse(std::ostream &out) const;

        /*
         * Name:
         *  Queue::swap(Queue&)
         *
         * Description:
         *  Swap two Queues.
         *
         * Parameters:
         *  rightHandSide - [IN] reference of a Queue object.
         *
         * Returns:
         *  None.
         *
         * Note:
         *  Swap mustn't throw any exception.
         */
        void swap(Queue &rightHandSide) noexcept;

    private:
        NodePtr   m_tail;
        size_type m_size;
}; // template class Queue

template <typename ElementType>
Queue<ElementType>::Queue() : m_tail(nullptr), m_size(0) {}

template <typename ElementType>
Queue<ElementType>::Queue(const Queue<ElementType> &original)
    : m_tail(nullptr), m_size(original.m_size) {
    if (!original.empty()) {
        try {
            m_tail = new Queue<ElementType>::LinkNode(original.front());
            m_tail->next = m_tail;

            Queue<ElementType>::NodePtr origHead = original.m_tail->next, 
                cur = origHead->next;
            while (cur != origHead) {
                m_tail->next =
                    new Queue<ElementType>::LinkNode(cur->data, m_tail->next);
                m_tail = m_tail->next;
                cur = cur->next;
            }
        } catch (std::bad_alloc &err) {
            std::cerr << "Can't allocate new LinkNode: "
                      << err.what() << std::endl;
            std::abort();
        }
    }
}

template <typename ElementType>
const Queue<ElementType> &
Queue<ElementType>::operator=(const Queue<ElementType> &rightHandSide) {
    Queue<ElementType> rhs(rightHandSide);
    swap(rhs);
    return *this;
}

template <typename ElementType>
Queue<ElementType>::~Queue() noexcept {
    if (!empty()) {
        Queue<ElementType>::NodePtr head = m_tail->next;

        while (head != m_tail) {
            m_tail->next = head->next;
            delete head;
            head = m_tail->next;
        }

        delete m_tail;
        m_tail = nullptr;
        m_size = 0;
    }
}

template <typename ElementType>
void Queue<ElementType>::push(const ElementType &elem) {
    try {
        if (empty()) {
            m_tail = new Queue<ElementType>::LinkNode(elem);
            m_tail->next = m_tail;
        } else {
            m_tail->next =
                new Queue<ElementType>::LinkNode(elem, m_tail->next);
            m_tail = m_tail->next;
        }
    } catch (std::bad_alloc &err) {
        std::cerr << "Can't allocate new LinkNode: "
                  << err.what() << std::endl;
        std::abort();
    }
    ++m_size;
}

template <typename ElementType>
bool Queue<ElementType>::pop() {
    if (!empty()) {
        Queue<ElementType>::NodePtr head = m_tail->next;

        if (head == m_tail) {
            m_tail = nullptr;
        } else {
            m_tail->next = head->next;
        }
        delete head;
        --m_size;

        return true;
    }

    return false;
}

template <typename ElementType>
ElementType & Queue<ElementType>::front() {
    return const_cast<ElementType>(
           static_cast<const Queue<ElementType> &>(*this)
           .front());
}

template <typename ElementType>
ElementType & Queue<ElementType>::back() {
    return const_cast<ElementType>(
           static_cast<const Queue<ElementType> &>(*this)
           .back());
}

template <typename ElementType>
const ElementType & Queue<ElementType>::front() const {
    return m_tail->next->data;
}

template <typename ElementType>
const ElementType & Queue<ElementType>::back() const {
    return m_tail->data;
}

template <typename ElementType>
typename Queue<ElementType>::size_type 
Queue<ElementType>::size() const {
    return m_size;
}

template <typename ElementType>
bool Queue<ElementType>::empty() const {
    return m_tail == nullptr;
}

template <typename ElementType>
void Queue<ElementType>::traverse(std::ostream &out) const {
    out << "{ [lxy::Queue]";
    if (!empty()) {
        Queue<ElementType>::NodePtr head = m_tail->next;
        out << " " << head->data;

        while (head != m_tail) {
            head = head->next;
            out << " " << head->data;
        }
    }
    out << " }";
}

template <typename ElementType>
void Queue<ElementType>::swap(Queue<ElementType> &rightHandSide)
    noexcept {
    using std::swap;

    Queue<ElementType>::NodePtr tmp = m_tail;
    m_tail = rightHandSide.m_tail;
    rightHandSide.m_tail = tmp;
    
    swap(m_size, rightHandSide.m_size);
}


/* Operator overloading for "<<". */
template <typename ElementType>
std::ostream &operator<<(std::ostream &out, const Queue<ElementType> &q) {
    q.traverse(out);
    return out;
}

/* Customization version `swap` for template type `ElementType`. */
template <typename ElementType>
void swap(Queue<ElementType> &leftHandSide, Queue<ElementType> &rightHandSide)
    noexcept(noexcept(leftHandSide.swap(rightHandSide))) {
    leftHandSide.swap(rightHandSide);
}

} // namespace lxy

#endif
