#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <list>
#include "Observer.hpp"

namespace util
{
    /** Klasa tematu obserwacji.
     */
    class Subject
    {
        public:
            /** Destruktor.
             */
            virtual ~Subject() = 0;

            /** Dodaj obserwatora od listy.
             *  \param observer adres obserwatora
             */
            void add(Observer *observer);
            /** Usuń obserwatora z listy.
             *  \param observer adres obserwatora
             */
            void remove(Observer *observer);

            /** Wywołaj metodę update() dla wszystkich obserwatorów.
             */
            void notify();

        private:
            std::list<Observer*> observers_;    /**< lista obserwatorów>*/
    };
}

#endif // SUBJECT_HPP
