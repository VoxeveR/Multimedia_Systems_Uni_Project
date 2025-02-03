// podstawowe funkcje
#ifndef SM2024_KOMPRESJE_H_INCLUDED
#define SM2024_KOMPRESJE_H_INCLUDED
#include <vector>
#include <iostream>
#include <fstream>

template <class T>
struct token {
    Uint16 tokLength;
    Uint16 shift;
    T rawValue;
    token() : tokLength(0), shift(0), rawValue(0) {}
    token(Uint16 tokLength, Uint16 shift, T rawValue): tokLength(tokLength), shift(shift), rawValue(rawValue) {};
};

template <typename T>
std::vector<token<T>> LZ77Kompresja(std::vector<T> input, int length) {
    const int windowSize = 4096; // Reduced window size for better efficiency
    const int maxMatchLength = 255; // Maximum match length
    std::vector<token<T>> resultArr;
    int position = 0;

    while (position < length) {
        // Calculate proper search boundaries
        int searchStart = std::max(0, position - windowSize);
        int searchEnd = position;
        
        int matchLength = 0;
        int matchDistance = 0;

        // Find longest match
        for (int i = searchStart; i < searchEnd; i++) {
            int currentMatchLength = 0;
            // Limit match length to maxMatchLength
            while (currentMatchLength < maxMatchLength &&
                   position + currentMatchLength < length &&
                   i + currentMatchLength < position &&
                   input[i + currentMatchLength] == input[position + currentMatchLength]) {
                currentMatchLength++;
            }

            if (currentMatchLength > matchLength) {
                matchLength = currentMatchLength;
                matchDistance = position - i;
            }
        }

        if (matchLength < 3) { // Only use matches of length 3 or more
            resultArr.push_back(token<T>(0, 0, input[position]));
            position++;
        } else {
            T nextChar = (position + matchLength < length) ? input[position + matchLength] : 0;
            resultArr.push_back(token<T>(matchLength, matchDistance, nextChar));
            position += matchLength + 1;
        }
    } 

    return resultArr;
};

template <typename T>
void LZ77Dekompresja(std::vector<token<T>> &tokens, std::vector<T> &output) {
    for (const token<T> tok : tokens) {
        if (tok.tokLength == 0) {
            output.push_back(tok.rawValue);

        } else {

            if (tok.shift > output.size()) {
                throw std::runtime_error("Invalid compression data: shift larger than available data");
            }
            
            size_t startPos = output.size() - tok.shift;

            for (size_t i = 0; i < tok.tokLength; ++i) {
                output.push_back(output[startPos + (i % tok.shift)]);
            }
            
            // Dodajemy następny znak
            output.push_back(tok.rawValue);

        }
    }

};


#endif // SM2024_FUNKCJE_H_INCLUDED
