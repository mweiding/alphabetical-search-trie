/*
 * Trie.h
 *
 */

#ifndef SRC_TRIE_H_
#define SRC_TRIE_H_

#include <iostream>
#include <stack>
#include <vector>

using namespace std;

template <class T, class E = char>
class Trie {
public:
    typedef basic_string<E> key_type;  // string=basic_string<char>
    typedef T mapped_type;             // englisches Wort
    typedef pair<const key_type, mapped_type>
            value_type;  // pair aus deutschem Wort(zusammengesetzt aus chars) und
    // den gemappten englischen Wort
    class TrieIterator;

private:
    static const E ESCAPE_CHAR = '\0';

    class Node {
    public:
        virtual ~Node() = default;  // virtueller Destruktor
        virtual void printRecursive(ostream& ostr, int layer) const = 0;
    };

    class InnerNode : public Node {
        friend class TrieIterator;
        friend class Trie; // so that find() can access children

    protected:
        map<E, Node*> children;

    public:
        void insert(
                key_type key,
                const mapped_type& value) {  // key_type = deutsches Wort,
            // mapped_type& value = englisches Wort
            if (key.empty()) {
                auto iterator = children.find(ESCAPE_CHAR);
                if (iterator != children.end()) {
                    Leaf* leaf = static_cast<Leaf*>(iterator->second);
                    leaf->updateValue(value);
                } else {
                    children.insert({ESCAPE_CHAR, new Leaf(value)});
                }
            } else {
                E first_char = key.at(0);
                InnerNode* innerNode;
                auto iterator = children.find(first_char);
                if (iterator != children.end()) {
                    innerNode = static_cast<InnerNode*>(iterator->second);
                } else {
                    innerNode = new InnerNode();
                    children.insert({first_char, innerNode});
                }
                innerNode->insert(key.substr(1, key.length()), value);
            }
        }

        bool erase(key_type key) {
            if (key.empty()) {
                auto iterator = children.find(ESCAPE_CHAR);
                if (iterator != children.end()) {
                    Leaf* leaf = static_cast<Leaf*>(iterator->second);
                    delete leaf;
                    children.erase(ESCAPE_CHAR);
                }
            } else {
                E first_char = key.at(0);
                auto iterator = children.find(first_char);
                if (iterator != children.end()) {
                    auto* innerNode = static_cast<InnerNode*>(iterator->second);
                    if (innerNode->erase(key.substr(1, key.length()))) {
                        delete innerNode;
                        children.erase(first_char);
                    }
                }
            }
            return children.empty();
        }

        void clear() {
            for (auto iter = children.begin(); iter != children.end(); ++iter) {
                if (iter->first == ESCAPE_CHAR) {
                    delete iter->second;
                } else {
                    auto* innerNode = static_cast<InnerNode*>(iter->second);
                    innerNode->clear();
                    delete innerNode;
                }
            }
            children.clear();
        }

        bool isEmpty() const {
            return children.empty();
        }

        void printRecursive(ostream& ostr, int layer = 0) const {
            for (auto iter = children.begin(); iter != children.end(); ++iter) {
                ostr << string(2 * layer, ' ');
                if (iter->first != ESCAPE_CHAR)
                    ostr << iter->first << endl;
                iter->second->printRecursive(ostr, layer + 1);
            }
        }
    };

    class Leaf : public Node {
    private:
        mapped_type value;

    public:
        Leaf(mapped_type value) : value(value) {}

        void updateValue(mapped_type new_value) {
            value = new_value;
        }

        mapped_type getValue() {
            return value;
        }

        void printRecursive(ostream& ostr, int) const {
            ostr << ":" << value << endl;
        }
    };

public:
    class TrieIterator {
        typedef TrieIterator iterator;

    protected:
        vector<pair<typename map<E, Node*>::iterator,
                typename map<E, Node*>::iterator>>
                iterStack;

    public:
        TrieIterator() = default;
        explicit TrieIterator(vector<pair<typename map<E, Node*>::iterator, typename map<E, Node*>::iterator>> stack) : iterStack(stack) {}

        void slideLeft(InnerNode* startNode) {
            // gehe vom gegebenen Knoten soweit nach unten bis das zeichen für
            // Blatt  ¨/0¨ in der map(first) steht
            //  überprüfe bei jedem knoten ob er einen rechten Nachbar hat
            iterStack.push_back(
                    {startNode->children.begin(), startNode->children.end()});

            while (iterStack.back().first->first != ESCAPE_CHAR) {
                startNode =
                        static_cast<InnerNode*>(iterStack.back().first->second);
                iterStack.push_back(
                        {startNode->children.begin(), startNode->children.end()});
            }
        }

        key_type getKey() {
            key_type result;
            for (auto iter = iterStack.begin(); iter != iterStack.end(); ++iter) {
                result += iter->first->first;
            }
            if (result.size() > 0) // remove \0 at the end of word
                result = result.substr(0, result.length() - 1);
            return result;
        }

        value_type operator*() {
            auto curr = iterStack.back();
            if (curr.first->first == ESCAPE_CHAR) {
                return {getKey(), static_cast<Leaf*>(curr.first->second)->getValue()};
            }
            return {getKey(), mapped_type()};
        }

        bool operator!=(const iterator& rhs) const {
            return !operator==(rhs);
        }

        bool operator==(const iterator& rhs) const {
            return iterStack == rhs.iterStack;
        }

        iterator& operator++() {
            // preincrement. move forward to the next value, next leaf
            while (!iterStack.empty()) {
                auto& childrenIter(iterStack.back().first);
                ++(childrenIter);
                if (childrenIter == iterStack.back().second) {
                    iterStack.pop_back();
                } else {
                    slideLeft(static_cast<InnerNode*>(childrenIter->second));
                    break;
                }
            }
            return *this;
        }

        iterator operator++(int) {  // postfix operator, dummy parameter
            iterator copy(*this);
            operator++();
            return copy;
        }
    };

    Trie() {}

    ~Trie() {
        clear();
    }

    typedef TrieIterator iterator;

    bool empty() const {
        return root.isEmpty();
    }

    void insert(const value_type& value) {
        root.insert(value.first, value.second);
    }

    void erase(const key_type& value) {
        root.erase(value);
    }

    void clear() {
        root.clear();
    }

    iterator find(const key_type& findKey) {
        vector<pair<typename map<E, Node*>::iterator, typename map<E, Node*>::iterator>> stack;
        InnerNode* current = &root;
        const auto findKeyCString = findKey.c_str();
        for (long unsigned int i = 0; i <= findKey.length(); ++i) {
            auto found = current->children.find(findKeyCString[i]);
            if (found != current->children.end()) {
                stack.push_back({found, current->children.end()});
                current = static_cast<InnerNode*>(found->second);
            } else {
                return end();
            }
        }
        return iterator(stack);
    }

    iterator begin() {
        if (empty())
            return end();
        iterator iter;
        iter.slideLeft(&root);
        return iter;
    }

    iterator end() {
        iterator iter = iterator();
        return iter;
    }

    void printOn(ostream& ostr) const {
        root.printRecursive(ostr);
    }

    InnerNode root;
};

template <class T, class E>
const E Trie<T, E>::ESCAPE_CHAR;

template <class T, class E>
ostream& operator<<(ostream& ostr, const Trie<T, E>& trie) {
    trie.printOn(ostr);
    return ostr;
}

#endif /* SRC_TRIE_H_ */

