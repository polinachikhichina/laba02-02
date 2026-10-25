#include "TextProcessor.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

TextProcessor::TextProcessor() : fileName("input.txt") {
}

TextProcessor::TextProcessor(const std::string& name) : fileName(name) {
    if (fileName.empty())
        throw std::invalid_argument("Имя файла не может быть пустым.");
}

TextProcessor::TextProcessor(const TextProcessor& other) : fileName(other.fileName) {
}

TextProcessor::~TextProcessor() {
}

void TextProcessor::setFileName(const std::string& name) {
    if (name.empty())
        throw std::invalid_argument("Имя файла не может быть пустым.");
    fileName = name;
}

const std::string& TextProcessor::getFileName() const { return fileName; }

void TextProcessor::process() const {
    std::ifstream file(fileName);
    if (!file.is_open())
        throw std::runtime_error("Не удалось открыть файл: " + fileName);

    std::ostringstream wholeText;
    wholeText << file.rdbuf();

    std::istringstream textStream(wholeText.str());
    std::string questions;
    std::string exclamations;
    std::string current;
    char ch;

    while (textStream.get(ch)) {
        current += ch;
        if (ch == '.' || ch == '?' || ch == '!') {
            std::string sentence = current;
            while (!sentence.empty() && (sentence.front() == ' ' || sentence.front() == '\n' ||
                                         sentence.front() == '\r' || sentence.front() == '\t')) {
                sentence.erase(sentence.begin());
            }
            while (!sentence.empty() && (sentence.back() == ' ' || sentence.back() == '\n' ||
                                         sentence.back() == '\r' || sentence.back() == '\t')) {
                sentence.pop_back();
            }

            if (ch == '?') questions += sentence + "\n";
            if (ch == '!') exclamations += sentence + "\n";
            current.clear();
        }
    }

    std::cout << "\n========== ВОПРОСИТЕЛЬНЫЕ ПРЕДЛОЖЕНИЯ ==========\n";
    if (questions.empty()) std::cout << "Вопросительных предложений нет.\n";
    else std::cout << questions;

    std::cout << "\n========== ВОСКЛИЦАТЕЛЬНЫЕ ПРЕДЛОЖЕНИЯ ==========\n";
    if (exclamations.empty()) std::cout << "Восклицательных предложений нет.\n";
    else std::cout << exclamations;
}
