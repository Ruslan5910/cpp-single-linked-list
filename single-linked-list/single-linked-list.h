#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

using namespace std;

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
        class BasicIterator {
            friend class SingleLinkedList;
            explicit BasicIterator(Node* node) 
            : node_(node) {}

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Type;
            using difference_type = std::ptrdiff_t;
            using pointer = ValueType*;
            using reference = ValueType&;

            BasicIterator() = default;

            BasicIterator(const BasicIterator<Type>& other) noexcept 
               : node_(other.node_) {}
            
            BasicIterator& operator=(const BasicIterator& rhs) = default;

            [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
                return node_ == rhs.node_;
            }

            [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
                return !(node_ == rhs.node_);
            }

            [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
                return node_ == rhs.node_;
            }

            [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
                return !(node_ == rhs.node_);
            }

            BasicIterator& operator++() noexcept {
                assert(node_ != nullptr);
                node_ = node_ -> next_node;
                return *this;
            }

            BasicIterator operator++(int) noexcept {
                auto old_value(*this);
                ++(*this);
                return old_value;
            }

            [[nodiscard]] reference operator*() const noexcept {
                assert(node_ != nullptr);
                return node_->value;
            }

            [[nodiscard]] pointer operator->() const noexcept {
                assert(node_ != nullptr);
                return &(node_->value);
            }

        private:
            Node* node_ = nullptr; 
        };
        
template <typename Container>
void CreateSingleLinkedList(const Container& container) {
    SingleLinkedList copy_container;
    auto pos_before_insert = copy_container.before_begin();
    for (auto it = container.begin(); it != container.end(); ++it) {
        pos_before_insert = copy_container.InsertAfter(pos_before_insert, *it);
    }
    swap(copy_container);
}

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;
    
    SingleLinkedList() = default;
        
    SingleLinkedList(std::initializer_list<Type> values) {
        CreateSingleLinkedList(values);
    }
    
    SingleLinkedList(const SingleLinkedList& other) {
        CreateSingleLinkedList(other);
    }
        
    ~SingleLinkedList() {
        Clear();
    }
    
    void Clear() {
        while (head_.next_node != nullptr) {
            Node *ptr = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete ptr;
        }
        size_ = 0;
    }
    
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList copy(rhs);
            swap(copy);
        }
        return *this;
    }
    
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* new_node = new Node(value, pos.node_ -> next_node);
        pos.node_ -> next_node = new_node;
        ++size_;
        return Iterator(new_node);
    }

    void PopFront() noexcept {
        assert(size_ != 0);
        Node* new_first_node = head_.next_node -> next_node;
        delete head_.next_node;
        head_.next_node = new_first_node;
        --size_;
    }
    
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr && pos.node_ -> next_node != nullptr);
        Node* new_after_pos = pos.node_ -> next_node -> next_node;
        delete pos.node_ -> next_node;
        pos.node_ -> next_node = new_after_pos;
        --size_;
        return Iterator(new_after_pos);
    }
    
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }
    
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }
    
    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }
    
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

private:
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lhs.GetSize() == rhs.GetSize() && equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
