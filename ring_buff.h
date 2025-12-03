#pragma one

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class RingBuff {
public:
    RingBuff() : head_(0), tail_(0), count_(0) {}

    // コピー/ムーブを削除
    RingBuff(RingBuff const&)            = delete;
    RingBuff& operator=(RingBuff const&) = delete;
    RingBuff(RingBuff&&)                 = delete;
    RingBuff& operator=(RingBuff&&)      = delete;

    // 要素を追加
    void push(T&& value)
    {
        buffer_[tail_] = std::move(value);
        tail_          = (tail_ + 1) % N;

        if (full()) {
            // フル状態の場合、headも進める（最古の要素を上書き）
            head_ = (head_ + 1) % N;
        }
        else {
            ++count_;
        }
    }

    // 要素を追加
    void push(const T& value)
    {
        buffer_[tail_] = value;
        tail_          = (tail_ + 1) % N;

        if (full()) {
            // フル状態の場合、headも進める（最古の要素を上書き）
            head_ = (head_ + 1) % N;
        }
        else {
            ++count_;
        }
    }

    // 要素を取り出す
    T pop()
    {
        if (empty()) {
            throw std::underflow_error("RingBuff is empty");
        }
        T value = buffer_[head_];
        head_   = (head_ + 1) % N;
        --count_;
        return value;
    }

    // バッファが空か
    bool empty() const { return count_ == 0; }

    // バッファが満杯か
    bool full() const { return count_ == N; }

    // 現在の要素数
    size_t size() const { return count_; }

    // バッファの最大容量
    size_t capacity() const { return N; }

    // バッファをクリア
    void clear()
    {
        head_  = 0;
        tail_  = 0;
        count_ = 0;
    }

private:
    std::array<T, N> buffer_;
    size_t           head_;
    size_t           count_;
    size_t           tail_;
};
