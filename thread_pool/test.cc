#include "thread_pool.hpp"
#include <iostream>

void task()
{ 
    std::cout << "task()" << std::endl;
}

class task_struct {
public:
    void task() {
         std::cout << "task_struct() " << std::endl;
    }

    void task_args(int i) {
        std::cout << "task_struct_args()" << i << std::endl;
    }
};

int main()
{
    thread_pool pool(100);

    /*
    for (int i = 0; i < 100; ++i)
        pool.enqueue([]() {
            std::cout << "ok" << std::endl;
        });
    */
    task_struct ts;
    /*
    for (int i = 0; i < 100; ++i)
        pool.enqueue(std::bind(&task_struct::task, &ts));
    */
    for (int i = 0; i < 100; ++i)
        pool.enqueue(std::bind(&task_struct::task_args, &ts, 100));

}
