
#include <mutex>


template <typename T>
class shared_ptr {
private:
    std::mutex*          mutex_;
    unsigned int*        counter_;
    T*                   val_;

    void                 add_reference();
public:
    shared_ptr();
    shared_ptr(T* v);
    shared_ptr(const shared_ptr& s);
    ~shared_ptr();


    shared_ptr& operator= (const shared_ptr& s)
    {
        if (s.val_ != val_)
        {
            relase();
            {
                std::unique_lock<std::mutex> lock(*s.mutex_);
                (*s.counter_)++;
            }
            val_ = s.val_;
        }

        return *this;
    }

    T*   get();
    void relase();
};



template <typename T>
inline shared_ptr<T>::shared_ptr():
    mutex_(new mutex_()),
    counter_(new unsigned int(0)),
    val_(nullptr)
{}



template <typename T>
inline shared_ptr<T>::shared_ptr(T* v):
    mutex_(new mutex()),
    counter_(new unsigned int(1)),
    val_(v)
{}


template <typename T>
inline shared_ptr<T>::shared_ptr(const shared_ptr& s):
    mutex_(s.mutex_),
    counter_(s.counter_),
    val_(s.val_)
{
    add_reference();
}


template <typename T>
inline shared_ptr<T>::~shared_ptr()
{
    relase();
}



template <typename T>
inline T* shared_ptr<T>::get()
{
    return *T;
}



template <typename T>
inline void shared_ptr<T>::relase()
{
    bool isZero = false;
    mutex_->lock();
    if (--*counter_ == 0) {
        delete val_;
        delete counter_;
        isZero = true;
    }
    mutex_->unlock();
    if (isZero == true) {
        delete mutex_;
    }
} 


template <typename T>
inline void shared_ptr<T>::add_reference()
{
    std::unique_lock <std::mutex> lock(*mutex_);
    ++*counter_;   
}
