#ifndef BST_H
#define BST_H

#include <stack>
#include <iostream>


namespace lxy
{

template <typename ElementType>
class BST {
    public:
        using size_type = unsigned int;

    private:
        class LinkNode {
            public:
                ElementType data;
                LinkNode *  left;
                LinkNode *  right;
                explicit LinkNode(const ElementType &val, 
                                  LinkNode *l=nullptr, LinkNode *r=nullptr)
                    : data(val), left(l), right(r) {}
        };

        using NodePtr = LinkNode *;

    public:
        /*
         * Name:
         *  BST::BST()
         *
         * Description:
         *  The default constructors of a BST based on linked list.
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
        BST();

        /*
         * Name:
         *  BST::BST(const BST &)
         *
         * Description:
         *  The copy constructors of a BST based on linked list.
         *
         * Parameters:
         *  original - [IN] const reference of a BST object.
         *
         * Returns:
         *  None.
         *
         * Note:
         *  None.
         */
        explicit BST(const BST &original);

        /*
         * Name:
         *  BST::operator=(const BST &)
         *
         * Description:
         *  The copy assignment operators of a BST based on linked list.
         *
         * Parameters:
         *  rightHandSide - [IN] const reference of a BST object.
         *
         * Returns:
         *  Const reference of this object.
         *
         * Note:
         *  The calling of `lhs = rhs` equals `lhs.operator=(rhs)`.
         */
        const BST & operator=(const BST &rightHandSide);

        /*
         * Name:
         *  BST::~BST()
         *
         * Description:
         *  The destructors of a BST based on a linked list.
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
        ~BST() noexcept;

        /*
         * Name:
         *  BST::insert(const ElementType &)
         *
         * Description:
         *  Insert an element to BST.
         *
         * Parameters:
         *  elem - [IN] element to be inserted.
         *
         * Returns:
         *  true  - success.
         *  false - failure.
         *
         * Note:
         *  None.
         */
        bool insert(const ElementType &elem);

        /*
         * Name:
         *  BST::remove(const ElementType &)
         *
         * Description:
         *  Remove an element of BST.
         *
         * Parameters:
         *  elem - [IN] element to be removed.
         *
         * Returns:
         *  true  - success.
         *  false - failure.
         *
         * Note:
         *  Generally, the operation will be failed when the given element
         *  is NOT a member of BST.
         */
        bool remove(const ElementType &elem);

        /*
         * Name:
         *  BST::find(const ElementType &)
         *
         * Description:
         *  Check whether a key is a member of BST.
         *
         * Parameters:
         *  elem - [IN] element to be checked.
         *
         * Returns:
         *  true  - in BST.
         *  false - not in BST.
         *
         * Note:
         *  None.
         */
        bool find(const ElementType &elem) const;

        /*
         * Name:
         *  BST::traverse(std::ostream &)
         *
         * Description:
         *  Traverse BST and redirect outputs to ostrearm "out"
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
         *  BST::swap(BST&)
         *
         * Description:
         *  Swap two BSTs.
         *
         * Parameters:
         *  rightHandSide - [IN] reference of a BST object.
         *
         * Returns:
         *  None.
         *
         * Note:
         *  Swap mustn't throw any exception.
         */
        void swap(BST &rightHandSide) noexcept;

        /*
         * Name: 
         *  BST::size()
         * 
         * Description:
         *  To get the number of elements in BST.
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
         *  BST::depth()
         * 
         * Description:
         *  To get the depth of BST.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  depth
         *
         * Note:
         *  None.
         */
        size_type depth() const;

    private:
        void clear(NodePtr root);
        NodePtr copy(NodePtr root);
        NodePtr remove_min(NodePtr root);
        size_type depth_impl(NodePtr root) const;

        NodePtr   m_root;
        size_type m_size;
}; // template class BST

template <typename ElementType>
BST<ElementType>::BST() : m_root(nullptr), m_size(0) {}

template <typename ElementType>
BST<ElementType>::BST(const BST<ElementType> &original)
    : m_root(nullptr), m_size(original.m_size) {
    if (original.size() != 0) {
        try{
            m_root = copy(original.m_root);
        } catch (std::bad_alloc &err) {
            std::cerr << "Can't allocate new LinkNode: "
                      << err.what() << std::endl;
            std::abort();
        }
    }
}

template <typename ElementType>
const BST<ElementType> &
BST<ElementType>::operator=(const BST<ElementType> &rightHandSide) {
    BST<ElementType> rhs(rightHandSide);
    swap(rhs);
    return *this;
}

template <typename ElementType>
BST<ElementType>::~BST() noexcept {
    if (size() != 0) {
        clear(m_root);
        m_root = nullptr;
        m_size = 0;
    }
}
template <typename ElementType>
bool BST<ElementType>::insert(const ElementType &elem) {
    BST<ElementType>::NodePtr parent = m_root, pos = m_root;

    while (pos != nullptr) {
        if (pos->data == elem) {
            return false;
        }
        parent = pos;
        pos = pos->data < elem ? pos->right : pos->left;
    }

    try {
        pos = new BST<ElementType>::LinkNode(elem);
    } catch (std::bad_alloc &err) {
        std::cerr << "Can't allocate new LinkNode: "
                  << err.what() << std::endl;
        std::abort();
    }

    if (parent == nullptr) {
        m_root = pos;
    } else {
        parent->data < elem ? parent->right = pos : parent->left = pos;
    }
    ++m_size;

    return true;
}

template <typename ElementType>
bool BST<ElementType>::remove(const ElementType &elem) {
    BST<ElementType>::NodePtr parent = m_root, pos = m_root;
    while (pos != nullptr) {
        if (pos->data == elem) {
            BST<ElementType>::NodePtr tmp;
            if (pos->left == nullptr) {
                tmp = pos->right;
            } else if (pos->right == nullptr) {
                tmp = pos->left;
            } else {
                tmp = remove_min(pos->right);
                tmp->left = pos->left;
                tmp->right = pos->right;
            }

            if (parent->data < elem) {
                parent->right = tmp;
            } else if (parent->data > elem) {
                parent->left = tmp;
            } else {
                m_root = tmp;
            }

            delete pos;
            --m_size;
            return true;
        }
        parent = pos;
        pos = pos->data < elem ? pos->right : pos->left;
    }
    return false;
}

template <typename ElementType>
bool BST<ElementType>::find(const ElementType &elem) const {
    BST<ElementType>::NodePtr parent = m_root, pos = m_root;
    while (pos != nullptr) {
        if (pos->data == elem) {
            return true;
        }
        parent = pos;
        pos = pos->data < elem ? pos->right : pos->left;
    }
    return false;
}

template <typename ElementType>
void BST<ElementType>::traverse(std::ostream &out) const {
    out << "{ [lxy::BST] ";
    if (size() != 0) {
        std::stack<BST<ElementType>::NodePtr> unvisited;
        BST<ElementType>::NodePtr node = m_root;
        while (node != nullptr || !unvisited.empty()) {
            while (node != nullptr) {
                unvisited.push(node);
                node = node->left;
            }
            node = unvisited.top();
            unvisited.pop();
            out << node->data << " ";
            node = node->right;
        }
    }
    out << "}";
}

template <typename ElementType>
void BST<ElementType>::swap(BST<ElementType> &rightHandSide) noexcept {
    using std::swap;

    BST<ElementType>::NodePtr tmp = m_root;
    m_root = rightHandSide.m_root;
    rightHandSide.m_root = tmp;

    swap(m_size, rightHandSide.m_size);
}

template <typename ElementType>
typename BST<ElementType>::size_type 
BST<ElementType>::size() const {
    return m_size;
}

template <typename ElementType>
typename BST<ElementType>::size_type 
BST<ElementType>::depth() const {
    return depth_impl(m_root);
}

template <typename ElementType>
void BST<ElementType>::clear(BST<ElementType>::NodePtr root) {
    if (root == nullptr) {
        return;
    }
    clear(root->left);
    clear(root->right);
    delete root;
}

template <typename ElementType>
typename BST<ElementType>::NodePtr
BST<ElementType>::copy(BST<ElementType>::NodePtr root) {
    if (root == nullptr) {
        return nullptr;
    }

    BST<ElementType>::NodePtr node = new BST<ElementType>::LinkNode(root->data);
    node->left = copy(root->left);
    node->right = copy(root->right);

    return node;
}

template <typename ElementType>
typename BST<ElementType>::NodePtr
BST<ElementType>::remove_min(BST<ElementType>::NodePtr root) {
    BST<ElementType>::NodePtr parent = root, pos = root;
    if (pos != nullptr) {
        while (pos->left != nullptr) {
            parent = pos;
            pos = pos->left;
        }
        parent->left = pos->right;
        pos->right = nullptr;
    }
    return pos;
}

template <typename ElementType>
typename BST<ElementType>::size_type 
BST<ElementType>::depth_impl(BST<ElementType>::NodePtr root) const {
    if (root == nullptr) {
        return 0;
    }

    return 1 + std::max(depth_impl(root->left), depth_impl(root->right));
}

template <typename ElementType>
std::ostream &operator<<(std::ostream &out, const BST<ElementType> &root) {
    root.traverse(out);
    return out;
}

template <typename ElementType>
void swap(BST<ElementType> &leftHandSide, BST<ElementType> &rightHandSide)
    noexcept(noexcept(leftHandSide.swap(rightHandSide))) {
    leftHandSide.swap(rightHandSide);
}

} //namespace lxy

#endif
