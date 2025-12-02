cmake_minimum_required(VERSION 3.14)
project(RingBuffTest)

# C++11を要求
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Testを取得
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)

# Windows環境でgtest_force_shared_crtオプションを設定
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# テストを有効化
enable_testing()

# テスト実行ファイルを作成
add_executable(
  ringbuff_test
  test_ringbuff.cpp
)

# Google Testとリンク
target_link_libraries(
  ringbuff_test
  GTest::gtest_main
)

# ヘッダーファイルのインクルードパスを追加
target_include_directories(
  ringbuff_test
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# テストの自動検出
include(GoogleTest)
gtest_discover_tests(ringbuff_test)