#pragma one

#include <array>
#include <optional>
#include <stdexcept>

template <typename T, size_t N>
class RingBuff {
public:
    RingBuff() : head_{0}, tail_{0}, count_{0} {}

    // コピー/ムーブを削除
    RingBuff(RingBuff const&)            = delete;
    RingBuff& operator=(RingBuff const&) = delete;
    RingBuff(RingBuff&&)                 = delete;
    RingBuff& operator=(RingBuff&&)      = delete;

    // 要素を追加
    void push(T&& value) noexcept
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
    void push_by_copy(const T& value)
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

    std::optional<T const*> front() const noexcept  // const版
    {
        if (empty()) {
            return std::nullopt;
        }
        return &buffer_[head_];
    }

    T pop_by_copy()  // copyで要素を取り出す
    {
        if (empty()) {
            throw std::underflow_error("RingBuff is empty");
        }
        T value = buffer_[head_];
        head_   = (head_ + 1) % N;
        --count_;
        return value;
    }

    T pop()  // moveで要素を取り出す
    {
        if (empty()) {
            throw std::underflow_error("RingBuff is empty");
        }
        T value = std::move(buffer_[head_]);
        head_   = (head_ + 1) % N;
        --count_;
        return value;
    }

    // バッファが空か
    bool empty() const noexcept { return count_ == 0; }

    // バッファが満杯か
    bool full() const noexcept { return count_ == N; }

    // 現在の要素数
    constexpr size_t size() const noexcept { return count_; }

    // バッファの最大容量
    constexpr size_t capacity() const noexcept { return N; }

    // バッファをクリア
    void clear() noexcept
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
