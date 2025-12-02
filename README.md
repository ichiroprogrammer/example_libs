# RingBuff クラステンプレート

std::arrayを使用したリングバッファの実装です。

## ファイル構成

- `RingBuff.h` - リングバッファのクラステンプレート
- `test_ringbuff.cpp` - Google Testの単体テスト
- `CMakeLists.txt` - CMakeビルド設定

## ビルド手順

```bash
# ビルドディレクトリを作成
mkdir build
cd build

# CMakeで設定
cmake ..

# ビルド
cmake --build .

# テスト実行
ctest
# または
./ringbuff_test
```

## 使用例

```cpp
#include "RingBuff.h"

int main() {
    RingBuff<int, 10> buffer;
    
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    
    int val = buffer.pop();  // val = 1
    
    return 0;
}
```

## 要件

- C++11以降
- CMake 3.14以降
- インターネット接続（初回ビルド時にGoogle Testをダウンロード）
