//
// Created by 31515 on 25-11-18.
//

export module SegmentTree;

import std;

bool isPowerOfTwo(int n) {
    if (n <= 0) return false;
    return (n & (n-1)) == 0;
}

export class SegmentTree {
private:
    std::vector<int> array;
    std::vector<int> tree;
    std::function<int(int, int)> operation;
    int operationFallback;
public:
    SegmentTree(const std::vector<int>& array, std::function<int(int, int)> operation, int operationFallback) : array(array), operation(operation), operationFallback(operationFallback) {
        this->initialSegmentTree(array);
        this->buildSegmentTree();
    }

    void initialSegmentTree(const std::vector<int>& array) {
        int treeSize;
        int arraySize = array.size();

        if (isPowerOfTwo(arraySize)) {
            treeSize = (2 * arraySize) - 1;
        } else {
            int currentPower = std::floor(std::log2(arraySize));
            int nextPower = currentPower + 1;
            int nextPowerOfTwoNumber = std::pow(2, nextPower);
            treeSize = (2 * nextPowerOfTwoNumber) - 1;
        }
        this->tree = std::vector<int>(treeSize, this->operationFallback);
    }

    void buildSegmentTree() {
        int leftIndex = 0;
        int rightIndex = array.size() - 1;
        int position = 0;
        this->buildTreeRecursively(leftIndex, rightIndex, position);
    }

    void buildTreeRecursively(int leftInputIndex, int rightInputIndex, int position) {
        if (leftInputIndex == rightInputIndex) {
            this->tree[position] = this->array[leftInputIndex];
            return;
        }

        const int middleIndex = (leftInputIndex + rightInputIndex) / 2;

        this->buildTreeRecursively(leftInputIndex, middleIndex, this->getLeftChildIndex(position));

        this->buildTreeRecursively(middleIndex + 1, rightInputIndex, this->getRightChildIndex(position));

        this->tree[position] = this->operation(
          this->tree[this->getLeftChildIndex(position)],
          this->tree[this->getRightChildIndex(position)]
        );
    }

    int getLeftChildIndex(int parentIndex) {
        return parentIndex * 2 + 1;
    }
    int getRightChildIndex(int parentIndex) {
        return parentIndex * 2 + 2;
    }
};