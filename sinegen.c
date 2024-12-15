#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265359

// 寫入 WAV 文件的基本頭部
void writeWavHeader(FILE *file, int sampleRate, int numSamples, int bitsPerSample) {
    uint16_t numChannels = 1;  // 單聲道
    uint16_t blockAlign = numChannels * (bitsPerSample / 8);
    uint32_t subchunk2Size = numSamples * blockAlign;
    uint32_t chunkSize = 36 + subchunk2Size;
    uint32_t byteRate = sampleRate * blockAlign;

    fwrite("RIFF", 1, 4, file);
    fwrite(&chunkSize, 4, 1, file);
    fwrite("WAVE", 1, 4, file);
    fwrite("fmt ", 1, 4, file);
    uint32_t subchunk1Size = 16;  // PCM 格式
    uint16_t audioFormat = 1;     // PCM
    fwrite(&subchunk1Size, 4, 1, file);
    fwrite(&audioFormat, 2, 1, file);
    fwrite(&numChannels, 2, 1, file);
    fwrite(&sampleRate, 4, 1, file);
    fwrite(&byteRate, 4, 1, file);
    fwrite(&blockAlign, 2, 1, file);
    fwrite(&bitsPerSample, 2, 1, file);
    fwrite("data", 1, 4, file);
    fwrite(&subchunk2Size, 4, 1, file);
}

// 生成不同波形的函數
double generateWaveform(int type, double t, int frequency) {
    switch (type) {
        case 0:  // 正弦波
            return sin(2.0 * PI * frequency * t);
        case 1:  // 鋸齒波
            return 2.0 * fmod(frequency * t, 1.0) - 1.0;
        case 2:  // 方波
            return (sin(2.0 * PI * frequency * t) > 0 ? 1 : -1);
        case 3:  // 三角波
            return 2.0 * fabs(2.0 * fmod(frequency * t, 1.0) - 1.0) - 1.0;
        default:
            return 0;
    }
}

// 生成 WAV 文件的函數
void generateWavFile(const char *fileName, int sampleRate, int frequency, double amplitude, double duration, int waveformType) {
    int numSamples = sampleRate * duration;  // 使用 double duration 計算樣本數
    FILE *file = fopen(fileName, "wb");

    if (!file) {
        printf("Error opening file: %s\n", fileName);
        return;
    }

    writeWavHeader(file, sampleRate, numSamples, 16);

    for (int i = 0; i < numSamples; i++) {
        double t = (double)i / sampleRate;
        int16_t sampleValue = (int16_t)(amplitude * generateWaveform(waveformType, t, frequency));
        fwrite(&sampleValue, sizeof(int16_t), 1, file);
    }

    fclose(file);
}

// 主函數
int main() {
    const int sampleRates[2] = {8000, 16000};  // 支援 8kHz 和 16kHz
    const int frequencies[10] = {100, 2000, 1000, 500, 250, 100, 2000, 1000, 500, 250};
    const double amplitudes[10] = {0, 31.25, 500, 2000, 4000, 44, 220, 440, 1760, 3960};
    const char *waveformNames[4] = {"Sine", "Sawtooth", "Square", "Triangle"};

    FILE *fp_8k = fopen("scp1.txt", "w");
    FILE *fp_16k = fopen("scp2.txt", "w");

    if (!fp_8k || !fp_16k) {
        printf("Error opening scp files.\n");
        return 1;
    }

    for (int waveformType = 0; waveformType < 4; waveformType++) {
        for (int i = 0; i < 10; i++) {
            for (int rateIdx = 0; rateIdx < 2; rateIdx++) {
                int sampleRate = sampleRates[rateIdx];
                char fileName[50];
                sprintf(fileName, "%dK_%s_%d.wav", sampleRate / 1000, waveformNames[waveformType], i);

                // 寫入對應的 scp 文件
                fprintf(rateIdx == 0 ? fp_8k : fp_16k, "%s\n", fileName);

                // 生成 WAV 文件
                generateWavFile(fileName, sampleRate, frequencies[i], amplitudes[i], 0.1, waveformType);
            }
        }
    }

    fclose(fp_8k);
    fclose(fp_16k);
    printf("All WAV files created successfully!\n");
    return 0;
}
