import librosa
import numpy as np
y, sr = librosa.load(librosa.util.example_audio_file())
D = librosa.stft(y)[:512].T
print(D.shape)
f = open('stft.txt', 'w')
for frame in D:
	for ch in frame:
		f.write('%f%+fi '% (ch.real,ch.imag))
	f.write('\n')
f.close()