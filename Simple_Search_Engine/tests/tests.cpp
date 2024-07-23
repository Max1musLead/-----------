#include "Index/index.h"
#include "Search/search.h"
#include <gtest/gtest.h>
#include <bits/stdc++.h>

/// HELPS FUNCTIONS BECAUSE CMAKE LOVES ME /// 
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

/// TESTS /// 
TEST(InputTest, IsValidTest) {
    EXPECT_TRUE(isValid({"term1", "AND", "term2"}));
    EXPECT_TRUE(isValid({"(", "term1", "AND", "term2", ")"}));
    EXPECT_TRUE(isValid({"term1", "OR", "(", "term2", "AND", "term3", ")"}));

    EXPECT_FALSE(isValid({"term1", "AND", "AND", "term2"}));
    EXPECT_FALSE(isValid({"term1", "(", "term2", "AND", "term3"}));
    EXPECT_FALSE(isValid({"term1", "OR", "term2", ")"}));
}

TEST(InputTest, InfixToPostfixTest) {
    EXPECT_EQ(infixToPostfix({"term1", "AND", "term2"}), std::vector<std::string>({"term1", "term2", "AND"}));
    EXPECT_EQ(infixToPostfix({"(", "term1", "AND", "term2", ")"}), std::vector<std::string>({"term1", "term2", "AND"}));
    EXPECT_EQ(infixToPostfix({"term1", "OR", "(", "term2", "AND", "term3", ")"}), std::vector<std::string>({"term1", "term2", "term3", "AND", "OR"}));
}

TEST(InputTest, SearchTest) {
    Search search;
    Index index("C:/Users/maksi/github-classroom/is-itmo-c-23/labwork11-Max1musLead");
    std::vector<andAll> answers;

    std::vector<std::string> terms = {"Bun"};
    answers = search.search(terms, 0);
    ASSERT_EQ(index.getDocumentName(std::get<1>(answers[0])), "Kolobok.txt");
    terms = {"Cinderella"};
    answers = search.search(terms, 0);
    ASSERT_EQ(index.getDocumentName(std::get<1>(answers[0])), "Zolushka.txt");
    terms = {"Snow-white"};
    answers = search.search(terms, 0);
    ASSERT_EQ(index.getDocumentName(std::get<1>(answers[0])), "7gnomov.txt");
}

TEST(InputTest, SearchTestAND) {
    Search search;
    Index index("C:/Users/maksi/github-classroom/is-itmo-c-23/labwork11-Max1musLead");
    std::vector<andAll> answers;

    std::vector<std::string> terms = {"123", "1231", "AND", "123123", "AND"};
    answers = search.search(terms, 0);
    ASSERT_EQ(index.getDocumentName(std::get<1>(answers[0])), "gth\\1231.txt");
    terms = {"1231", "123123", "AND"};
    answers = search.search(terms, 0);
    ASSERT_EQ(index.getDocumentName(std::get<1>(answers[0])), "gth\\1231.txt");
}

TEST(InputTest, SearchTestOR) {
    Search search;
    Index index("C:/Users/maksi/github-classroom/is-itmo-c-23/labwork11-Max1musLead");
    std::vector<andAll> answers;

    std::vector<std::string> terms = {"hnfsdgui", "BuN", "OR"};
    answers = search.search(terms, 0);

    ASSERT_EQ(index.getDocumentName(std::get<1>(answers[0])), "gth\\456.txt");
    ASSERT_EQ(index.getDocumentName(std::get<1>(answers[1])), "Kolobok.txt");

    terms = {"hnfsdgui", "BuN", "OR", "sdfgfgds", "OR"};
    answers = search.search(terms, 0);
    ASSERT_EQ(index.getDocumentName(std::get<1>(answers[0])), "gth\\456.txt");
    ASSERT_EQ(index.getDocumentName(std::get<1>(answers[1])), "Kolobok.txt");
}