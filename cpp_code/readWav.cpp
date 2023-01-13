#include <iostream>
#include <fstream>
#include <vector>
#include <string>


// Structure to store WAV file header information
struct WAVHeader {
    char chunkId[4];
    int chunkSize;
    char format[4];
    char subchunk1Id[4];
    int subchunk1Size;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bitsPerSample;
    char subchunk2Id[4];
    int subchunk2Size;
};

// Function to read a WAV file and return a vector of samples
std::vector<std::vector<double>> readWAVFile(const std::string& fileName) {
    // Open the WAV file
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << fileName << std::endl;
        return {};
    }

    // Read the WAV file header
    WAVHeader header;
    file.read((char*)&header, sizeof(WAVHeader));

    // Check that the file is a valid WAV file
    if (std::string(header.chunkId, 4) != "RIFF" || std::string(header.format, 4) != "WAVE" ||
        std::string(header.subchunk1Id, 4) != "fmt " || std::string(header.subchunk2Id, 4) != "data") {
        std::cout << "Invalid WAV file: " << fileName << std::endl;
        return {};
    }

    // Extract the number of channels, bits per sample, and number of samples from the header
    int numChannels = header.numChannels;
    int bitsPerSample = header.bitsPerSample;
    int numSamples = header.subchunk2Size * 8 / bitsPerSample;
    int numSamplesPerChannel = numSamples / numChannels;

    // Read the audio data from the file
    std::vector<std::vector<double>> samples(numChannels);
    for (int i = 0; i < numSamplesPerChannel; i++) {
        for (int channel = 0; channel < numChannels; channel++) {
            if (bitsPerSample == 8) {
                // 8-bit samples are unsigned
                unsigned char sample;
                file.read((char*)&sample, 1);
                samples[channel].push_back((double)sample - 128.0);
            } else if (bitsPerSample == 16) {
                // 16-bit samples are signed
                short sample;
                file.read((char*)&sample, 2);
                samples[channel].push_back((double)sample);
            } else {
                std::cout << "Unsupported bits per sample: " << bitsPerSample << std::endl;
                return {};
            }
        }
    }
    return samples;
}

int main() {
    // Example usage of the readWAVFile() function
    std::string fileName = "/home/shripad/college/audio_proj/dataset/FalseTest.wav";
    std::vector<std::vector<double>> samples = readWAVFile(fileName);

    if (!samples.empty()) {
        for (int channel = 0; channel < samples.size(); channel++) {
            std::cout << "Channel " << channel << " samples:" << std::endl;
            for (int i = 0; i < samples[channel].size(); i++) {
                std::cout << samples[channel][i] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}