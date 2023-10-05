#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <functional>

template<typename T>
class Sort {
private:
    std::vector<T> vec;
    std::function<void(std::vector<T>&)> sortFunction;

public:
    Sort(std::vector<T> vec, std::function<void(std::vector<T>&)> sortFunction)
        : vec(std::move(vec)), sortFunction(std::move(sortFunction)) {}

    void sort() {
        auto start = std::chrono::high_resolution_clock::now();
        sortFunction(vec);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> diff = end-start;
        std::cout << "Time to sort: " << diff.count() << " s\n";
    }

    void print() const {
        for(const auto& num : vec) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
};

template<typename T>
void merge(std::vector<T>& vec, const std::vector<T>& left, const std::vector<T>& right) {
    auto it = vec.begin();
    auto left_it = left.begin();
    auto right_it = right.begin();

    while (left_it != left.end() && right_it != right.end()) {
        if (*left_it < *right_it) {
            *it = *left_it;
            ++left_it;
        } else {
            *it = *right_it;
            ++right_it;
        }
        ++it;
    }

    while (left_it != left.end()) {
        *it = *left_it;
        ++left_it;
        ++it;
    }

    while (right_it != right.end()) {
        *it = *right_it;
        ++right_it;
        ++it;
    }
}

template<typename T>
void mergeSort(std::vector<T>& vec) {
    if (vec.size() <= 1) return;

    typename std::vector<T>::iterator middle = vec.begin() + (vec.size() / 2);
    std::vector<T> left(vec.begin(), middle);
    std::vector<T> right(middle, vec.end());

    mergeSort(left);
    mergeSort(right);
    merge(vec, left, right);
}

template<typename T>
void bubbleSort(std::vector<T>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        for (size_t j = 0; j < vec.size() - i - 1; ++j) {
            if (vec[j] > vec[j + 1]) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

int main() {
    // For integers
    std::vector<int> vec(1000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for(int i=0; i<1000; i++) {
        vec[i] = dis(gen);
    }

    Sort<int> s(std::move(vec), bubbleSort<int>);

    s.sort();
    s.print();

    // For strings
    // ... (similar code here) ...

    return 0;
}

