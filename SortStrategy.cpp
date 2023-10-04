#include <iostream>
#include <vector>
#include <random>
#include <iterator>
#include <chrono>
#include <memory>

template<typename T>
class SortStrategy {
    public:
        virtual void sort(std::vector<T>& vec) = 0;

        void timedSort(std::vector<T>& vec) {
            auto start = std::chrono::high_resolution_clock::now();
            sort(vec);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> diff = end-start;
            std::cout << "Time to sort: " << diff.count() << " s\n";
        }
};

template<typename T>
class MergeSort : public SortStrategy<T> {
    private:
        void merge(typename std::vector<T>::iterator start, typename std::vector<T>::iterator mid, typename std::vector<T>::iterator end) {
            std::vector<T> left(start, mid);
            std::vector<T> right(mid, end);

            auto it = start;
            auto left_it = left.begin();
            auto right_it = right.begin();

            while (left_it != left.end() && right_it != right.end()) {
                if (*left_it < *right_it) {
                    *it = std::move(*left_it);
                    ++left_it;
                } else {
                    *it = std::move(*right_it);
                    ++right_it;
                }
                ++it;
            }

            while (left_it != left.end()) {
                *it = std::move(*left_it);
                ++left_it;
                ++it;
            }

            while (right_it != right.end()) {
                *it = std::move(*right_it);
                ++right_it;
                ++it;
            }
        }

    public:
        void sort(std::vector<T>& vec) override {
            if (vec.size() <= 1) return;

            auto mid = vec.begin() + vec.size() / 2;

            std::vector<T> left(vec.begin(), mid);
            std::vector<T> right(mid, vec.end());

            sort(left);
            sort(right);
            merge(vec.begin(), vec.begin() + left.size(), vec.end());
        }
};

template<typename T>
class QuickSort : public SortStrategy<T> {
    private:
        typename std::vector<T>::iterator partition(typename std::vector<T>::iterator start, typename std::vector<T>::iterator end) {
            auto pivot = *(end - 1);
            auto i = start - 1;

            for (auto j = start; j != end; ++j) {
                if (*j <= pivot) {
                    ++i;
                    std::iter_swap(i, j);
                }
            }

            return i;
        }

    public:
        void sort(std::vector<T>& vec) override {
            quickSort(vec.begin(), vec.end());
        }

        void quickSort(typename std::vector<T>::iterator start, typename std::vector<T>::iterator end) {
            if (start < end) {
                auto pi = partition(start, end);

                quickSort(start, pi);
                quickSort(pi + 1, end);
            }
        }
};

template<typename T>
class BubbleSort : public SortStrategy<T> {
public:
    void sort(std::vector<T>& vec) override {
        for (size_t i = 0; i < vec.size(); ++i) {
            for (size_t j = 0; j < vec.size() - i - 1; ++j) {
                if (vec[j] > vec[j + 1]) {
                    std::swap(vec[j], vec[j + 1]);
                }
            }
        }
    }
};

template<typename T>
class Sort {
    private:
        std::vector<T> vec;
        std::unique_ptr<SortStrategy<T>> strategy;

    public:
        Sort(std::vector<T> vec, std::unique_ptr<SortStrategy<T>> strategy)
            : vec(std::move(vec)), strategy(std::move(strategy)) {}

        void sort() {
            strategy->timedSort(vec);
        }

        void print() const {
            for(const auto& num : vec) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
};

int main() {
    std::vector<int> vec1(1000);
    std::vector<int> vec2(1000);
    std::vector<int> vec3(1000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for(int i=0; i<1000; i++)
        vec1[i] = dis(gen);
    for(int i=0; i<1000; i++)
        vec2[i] = dis(gen);
    for(int i=0; i<1000; i++)
        vec3[i] = dis(gen);

    Sort<int> s1(std::move(vec1), std::make_unique<MergeSort<int>>());
    Sort<int> s2(std::move(vec2), std::make_unique<QuickSort<int>>());
    Sort<int> s3(std::move(vec3), std::make_unique<BubbleSort<int>>());

    s1.sort();
    s2.sort();
    s3.sort();

    s1.print();
    s2.print();
    s3.print();

    return 0;
}

