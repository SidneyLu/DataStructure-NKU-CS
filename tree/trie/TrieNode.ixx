//
// Created by 31515 on 25-11-18.
//

export module TrieNode;

import std;

export class TrieNode {
private:
    char character;
    bool isCompleteWord;
    std::unordered_map<char, TrieNode*> children;
    public:
        explicit TrieNode(char character, bool isCompleteWord = false) : character(character), isCompleteWord(isCompleteWord) {}

        TrieNode* getChild(char character) {
            if (!children.contains(character)) {
                return nullptr;
            }
            return children[character];
        }

        TrieNode* addChild(char character, bool isCompleteWord = false) {
            if (children.contains(character)) {
                return children[character];
            }
            children[character] = new TrieNode(character, isCompleteWord);
            return children[character];
        }

        TrieNode* removeChild(char character) {
            if (!children.contains(character)) {
                std::cerr<<"TrieNode::removeChild: character "<<character<<" not found"<<std::endl;
                return this;
            }
            TrieNode* child = children[character];
            if (child != nullptr && !child->isCompleteWord && !child->hasChildren()) {
                delete child;
                children.erase(character);
            }
            return this;
        }

        bool hasChild(char character) {
            return children.contains(character);
        }

        bool hasChildren() {
            return children.size() > 0;
        }

        std::string suggestChildren() {
            std::string result;
            for (const auto& child : children) {
                result += child.first;
            }
            return result;
        }

        std::string toString() {
            std::stringstream ss;
            ss<<"TrieNode{"<<character<<", "<<isCompleteWord<<", [";
            std::string suggested = suggestChildren();
            for (int i = 0; i < suggested.size(); i++) {
                if (i < suggested.size()-1) {
                    ss<<suggested[i]<<',';
                } else {
                    ss<<suggested[i];
                }
            }
            ss<<"]}";
            return ss.str();
        }
};