from pydub import AudioSegment

import wave
import contextlib
import librosa.display


def getLengthOfWavFile(filePath):   #complete path NOT relative path
    with contextlib.closing(wave.open(filePath,'r')) as f:
        frames = f.getnframes()
        rate = f.getframerate()
        duration = frames / float(rate)
        return duration

# print(getLengthOfWavFile('/home/shripad/college/audio_proj/utils/file1.wav'))
#this function uses AudioSegment library, USE TIME SLICING IN SECONDS. (Below works in msec.)
# def Split_into_Nsec_WavFiles(N_msec, sourceFile ,destinationFolder):
#     start_time, end_time = 0, N_msec
#     count = 0
#     while start_time + N_msec <= getLengthOfWavFile(sourceFile):
#         newAudioSample = AudioSegment.from_wav(sourceFile)
#         newAudioSample = newAudioSample[start_time:end_time]
#         count +=1
#         start_time = end_time
#         end_time += N_msec
#         newAudioSample.export('file' + str(count) + '.wav', format= "wav")


fname = '/home/shripad/college/audio_proj/dataset/Electric_Sawing_Sound.wav'
# Split_into_Nsec_WavFiles(850, fname, 'dataset/n_sec_samples')

# import required libraries
from pydub import AudioSegment
from pydub.playback import play

def playWavFile(fileName):
    wav_file = AudioSegment.from_file(fileName,format = "wav")
    play(wav_file)
# playWavFile('/home/shripad/college/audio_proj/utils/file2.wav')


def Split_into_Nsec_WavFiles(N_sec, sourceFile ,destinationFolder):
    start_time, end_time = 0, N_sec
    for idx in range(1,int(getLengthOfWavFile(sourceFile)/N_sec)+1):
        newAudioSample = AudioSegment.from_wav(sourceFile)
        newAudioSample = newAudioSample[start_time*1000:end_time*1000]
        start_time = end_time
        end_time += N_sec
        newAudioSample.export('file' + str(idx) + '.wav', format= "wav")

# Split_into_Nsec_WavFiles(.85, "/home/shripad/college/audio_proj/dataset/Forest_Ambient_Sound.wav", 'dataset/n_sec_samples')

import librosa
import matplotlib.pyplot as plt

def generateTimeSeriesGraph(filePath):
  x, sr = librosa.load(filePath, sr=44100)
  plt.figure(figsize=(14, 5))
  librosa.display.waveplot(x, sr=sr)

def generateLogSpectograms(filePath):
  x, sr = librosa.load(filePath, sr=44100)
  X = librosa.stft(x)
  Xdb = librosa.amplitude_to_db(abs(X))
  plt.figure(figsize=(14, 5))
  librosa.display.specshow(Xdb, sr=sr, x_axis='time', y_axis='log')
  plt.colorbar()
  return plt

from os import listdir
def exportSpectogramsImages(audioSourceFolder, destinationFolder):
  allFiles = [f for f in listdir(audioSourceFolder)]
  for i,file in enumerate(allFiles, start=1):
    fig = generateLogSpectograms(audioSourceFolder + '/' + file)
    fig.savefig(destinationFolder + '/' + "file" + str(i) + ".png")


# export spectograms to spectograms folder
# 1. Saw Sound
# exportSpectogramsImages(audioSourceFolder = "/home/shripad/college/audio_proj/dataset/TrimmedSawSound" ,destinationFolder = "/home/shripad/college/audio_proj/spectograms/SawSound")
# 2. Ambient Sound
# exportSpectogramsImages(audioSourceFolder = "/home/shripad/college/audio_proj/dataset/TrimmedAmbientSound" ,destinationFolder = "/home/shripad/college/audio_proj/spectograms/AmbientSound")

def testImportFunction():
  print("hello")

# fig = generateLogSpectograms("/home/shripad/college/audio_proj/dataset/electric_chainsaw-25291.mp3")
# fig.savefig("/home/shripad/college/audio_proj/dataset/justAnothjer.png")

