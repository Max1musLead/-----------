// #include <iostream>
// #include <memory>
// #include <iterator>
// #include <vector>

// struct in_order_tag {};
// struct pre_order_tag {};
// struct post_order_tag {};

// template <typename ValueType>
// struct BSTNode {
//     ValueType value;
//     BSTNode* left = nullptr;
//     BSTNode* right = nullptr;
//     BSTNode* parent = nullptr;
//     bool paint = 0;

//     BSTNode(const ValueType& v){
//         value = v;
//     }
// };

// template<typename ValueType, typename Tag>
// class Iterator {
//     private:
//         template<typename, typename, typename> friend class BST;
//         BSTNode<ValueType>* node;

//     public:
//         using value_type = ValueType;
//         using pointer = ValueType*;
//         using reference = ValueType&;
//         using difference_type = std::ptrdiff_t;
//         using iterator_category = std::bidirectional_iterator_tag;

//         explicit Iterator(BSTNode<value_type>* new_node = nullptr) {
//             node = new_node;
//         }

//         BSTNode<ValueType>* GetNode() {
//             return node;
//         }

//         reference operator*() const { 
//             return node->value; 
//         }

//         pointer operator->() const { 
//             return &(node->value); 
//         }

//         bool operator==(const Iterator& other) const {
//             return node == other.node;
//         }

//         bool operator!=(const Iterator& other) const {
//             return !(*this == other);
//         }

//         Iterator& operator++() {
//             if (node == nullptr) {
//                 return *this;
//             }

//             if (std::is_same<Tag, in_order_tag>::value) {
//                 if (node->left != nullptr && !node->left->paint) {
//                     while (node->left != nullptr && !node->left->paint) {
//                         node = node->left;
//                     }
//                 } else if (node->right != nullptr && !node->right->paint) {
//                     node = node->right;
//                     while (node->left != nullptr && !node->left->paint) {
//                         node = node->left;
//                     }
//                 } else {
//                     while (node->parent && (node->parent->right == node || node->parent->right == nullptr) && node->parent->paint) {
//                         node = node->parent;
//                     }

//                     if (node->parent && !node->parent->paint) {
//                         node = node->parent;
//                     }
//                 }
//             } else if (std::is_same<Tag, pre_order_tag>::value) {
//                 if (!node->paint) {
//                 } else if (node->left != nullptr ) {
//                     node = node->left;
//                 } else if (node->right != nullptr) {
//                     node = node->right;
//                 } else {
//                     while (node->parent && (node->parent->right == node || node->parent->right == nullptr)) {
//                         node = node->parent;
//                     }
//                     if (node->parent && node->parent->right != nullptr) {
//                         node = node->parent->right;
//                     }
//                 }
//             } else if (std::is_same<Tag, post_order_tag>::value) {
//                 if (node->left != nullptr && !node->left->paint) {
//                     while (node->left != nullptr && !node->left->paint) {
//                         node = node->left;
//                     }
//                     while (node->right != nullptr && !node->right->paint) {
//                         node = node->right;
//                     }
//                 } else if (node->right != nullptr && !node->right->paint) {
//                     while (node->right != nullptr && !node->right->paint) {
//                         node = node->right;
//                     }
//                     if (node->left != nullptr && !node->left->paint) {
//                         while (node->left != nullptr && !node->left->paint) {
//                             node = node->left;
//                         }
//                     }
//                 } else {
//                     while (node->parent != nullptr && node->paint) {
//                         node = node->parent;
//                     }
//                     if (node->left != nullptr && !node->left->paint) {
//                         while (node->left != nullptr && !node->left->paint) {
//                             node = node->left;
//                         }
//                         while (node->right != nullptr && !node->right->paint) {
//                             node = node->right;
//                         }
//                     } else if (node->right != nullptr && !node->right->paint) {
//                         while (node->right != nullptr && !node->right->paint) {
//                             node = node->right;
//                             if (node->left != nullptr && !node->left->paint) {
//                                 while (node->left != nullptr && !node->left->paint) {
//                                     node = node->left;
//                                 }
//                             }
//                         }
//                         if (node->left != nullptr && !node->left->paint) {
//                             while (node->left != nullptr && !node->left->paint) {
//                                 node = node->left;
//                             }
//                         }
//                     }
//                 }
//             }
            
//             node->paint = 1;
//             return *this;
//         }

//         Iterator& operator++(int) {
//             if (node == nullptr) {
//                 return *this;
//             }

//             if (std::is_same<Tag, in_order_tag>::value) {
//                 if (node->left != nullptr && !node->left->paint) {
//                     while (node->left != nullptr && !node->left->paint) {
//                         node = node->left;
//                     }
//                 } else if (node->right != nullptr && !node->right->paint) {
//                     node = node->right;
//                     while (node->left != nullptr && !node->left->paint) {
//                         node = node->left;
//                     }
//                 } else {
//                     while (node->parent && (node->parent->right == node || node->parent->right == nullptr) && node->parent->paint) {
//                         node = node->parent;
//                     }

//                     if (node->parent && !node->parent->paint) {
//                         node = node->parent;
//                     }
//                 }
//             } else if (std::is_same<Tag, pre_order_tag>::value) {
//                 if (!node->paint) {
//                 } else if (node->left != nullptr ) {
//                     node = node->left;
//                 } else if (node->right != nullptr) {
//                     node = node->right;
//                 } else {
//                     while (node->parent && (node->parent->right == node || node->parent->right == nullptr)) {
//                         node = node->parent;
//                     }
//                     if (node->parent && node->parent->right != nullptr) {
//                         node = node->parent->right;
//                     }
//                 }
//             } else if (std::is_same<Tag, post_order_tag>::value) {
//                 if (node->left != nullptr && !node->left->paint) {
//                     while (node->left != nullptr && !node->left->paint) {
//                         node = node->left;
//                     }
//                     while (node->right != nullptr && !node->right->paint) {
//                         node = node->right;
//                     }
//                 } else if (node->right != nullptr && !node->right->paint) {
//                     while (node->right != nullptr && !node->right->paint) {
//                         node = node->right;
//                     }
//                     if (node->left != nullptr && !node->left->paint) {
//                         while (node->left != nullptr && !node->left->paint) {
//                             node = node->left;
//                         }
//                     }
//                 } else {
//                     while (node->parent != nullptr && node->paint) {
//                         node = node->parent;
//                     }
//                     if (node->left != nullptr && !node->left->paint) {
//                         while (node->left != nullptr && !node->left->paint) {
//                             node = node->left;
//                         }
//                         while (node->right != nullptr && !node->right->paint) {
//                             node = node->right;
//                         }
//                     } else if (node->right != nullptr && !node->right->paint) {
//                         while (node->right != nullptr && !node->right->paint) {
//                             node = node->right;
//                             if (node->left != nullptr && !node->left->paint) {
//                                 while (node->left != nullptr && !node->left->paint) {
//                                     node = node->left;
//                                 }
//                             }
//                         }
//                         if (node->left != nullptr && !node->left->paint) {
//                             while (node->left != nullptr && !node->left->paint) {
//                                 node = node->left;
//                             }
//                         }
//                     }
//                 }
//             }
            
//             node->paint = 1;
//             return *this;
//         }

//         Iterator& operator--() {
//             if (node == nullptr) {
//                 return *this;
//             }

//             if (std::is_same<Tag, in_order_tag>::value) {
//                 if (node->paint) {
//                     node->paint = 0;
//                     return *this;
//                 }
                
//                 if (node->left != nullptr && node->left->paint) {
//                     node = node->left;
//                     if (node->right != nullptr && node->right->paint) {
//                         while (node->right != nullptr && node->right->paint) {
//                             node = node->right;
//                         }
//                     }
//                     while (node->left != nullptr && node->left->paint) {
//                         node = node->left;
//                     }
//                 } else if (node->right != nullptr && node->right->paint) {
//                     node = node->right;
//                     while (node->left != nullptr && node->left->paint) {
//                         node = node->left;
//                     }

//                 } else {
//                     while (node->parent->parent != nullptr && node->parent->parent->paint) {
//                         node = node->parent;
//                     }
                    
//                     if (node->parent && node->parent->paint && !node->paint) {
//                         node = node->parent;
//                     }
//                 }
//             } else if (std::is_same<Tag, pre_order_tag>::value) {
//                 if (node->paint) {
//                 } else if (node->right != nullptr && node->right->paint) {
//                     node = node->right;
//                 } else if (node->left != nullptr && node->left->paint) {
//                     node = node->left;
//                 } else {
//                     while (node->parent != nullptr && !node->paint && node->parent->parent != nullptr) {
//                         node = node->parent;
//                     }

//                     if (node->left != nullptr && node->left->paint) {
//                         node = node->left;
//                     }

//                     if (node->parent && (node->parent->left != nullptr) && !node->paint) {
//                         node = node->parent->left;
//                         if (node->right != nullptr && node->right->paint) {
//                             while (node->right != nullptr && node->right->paint) {
//                                 node = node->right;
//                             }
//                         }
//                         if (node->parent->parent == nullptr) {
//                             node = node->parent;
//                         }
//                     }
//                 }
//             } else if (std::is_same<Tag, post_order_tag>::value) {
//                 if (node->paint) {
//                 } else if (node->right != nullptr && node->right->paint) {
//                     node = node->right;
//                 } else if (node->left != nullptr && node->left->paint) {
//                     node = node->left;
//                 } else {
//                     while (node->parent && !node->parent->paint && node->parent->parent) {
//                         node = node->parent;
//                         if (node->left != nullptr && node->left->paint) {
//                             node = node->left;
//                             break;
//                         }
//                     }
//                     if (node->parent && node->parent->left != nullptr && !node->paint) {
//                         if (node->parent->left->paint) {
//                             node = node->parent->left;
//                         }
//                     }
//                 }
//             }
            
//             node->paint = 0;
//             return *this;
//         }

//         Iterator& operator--(int) {
//             if (node == nullptr) {
//                 return *this;
//             }

//             if (std::is_same<Tag, in_order_tag>::value) {
//                 if (node->paint) {
//                     node->paint = 0;
//                     return *this;
//                 }
                
//                 if (node->left != nullptr && node->left->paint) {
//                     node = node->left;
//                     if (node->right != nullptr && node->right->paint) {
//                         while (node->right != nullptr && node->right->paint) {
//                             node = node->right;
//                         }
//                     }
//                     while (node->left != nullptr && node->left->paint) {
//                         node = node->left;
//                     }
//                 } else if (node->right != nullptr && node->right->paint) {
//                     node = node->right;
//                     while (node->left != nullptr && node->left->paint) {
//                         node = node->left;
//                     }

//                 } else {
//                     while (node->parent->parent != nullptr && node->parent->parent->paint) {
//                         node = node->parent;
//                     }
                    
//                     if (node->parent && node->parent->paint && !node->paint) {
//                         node = node->parent;
//                     }
//                 }
//             } else if (std::is_same<Tag, pre_order_tag>::value) {
//                 if (node->paint) {
//                 } else if (node->right != nullptr && node->right->paint) {
//                     node = node->right;
//                 } else if (node->left != nullptr && node->left->paint) {
//                     node = node->left;
//                 } else {
//                     while (node->parent != nullptr && !node->paint && node->parent->parent != nullptr) {
//                         node = node->parent;
//                     }

//                     if (node->left != nullptr && node->left->paint) {
//                         node = node->left;
//                     }

//                     if (node->parent && (node->parent->left != nullptr) && !node->paint) {
//                         node = node->parent->left;
//                         if (node->right != nullptr && node->right->paint) {
//                             while (node->right != nullptr && node->right->paint) {
//                                 node = node->right;
//                             }
//                         }
//                         if (node->parent->parent == nullptr) {
//                             node = node->parent;
//                         }
//                     }
//                 }
//             } else if (std::is_same<Tag, post_order_tag>::value) {
//                 if (node->paint) {
//                 } else if (node->right != nullptr && node->right->paint) {
//                     node = node->right;
//                 } else if (node->left != nullptr && node->left->paint) {
//                     node = node->left;
//                 } else {
//                     while (node->parent && !node->parent->paint && node->parent->parent) {
//                         node = node->parent;
//                         if (node->left != nullptr && node->left->paint) {
//                             node = node->left;
//                             break;
//                         }
//                     }
//                     if (node->parent && node->parent->left != nullptr && !node->paint) {
//                         if (node->parent->left->paint) {
//                             node = node->parent->left;
//                         }
//                     }
//                 }
//             }
            
//             node->paint = 0;
//             return *this;
//         }

// };
