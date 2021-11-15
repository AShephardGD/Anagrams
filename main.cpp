#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

bool prediction(const char& c1, const char& c2) {
    return c1 == ' ' && c1 == c2;
}

int main(int agrc, char* argv[])
{
    if (agrc != 2) {
        return -1;
    }
    std::ifstream file;
    file.open("dictionary.txt");
    std::map<std::string, size_t> dictionary;
    std::string readingString;
    while (std::getline(file, readingString)) {
        std::string word = readingString.substr(0, readingString.find(','));
        int value = std::stoi(readingString.substr(readingString.find(' ') + 1));
        dictionary[word] = value;
    }
    file.close();
    file.open(argv[1]);
    while (std::getline(file, readingString)) {
        if (readingString != "") {
            std::vector<std::string> sentenceWords;
            auto it = std::unique(readingString.begin(), readingString.end(), prediction);
            readingString.erase(it, readingString.end());
            if (readingString[0] == ' ') {
                readingString.erase(0, 1);
            }
            if (readingString[readingString.size() - 1] == ' ') {
                readingString.erase(readingString.size() - 1);
            }
            size_t sentenceLen = readingString.size();
            sentenceWords.push_back("");
            size_t wordsNum = 0;
            for (size_t i = 0; i < sentenceLen; ++i) {
                if (readingString[i] == ' ') {
                    sentenceWords.push_back("");
                    ++wordsNum;
                }
                else {
                    sentenceWords[wordsNum] = sentenceWords[wordsNum] + readingString[i];
                }
            }
            std::vector<std::vector<std::string>> matched(wordsNum + 1, std::vector<std::string>());
            for (size_t i = 0; i <= wordsNum; ++i) {
                for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
                    std::string dictionaryWord = (*it).first;
                    if (std::is_permutation(dictionaryWord.begin(), dictionaryWord.end(),
                                            sentenceWords[i].begin(), sentenceWords[i].end())) {
                        matched[i].push_back(dictionaryWord);
                    }
                }
                if (matched[i].size() == 0) {
                    throw std::runtime_error("There is no such word in dictionary - " + sentenceWords[i]);
                }
            }
            std::vector<size_t> indexes(wordsNum + 1, 0);
            size_t sentencesNum = 1;
            for (size_t i = 0; i < wordsNum; ++i) {
                sentencesNum *= matched[i].size();
            }
            std::multimap<size_t, std::vector<std::string>, std::greater<size_t>> resultSentences;
            for (size_t i = 0; i < sentencesNum; ++i) {
                size_t index = i, resultValue = 0;
                std::vector<std::string> resultSentence(wordsNum + 1);
                for (size_t j = wordsNum; j <= wordsNum; --j) {
                    resultSentence[j] = matched[j][index % matched[j].size()];
                    resultValue += dictionary[resultSentence[j]];
                    index /= matched[j].size();
                }
                std::sort(resultSentence.begin(), resultSentence.end());
                resultSentences.insert(std::pair<size_t, std::vector<std::string>>(resultValue, resultSentence));
            }
            for (auto it = resultSentences.begin(); it != resultSentences.end(); ++it) {
                do {
                    for (auto iter = (*it).second.begin(); iter != (*it).second.end(); ++iter) {
                        std::cout << (*iter) << ' ';
                    }
                    std::cout << std::endl;
                } while (std::next_permutation((*it).second.begin(), (*it).second.end()));
            }
        }
    }
    return 0;
}
