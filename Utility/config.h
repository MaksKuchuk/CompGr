#ifndef CONFIG_H
#define CONFIG_H


class Config
{
public:
    Config() = delete;

    // cfg
    static inline bool isDarkTheme = true;
    static inline bool waveletFill = false;
    static inline bool multipleBias = true;
};

#endif // CONFIG_H
