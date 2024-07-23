#pragma once

#include "../Index/index.h"
#include <cmath>
#include <map>
#include <stack>
#include <queue>

const int MAX_RESULTS = 10;
using andAll = std::tuple<double, std::size_t, std::list<std::size_t>>; // Metrica docID listStrok

struct Comparator {
    bool operator()(const andAll& a, const andAll& b) {
        return std::get<0>(a) > std::get<0>(b);
    };
};

using prqueue = std::priority_queue<andAll, std::vector<andAll>, Comparator>;

class Search {
public:
    Search(std::filesystem::path path = "C:/Users/maksi/github-classroom/is-itmo-c-23/labwork11-Max1musLead") : index_(path) {}

    std::vector<andAll> search(const std::vector<std::string>& words, int checkAlls);
    double TFIDF(const std::size_t termId, const std::size_t docID);
    double BM25TFIDF(const std::size_t termId, const std::size_t docID);

private:
    prqueue searchAl(const std::vector<std::string>& terms);
    prqueue searchAND(const std::vector<std::string>& terms);
    prqueue searchAND(prqueue& left, prqueue& right);
    prqueue searchOR(const std::vector<std::string>& terms);
    prqueue searchOR(prqueue& left, prqueue& right);

    Index index_;
};


