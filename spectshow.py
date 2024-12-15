import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
import sys

def main(in_wav, in_txt, out_pdf):
    # 讀取 WAV 文件
    sample_rate, waveform = wavfile.read(in_wav)
    if waveform.ndim > 1:  # 多聲道轉單聲道
        waveform = np.mean(waveform, axis=1)
    
    # 計算時間軸
    time = np.arange(0, len(waveform)) / sample_rate

    # 讀取頻譜數據
    spectrogram_data = np.loadtxt(in_txt)
    num_freq_bins = spectrogram_data.shape[1]  # 總頻率 bin 數量

    # 計算頻率軸
    freq_axis = np.linspace(0, sample_rate / 2, num_freq_bins)  # Y 軸：頻率 (Hz)
    time_axis = np.linspace(0, len(waveform) / sample_rate, spectrogram_data.shape[0])  # X 軸：時間 (s)

    # 將頻譜數據轉換為 dB
    array_dB = 10 * np.log10(spectrogram_data + 1e-10)

    # 繪製圖表
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
    fig.patch.set_facecolor('black')  # 全圖背景設置為黑色

    # 波形圖
    ax1.plot(time, waveform, lw=0.5, color='lime')
    ax1.set_title('Waveform', color='white')
    ax1.set_xlabel('Time (seconds)', color='white')
    ax1.set_ylabel('Amplitude', color='white')
    ax1.set_facecolor('black')
    ax1.tick_params(colors='white')
    ax1.grid(True, color='gray', linestyle='--', linewidth=0.5)

    # 頻譜圖
    c = ax2.pcolormesh(time_axis, freq_axis, array_dB.T, cmap='gray', shading='auto')
    ax2.set_title('Spectrogram', color='white')
    ax2.set_xlabel('Time (seconds)', color='white')
    ax2.set_ylabel('Frequency (Hz)', color='white')
    ax2.set_facecolor('black')
    ax2.tick_params(colors='white')

    # 添加顏色條
    fig.colorbar(c, ax=ax2, label='Power (dB)', orientation='vertical')

    # 保存 PDF 文件
    plt.tight_layout()
    plt.savefig(out_pdf, format='pdf', facecolor=fig.get_facecolor())
    plt.close()

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python3 spectshow.py <input_wav> <input_txt> <output_pdf>")
        sys.exit(1)

    in_wav = sys.argv[1]
    in_txt = sys.argv[2]
    out_pdf = sys.argv[3]

    main(in_wav, in_txt, out_pdf)
