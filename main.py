from utils import utils
from PIL import Image, ImageChops
import math
import numpy as np
import time

# export spectograms to spectograms folder
# 1. Ambient Sound
# exportSpectogramsImages(audioSourceFolder = "/home/shripad/college/audio_proj/dataset/TrimmedSawSound" ,destinationFolder = "/home/shripad/college/audio_proj/spectograms/SawSound")

# utils.testImportFunction()

"""
    "listen" to wav file. For every 1 sec of audio file, read only the first 0.85 sec sample of it. Then, apply RMSE of the generate spectograms to check correlation between ref and that img"

"""
from pydub import AudioSegment
from pydub.playback import play


def ListenAudioInputFile(AudioSourcePath, RefSpectogram):
    start_time = 0
    RMSE_THRESHOLD = 0.22
    sampling_period = 0.85 #seconds
    # end_time = start_time+sampling_period
    # print(int(utils.getLengthOfWavFile(AudioSourcePath)))
    # while start_time + 1*1000 <= int(utils.getLengthOfWavFile(AudioSourcePath)*1000):
    #     newAudioSample = AudioSegment.from_wav(AudioSourcePath)
    #     newAudioSample = newAudioSample[start_time*1000:start_time*1000 + sampling_period*1000]
        
    #     spectogram = utils.generateLogSpectograms(AudioSourcePath)
    #     # print(spectogram)
    #     # print(ComputeRMSEImages(spectogram, RefSpectogram))
    #     spectogram.savefig("/home/shripad/college/audio_proj/spectograms/Testing/spectogram.png")
    #     if ComputeRMSEImages("/home/shripad/college/audio_proj/spectograms/Testing/spectogram.png", RefSpectogram) > RMSE_THRESHOLD:
            
    #         print("ALERT!!")
    #         return True

    #     start_time += 1000

    # return False
    for i in range(int(utils.getLengthOfWavFile(AudioSourcePath))+1):
        # print(i)
        newAudioSample = AudioSegment.from_wav(AudioSourcePath)
        # play(newAudioSample)
        newAudioSample = newAudioSample[start_time*1000: (start_time+ sampling_period)*1000]
        newAudioSample.export(f'/home/shripad/college/audio_proj/spectograms/Testing/AudioFiles/AudioFile{i}.wav', format= "wav")

        spectogram = utils.generateLogSpectograms(f'/home/shripad/college/audio_proj/spectograms/Testing/AudioFiles/AudioFile{i}.wav')
        spectogram.savefig(f"/home/shripad/college/audio_proj/spectograms/Testing/spectogram{i}.png")
        # print(ComputeRMSEImages(f"/home/shripad/college/audio_proj/spectograms/Testing/spectogram{i}.png",RefSpectogram),i)
        if ComputeRMSEImages(f"/home/shripad/college/audio_proj/spectograms/Testing/spectogram{i}.png", RefSpectogram) < RMSE_THRESHOLD:    
            print("ALERT!!")
            return True

        start_time += 1

    return False


def ComputeRMSEImages(im1, im2):
    im1, im2 = Image.open(im1), Image.open(im2)
    errors = np.asarray(ImageChops.difference(im1, im2)) / 255
    return math.sqrt(np.mean(np.square(errors)))

# if ComputeRMSEImages("/home/shripad/college/audio_proj/spectograms/SawSound/file2.png", "/home/shripad/college/audio_proj/spectograms/AmbientSound/file1.png") < 0.20:    
#     print("ALERT!!")


print(ListenAudioInputFile("/home/shripad/college/audio_proj/dataset/TrueTest.wav","/home/shripad/college/audio_proj/spectograms/SawSound/file2.png"))
# print(ComputeRMSEImages("/home/shripad/college/audio_proj/spectograms/SawSound/file2.png","/home/shripad/college/audio_proj/spectograms/SawSound/file3.png"))