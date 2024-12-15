gcc -o sinegen sinegen.c -lm
./sinegen

gcc -o cascade cascade.c -lm
./cascade scp1.txt s-8k.wav
./cascade scp2.txt s-16k.wav

gcc -o spectrogram spectrogram.c -lm
./spectrogram 0.032 rectangular 0.032 0.01 s-8k.wav s-8k.Set1.txt
./spectrogram 0.032 hamming 0.032 0.01 s-8k.wav s-8k.Set2.txt
./spectrogram 0.03 rectangular 0.032 0.01 s-8k.wav s-8k.Set3.txt
./spectrogram 0.03 hamming 0.032 0.01 s-8k.wav s-8k.Set4.txt
./spectrogram 0.032 rectangular 0.032 0.01 s-16k.wav s-16k.Set1.txt
./spectrogram 0.032 hamming 0.032 0.01 s-16k.wav s-16k.Set2.txt
./spectrogram 0.03 rectangular 0.032 0.01 s-16k.wav s-16k.Set3.txt
./spectrogram 0.03 hamming 0.032 0.01 s-16k.wav s-16k.Set4.txt

./spectrogram 0.032 rectangular 0.032 0.01 aeueo-8kHz.wav aeueo-8kHz.Set1.txt
./spectrogram 0.032 hamming 0.032 0.01 aeueo-8kHz.wav aeueo-8kHz.Set2.txt
./spectrogram 0.03 rectangular 0.032 0.01 aeueo-8kHz.wav aeueo-8kHz.Set3.txt
./spectrogram 0.03 hamming 0.032 0.01 aeueo-8kHz.wav aeueo-8kHz.Set4.txt
./spectrogram 0.032 rectangular 0.032 0.01 aeueo-16kHz.wav aeueo-16kHz.Set1.txt
./spectrogram 0.032 hamming 0.032 0.01 aeueo-16kHz.wav aeueo-16kHz.Set2.txt
./spectrogram 0.03 rectangular 0.032 0.01 aeueo-16kHz.wav aeueo-16kHz.Set3.txt
./spectrogram 0.03 hamming 0.032 0.01 aeueo-16kHz.wav aeueo-16kHz.Set4.txt

python3 spectshow.py s-8k.wav s-8k.Set1.txt s-8k.Set1.pdf
python3 spectshow.py s-8k.wav s-8k.Set2.txt s-8k.Set2.pdf
python3 spectshow.py s-8k.wav s-8k.Set3.txt s-8k.Set3.pdf
python3 spectshow.py s-8k.wav s-8k.Set4.txt s-8k.Set4.pdf
python3 spectshow.py s-16k.wav s-16k.Set1.txt s-16k.Set1.pdf
python3 spectshow.py s-16k.wav s-16k.Set2.txt s-16k.Set2.pdf
python3 spectshow.py s-16k.wav s-16k.Set3.txt s-16k.Set3.pdf
python3 spectshow.py s-16k.wav s-16k.Set4.txt s-16k.Set4.pdf
python3 spectshow.py aeueo-8kHz.wav aeueo-8kHz.Set1.txt aeueo-8kHz.Set1.pdf
python3 spectshow.py aeueo-8kHz.wav aeueo-8kHz.Set2.txt aeueo-8kHz.Set2.pdf
python3 spectshow.py aeueo-8kHz.wav aeueo-8kHz.Set3.txt aeueo-8kHz.Set3.pdf
python3 spectshow.py aeueo-8kHz.wav aeueo-8kHz.Set4.txt aeueo-8kHz.Set4.pdf
python3 spectshow.py aeueo-16kHz.wav aeueo-16kHz.Set1.txt aeueo-16kHz.Set1.pdf
python3 spectshow.py aeueo-16kHz.wav aeueo-16kHz.Set2.txt aeueo-16kHz.Set2.pdf
python3 spectshow.py aeueo-16kHz.wav aeueo-16kHz.Set3.txt aeueo-16kHz.Set3.pdf
python3 spectshow.py aeueo-16kHz.wav aeueo-16kHz.Set4.txt aeueo-16kHz.Set4.pdf

