cmake_minimum_required(VERSION 3.14)
project(RingBuffTest)

# C++14を要求
set(CMAKE_CXX_STANDARD 14)
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
  example_libs_test
  ring_buff_test.cpp
  limited_vector_test.cpp

)

# Google Testとリンク
target_link_libraries(
  example_libs_test
  GTest::gtest_main
)

# ヘッダーファイルのインクルードパスを追加
target_include_directories(
  example_libs_test
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# テストの自動検出
include(GoogleTest)
gtest_discover_tests(example_libs_test)

# clang-formatターゲットの追加（対象ファイルを明示的に指定）
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
  add_custom_target(
    format
    COMMAND ${CLANG_FORMAT} -i 
      ${CMAKE_CURRENT_SOURCE_DIR}/ring_buff.h
      ${CMAKE_CURRENT_SOURCE_DIR}/ring_buff_test.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/limited_vector.h
      ${CMAKE_CURRENT_SOURCE_DIR}/limited_vector_test.cpp
    COMMENT "Running clang-format on source files"
    VERBATIM
  )
else()
  message(WARNING "clang-format not found. 'format' target will not be available.")
endif()
