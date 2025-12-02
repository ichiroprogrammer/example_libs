#include "RingBuff.h"
#include <gtest/gtest.h>

// 基本操作のテスト
TEST(RingBuffTest, BasicOperations) {
    RingBuff<int, 5> rb;
    
    EXPECT_TRUE(rb.empty());
    EXPECT_FALSE(rb.full());
    EXPECT_EQ(rb.size(), 0);
    EXPECT_EQ(rb.capacity(), 5);
    
    rb.push(10);
    EXPECT_FALSE(rb.empty());
    EXPECT_EQ(rb.size(), 1);
    
    int val = rb.pop();
    EXPECT_EQ(val, 10);
    EXPECT_TRUE(rb.empty());
    EXPECT_EQ(rb.size(), 0);
}

// 容量いっぱいまで追加
TEST(RingBuffTest, FillToCapacity) {
    RingBuff<int, 3> rb;
    
    rb.push(1);
    rb.push(2);
    rb.push(3);
    
    EXPECT_TRUE(rb.full());
    EXPECT_EQ(rb.size(), 3);
}

// 満杯時のpushで例外発生
TEST(RingBuffTest, OverflowException) {
    RingBuff<int, 2> rb;
    
    rb.push(1);
    rb.push(2);
    
    EXPECT_THROW(rb.push(3), std::overflow_error);
}

// 空の時のpopで例外発生
TEST(RingBuffTest, UnderflowException) {
    RingBuff<int, 3> rb;
    
    EXPECT_THROW(rb.pop(), std::underflow_error);
}

// FIFO動作の確認
TEST(RingBuffTest, FIFOBehavior) {
    RingBuff<int, 5> rb;
    
    rb.push(10);
    rb.push(20);
    rb.push(30);
    
    EXPECT_EQ(rb.pop(), 10);
    EXPECT_EQ(rb.pop(), 20);
    EXPECT_EQ(rb.pop(), 30);
}

// リングとして一周する動作
TEST(RingBuffTest, WrapAround) {
    RingBuff<int, 3> rb;
    
    rb.push(1);
    rb.push(2);
    rb.push(3);
    
    EXPECT_EQ(rb.pop(), 1);
    EXPECT_EQ(rb.pop(), 2);
    
    rb.push(4);
    rb.push(5);
    
    EXPECT_EQ(rb.pop(), 3);
    EXPECT_EQ(rb.pop(), 4);
    EXPECT_EQ(rb.pop(), 5);
    EXPECT_TRUE(rb.empty());
}

// clearのテスト
TEST(RingBuffTest, Clear) {
    RingBuff<int, 5> rb;
    
    rb.push(1);
    rb.push(2);
    rb.push(3);
    
    rb.clear();
    
    EXPECT_TRUE(rb.empty());
    EXPECT_EQ(rb.size(), 0);
    
    rb.push(10);
    EXPECT_EQ(rb.pop(), 10);
}

// 文字列型でのテスト
TEST(RingBuffTest, StringType) {
    RingBuff<std::string, 3> rb;
    
    rb.push("hello");
    rb.push("world");
    
    EXPECT_EQ(rb.pop(), "hello");
    EXPECT_EQ(rb.pop(), "world");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}