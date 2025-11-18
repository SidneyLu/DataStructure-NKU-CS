//
// Created by 31515 on 25-11-15.
//

export module FenwickTree;

import std;

export template <typename T>
class FenwickTree {
private:
    int arraySize;
    std::vector<T> treeArray;
public:
    explicit FenwickTree(int size) {
        this->arraySize = size;
        this->treeArray.resize(arraySize+1);
        for (T& element : treeArray) {
            element = 0;
        }
    }

    FenwickTree& increase(int position, T value) {
        if (position < 1 || position > this->arraySize) {
            throw std::out_of_range("Position out of range");
        }

        for (int i = position; i <= this->arraySize; i += (i & -i)) {
            this->treeArray[i] += value;
        }
        return *this;
    }

    T query(int position) {
        if (position < 1 || position > this->arraySize) {
            throw std::out_of_range("Position out of range");
        }

        T sum = 0;

        for (int i = position; i > 0; i -= (i & -i)) {
            sum += this.treeArray[i];
        }

        return sum;
    }

    T queryRange(int leftIndex, int rightIndex) {
        if (leftIndex > rightIndex) {
            throw std::out_of_range("Left index cannot be greater than right index");
        }

        if (leftIndex == 1) {
            return this->query(rightIndex);
        }

        return this->query(rightIndex) - this->query(rightIndex) - this->query(leftIndex - 1);
    }
};