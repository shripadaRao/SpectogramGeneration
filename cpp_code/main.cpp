#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <chrono>//exec
using namespace std::chrono;



// Function to calculate the Euclidean distance between two audio frames
double euclideanDistance(const std::vector<double>& frame1, const std::vector<double>& frame2) {
    double distance = 0;
    for (int i = 0; i < frame1.size(); i++) {
        distance += (frame1[i] - frame2[i]) * (frame1[i] - frame2[i]);
    }
    return sqrt(distance);
}

// Function to perform DTW and calculate the similarity between two audio files
double dtw(const std::vector<std::vector<double>>& audio1, const std::vector<std::vector<double>>& audio2) {
    // Initialize the DTW matrix
    std::vector<std::vector<double>> dtwMatrix(audio1.size(), std::vector<double>(audio2.size()));
    dtwMatrix[0][0] = euclideanDistance(audio1[0], audio2[0]);
    // Fill in the first column of the DTW matrix
    for (int i = 1; i < audio1.size(); i++) {
        dtwMatrix[i][0] = dtwMatrix[i-1][0] + euclideanDistance(audio1[i], audio2[0]);
    }
    // Fill in the first row of the DTW matrix
    for (int j = 1; j < audio2.size(); j++) {
        dtwMatrix[0][j] = dtwMatrix[0][j-1] + euclideanDistance(audio1[0], audio2[j]);
    }
    // Fill in the rest of the DTW matrix
    for (int i = 1; i < audio1.size(); i++) {
        for (int j = 1; j < audio2.size(); j++) {
            double minValue = std::min(std::min(dtwMatrix[i-1][j], dtwMatrix[i][j-1]), dtwMatrix[i-1][j-1]);
            dtwMatrix[i][j] = euclideanDistance(audio1[i], audio2[j]) + minValue;
        }
    }
    // Return the minimum distance between the two audio files
    return dtwMatrix[audio1.size()-1][audio2.size()-1];
}

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
    auto start = high_resolution_clock::now();
    // Example usage of the dtw() function
    // std::vector<std::vector<double>> audio1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    // std::vector<std::vector<double>> audio2 = {{20, 3, 40}, {5, 6, 7}, {8, 9, 10}};
    std::vector<std::vector<double>> audio1 = readWAVFile("/home/shripad/college/audio_proj/dataset/TrimmedSawSound/file2.wav");
    std::vector<std::vector<double>> audio2 = readWAVFile("/home/shripad/college/audio_proj/dataset/TrimmedSawSound/file3.wav");
    double similarity = dtw(audio1, audio2);
    std::cout << "Similarity: " << similarity << std::endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << duration.count() << std::endl;
    return 0;
}
