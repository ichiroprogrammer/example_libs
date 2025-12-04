#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "measure_performance.h"
#include "ring_buff.h"

// 基本操作のテスト
TEST(RingBuffTest, BasicOperations)
{
    RingBuff<int, 5> rb;

    ASSERT_TRUE(rb.empty());
    ASSERT_FALSE(rb.full());
    ASSERT_EQ(rb.size(), 0);
    ASSERT_EQ(rb.capacity(), 5);

    rb.push(10);
    ASSERT_FALSE(rb.empty());
    ASSERT_EQ(rb.size(), 1);

    auto val0 = rb.pop();
    ASSERT_EQ(val0 ? *val0 : 0, 10);
    auto val1 = rb.pop();
    ASSERT_TRUE(rb.empty());
    auto val2 = rb.pop();
    ASSERT_FALSE(val2);
    ASSERT_EQ(rb.size(), 0);
}

// 容量いっぱいまで追加
TEST(RingBuffTest, FillToCapacity)
{
    RingBuff<int, 3> rb;

    rb.push(1);
    rb.push(2);
    rb.push(3);

    ASSERT_TRUE(rb.full());
    ASSERT_EQ(rb.size(), 3);
}

// 満杯時のpushで上書き動作
TEST(RingBuffTest, OverwriteWhenFull)
{
    RingBuff<int, 3> rb;

    rb.push(1);
    rb.push(2);
    rb.push(3);

    ASSERT_TRUE(rb.full());

    // 4番目の要素を追加（1を上書き）
    rb.push(4);

    ASSERT_TRUE(rb.full());
    ASSERT_EQ(rb.size(), 3);

    // 最古の要素(1)は上書きされ、2から取り出せる
    ASSERT_EQ(rb.pop(), 2);
    ASSERT_EQ(rb.pop(), 3);
    ASSERT_EQ(rb.pop(), 4);
    ASSERT_TRUE(rb.empty());
}

// 空の時のpopで例外発生
TEST(RingBuffTest, UnderflowException)
{
    RingBuff<int, 3> rb;

    auto pop = rb.pop();
    ASSERT_FALSE(pop);
}

// FIFO動作の確認
TEST(RingBuffTest, FIFOBehavior)
{
    RingBuff<int, 5> rb;

    rb.push(10);
    rb.push(20);
    rb.push(30);

    ASSERT_EQ(rb.pop(), 10);
    ASSERT_EQ(rb.pop(), 20);
    ASSERT_EQ(rb.pop(), 30);
}

// リングとして一周する動作
TEST(RingBuffTest, WrapAround)
{
    RingBuff<int, 3> rb;

    rb.push(1);
    rb.push(2);
    rb.push(3);

    ASSERT_EQ(rb.pop(), 1);
    ASSERT_EQ(rb.pop(), 2);

    rb.push(4);
    rb.push(5);

    ASSERT_EQ(rb.pop(), 3);
    ASSERT_EQ(rb.pop(), 4);
    ASSERT_EQ(rb.pop(), 5);
    ASSERT_TRUE(rb.empty());
}

// clearのテスト
TEST(RingBuffTest, Clear)
{
    RingBuff<int, 5> rb;

    rb.push(1);
    rb.push(2);
    rb.push(3);

    rb.clear();

    ASSERT_TRUE(rb.empty());
    ASSERT_EQ(rb.size(), 0);

    rb.push(10);
    ASSERT_EQ(rb.pop(), 10);
}

// 文字列型でのテスト
TEST(RingBuffTest, StringType)
{
    RingBuff<std::string, 3> rb;

    rb.push("hello");
    rb.push("world");

    ASSERT_EQ(rb.pop(), "hello");
    ASSERT_EQ(rb.pop(), "world");
}

// 連続した上書き動作
TEST(RingBuffTest, ContinuousOverwrite)
{
    RingBuff<int, 3> rb;

    // 1, 2, 3を追加（満杯）
    rb.push(1);
    rb.push(2);
    rb.push(3);

    // 4, 5を追加（1, 2を上書き）
    rb.push(4);
    rb.push(5);

    ASSERT_TRUE(rb.full());

    // 3, 4, 5が取り出せる
    ASSERT_EQ(rb.pop(), 3);
    ASSERT_EQ(rb.pop(), 4);
    ASSERT_EQ(rb.pop(), 5);
    ASSERT_TRUE(rb.empty());

    auto const* front = rb.front();
    ASSERT_FALSE(front);
}

TEST(RingBuffTest, vector)
{
    using vector_int8_t = std::vector<uint8_t>;
    RingBuff<vector_int8_t, 3> rb;

    vector_int8_t v0{1, 2, 3};
    rb.push(std::move(v0));
    ASSERT_EQ(v0.size(), 0);

    auto const& rb_const = rb;
    auto const* front0   = rb_const.front();

    ASSERT_TRUE(front0);
    ASSERT_EQ(front0->size(), 3);

    rb.push(vector_int8_t{2, 3});
    rb.push(vector_int8_t{3});
    ASSERT_TRUE(rb.full());

    rb.push(vector_int8_t{});
    auto const* front1 = rb.front();
    ASSERT_EQ(front1 ? front1->size() : -1, 2);

    rb.push(vector_int8_t{});
    ASSERT_TRUE(rb.full());
    auto const& front2 = rb.front();
    ASSERT_TRUE(front2);
    ASSERT_EQ(front2 ? front2->size() : -1, 1);

    auto v0_pop = rb.pop();
    ASSERT_EQ(v0_pop->size(), 1);
    ASSERT_FALSE(rb.full());

    vector_int8_t v{1, 2, 3, 4};
    rb.push(std::move(v));
    ASSERT_TRUE(rb.full());

    auto v1_pop = rb.pop();
    ASSERT_EQ((vector_int8_t{}), v1_pop);

    auto v2_pop = rb.pop();
    ASSERT_EQ((vector_int8_t{}), v2_pop);

    auto v_pop = rb.pop();
    ASSERT_EQ(4, v_pop->size());
}

TEST(RingBuffTest, DISABLED_perf_push)
{
    using vector_int8_t = std::vector<uint8_t>;
    RingBuff<vector_int8_t, 3> rb;

    constexpr uint32_t count = 0xff'ffff;

    auto perf0 = [&rb]() {
        vector_int8_t v0(100000, 1);
        rb.push_by_copy(v0);
    };

    auto perf1 = [&rb]() {
        vector_int8_t v0(100000, 1);
        rb.push(std::move(v0));
    };

    auto perf0_ms = MeasurePerformance(count, perf0).count();
    auto perf1_ms = MeasurePerformance(count, perf1).count();
    std::cout << "push(copy): " << perf0_ms << " ms" << std::endl;
    std::cout << "push(move): " << perf1_ms << " ms" << std::endl;
    // 著者の環境での結果
    // push(copy): 80545 ms
    // push(move): 37093 ms
}

TEST(RingBuffTest, DISABLED_perf_pop)
{
    using vector_int8_t = std::vector<uint8_t>;
    RingBuff<vector_int8_t, 3> rb;

    constexpr uint32_t count    = 0xff'ffff;
    constexpr uint32_t vec_size = 100000;

    auto perf0 = [&rb, vec_size = vec_size]() {
        vector_int8_t v0(vec_size, 1);
        rb.push(std::move(v0));
        auto v1 = rb.pop_by_copy();
        ASSERT_EQ(v1->size(), vec_size);
    };

    auto perf1 = [&rb, vec_size = vec_size]() {
        vector_int8_t v0(vec_size, 1);
        rb.push(std::move(v0));
        auto const* front = rb.front();
        auto        v1    = rb.pop();
        ASSERT_EQ(v1->size(), vec_size);
        ASSERT_EQ(front->size(), 0);
    };

    auto perf0_ms = MeasurePerformance(count, perf0).count();
    auto perf1_ms = MeasurePerformance(count, perf1).count();
    std::cout << "pop_by_copy: " << perf0_ms << " ms" << std::endl;
    std::cout << "pop(move):   " << perf1_ms << " ms" << std::endl;
    // 著者の環境での結果
    // pop_by_copy: 85104 ms
    // pop(move):   40699 ms
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
