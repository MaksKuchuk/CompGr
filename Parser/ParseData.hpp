class ParseData {
    // number of channels
    unsigned long long amountOfChannels;
    // number of samples
    unsigned long long amountOfSamples;
    // sampling frequency (Hz)
    unsigned double Hz;
    // recording start datetime (dd-mm-yyyy hh:mm:ss.sss)
    std::string startTime;
    // recording stop datetime (dd-mm-yyyy hh:mm:ss.sss)
    std::string stopTime;
    // duration
    unsigned long long days;
    unsigned long long hours;
    unsigned long long minutes;
    double seconds;
    // data
    double** channels
public:

    unsigned long long getAmountOfChannels() {
        return amountOfChannels;
    };

    unsigned long long getAmountOfSamples() {
        return amountOfSamples;
    };

    unsigned double getHz() {
        return Hz;
    }

    std::string getStartTime() {
        return startTime;
    }

    std::string getStopTime() {
        return stopTime;
    }

    std::tuple<unsigned long long, unsigned long long, unsigned long long, double> getDuration() {
        return std::tuple(days, hours, minutes, seconds);
    }

    // return n-th channel
    double* getChannel(long long n) {
        return channels[n];
    }
};