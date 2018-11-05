#ifndef _SHAPE_H
#define _SHAPE_H

#include <string>


namespace lxy {

class Shape {
    public:
        virtual void draw() const = 0;
        virtual void error(const std::string &);
        int object_id() const;
};

class Rectangle : public Shape {
    public:
        explicit Rectangle(int len=0, int hgt=0);
        virtual void draw() const override;
        virtual void error(const std::string &) override;

    private:
        int m_length, m_height;
};

}

#endif
