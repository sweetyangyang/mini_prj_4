#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265359

// 計算離散傅立葉變換 (DFT)
void computeDFT(const double *input, double *output_real, double *output_imag, int N) {
    for (int k = 0; k < N; ++k) {
        output_real[k] = 0.0;
        output_imag[k] = 0.0;
        for (int n = 0; n < N; ++n) {
            double angle = 2.0 * PI * k * n / N;
            output_real[k] += input[n] * cos(angle);
            output_imag[k] -= input[n] * sin(angle);
        }
    }
}

// 計算頻譜的幅值並儲存到文件中
void calculateMagnitude(const double *real, const double *imag, double *magnitude, int N, const char *outputFile) {
    FILE *output_txt = fopen(outputFile, "a+");
    if (!output_txt) {
        perror("Error opening output text file");
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < N; k++) {
        magnitude[k] = sqrt(real[k] * real[k] + imag[k] * imag[k]);
        fprintf(output_txt, "%d ", (int)ceil(magnitude[k])); // 將幅值取上整後儲存
    }
    fprintf(output_txt, "\n");
    fclose(output_txt);
}

// 生成窗口函數並應用於音訊數據
double applyWindow(int size, const char *type, int i, int data) {
    if (strcmp(type, "hamming") == 0) {
        return (0.54 - 0.46 * cos(2.0 * PI * i / (size - 1))) * data;
    } else if (strcmp(type, "rectangular") == 0) {
        return 1.0 * data;
    }
    return 0.0;
}

// WAV 文件標頭結構
typedef struct {
    char     chunkID[4];
    uint32_t chunkSize;
    char     format[4];
    char     subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char     subchunk2ID[4];
    uint32_t subchunk2Size;
} WavHeader;

// 生成頻譜圖
void spectrogram(double w_size, const char *w_type, double dft_size, double f_itv, const char *wav_in, const char *spec_out) {
    // 開啟 WAV 文件
    FILE *input_wav = fopen(wav_in, "rb");
    if (!input_wav) {
        perror("Error opening input WAV file");
        exit(EXIT_FAILURE);
    }

    // 讀取 WAV 標頭
    WavHeader header;
    fread(&header, sizeof(WavHeader), 1, input_wav);

    // 計算總樣本數
    uint32_t total_samples = header.subchunk2Size / sizeof(int16_t);
    int16_t *data_samples = (int16_t *)malloc(total_samples * sizeof(int16_t));
    if (!data_samples) {
        perror("Error allocating memory for samples");
        fclose(input_wav);
        exit(EXIT_FAILURE);
    }

    // 讀取音訊數據
    fread(data_samples, sizeof(int16_t), total_samples, input_wav);

    // 設定窗口大小和頻譜計算相關參數
    int N = (int)(header.sampleRate * w_size); // 每個窗口的樣本數
    int frame_count = (total_samples / header.sampleRate) / f_itv;
    double *real = (double *)calloc(N, sizeof(double));
    double *imag = (double *)calloc(N, sizeof(double));
    double *magnitude = (double *)calloc(N, sizeof(double));

    if (!real || !imag || !magnitude) {
        perror("Error allocating memory for DFT buffers");
        free(data_samples);
        fclose(input_wav);
        exit(EXIT_FAILURE);
    }

    for (int frame = 0; frame < frame_count; frame++) {
        double start_time = frame * f_itv;
        double end_time = start_time + w_size;
        if (end_time > (double)total_samples / header.sampleRate) {
            end_time = (double)total_samples / header.sampleRate;
        }

        int start_sample = (int)(start_time * header.sampleRate);
        int end_sample = (int)(end_time * header.sampleRate);
        int current_window_size = end_sample - start_sample;

        for (int i = 0; i < current_window_size; i++) {
            if (i < N) {
                double windowed_sample = applyWindow(current_window_size, w_type, i, data_samples[start_sample + i]);
                real[i] = windowed_sample;
                imag[i] = 0.0;
            }
        }

        // 計算 DFT
        computeDFT(real, real, imag, N);
        // 計算幅值並寫入文件
        calculateMagnitude(real, imag, magnitude, N, spec_out);
    }

    // 釋放資源
    free(real);
    free(imag);
    free(magnitude);
    free(data_samples);
    fclose(input_wav);
}

// 主程式
int main(int argc, char *argv[]) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s <window_size> <window_type> <dft_size> <frame_interval> <input_wav> <output_txt>\n", argv[0]);
        return EXIT_FAILURE;
    }

    double w_size = atof(argv[1]);
    const char *window_type = argv[2];
    double dft_size = atof(argv[3]);
    double f_itv = atof(argv[4]);
    const char *input_wav = argv[5];
    const char *output_txt = argv[6];

    spectrogram(w_size, window_type, dft_size, f_itv, input_wav, output_txt);
    printf("Spectrogram successfully created and saved to %s\n", output_txt);

    return EXIT_SUCCESS;
}
