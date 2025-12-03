#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

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

    int val = rb.pop();
    ASSERT_EQ(val, 10);
    ASSERT_TRUE(rb.empty());
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

    ASSERT_THROW(rb.pop(), std::underflow_error);
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
}

TEST(RingBuffTest, vector)
{
    using vector_int8_t = std::vector<uint8_t>;
    RingBuff<vector_int8_t, 3> rb;

    vector_int8_t v0{1, 2, 3};

    rb.push(std::move(v0));
    rb.push(vector_int8_t{2, 3});
    rb.push(vector_int8_t{3});
    ASSERT_EQ(v0.size(), 0);

    ASSERT_TRUE(rb.full());

    auto v0_pop = rb.pop();
    ASSERT_EQ(v0_pop.size(), 3);
    ASSERT_FALSE(rb.full());

    vector_int8_t v{1, 2, 3, 4};
    rb.push(std::move(v));
    ASSERT_TRUE(rb.full());

    auto v1_pop = rb.pop();
    ASSERT_EQ((vector_int8_t{2, 3}), v1_pop);

    auto v2_pop = rb.pop();
    ASSERT_EQ((vector_int8_t{3}), v2_pop);

    ASSERT_EQ(rb.size(), 1);

    auto v_pop = rb.pop();
    ASSERT_EQ(4, v_pop.size());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
