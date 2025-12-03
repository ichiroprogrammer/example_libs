#include <gtest/gtest.h>

#include <string>

#include "limited_vector.h"

// 基本操作のテスト
TEST(LimitedVectorTest, BasicOperations)
{
    LimitedVector<int, 5> lv;

    ASSERT_TRUE(lv.empty());
    ASSERT_FALSE(lv.is_full());
    ASSERT_EQ(lv.size(), 0);
    ASSERT_EQ(lv.capacity(), 5);
    ASSERT_EQ(lv.max_size(), 5);

    lv.push_back(10);
    ASSERT_FALSE(lv.empty());
    ASSERT_EQ(lv.size(), 1);
    ASSERT_EQ(lv[0], 10);
}

// 最大容量まで追加
TEST(LimitedVectorTest, FillToCapacity)
{
    LimitedVector<int, 3> lv;

    lv.push_back(1);
    lv.push_back(2);
    lv.push_back(3);

    ASSERT_TRUE(lv.is_full());
    ASSERT_EQ(lv.size(), 3);
}

// 満杯時のpush_backで例外
TEST(LimitedVectorTest, PushBackWhenFull)
{
    LimitedVector<int, 2> lv;

    lv.push_back(1);
    lv.push_back(2);

    ASSERT_THROW(lv.push_back(3), std::length_error);
}

// emplace_backのテスト
TEST(LimitedVectorTest, EmplaceBack)
{
    LimitedVector<std::string, 3> lv;

    lv.emplace_back("hello");
    lv.emplace_back("world");

    ASSERT_EQ(lv.size(), 2);
    ASSERT_EQ(lv[0], "hello");
    ASSERT_EQ(lv[1], "world");

    lv.emplace_back("test");
    ASSERT_TRUE(lv.is_full());

    ASSERT_THROW(lv.emplace_back("overflow"), std::length_error);
}

// 初期化リストコンストラクタ
TEST(LimitedVectorTest, InitializerList)
{
    LimitedVector<int, 5> lv{1, 2, 3};

    ASSERT_EQ(lv.size(), 3);
    ASSERT_EQ(lv[0], 1);
    ASSERT_EQ(lv[1], 2);
    ASSERT_EQ(lv[2], 3);
}

// 初期化リストが最大サイズを超える場合
TEST(LimitedVectorTest, InitializerListExceedsMax)
{
    ASSERT_THROW((LimitedVector<int, 2>{1, 2, 3}), std::length_error);
}

// pop_backのテスト
TEST(LimitedVectorTest, PopBack)
{
    LimitedVector<int, 5> lv{1, 2, 3};

    lv.pop_back();
    ASSERT_EQ(lv.size(), 2);
    ASSERT_EQ(lv.back(), 2);
}

// clearのテスト
TEST(LimitedVectorTest, Clear)
{
    LimitedVector<int, 5> lv{1, 2, 3};

    lv.clear();
    ASSERT_TRUE(lv.empty());
    ASSERT_EQ(lv.size(), 0);

    lv.push_back(10);
    ASSERT_EQ(lv[0], 10);
}

// イテレータのテスト
TEST(LimitedVectorTest, Iterator)
{
    LimitedVector<int, 5> lv{1, 2, 3, 4, 5};

    int sum = 0;
    for (auto it = lv.begin(); it != lv.end(); ++it) {
        sum += *it;
    }
    ASSERT_EQ(sum, 15);

    // range-based for
    sum = 0;
    for (auto const& val : lv) {
        sum += val;
    }
    ASSERT_EQ(sum, 15);
}

// moveのテスト
TEST(LimitedVectorTest, Move)
{
    LimitedVector<int, 5> lv1{1, 2, 3};

    LimitedVector<int, 5> lv2 = std::move(lv1);

    ASSERT_EQ(lv2.size(), 3);
    ASSERT_EQ(lv2[0], 1);
    ASSERT_EQ(lv2[1], 2);
    ASSERT_EQ(lv2[2], 3);

    // move後のlv1は空（実装依存だが、通常は空）
    ASSERT_TRUE(lv1.empty());
}

// atメソッドのテスト
TEST(LimitedVectorTest, AtMethod)
{
    LimitedVector<int, 5> lv{10, 20, 30};

    ASSERT_EQ(lv.at(0), 10);
    ASSERT_EQ(lv.at(1), 20);
    ASSERT_EQ(lv.at(2), 30);

    ASSERT_THROW(lv.at(3), std::out_of_range);
}

// front/backのテスト
TEST(LimitedVectorTest, FrontBack)
{
    LimitedVector<int, 5> lv{10, 20, 30};

    ASSERT_EQ(lv.front(), 10);
    ASSERT_EQ(lv.back(), 30);

    lv.front() = 100;
    lv.back()  = 300;

    ASSERT_EQ(lv.front(), 100);
    ASSERT_EQ(lv.back(), 300);
}
