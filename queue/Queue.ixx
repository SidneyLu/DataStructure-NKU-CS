//
// Created by 31515 on 25-11-16.
//

export module Queue;

import LinkedListNode;
import LinkedList;

import std;

export template <typename T, typename F>
class Queue {
private:
    LinkedList<T, F>* linkedList;

public:
    Queue() {
        this->linkedList = new LinkedList<T, F>();
    }

    bool isEmpty() {
        return !this->linkedList->head;
    }

    std::optional<T> peek() {
        if (this->isEmpty()) {
            return std::nullopt;
        }

        return this->linkedList->head->value;
    }

    void enqueue(const T& value) {
        this->linkedList->append(value);
    }

    std::optional<T> dequeue() {
        const auto removedHead = this->linkedList->removeHead();
        if (removedHead!=std::nullopt) { return removedHead->value; }
        return std::nullopt;
    }

    std::string toString(std::function<std::string(const T&)> callback) {
        return this->linkedList->toString(callback);
    }

    ~Queue() {}
};