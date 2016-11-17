// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <vector>
#include <string>

void func()
{
    std::cout << "letest";
}

int main()
{
    std::thread t1(func);
    std::thread t2(func);

    t1.join();
    t2.join();
    return 0;
}
