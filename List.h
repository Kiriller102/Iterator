#include <vector>
#include <exception>

using namespace std;

#pragma once

enum class Iterators {
    STEP,
    VALUE,
    PREDICATE
};

class ListIterator;

class List {
public:
    vector<int> vec_;
    size_t top_ = 0;
public:
    friend class ListIterator;

    explicit List(size_t aSize) {
        vec_.resize(aSize);
    }

    bool append(int a) {
        if (top_ == vec_.size())
            return false;
        vec_[top_++] = a;
        return true;
    }

    bool remove() {
        if (top_ == 0)
            return false;
        --top_;
        return true;
    }

    template<typename T>
    ListIterator *createIterator(Iterators its, T arg) const;
};

class ListIterator {
protected:
    friend class List;

    const List *pList;
    size_t current{};
public:
    explicit ListIterator(const List *aPList) : pList(aPList) {}

    virtual bool first() = 0;

    virtual bool next() = 0;

    virtual int currentItem() = 0;
};

class ListIteratorStep : public ListIterator {
private:
    int step_;
public:
    friend class List;

    explicit ListIteratorStep(const List *aPList, int aStep = 1)
            : ListIterator(aPList) {
        step_ = aStep - 1;
    }

    int GetStep() {
        return step_;
    }

    bool first() override {
        current = pList->top_ - 1;
        for (auto i = 0; current != -1 && i < step_; i++)
            --current;
        return current != -1;
    }

    bool next() override {
        if (current == -1) {
            return false;
        }
        --current;
        for (auto i = 0; current != -1 && i < step_; i++)
            --current;
        return current != -1;
    }

    int currentItem() override {
        if (current == -1)
            throw runtime_error("Can't obtain current Item!");
        return pList->vec_[current];
    }
};

class ListIteratorPredicate : public ListIterator {
private:
    void *predicate_;

public:
    template<class T>
    ListIteratorPredicate(const List *aPList, T aPredicate)
            : ListIterator(aPList) {
        predicate_ = reinterpret_cast<void *>(aPredicate);
    }

    bool first() override {
        current = pList->top_ - 1;
        auto pred = reinterpret_cast<bool (*)(int)>(predicate_);
        for (auto i = 0; current != -1 && !pred(currentItem()); i++)
            --current;
        return current != -1;
    }

    bool next() override {
        if (current == -1) {
            return false;
        }
        --current;
        auto pred = reinterpret_cast<bool (*)(int)>(predicate_);
        for (auto i = 0; current != -1 && !pred(currentItem()); i++)
            --current;
        return current != -1;
    }

    int currentItem() override {
        if (current == -1)
            throw runtime_error("Can't obtain current Item!");
        return pList->vec_[current];
    }
};

class ListIteratorValue : public ListIterator {
private:
    int value;
public:
    friend class List;

    ListIteratorValue(const List *aPList, int aValue) : ListIterator(aPList) {
        value = aValue;
    }

    bool first() override {
        current = pList->top_ - 1;
        while (current != -1 && pList->vec_[current] != value)
            --current;
        return current != -1;
    }

    bool next() override {
        if (current == -1) {
            return false;
        }
        --current;
        while (current != -1 && pList->vec_[current] != value)
            --current;
        return current != -1;
    }

    int currentItem() override {
        if (current == -1)
            throw runtime_error("Can't obtain current Item!");
        return pList->vec_[current];
    }
};

template<typename T>
ListIterator *List::createIterator(Iterators its, T arg) const {
    switch (its) {
        case Iterators::STEP :
            return new ListIteratorStep(this, arg);
        case Iterators::VALUE :
            return new ListIteratorValue(this, arg);
        case Iterators::PREDICATE :
            return new ListIteratorPredicate(this, arg);
        default :
            return nullptr;
    }
}
