#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>

class TextProcessor {
private:
    std::string fileName;

public:
    TextProcessor();
    explicit TextProcessor(const std::string& name);
    TextProcessor(const TextProcessor& other);
    ~TextProcessor();

    void setFileName(const std::string& name);
    const std::string& getFileName() const;

    void process() const;
};

#endif
