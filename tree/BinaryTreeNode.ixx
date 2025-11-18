export module BinaryTreeNode;

import std;

export template <typename T, typename F> class BinaryTreeNode;

export template <typename T>
class NodeValueComparator {
public:
    bool equal(const BinaryTreeNode<T, NodeValueComparator<T>>& a,
               const BinaryTreeNode<T, NodeValueComparator<T>>& b) const {
        return a.getValue() == b.getValue();  // 比较节点值
    }
    bool equal(const T& a, const T& b) const {
        return a == b;
    }
    bool lessThan(const T& a, const T& b) const {
        return a < b;
    }
    bool greaterThan(const T& a, const T& b) const {
        return a > b;
    }
};

export enum class Color { Black, Red };

// 0 = black, 1 = red
export template<typename T, typename F = NodeValueComparator<T>>
class BinaryTreeNode {
protected:
    T value;
    F nodeComparator;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
    BinaryTreeNode* parent;
    Color color;

public:
    explicit BinaryTreeNode(T value = 0, const F& nodeComparator = F{})
        : value(value),
          nodeComparator(nodeComparator),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}

    BinaryTreeNode(const BinaryTreeNode& other)
       : value(other.value),
         nodeComparator(other.nodeComparator),
         left(nullptr),
         right(nullptr),
         parent(nullptr)
   {
        if (other.left != nullptr) {
            left = new BinaryTreeNode(*other.left);
            left->parent = this;
        }

        if (other.right != nullptr) {
            right = new BinaryTreeNode(*other.right);
            right->parent = this;
        }
   }

    int leftHeight() const {
        if (left == nullptr) {
            return 0;
        }
        return left->Height();
    }

    int rightHeight() const {
        if (right == nullptr) {
            return 0;
        }
        return right->Height();
    }

    int Height() const {
        return std::max(leftHeight(), rightHeight()) + 1;
    }

    int balanceFactor() const {
        return this->leftHeight() - this->rightHeight();
    }

    BinaryTreeNode* uncle() {
        if (!this->parent) {
            return nullptr;
        }

        if (!this->parent->parent) {
            return nullptr;
        }

        if (!this->parent->parent->left || !this->parent->parent->right) {
            return nullptr;
        }

        if (this->parent == this->parent->parent->left) {
            return this->parent->parent->right;
        }
        else {
            return this->parent->parent->left;
        }
    }

    T getValue() const {
        return this->value;
    }

    BinaryTreeNode& setValue(T value) {
        this->value = value;
        return *this;
    }

    BinaryTreeNode& setLeft(BinaryTreeNode* node) {
        if (this->left) {
            this->left->parent = nullptr;
        }
        this->left = node;
        if (this->left) {
            this->left->parent = this;
        }
        return *this;
    }

    BinaryTreeNode& setRight(BinaryTreeNode* node) {
        if (this->right) {
            this->right->parent = nullptr;
        }
        this->right = node;
        if (this->right) {
            this->right->parent = this;
        }
        return *this;
    }

    bool removeChild(BinaryTreeNode* nodeToRemove) {
        if (this->left && this->nodeComparator.equal(*nodeToRemove, *this->left)) {
            delete this->left;
            this->left = nullptr;
            return true;
        }
        if (this->right && this->nodeComparator.equal(*nodeToRemove, *this->right)) {
            delete this->right;
            this->right = nullptr;
            return true;
        }
        return false;
    }

    bool replaceChild(BinaryTreeNode* nodeToReplace, BinaryTreeNode* replacementNode) {
        if (!nodeToReplace || !replacementNode) {
            return false;
        }

        if (this->left && this.nodeComparator.equal(*nodeToReplace, *this->left)) {
            delete this->left;
            this->left = replacementNode;
            return true;
        }

        if (this->right && this.nodeComparator.equal(*nodeToReplace, *this->right)) {
            delete this->right;
            this->right = replacementNode;
            return true;
        }

        return false;
    }

    std::vector<T> traverseInOrder() {
        std::vector<T> v1 = this->left->traverseInOrder();
        std::vector<T> v2 = this->right->traverseInOrder();
        v1.push_back(this->value);
        v1.insert(v1.end(), std::make_move_iterator(v2.begin()), std::make_move_iterator(v2.end()));
        return v1;
    }

    std::string toString() {
        std::vector<T> order = this->traverseInOrder();
        std::stringstream ss;
        for (size_t i = 0; i < order.size(); ++i) {
            ss << order[i];
            if (i != order.size() - 1) {
                ss << ", ";
            }
        }
        return ss.str();
    }

    virtual ~BinaryTreeNode() {
        // 递归删除左右子节点，防止内存泄漏
        if (left != nullptr) {
            delete left;
            left = nullptr;
        }
        if (right != nullptr) {
            delete right;
            right = nullptr;
        }
    }
};