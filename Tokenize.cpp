#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>

#include <string>
#include <algorithm>

// (1 + 2) * 3 / 4 + 5 * (6 - 7)

// 1. Токенизация
// 2. Парсер (построение дерева разбора выражения)

// +, -, *, /, %

struct OpeningBracket {};

struct ClosingBracket {};

struct Number {
    int value;
};

struct UnknownToken {
    std::string value;
};

struct MinToken {};

struct AbsToken {};

struct MaxToken {};

struct SqrToken {};

struct CommaToken {};

struct Plus {};

struct Minus {};

struct Multiply {};

struct Modulo {};

struct Divide {};

using Token = std::variant<OpeningBracket, ClosingBracket, Number,
                            UnknownToken, MinToken, AbsToken,
                            Plus, Minus, Multiply, Modulo, Divide,
                            MaxToken, SqrToken, CommaToken>;

// 1234

const std::unordered_map<char, Token> kSymbol2Token{
    {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'/', Divide{}}, {'%', Modulo{}}, 
    {'(', OpeningBracket{}}, {')', ClosingBracket{}}, {',', CommaToken{}}
};

const std::unordered_map<std::string, Token> kFunction2Token{
    {"min", MinToken{}}, {"max", MaxToken{}}, {"abs", AbsToken{}}, {"sqr", SqrToken{}}
};

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size()) {
            ++pos;
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    --pos;
    return Number{ value };
}

// Copying to use transform 
Token GetFunction(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(),
        [](unsigned char c) { return std::tolower(c); } // just converts to lower case
    );
    if (auto it = kFunction2Token.find(name); it != kFunction2Token.end()) {
        return it->second;
    }
    return UnknownToken{ name };
}

Token ParseName(const std::string& input, size_t& pos) {
    std::string name = "";
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isalpha(symbol)) {
        name += symbol;
        if (pos == input.size()-1) {
            ++pos;
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]); 
    }
    --pos;
    return GetFunction(name);
}

std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;
    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);
        if (std::isspace(symbol)) {
            //++pos;
        }
        else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        }
        else if (std::isalpha(symbol)) {
            tokens.emplace_back(ParseName(input, pos));
        }
        else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            tokens.emplace_back(it->second);
        }
        else {
            tokens.emplace_back(UnknownToken{ std::string(1, symbol) });
        }
        ++pos;
    }
    return tokens;
}

