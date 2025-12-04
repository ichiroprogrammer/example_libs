#pragma once
#include <chrono>

/// @brief 関数fをount回実行し、その実行時間を計測する。
/// @tparam FUNC 実行する関数オブジェクトの型
/// @param count 関数を実行する回数
/// @param f 実行する関数オブジェクト
/// @return 実行時間
namespace Inner_ {
inline void DisplayProgressBar(uint32_t progress)
{
    std::cout << "\r[";
    uint32_t const bar_width = 50;
    uint32_t const pos       = bar_width * progress / 100;
    for (uint32_t j = 0; j < bar_width; ++j) {
        if (j < pos) {
            std::cout << "=";
        }
        else if (j == pos) {
            std::cout << ">";
        }
        else {
            std::cout << " ";
        }
    }
    std::cout << "] " << progress << "% " << std::flush;
}

template <typename FUNC>
void ExecuteWithProgress(uint32_t count, FUNC f)
{
    uint32_t last_reported = 0;

    for (auto i = 0U; i < count; ++i) {
        f();

        uint32_t const progress = static_cast<uint32_t>((static_cast<double>(i + 1) / count) * 100);

        // 5%刻みまたは完了時に表示
        if (progress >= last_reported + 5 || (i + 1) == count) {
            DisplayProgressBar(progress);
            last_reported = progress;
        }
    }
    std::cout << std::endl;
}
}  // namespace Inner_

template <typename FUNC>
std::chrono::milliseconds MeasurePerformance(uint32_t count, FUNC f, bool show_progress = true)
{
    auto const start = std::chrono::system_clock::now();

    if (show_progress) {
        Inner_::ExecuteWithProgress(count, f);
    }
    else {
        for (auto i = 0U; i < count; ++i) {
            f();
        }
    }

    auto const stop = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
}
