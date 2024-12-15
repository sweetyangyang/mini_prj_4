#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 44  // WAV 標頭大小

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

// 寫入 WAV 文件的標頭
void writeWavHeader(FILE *file, const WavHeader *header) {
    fwrite(header, sizeof(WavHeader), 1, file);
}

// 讀取 WAV 文件的標頭
void readWavHeader(FILE *file, WavHeader *header) {
    fread(header, sizeof(WavHeader), 1, file);
}

// 合併 WAV 文件
void mergeWavFiles(const char *inputListFile, const char *outputFile) {
    FILE *output = fopen(outputFile, "wb");
    if (!output) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    FILE *listFile = fopen(inputListFile, "r");
    if (!listFile) {
        perror("Error opening input list file");
        fclose(output);
        exit(EXIT_FAILURE);
    }

    WavHeader header;
    char inputFileName[100];
    uint32_t totalDataSize = 0;  // 計算所有 WAV 文件的 data chunk 大小

    int isFirstFile = 1;  // 標記是否是第一個文件

    // 遍歷輸入文件列表
    while (fscanf(listFile, "%s", inputFileName) == 1) {
        FILE *input = fopen(inputFileName, "rb");
        if (!input) {
            fprintf(stderr, "Error opening input file: %s\n", inputFileName);
            continue;  // 略過無法打開的文件
        }

        // 讀取標頭並累加 data chunk 大小
        readWavHeader(input, &header);
        totalDataSize += header.subchunk2Size;

        // 如果是第一個文件，寫入輸出文件的標頭
        if (isFirstFile) {
            writeWavHeader(output, &header);
            isFirstFile = 0;
        }

        // 複製 data chunk 到輸出文件
        int16_t buffer;
        for (uint32_t i = 0; i < header.subchunk2Size / sizeof(int16_t); i++) {
            fread(&buffer, sizeof(int16_t), 1, input);
            fwrite(&buffer, sizeof(int16_t), 1, output);
        }

        fclose(input);
    }

    // 更新輸出文件的標頭
    fseek(output, 0, SEEK_SET);  // 返回文件開頭
    header.subchunk2Size = totalDataSize;
    header.chunkSize = totalDataSize + HEADER_SIZE - 8;
    writeWavHeader(output, &header);

    // 清理資源
    fclose(listFile);
    fclose(output);

    printf("All WAV files merged successfully into: %s\n", outputFile);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_list.txt> <output.wav>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *inputListFile = argv[1];
    const char *outputFile = argv[2];

    mergeWavFiles(inputListFile, outputFile);

    return EXIT_SUCCESS;
}
