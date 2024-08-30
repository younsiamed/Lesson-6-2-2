#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <future>

int find_min_element(const std::vector<int>& arr, int start, int end) {
    int min_index = start;
    for (int i = start + 1; i < end; ++i) {
        if (arr[i] < arr[min_index]) {
            min_index = i;
        }
    }
    return min_index;
}

void selection_sort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; ++i) {
        std::promise<int> promise;
        std::future<int> future = promise.get_future();

        std::thread search_thread([&arr, i, n, &promise]() {
            int min_index = find_min_element(arr, i, n);
            promise.set_value(min_index);
            });

        search_thread.join(); // Ждем завершения потока
        int min_index = future.get();

        // Меняем местами текущий элемент и найденный минимальный
        if (min_index != i) {
            std::swap(arr[i], arr[min_index]);
        }
    }
}

int main() {
    std::vector<int> arr = { 1, 7, 100, 9, 3 };

    std::cout << "Original array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    selection_sort(arr);

    std::cout << "Sorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
