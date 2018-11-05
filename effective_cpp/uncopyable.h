#ifndef _UNCOPYABLE_H
#define _UNCOPYABLE_H


/*
 * Uncopyable is a base class which is designed to forbid
 * copying and assigning.
 * Derived classes should operate `private inherit` from
 * Uncopyable in order to forbid copying and assigning.
 * For example:
 *     class Derived : private Uncopyable {
 *         ...
 *     };
 */
class Uncopyable {
    protected:
        Uncopyable() {}
        ~Uncopyable() {}

    private:
        /* DO NOT permit copy and assignment */
        Uncopyable(const Uncopyable &);
        Uncopyable &operator=(const Uncopyable &);
};

#endif
