#include "ParseDataWav.hpp"

#include <algorithm>
#include <fstream>
#include <string>
#include <filesystem>

void ParseDataWav::parse(const std::filesystem::path &wav_path) {
    std::ifstream wav_file(wav_path);

    wav_file.read(reinterpret_cast<char*>(&header), sizeof(header));
    char* audioData = new char[header.subchunk2Size];
    wav_file.read(audioData, header.subchunk2Size);

    amountOfChannels = header.numChannels;
    amountOfSamples = header.subchunk2Size / (amountOfChannels * header.bitsPerSample / 8);
    Hz = header.sampleRate;
    startTime = "01-01-2000 00:00:00.000";
    stopTime = "01-01-2000 00:00:00.000";
    totalSeconds = amountOfSamples / header.sampleRate;
    setDuration(totalSeconds);

    channels_names.resize(amountOfChannels);
    for (uint32_t i = 0; i < amountOfChannels; ++i) {
        channels_names[i] = QString::number(i + 1);
    }

    channels.resize(amountOfChannels);
    for (int i = 0; i < amountOfChannels; ++i)
        channels[i].resize(amountOfSamples);

    extremums.resize(amountOfChannels);
    for (uint32_t i = 0; i < amountOfSamples; ++i) {
        for (uint16_t j = 0; j < amountOfChannels; ++j) {
            uint64_t index = i * amountOfChannels + j;
            uint32_t offset = index * (header.bitsPerSample / 8);
            if (header.bitsPerSample == 16) {
                int16_t sampleValue = *reinterpret_cast<int16_t*>(&audioData[offset]);
                if (i == 0) {
                    extremums[j].first = extremums[j].second = sampleValue;
                }
                else {
                    extremums[j].first = std::min(extremums[j].first, static_cast<double>(sampleValue));
                    extremums[j].second = std::max(extremums[j].second, static_cast<double>(sampleValue));
                }
                channels[j][i] = static_cast<double>(sampleValue);
            }
            else if (header.bitsPerSample == 24) {
                int32_t sampleValue = (*reinterpret_cast<int32_t*>(&audioData[offset])) >> 8;
                if (i == 0) {
                    extremums[j].first = extremums[j].second = sampleValue;
                }
                else {
                    extremums[j].first = std::min(extremums[j].first, static_cast<double>(sampleValue));
                    extremums[j].second = std::max(extremums[j].second, static_cast<double>(sampleValue));
                }
                channels[j][i] = static_cast<double>(sampleValue);
            }
        }
    }
    delete[] audioData;
}
