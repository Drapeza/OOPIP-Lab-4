#pragma once
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <unordered_set>
#include "transaction.h"

class BankOperations
{
    std::map<std::string, Transaction> mp;
    std::multimap<double, Transaction> mmp;
    std::set<Transaction> st;
    std::multiset<Transaction, ByAmount> mst;
    std::unordered_map<std::string, Transaction> ump;
    std::unordered_multimap<std::string, Transaction> ummp;
    std::unordered_set<std::string> ust;
    std::unordered_multiset<std::string> umst;

public:
    void add(const Transaction &t);
    bool remove(const std::string &id);
    bool edit(const std::string &id, double newAmount, const std::string &newType);
    void find(const std::string &id) const;
    void printAsc(std::ostream &out) const;
    void printDesc(std::ostream &out) const;

    void printSet(std::ostream &out) const;
    void printMultisetAsc(std::ostream &out) const;
    void printMultisetDesc(std::ostream &out) const;
    void printByAmount(double amount, std::ostream &out) const;
    void printAmountRange(double lo, double hi, std::ostream &out) const;

    void printUnorderedMap(std::ostream &out) const;
    void findFast(const std::string &id, std::ostream &out) const;
    void printByAccount(const std::string &accountID, std::ostream &out) const;

    void printUnorderedSet(std::ostream &out) const;
    bool exists(const std::string &id) const;
    void printTypes(std::ostream &out) const;
    void printTypeCount(const std::string &type, std::ostream &out) const;

    bool saveToFile(const std::string &path) const;
    void filterByType(const std::string &type, std::ostream &out) const;
    void printMap(std::ostream &out) const;
    void printUnorderedMultimap(std::ostream &out) const;
};