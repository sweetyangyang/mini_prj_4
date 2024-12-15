import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
import sys

def main(in_wav, in_txt, out_pdf):
    # 讀取 WAV 文件
    sample_rate, waveform = wavfile.read(in_wav)
    if waveform.ndim > 1:  # 如果是多聲道音頻，轉為單聲道
        waveform = np.mean(waveform, axis=1)
    
    # 計算時間軸
    time = np.arange(0, len(waveform)) / sample_rate

    # 讀取頻譜數據
    spectrogram_data = np.loadtxt(in_txt)  # 頻譜數據，每行是時間，每列是頻率 bin
    num_freq_bins = spectrogram_data.shape[1]  # 頻率 bin 的數量
    
    # 計算頻率軸
    freq_axis = np.linspace(0, sample_rate / 2, num_freq_bins)  # Nyquist 頻率範圍

    # 計算 dB 值
    array_dB = 10 * np.log10(spectrogram_data + 1e-10)

    # 繪圖
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=False)

    # 1. 繪製波形圖
    ax1.plot(time, waveform, lw=0.2, color='blue')
    ax1.set_title('Waveform')
    ax1.set_xlabel('Time (seconds)')
    ax1.set_ylabel('Amplitude')
    ax1.grid(True)

    # 2. 繪製頻譜圖，Y 軸為實際頻率
    time_axis = np.linspace(0, len(waveform) / sample_rate, spectrogram_data.shape[0])
    c = ax2.pcolormesh(time_axis, freq_axis, array_dB.T, cmap='viridis', shading='auto')
    ax2.set_title('Spectrogram')
    ax2.set_xlabel('Time (seconds)')
    ax2.set_ylabel('Frequency (Hz)')
    fig.colorbar(c, ax=ax2, label='Power (dB)')

    # 保存 PDF 文件
    plt.tight_layout()
    plt.savefig(out_pdf, format='pdf')
    plt.close()

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python3 spectshow.py <input_wav> <input_txt> <output_pdf>")
        sys.exit(1)

    in_wav = sys.argv[1]
    in_txt = sys.argv[2]
    out_pdf = sys.argv[3]

    main(in_wav, in_txt, out_pdf)
