# example_libs

std::arrayを使用したリングバッファの実装

## ファイル構成

- `ring_buff.h` - リングバッファのクラステンプレート
- `ring_buff_test.cpp` - Google Testの単体テスト

# LimitedVector

上限付きvectorの実装。
std::vectorのpublic派生でも実装量は減るが、
std::vectorのpublic派生されることを想定していないため、std::vectorの包含で実装した。

std::arrayはmoveが遅いため、std::arrayではだめ。

## ファイル構成

- `limited_vector.h`リングバッファのクラステンプレート
- `limited_vector_test.cpp`


## ビルド手順

```bash
    $./build.sh     # でビルド

    $./build.sh -h  # ヘルプメッセージ
```

## 要件

- C++14以降
- CMake 3.14以降
- インターネット接続（初回ビルド時にGoogle Testをダウンロード）
- clang-format（オプション、コードフォーマット用）
