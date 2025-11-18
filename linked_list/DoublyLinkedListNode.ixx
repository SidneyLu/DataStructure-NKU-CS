export module DoublyLinkedListNode;

import std;

export template <typename T>
class DoublyLinkedListNode {
public:

    T value;
    DoublyLinkedListNode<T>* next;
    DoublyLinkedListNode<T>* previous;

    DoublyLinkedListNode(T value, DoublyLinkedListNode<T>* next, DoublyLinkedListNode<T>* previous) {
        this->value = value;
        this->next = next;
        this->previous = previous;
    }

    std::string toString(std::function<std::string(const T&)> callback) {
        return callback ? callback(this->value) : std::to_string(this->value);
    }

    ~DoublyLinkedListNode() {
        this->next = nullptr;
        this->previous = nullptr;
    }
};