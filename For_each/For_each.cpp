#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <algorithm>
#include <numeric>

template <typename Iterator, typename Function>
void parallel_for_each(Iterator begin, Iterator end, Function func) {
    auto length = std::distance(begin, end);

    // Определяем размер блока
    auto min_block_size = 1000; // Вы можете изменить это значение
    if (length < min_block_size) {
        std::for_each(begin, end, func);
        return;
    }

    auto mid = begin + length / 2;

    // Создаем два promise для синхронизации
    std::promise<void> promise1;
    std::future<void> future1 = promise1.get_future();
    std::promise<void> promise2;
    std::future<void> future2 = promise2.get_future();

    // Запускаем асинхронные задачи
    std::thread t1([begin, mid, func, &promise1]() {
        try {
            std::for_each(begin, mid, func);
            promise1.set_value();
        }
        catch (...) {
            promise1.set_exception(std::current_exception());
        }
        });

    std::thread t2([mid, end, func, &promise2]() {
        try {
            std::for_each(mid, end, func);
            promise2.set_value();
        }
        catch (...) {
            promise2.set_exception(std::current_exception());
        }
        });

    // Ждем завершения задач
    future1.wait();
    future2.wait();

    // Ожидаем завершения потоков
    t1.join();
    t2.join();
}

int main() {
    std::vector<int> data(100);
    std::iota(data.begin(), data.end(), 1);

    parallel_for_each(data.begin(), data.end(), [](int& x) {
        x = x * x; // Просто пример обработки
        });

    for (const auto& x : data) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
