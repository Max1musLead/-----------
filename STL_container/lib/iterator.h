#pragma once

#include <iostream>
#include <memory>
#include <iterator>
#include <vector>

struct in_order_tag {};
struct pre_order_tag {};
struct post_order_tag {};

template <typename ValueType>
struct BSTNode {
    ValueType value;
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;

    BSTNode(const ValueType& v){
        value = v;
    }
};

template<typename ValueType, typename Tag>
class Iterator {
    private:
        BSTNode<ValueType>* root = nullptr;
        BSTNode<ValueType>* node = nullptr;
        int target = 0;
        int m_size = 0;

    public:
        using value_type = ValueType;
        using pointer = ValueType*;
        using reference = ValueType&;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;

        explicit Iterator(BSTNode<ValueType>* n_root, BSTNode<ValueType>* n_node = nullptr, int n_size = 0) {
            root = n_root;
            node = n_node;
            m_size = n_size;
        }

        explicit Iterator(BSTNode<ValueType>* n_root, int n_size = 0) {
            root = n_root;
            m_size = n_size;
        }

        BSTNode<ValueType>* GetNode() {
            return node;
        }

        reference operator*() const {
            if (node == nullptr) {
                throw std::out_of_range("Iterator cannot dereference end iterator");
            }
            return node->value;
        }

        pointer operator->() const { 
            return &(node->value); 
        }

        bool operator==(const Iterator& other) const {
            return this->node == other.node;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        void InOrderVisit(BSTNode<ValueType>* current, BSTNode<ValueType>*& result, int& steps) {
            if (current == nullptr || steps < 0) {
                return;
            }

            InOrderVisit(current->left, result, steps);

            if (steps == 0) {
                result = current;
                steps--;
                return;
            }
            steps--;

            InOrderVisit(current->right, result, steps);
        }

        void PreOrderVisit(BSTNode<ValueType>* current, BSTNode<ValueType>*& result, int& steps) {
            if (current == nullptr || steps < 0) {
                return;
            } 

            if (steps == 0) {
                result = current;
                steps--;
                return;
            }
            steps--;

            PreOrderVisit(current->left, result, steps);
            PreOrderVisit(current->right, result, steps);
        }

        void PostOrderVisit(BSTNode<ValueType>* current, BSTNode<ValueType>*& result, int& steps) {
            if (current == nullptr || steps < 0) {
                return; 
            } 

            PostOrderVisit(current->left, result, steps);
            PostOrderVisit(current->right, result, steps);

            if (steps == 0) {
                result = current;
                steps--;
                return;
            }
            steps--;
        }

        Iterator& operator++() {
            if (root == nullptr || (m_size == target)) {
                return *this;
            }

            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int steps = target++;
                InOrderVisit(root, result, steps);
                node = result;
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int steps = target++;
                PreOrderVisit(root, result, steps);
                node = result;
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int steps = target++;
                PostOrderVisit(root, result, steps);
                node = result;
            }
            
            return *this;
        }

        Iterator& operator++(int) {
            if (root == nullptr || (m_size == target)) {
                return *this;
            }

            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int steps = target++;
                InOrderVisit(root, result, steps);
                node = result;
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int steps = target++;
                PreOrderVisit(root, result, steps);
                node = result;
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int steps = target++;
                PostOrderVisit(root, result, steps);
                node = result;
            }
            
            return *this;
        }

        Iterator& operator--() {
            if (root == nullptr || (target == 0)) {
                return *this;
            }

            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int tempTarget = target - 1;

                if (tempTarget < 0) {
                    return *this;
                }

                int tempSteps = tempTarget;
                InOrderVisit(root, result, tempSteps);

                if (result != nullptr) {
                    node = result;
                    target = tempTarget;
                }
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int tempTarget = target - 1;

                if (tempTarget < 0) {
                    return *this;
                }

                int tempSteps = tempTarget;
                PreOrderVisit(root, result, tempSteps);

                if (result != nullptr) {
                    node = result;
                    target = tempTarget;
                }
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int tempTarget = target - 1;

                if (tempTarget < 0) {
                    return *this;
                }
                
                int tempSteps = tempTarget;
                PostOrderVisit(root, result, tempSteps);

                if (result != nullptr) {
                    node = result;
                    target = tempTarget;
                }
            }
            
            return *this;
        }

        Iterator& operator--(int) {
            if (root == nullptr || (target == 0)) {
                return *this;
            }

            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int tempTarget = target - 1;

                if (tempTarget < 0) {
                    return *this;
                }

                int tempSteps = tempTarget;
                InOrderVisit(root, result, tempSteps);

                if (result != nullptr) {
                    node = result;
                    target = tempTarget;
                }
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int tempTarget = target - 1;

                if (tempTarget < 0) {
                    return *this;
                }

                int tempSteps = tempTarget;
                PreOrderVisit(root, result, tempSteps);

                if (result != nullptr) {
                    node = result;
                    target = tempTarget;
                }
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                BSTNode<ValueType>* result = nullptr;
                int tempTarget = target - 1;

                if (tempTarget < 0) {
                    return *this;
                }
                
                int tempSteps = tempTarget;
                PostOrderVisit(root, result, tempSteps);

                if (result != nullptr) {
                    node = result;
                    target = tempTarget;
                }
            }
            
            return *this;
        }
};
