//
// Created by 31515 on 25-11-16.
//

export module Stack;

import LinkedListNode;
import LinkedList;

import std;

export template <typename T, typename F>
class Stack {
private:
    LinkedList<T, F>* linkedList;

public:
    Stack() {
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

    void push(const T& value) {
        this->linkedList->prepend(value);
    }

    std::optional<T> pop() {
        const auto removedHead = this->linkedList->removeHead();
        if (removedHead!=std::nullopt) { return removedHead->value; }
        return std::nullopt;
    }

    std::string toString(std::function<std::string(const T&)> callback) {
        return this->linkedList->toString(callback);
    }

    ~Stack() {}
};