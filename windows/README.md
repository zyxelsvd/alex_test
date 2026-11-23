# Windows 簡易計算機

開啟 `dist/Calculator.exe` 即可使用，不需要安裝 Python、Node.js 或其他執行環境。目標系統為 Windows 10 / 11（64 位元）。

- 滑鼠或數字鍵：輸入數字與小數點。
- `+`、`-`、`*`、`/`：加減乘除。
- `Enter` 或 `=`：計算結果。
- `C` 或 `Esc`：清除全部。
- 連續運算依輸入順序計算，例如 `2 + 3 × 4 =` 得到 `20`。
- 除以零會顯示錯誤，按 C 或輸入新數字可繼續。

## 重新編譯

Windows 安裝 MinGW-w64 GCC 後，執行本目錄的 `build.bat`。

Linux 有 MinGW-w64 時：

```sh
mkdir -p dist
x86_64-w64-mingw32-gcc windows/calculator.c -std=c11 -O2 -Wall -Wextra -municode -mwindows -static -s -o dist/Calculator.exe -luser32 -lgdi32
```

運算核心測試：

```sh
gcc windows/test_calculator.c -std=c11 -Wall -Wextra -o /tmp/test-calculator
/tmp/test-calculator
```
