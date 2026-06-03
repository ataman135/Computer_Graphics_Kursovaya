// Библиотека для работы с событиями
// Доздравствует велосипедостроение!
#pragma once

#include <list>
#include <functional>

#ifdef EVENT_MULTITHREADING
#include <mutex>
#endif

// Объект этого класса хранит в себе массив указателей на функции
// которые последовательно исполняются
// при вызове exec()
// SENDTYPE - тип отправителя события (сигнала)
// ARGTYPE  - тип параметра сигнала
template <class SENDTYPE, class ARGTYPE>
class Event
{
    // Определяем тип - указатель на функцию.
    // Первым аргументом функция должна принимать указатель на отправителя события,
    // вторым аргумент - тип события.
    typedef std::function<void(SENDTYPE, ARGTYPE)> event_func_type;
    // Итератор на функцию в списке
    typedef typename std::list<event_func_type>::const_iterator event_func_type_const_iterator;

    // Список функций
    std::list<event_func_type> events;


#ifdef EVENT_MULTITHREADING
    // Блокировщик для многопоточности
    std::mutex event_lock;
#endif

  public:
    // Добавление одиночной функции
    event_func_type_const_iterator reaction(event_func_type event_func)
    {
#ifdef EVENT_MULTITHREADING
        event_lock.lock();
#endif
        events.push_back(event_func);
#ifdef EVENT_MULTITHREADING
        event_lock.unlock();
#endif
        return --events.end();
    }

    // Добавление функции-члена какого нибудь класса
    template <class C> event_func_type_const_iterator reaction(C* cls, void (C::*f)(SENDTYPE, ARGTYPE))
    {
        using namespace std::placeholders;
        event_func_type f1;
        f1 = std::bind(f, cls, _1, _2);

#ifdef EVENT_MULTITHREADING
        event_lock.lock();
#endif


        events.push_back(f1);



#ifdef EVENT_MULTITHREADING
        event_lock.unlock();
#endif



        return --events.end();
    }

    // Удаление функцию по ее итератору
    void remove_reaction(event_func_type_const_iterator& it)
    {
#ifdef EVENT_MULTITHREADING
        event_lock.lock();
#endif

        events.erase(it);


#ifdef EVENT_MULTITHREADING
        event_lock.unlock();
#endif
    }

    // Удаление функцию по ее указателю
    template <class C> void remove_reaction(C* cls, void (C::*f)(SENDTYPE, ARGTYPE))
    {
        using namespace std::placeholders;
        event_func_type f1;
        f1 = std::bind(f, cls, _1, _2);

#ifdef EVENT_MULTITHREADING
        event_lock.lock();
#endif    

        events.remove(f1);    


#ifdef EVENT_MULTITHREADING
        event_lock.unlock();
#endif

    }

    // Удаление функцию по ее итератору
    void remove_reaction(event_func_type f)
    {
#ifdef EVENT_MULTITHREADING
        event_lock.lock();
#endif
        
        events.remove(f);


#ifdef EVENT_MULTITHREADING
        event_lock.unlock();
#endif
        
    }

    // Удаление всего
    void remove_all_reations()
    {
#ifdef EVENT_MULTITHREADING
        event_lock.lock();
#endif
        
        events.clear();
#ifdef EVENT_MULTITHREADING
        event_lock.unlock();
#endif        
    }

    // Выполнить все функции.
    // Первым аргументом отправить во все функции указатель на отправителя сигнала,
    // вторым - параметры сигнала.
    void exec(SENDTYPE sender, ARGTYPE args)
    {
#ifdef EVENT_MULTITHREADING
        event_lock.lock();
#endif
        
        for (auto x : events)
            x(sender, args);

#ifdef EVENT_MULTITHREADING
        event_lock.unlock();
#endif
        
    }
};
