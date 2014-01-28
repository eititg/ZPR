#include "Subject.hpp"

namespace util
{
    Subject::~Subject()
    {
    }

    void Subject::add(Observer *observer)
    {
        observers_.push_back(observer);
    }

    void Subject::remove(Observer *observer)
    {
        observers_.remove(observer);
    }

    void Subject::notify()
    {
        std::list<Observer*>::iterator it;

        for (it = observers_.begin(); it != observers_.end(); ++it)
            (*it)->update();
    }
}
