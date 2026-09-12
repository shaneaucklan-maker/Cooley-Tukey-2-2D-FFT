#include "pch.h"
#pragma once
#include <vector>
#include <complex>
#include <cmath>

using Complex = std::complex<double>;
const double PI = 3.14159265358979323846;

class CFFTEngine
{
public:
    // 2D FFT를 위한 2의 거듭제곱 크기 계산
    static int GetNextPowerOfTwo(int n) {
        int p = 1;
        while (p < n) p <<= 1;
        return p;
    }

    // 1D Cooley-Tukey FFT
    static void FFT1D(std::vector<Complex>& x) {
        size_t N = x.size();
        if (N <= 1) return;

        std::vector<Complex> even(N / 2), odd(N / 2);
        for (size_t i = 0; i < N / 2; ++i) {
            even[i] = x[i * 2];
            odd[i] = x[i * 2 + 1];
        }

        FFT1D(even);
        FFT1D(odd);

        for (size_t k = 0; k < N / 2; ++k) {
            Complex t = std::polar(1.0, -2.0 * PI * k / static_cast<double>(N)) * odd[k];
            x[k] = even[k] + t;
            x[k + N / 2] = even[k] - t;
        }
    }

    // 2D FFT (행 단위 후 열 단위 처리)
    static void FFT2D(std::vector<std::vector<Complex>>& data, int width, int height) {
        for (int y = 0; y < height; ++y) {
            FFT1D(data[y]);
        }

        for (int x = 0; x < width; ++x) {
            std::vector<Complex> col(height);
            for (int y = 0; y < height; ++y) {
                col[y] = data[y][x];
            }
            FFT1D(col);
            for (int y = 0; y < height; ++y) {
                data[y][x] = col[y];
            }
        }
    }

    // DC(저주파) 성분을 중앙으로 이동
    static void ShiftFFT(std::vector<std::vector<double>>& spectrum, int width, int height) {
        int halfW = width / 2;
        int halfH = height / 2;

        for (int y = 0; y < halfH; ++y) {
            for (int x = 0; x < halfW; ++x) {
                std::swap(spectrum[y][x], spectrum[y + halfH][x + halfW]);
                std::swap(spectrum[y + halfH][x], spectrum[y][x + halfW]);
            }
        }
    }
};
