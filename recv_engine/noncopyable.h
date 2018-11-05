//noncopyable.h

#ifndef _XDF_NONCOPYABLE_H_
#define _XDF_NONCOPYABLE_H_

namespace xdf
{

    class Noncopyable
    {
    public:
        Noncopyable() {}
        ~Noncopyable() {}
    private:
        // emphasize the following members are private
        Noncopyable( const Noncopyable & );
        const Noncopyable &operator=( const Noncopyable & );
    };

} //namespace xdf

#endif  //_XDF_NONCOPYABLE_H_
