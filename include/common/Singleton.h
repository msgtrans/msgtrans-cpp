/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_SINGLETON_H
#define MSGTRANS_CLIENT_SINGLETON_H

#include <mutex>
#include <cstdlib>
#include <assert.h>

namespace base
{
    template<class T>
    class Singleton
    {
    public:
        static T& instance()
        {
            //pthread_once(&flgonce_, &Singleton::init);
            std::call_once(flgonce_,&Singleton::init);
            assert(obj_ != NULL);
            return *obj_;
        }

    private:
        Singleton();
        ~Singleton();

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

        static void init()
        {
            obj_ = new T();
            if(obj_ != NULL)
            {
                atexit(Singleton::release);
            }
        }

        static void release()
        {
            typedef char type_must_be_complete[sizeof(T) == 0 ? -1 : 1];
            type_must_be_complete dummy; (void)dummy;

            delete obj_;
            obj_ = NULL;
        }

        static T*				obj_;
        //static pthread_once_t	flgonce_;
        static std::once_flag	flgonce_ ;
    };

    template<class T>
    T* Singleton<T>::obj_ = NULL;

    template<class T>
//pthread_once_t Singleton<T>::flgonce_ = PTHREAD_ONCE_INIT;
   std::once_flag Singleton<T>::flgonce_;
}

#endif //MSGTRANS_CLIENT_SINGLETON_H
