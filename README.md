# AHC001のビジュアライザ

https://htmlpreview.github.io/?https://github.com/kanpurin/ahc001/blob/main/src/index.html

![visualizer](https://user-images.githubusercontent.com/56508025/147786054-f880f3ec-4790-4d68-93a3-9ee201be9c24.png)

- 問題 https://atcoder.jp/contests/ahc001/tasks/ahc001_a

inputに入力ファイル、logにログファイルを選択してください。

## ログファイルの形式
```
a_0 b_0 c_0 d_0
.
.
.
a_{n-1} b_{n-1} c_{n-1} d_{n-1}
```
という形式でT回(Tはログの出力回数)書かれたファイルである必要があります。nは入力ファイルからわかるので、ログファイルの行数とnの値からTの値は自動で計算されます。ログファイル例はsampleを参照してください。

## 注意
speedを大きくし過ぎると重くなります。50でも若干重いです。