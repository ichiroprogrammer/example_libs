#pragma once

#include <stdexcept>
#include <vector>

template <typename T, size_t max>
class LimitedVector {
public:
    using value_type      = T;
    using size_type       = typename std::vector<T>::size_type;
    using reference       = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;
    using iterator        = typename std::vector<T>::iterator;
    using const_iterator  = typename std::vector<T>::const_iterator;

    LimitedVector() { vec_.reserve(max); }

    // コピー/ムーブコンストラクタ
    LimitedVector(LimitedVector const&)            = default;
    LimitedVector(LimitedVector&&)                 = default;
    LimitedVector& operator=(LimitedVector const&) = default;
    LimitedVector& operator=(LimitedVector&&)      = default;

    // 初期化リストコンストラクタ
    LimitedVector(std::initializer_list<T> init) : vec_(init)
    {
        if (init.size() > max) {
            throw std::length_error("LimitedVector: initializer list exceeds max size");
        }
        vec_.reserve(max);
    }

    // 要素を追加
    void push_back(const T& value)
    {
        if (is_full()) {
            throw std::length_error("LimitedVector is full");
        }
        vec_.push_back(value);
    }

    void push_back(T&& value)
    {
        if (is_full()) {
            throw std::length_error("LimitedVector is full");
        }
        vec_.push_back(std::move(value));
    }

    template <typename... Args>
    void emplace_back(Args&&... args)
    {
        if (is_full()) {
            throw std::length_error("LimitedVector is full");
        }
        vec_.emplace_back(std::forward<Args>(args)...);
    }

    // 要素を削除
    void pop_back() { vec_.pop_back(); }

    // 容量チェック
    bool is_full() const { return vec_.size() >= max; }

    bool empty() const { return vec_.empty(); }

    size_type size() const { return vec_.size(); }

    constexpr size_type capacity() const { return max; }

    constexpr size_type max_size() const { return max; }

    // 要素アクセス
    reference operator[](size_type pos) { return vec_[pos]; }

    const_reference operator[](size_type pos) const { return vec_[pos]; }

    reference at(size_type pos) { return vec_.at(pos); }

    const_reference at(size_type pos) const { return vec_.at(pos); }

    reference front() { return vec_.front(); }

    const_reference front() const { return vec_.front(); }

    reference back() { return vec_.back(); }

    const_reference back() const { return vec_.back(); }

    T* data() { return vec_.data(); }

    const T* data() const { return vec_.data(); }

    // イテレータ
    iterator begin() { return vec_.begin(); }

    const_iterator begin() const { return vec_.begin(); }

    const_iterator cbegin() const { return vec_.cbegin(); }

    iterator end() { return vec_.end(); }

    const_iterator end() const { return vec_.end(); }

    const_iterator cend() const { return vec_.cend(); }

    // その他の操作
    void clear() { vec_.clear(); }

    void reserve(size_type new_cap)
    {
        if (new_cap > max) {
            throw std::length_error("LimitedVector: reserve exceeds max size");
        }
        vec_.reserve(new_cap);
    }

private:
    std::vector<T> vec_;
};
