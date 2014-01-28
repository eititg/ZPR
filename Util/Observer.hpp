#ifndef OBSERVER_HPP
#define OBSERVER_HPP

namespace util
{
    /** Klasa obserwatora.
     */
    class Observer
    {
        public:
            /** Destruktor.
             */
            virtual ~Observer();

            /** Metoda wywoływana przez obiekt obserwowany.
             */
            virtual void update() = 0;
    };
}

#endif // OBSERVER_HPP
