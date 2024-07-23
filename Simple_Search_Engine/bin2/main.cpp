#include "Search/search.h"
#include "Parser/parser.h"
#include <stack>
#include <chrono>

bool isOperator(const std::string& word) {
    return word == "AND" || word == "OR";
}

bool isOperand(const std::string& word) {
    return word != "AND" && word != "OR" && word != "(" && word != ")";
}

std::vector<std::string> concatenate(const std::string& input) {
    std::istringstream iss(input);
    std::string word;
    std::vector<std::string> words;

    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

bool isValid(const std::vector<std::string>& words) {
    std::string lastToken = "";
    int depth = 0;

    for (const auto& word : words) {
        if (word == "(") {
            depth++;
            if (lastToken != "" && !isOperator(lastToken) && lastToken != "(") {
                return false;
            }
        } else if (word == ")") {
            if (depth == 0 || isOperator(lastToken) || lastToken == "(") {
                return false;
            }
            depth--;
        } else if (isOperator(word)) {
            if (lastToken == "" || lastToken == "(" || isOperator(lastToken)) {
                return false;
            }
        } else if (isOperand(word)) {
            if (lastToken != "" && lastToken != "(" && !isOperator(lastToken)) {
                return false;
            }
        }
        lastToken = word;
    }

    return depth == 0 && (!lastToken.empty() && !isOperator(lastToken));
}

int getPriority(const std::string& op) {
    if (op == "AND") { return 2; }
    else if (op == "OR") { return 1; }
    return 0;
}

std::vector<std::string> infixToPostfix(const std::vector<std::string>& words) {
    std::vector<std::string> result;
    std::stack<std::string> operators;

    for (const auto& word : words) {
        if (isOperand(word)) {
            result.push_back(word);
        } else if (word == "(") {
            operators.push(word);
        } else if (word == ")") {
            while (!operators.empty() && operators.top() != "(") {
                result.push_back(operators.top());
                operators.pop();
            }
            operators.pop();
        } else if (isOperator(word)) {
            while (!operators.empty() && getPriority(operators.top()) >= getPriority(word)) {
                result.push_back(operators.top());
                operators.pop();
            }
            operators.push(word);
        }
    }

    while (!operators.empty()) {
        result.push_back(operators.top());
        operators.pop();
    }

    return result;
}

int main(int argc, char** argv) {
    std::string reader;
    Search search;

    while(true) {
        std::getline(std::cin, reader);

        if (reader == "stop") {
            break;
        }
        std::vector<std::string> words = concatenate(reader);

        if (!isValid(words)) {
            std::cerr << "Incorrect request \n";
            continue;
        }

        int allAND = 1, allOR = 2;
        for (const std::string& word : words) {
            if (words.size() == 1) {
                allAND = 0;
                allOR = 0;
            }
            if (word == "AND" && allOR == 2) {
                allOR = 0;
            }
            if (word == "OR" && allAND == 1) {
                allAND = 0;
            }
            if (!allAND && !allOR) {
                break;
            }
        }
        int checkAlls = std::max(allAND, allOR);

        std::vector<std::string> postfix = infixToPostfix(words);

        auto start = std::chrono::high_resolution_clock::now();
        search.search(postfix, checkAlls);

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    }
}
