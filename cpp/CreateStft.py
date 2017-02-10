import librosa
import numpy as np
y, sr = librosa.load(librosa.util.example_audio_file())
D = librosa.stft(y)[:511]\
print(D.shape)
