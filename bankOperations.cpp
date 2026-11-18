#include "bankOperations.h"
#include <fstream>
#include <set>
#include <unordered_map>
#include <unordered_set>

void BankOperations::add(const Transaction &t)
{
    mp.insert(std::make_pair(t.getTransactionID(), t));
    mmp.insert(std::make_pair(t.getAmount(), t));
    st.insert(t);
    mst.insert(t);
    ump.insert(std::make_pair(t.getTransactionID(), t));
    ummp.insert(std::make_pair(t.getFromID(), t));
    ust.insert(t.getTransactionID());
    umst.insert(t.getType());
}

void BankOperations::find(const std::string &id) const
{
    std::map<std::string, Transaction>::const_iterator it = mp.find(id);
    if (it != mp.end())
        it->second.print(std::cout);
    else
        std::cout << "Операция не найдена\n";
}

bool BankOperations::remove(const std::string &id)
{
    std::map<std::string, Transaction>::iterator it = mp.find(id);
    if (it == mp.end())
        return false;

    std::pair<std::multimap<double, Transaction>::iterator,
              std::multimap<double, Transaction>::iterator>
        range = mmp.equal_range(it->second.getAmount());
    for (std::multimap<double, Transaction>::iterator m = range.first; m != range.second; m++)
    {
        if (m->second.getTransactionID() == id)
        {
            mmp.erase(m);
            break;
        }
    }
    st.erase(it->second);

    std::pair<std::multiset<Transaction, ByAmount>::iterator,
              std::multiset<Transaction, ByAmount>::iterator>
        r = mst.equal_range(it->second);
    for (std::multiset<Transaction, ByAmount>::iterator s = r.first; s != r.second; s++)
    {
        if (s->getTransactionID() == id)
        {
            mst.erase(s);
            break;
        }
    }
    ump.erase(id);

    std::pair<std::unordered_multimap<std::string, Transaction>::iterator,
              std::unordered_multimap<std::string, Transaction>::iterator>
        u = ummp.equal_range(it->second.getFromID());
    for (std::unordered_multimap<std::string, Transaction>::iterator x = u.first; x != u.second; x++)
    {
        if (x->second.getTransactionID() == id)
        {
            ummp.erase(x);
            break;
        }
    }
    ust.erase(id);

    std::unordered_multiset<std::string>::iterator ty = umst.find(it->second.getType());
    if (ty != umst.end())
        umst.erase(ty);
    mp.erase(it);
    return true;
}

bool BankOperations::edit(const std::string &id, double newAmount, const std::string &newType)
{
    std::map<std::string, Transaction>::iterator it = mp.find(id);
    if (it == mp.end())
        return false;

    Transaction copy = it->second;
    copy.setAmount(newAmount);
    copy.setType(newType);
    remove(id);
    add(copy);
    return true;
}

void BankOperations::printAsc(std::ostream &out) const
{
    for (std::multimap<double, Transaction>::const_iterator it = mmp.begin(); it != mmp.end(); it++)
        it->second.print(out);
}

void BankOperations::printDesc(std::ostream &out) const
{
    for (std::multimap<double, Transaction>::const_reverse_iterator it = mmp.rbegin(); it != mmp.rend(); it++)
        it->second.print(out);
}
void BankOperations::printSet(std::ostream &out) const
{
    for (std::set<Transaction>::const_iterator it = st.begin(); it != st.end(); it++)
        it->print(out);
}

void BankOperations::printMultisetAsc(std::ostream &out) const
{
    for (std::multiset<Transaction, ByAmount>::const_iterator it = mst.begin(); it != mst.end(); it++)
        it->print(out);
}

void BankOperations::printMultisetDesc(std::ostream &out) const
{
    for (std::multiset<Transaction, ByAmount>::const_reverse_iterator it = mst.rbegin(); it != mst.rend(); it++)
        it->print(out);
}

void BankOperations::printByAmount(double amount, std::ostream &out) const
{
    Transaction key;
    key.setAmount(amount);
    std::pair<std::multiset<Transaction, ByAmount>::const_iterator,
              std::multiset<Transaction, ByAmount>::const_iterator>
        r = mst.equal_range(key);
    if (r.first == r.second)
    {
        out << "Операций с такой суммой нет\n";
        return;
    }
    for (std::multiset<Transaction, ByAmount>::const_iterator it = r.first; it != r.second; it++)
        it->print(out);
}

void BankOperations::printAmountRange(double lo, double hi, std::ostream &out) const
{
    Transaction keyLo, keyHi;
    keyLo.setAmount(lo);
    keyHi.setAmount(hi);
    std::multiset<Transaction, ByAmount>::const_iterator from = mst.lower_bound(keyLo);
    std::multiset<Transaction, ByAmount>::const_iterator to = mst.upper_bound(keyHi);
    if (from == to)
    {
        out << "В этом диапазоне операций нет\n";
        return;
    }
    for (std::multiset<Transaction, ByAmount>::const_iterator it = from; it != to; it++)
        it->print(out);
}
void BankOperations::printUnorderedMap(std::ostream &out) const
{
    for (std::unordered_map<std::string, Transaction>::const_iterator it = ump.begin(); it != ump.end(); it++)
        it->second.print(out);
}

void BankOperations::findFast(const std::string &id, std::ostream &out) const
{
    std::unordered_map<std::string, Transaction>::const_iterator it = ump.find(id);
    if (it != ump.end())
        it->second.print(out);
    else
        out << "Операция не найдена\n";
}

void BankOperations::printByAccount(const std::string &accountID, std::ostream &out) const
{
    std::pair<std::unordered_multimap<std::string, Transaction>::const_iterator,
              std::unordered_multimap<std::string, Transaction>::const_iterator>
        r = ummp.equal_range(accountID);
    if (r.first == r.second)
    {
        out << "Операций этого счёта нет\n";
        return;
    }
    out << "Количество операций: " << ummp.count(accountID) << "\n";
    for (std::unordered_multimap<std::string, Transaction>::const_iterator it = r.first; it != r.second; it++)
        it->second.print(out);
}
void BankOperations::printUnorderedSet(std::ostream &out) const
{
    for (std::unordered_set<std::string>::const_iterator it = ust.begin(); it != ust.end(); it++)
    {
        std::map<std::string, Transaction>::const_iterator m = mp.find(*it);
        if (m != mp.end())
            m->second.print(out);
    }
}

bool BankOperations::exists(const std::string &id) const
{
    return ust.find(id) != ust.end();
}

void BankOperations::printTypes(std::ostream &out) const
{
    for (std::unordered_multiset<std::string>::const_iterator it = umst.begin(); it != umst.end(); it++)
        out << *it << "\n";
}

void BankOperations::printTypeCount(const std::string &type, std::ostream &out) const
{
    out << type << ": " << umst.count(type) << "\n";
}

bool BankOperations::saveToFile(const std::string &path) const
{
    std::ofstream f(path);
    if (!f)
        return false;

    f << "=== map (по ID) ===\n";
    for (std::map<std::string, Transaction>::const_iterator it = mp.begin(); it != mp.end(); it++)
        it->second.print(f);

    f << "\n=== multimap (по возрастанию суммы) ===\n";
    printAsc(f);

    f << "\n=== set (по ID) ===\n";
    printSet(f);

    f << "\n=== multiset (по возрастанию суммы) ===\n";
    printMultisetAsc(f);

    f << "\n=== unordered_map ===\n";
    printUnorderedMap(f);

    f << "\n=== unordered_set ===\n";
    printUnorderedSet(f);

    f << "\n=== Количество операций по типам (unordered_multiset) ===\n";
    printTypes(f);
    return true;
}

void BankOperations::filterByType(const std::string &type, std::ostream &out) const
{
    int count = 0;
    for (std::map<std::string, Transaction>::const_iterator it = mp.begin(); it != mp.end(); it++)
    {
        if (it->second.getType() == type)
        {
            it->second.print(out);
            count++;
        }
    }
    if (count == 0)
        out << "Операций такого типа нет\n";
}

void BankOperations::printMap(std::ostream &out) const
{
    for (std::map<std::string, Transaction>::const_iterator it = mp.begin(); it != mp.end(); it++)
        it->second.print(out);
}

void BankOperations::printUnorderedMultimap(std::ostream &out) const
{
    for (std::unordered_multimap<std::string, Transaction>::const_iterator it = ummp.begin(); it != ummp.end(); it++)
        it->second.print(out);
}