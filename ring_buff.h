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

    T const* front() const noexcept
    {
        if (empty()) {
            return nullptr;
        }
        return &buffer_[head_];
    }

    std::optional<T> pop_by_copy()  // copyで要素を取り出す
    {
        if (empty()) {
            return std::nullopt;
        }
        T value = buffer_[head_];
        head_   = (head_ + 1) % N;
        --count_;
        return value;
    }

    std::optional<T> pop() noexcept  // moveで要素を取り出す
    {
        if (empty()) {
            return std::nullopt;
        }
        T value = std::move(buffer_[head_]);
        head_   = (head_ + 1) % N;
        --count_;
        return value;
    }

    bool empty() const noexcept { return count_ == 0; }

    bool full() const noexcept { return count_ == N; }

    constexpr size_t size() const noexcept { return count_; }

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
