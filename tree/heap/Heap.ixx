//
// Created by 31515 on 25-11-16.
//

export module Heap;

import std;

export template <typename T, typename F>
class Heap {
protected:
    std::vector<T> heapContainer;
    F comparator;
public:
    Heap(const F& comparator) : comparator(comparator) {}

    int getLeftChildIndex(int parentIndex) const {
        return 2 * parentIndex + 1;
    }
    
    int getRightChildIndex(int parentIndex) const {
        return 2 * parentIndex + 2;
    }
    
    int getParentIndex(int childIndex) const {
        return std::floor((childIndex - 1.0) / 2.0);
    }

    bool hasParent(int childIndex) const {
        return this->getParentIndex(childIndex) >= 0;
    }

    bool hasLeftChild(int parentIndex) const {
        return this->getLeftChildIndex(parentIndex) < this->heapContainer.size();
    }
    
    bool hasRightChild(int parentIndex) const {
        return this->getRightChildIndex(parentIndex) < this->heapContainer.size();
    }

    T leftChild(int parentIndex) const {
        return this->heapContainer[this->getLeftChildIndex(parentIndex)];
    }
    
    T rightChild(int parentIndex) const {
        return this->heapContainer[this->getRightChildIndex(parentIndex)];
    }

    T parent(int childIndex) const {
        return this->heapContainer[this->getParentIndex(childIndex)];
    }

    void swap(int index1, int index2) {
        std::swap(this->heapContainer[index1], this->heapContainer[index2]);
    }

    std::optional<T> peek() {
        if (this->heapContainer.empty()) {
            return std::nullopt;
        }
        return this->heapContainer.front();
    }

    std::optional<T> poll() {
        if (this->heapContainer.empty()) {
            return std::nullopt;
        }
        T item = this->heapContainer.front();
        this->heapContainer.front() = this->heapContainer.back();
        this->heapContainer.pop_back();
        this->heapifyDown();
        return item;
    }

     Heap& add(const T& item) {
        this->heapContainer.push_back(item);
        this->heapifyUp();
        return *this;
    }

    virtual Heap& remove(const T& item) {
        const int numberOfItemsToRemove = this->find(item).size();
        for (int i = 0; i < numberOfItemsToRemove; i++) {
            const int indexToRemove = this->find(item).pop();
            if (indexToRemove == this->heapContainer.size() - 1) {
                this->heapContainer.pop_back();
            } else {
                this->heapContainer[indexToRemove] = this->heapContainer.back();
                this->heapContainer.pop_back();

                // 安全判断：只有当有左子节点时才考虑使用heapifyDown
                bool useHeapifyDown = false;
                if (this->hasLeftChild(indexToRemove)) {
                    // 当没有父节点或者父节点与当前节点满足顺序关系时，使用heapifyDown
                    if (!this->hasParent(indexToRemove)) {
                        useHeapifyDown = true;
                    } else {
                        // 只有当确认有父节点时才调用parent()方法
                        useHeapifyDown = this->inOrder(this->parent(indexToRemove), this->heapContainer[indexToRemove]);
                    }
                }
                
                if (useHeapifyDown) {
                    this->heapifyDown(indexToRemove);
                } else {
                    this->heapifyUp(indexToRemove);
                }
            }
        }
        return *this;
    }

    std::stack<int> find(const T& item) {
        std::stack<int> indices;
        for (int i = 0; i < this->heapContainer.size(); i++) {
            if (comparator->equal(this->heapContainer[i], item)) {
                indices.push(i);
            }
        }
        return indices;
    }

    bool isEmpty() {
        return this->heapContainer.empty();
    }

    std::string toString() {
        std::stringstream ss;
        ss << "[";
        for (int i = 0; i < this->heapContainer.size(); i++) {
            ss << this->heapContainer[i];
            if (i < this->heapContainer.size() - 1) {
                ss << ", ";
            }
        }
        ss << "]";
        return ss.str();
    }

    void heapifyUp(int customStartIndex = 0) {
        int currentIndex = customStartIndex;

        while (this->hasParent(currentIndex)) {
            int parentIndex = getParentIndex(currentIndex);

            // 修复：传递元素而非索引给inOrder
            if (this->inOrder(this->parent(currentIndex), this->heapContainer[currentIndex])) break;

            this->swap(currentIndex, parentIndex);
            currentIndex = parentIndex;
        }
    }

    void heapifyDown(int customStartIndex = 0) {
        int currentIndex = customStartIndex;

        while (this->hasLeftChild(currentIndex)) {
            int nextIndex = 0;

            if (this->hasRightChild(currentIndex) && this->inOrder(this->rightChild(currentIndex), this->leftChild(currentIndex))) {
                nextIndex = this->getRightChildIndex(currentIndex);
            } else {
                nextIndex = this->getLeftChildIndex(currentIndex);
            }

            // 修复：传递元素而非索引给inOrder
            if (this->inOrder(this->heapContainer[currentIndex], this->heapContainer[nextIndex])) break;

            this->swap(currentIndex, nextIndex);
            currentIndex = nextIndex;
        }
    }

    virtual bool inOrder(const T& a, const T& b) const = 0;

    virtual ~Heap() {}
};

export template <typename T, typename F>
class MinHeap : public Heap<T, F> {
public:
    explicit MinHeap(const F& comparator) : Heap<T, F>(comparator) {}

    bool inOrder(const T& a, const T& b) const override {
        return this->comparator->lessEqualThan(a, b);
    }
};