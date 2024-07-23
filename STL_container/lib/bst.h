#pragma once

#include "iterator.h"

template <typename _type, typename Tag, typename compare = std::less<_type>, typename allocator = std::allocator<BSTNode<_type>>>
class BST {
    public:
        using value_type = _type ;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using iterator = Iterator<value_type, in_order_tag>;
        using const_iterator = const Iterator<value_type, in_order_tag>;
        using size_type = std::size_t;
        using allocator_traits = std::allocator_traits<allocator>;
        
    private:
        BSTNode<value_type>* root;
        [[no_unique_address]] compare comp;
        [[no_unique_address]] allocator alloc;
        size_type m_size = 0;

        void DeleteTree(BSTNode<value_type>* node) {
            if (node != nullptr) {
                DeleteTree(node->left);
                DeleteTree(node->right);

                allocator_traits::destroy(alloc, node);
                allocator_traits::deallocate(alloc, node, 1);
            }

            return;
        }

        BSTNode<value_type>* Search(BSTNode<value_type>* node, int value) {
            if (node == nullptr) {
                return nullptr;
            } else if (value < node->value) {
                return Search(node->left, value);
            } else if (value > node->value) {
                return Search(node->right, value);
            } else {
                return node;
            }
        }

        bool Contains(BSTNode<value_type>* node, const value_type& key) {
            while (node != nullptr) {
                if(comp(key, node->value)) {
                    node = node->left;
                } else if(comp(node->value, key)) {
                    node = node->right;
                } else {
                    return true;
                }
            }

            return false;
        }

        BSTNode<value_type>* copy(const BSTNode<value_type>* other) {
            if (other == nullptr) {
                return nullptr;
            }

            BSTNode<value_type>* new_node = nullptr;
            try {
                new_node = allocator_traits::allocate(alloc, 1);
                try {
                    allocator_traits::construct(alloc, new_node, other->value);
                } catch (std::bad_alloc& e) {
                    allocator_traits::deallocate(alloc, new_node, 1);
                    throw;
                }

                try {
                    new_node->left = copy(other->left);
                } catch (std::bad_alloc& e) {
                    DeleteTree(new_node->left);
                    throw;
                }
                
                try {
                    new_node->right = copy(other->right);
                } catch (std::bad_alloc& e) {
                    DeleteTree(new_node->right);
                    throw;
                }
                
            } catch (std::bad_alloc& e) {
                DeleteTree(new_node);
                throw;
            }

            return new_node;
        }

        void PreorderTraversal(const BSTNode<value_type>* node) {
            if (node != nullptr) {
                std::cout << node->value << " ";
                PreorderTraversal(node->left);
                PreorderTraversal(node->right);
            }
        }

        BSTNode<_type>* FindMin(BSTNode<value_type>* node) {
            while (node && node->left != nullptr) {
                node = node->left;
            }
                
            return node;
        }

        BSTNode<_type>* DeleteNode(BSTNode<value_type>* node, const value_type& value) {
            if (node == nullptr) {
                return nullptr;
            }
            
            if (comp(value, node->value)) {
                node->left = DeleteNode(node->left, value);
            } else if (comp(node->value, value)) {
                node->right = DeleteNode(node->right, value);
            } else {
                if (node->left != nullptr && node->right != nullptr) {
                    node->value = FindMin(node->right)->value;
                    node->right = DeleteNode(node->right, node->value);
                } else {
                    BSTNode<value_type>* temp = node;
                    if (node->left == nullptr) {
                        node = node->right;
                    } else if (node->right == nullptr) {
                        node = node->left;
                    }

                    allocator_traits::destroy(alloc, temp);
                    allocator_traits::deallocate(alloc, temp, 1);
                    m_size--;
                }
            }

            return node;
        }
        
        BSTNode<_type>* Insert(BSTNode<value_type>* node, const value_type& value) {
            if (node == nullptr) {
                BSTNode<value_type>* new_node = allocator_traits::allocate(alloc, 1);
                allocator_traits::construct(alloc, new_node, value);
                m_size++;
                return new_node;
            }

            if (comp(value, node->value)) {
                node->left = Insert(node->left, value);
            } else {
                node->right = Insert(node->right, value);
            }

            return node;
        }

        BSTNode<_type>* Merge(BSTNode<value_type>* root1, BSTNode<value_type>* root2) {
            if (root2 != nullptr) {
                root1 = Insert(root1, root2->value);
                Merge(root1, root2->left);
                Merge(root1, root2->right);
            }

            return root1;
        }

        int CountKey(BSTNode<value_type>* node, const value_type& k) {
            if (node == nullptr) {
                return 0;
            }
            if (node->value == k) {
                return CountKey(node->left, k) + CountKey(node->right, k) + 1;
            } else {
                return CountKey(node->left, k) + CountKey(node->right, k);
            }
        }

        bool Comparison(BSTNode<value_type>* first, BSTNode<value_type>* second){
            if (first == nullptr && second == nullptr) {
                return true;
            }
            if ((first == nullptr && second != nullptr) || (first != nullptr && second == nullptr)) {
                return false;
            }
            if (first->value != second->value) {
                return false;
            }

            return Comparison(first->left, second->left) && Comparison(first->right, second->right);
        }

    public:
        BST() {
            root = nullptr;
        }

        BST(const BST<value_type, Tag>& other) {
            if (other.root != nullptr) {
                if (allocator_traits::propagate_on_container_copy_assignment::value) {
                    alloc = other.alloc;
                }

                root = copy(other.root);
                comp = other.comp;
            }
        }

        BSTNode<value_type>* GetRoot() {
            return root;
        }

        BST& operator=(const BST<value_type, Tag>& other) {
            if (other.root != nullptr) {
                DeleteTree(root);

                if (allocator_traits::propagate_on_container_copy_assignment::value) {
                    alloc = other.alloc;
                }

                root = copy(other.root);
                comp = other.comp;
            }

            return *this;
        }

        bool operator==(const BST<value_type, Tag>& other) {
            return Comparison(this->root, other.root);
        }

        bool operator!=(const BST<value_type, Tag>& other) {
            return !(*this == other);
        }

        void print() {
            if (root != nullptr) {
                PreorderTraversal(root);
                std::cout << "\n";
            }
        }

        void clear() {
            DeleteTree(root);
            m_size = 0;
            root = nullptr;
        }

        int count(const value_type& k) {
            return CountKey(root, k);
        }

        void erase(const value_type& value) {
            while (root != nullptr && Search(root, value) != nullptr) {
                root = DeleteNode(root, value);
            }
        }

        bool contains(const value_type& value) {
            return Contains(root, value);
        }

        void merge(BST<value_type, Tag>& other) {
            if (other.root != nullptr) {
                root = Merge(root, other.root);
            }
        }

        void find(const value_type& value) {
            if (root != nullptr) {
                Search(root, value);
            }
        }

        void extract(const value_type& value) {
            if (root != nullptr) {
                root = DeleteNode(root, value);
            }
        }

        void insert(const value_type& value) {
            root = Insert(root, value);
        }

        void swap(BST<value_type, Tag>& other) {
            if (this->root == nullptr) {
                this->root = other.root;
                other.root = nullptr;
            } else if (other.root == nullptr) {
                other.root = this->root;
                this->root = nullptr;
            } else {
                BSTNode<value_type>* temp = this->root;
                this->root = other.root;
                other.root = temp;
            }
        }

        int size() {
            return std::distance(this->begin(), this->end());
        }

        bool empty() {
            return this->begin().GetNode() == nullptr && this->end().GetNode() == nullptr;
        }

        ~BST() {
            DeleteTree(root);
        }
///////////////////////////////////////////////////////////////////////        
        using AllIterator = Iterator<value_type, Tag>;

        AllIterator begin() const {
            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                BSTNode<value_type>* current = root;
                while (current && current->left != nullptr) {
                    current = current->left;
                }
                return AllIterator(root, current, this->m_size);
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                return AllIterator(root, root, this->m_size);
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                BSTNode<value_type>* current = root;
                while (current != nullptr) {
                    if (current->left != nullptr) {
                        current = current->left;
                    } else if (current->right != nullptr) {
                        current = current->right;
                    } else {
                        break;
                    }
                }
                return AllIterator(root, current, this->m_size);
            }
        }
        AllIterator end() const {
            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                BSTNode<value_type>* current = root;
                while (current != nullptr && current->right != nullptr) {
                    current = current->right;
                }
                return AllIterator(root, current, this->m_size);
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                BSTNode<value_type>* current = root;
                while (current != nullptr) {
                    if (current->right != nullptr) {
                        current = current->right;
                    } else if (current->left != nullptr) {
                        current = current->left;
                    } else {
                        break;
                    }
                }
                return AllIterator(root, current, this->m_size);
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                return AllIterator(root, root, this->m_size); 
            }
        }

        const AllIterator cbegin() const {
            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                BSTNode<value_type>* current = root;
                while (current && current->left != nullptr) {
                    current = current->left;
                }
                return AllIterator(root, current, this->m_size);
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                return AllIterator(root, root, this->m_size);
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                BSTNode<value_type>* current = root;
                while (current != nullptr) {
                    if (current->left != nullptr) {
                        current = current->left;
                    } else if (current->right != nullptr) {
                        current = current->right;
                    } else {
                        break;
                    }
                }
                return AllIterator(root, current, this->m_size);
            }
        }
        const AllIterator cend() const {
            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                BSTNode<value_type>* current = root;
                while (current != nullptr && current->right != nullptr) {
                    current = current->right;
                }
                return AllIterator(root, current, this->m_size);
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                BSTNode<value_type>* current = root;
                while (current != nullptr) {
                    if (current->right != nullptr) {
                        current = current->right;
                    } else if (current->left != nullptr) {
                        current = current->left;
                    } else {
                        break;
                    }
                }
                return AllIterator(root, current, this->m_size);
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                return AllIterator(root, root, this->m_size); 
            }
        }
//////////////////////////////////////////////////////////////////////////////////
        using ReverseAllIterator = std::reverse_iterator<AllIterator>;

        ReverseAllIterator rbegin() {
            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                return ReverseAllIterator(end());
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                return ReverseAllIterator(end());
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                return ReverseAllIterator(end());
            }
        }

        ReverseAllIterator rend() {
            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                return ReverseAllIterator(begin());
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                return ReverseAllIterator(begin());
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                return ReverseAllIterator(begin());
            }
        }

        const ReverseAllIterator crbegin() {
            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                return ReverseAllIterator(end());
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                return ReverseAllIterator(end());
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                return ReverseAllIterator(end());
            }
        }

        const ReverseAllIterator crend() {
            if constexpr (std::is_same_v<Tag, in_order_tag>) {
                return ReverseAllIterator(begin());
            } else if constexpr (std::is_same_v<Tag, pre_order_tag>) {
                return ReverseAllIterator(begin());
            } else if constexpr (std::is_same_v<Tag, post_order_tag>) {
                return ReverseAllIterator(begin());
            }
        }
};
