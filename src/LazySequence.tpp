#include "LazySequence.hpp"
#include "Optional.hpp"
#include <stdexcept>
#include <iostream>

template <typename T>
LazySequence<T>::DefaultGenerator::DefaultGenerator(
    LazySequence<T>* owner,
    std::function<T(Sequence<T>*)> rule,
    size_t arity)
    : owner(owner), rule(rule), arity(arity) {}

// template <typename T>
// T LazySequence<T>::DefaultGenerator::GetNext() {
//     auto* prev = new ArraySequence<T>();
//     size_t count = owner->materialized->GetCount();
//     size_t start = count > arity ? count - arity : 0;
    
//     for (size_t i = start; i < count; ++i) {
//         prev->Append(owner->materialized->Get(i));
//     }
    
//     T result = rule(prev);
//     delete prev;
//     return result;
// }

// template <typename T>
// T LazySequence<T>::DefaultGenerator::GetNext() {
//     if (arity == 0) {
//         return rule(nullptr);
//     }
    
//     auto* prev = new ArraySequence<T>();
//     size_t count = owner->materialized->GetCount();
//     size_t start = count > arity ? count - arity : 0;
    
//     for (size_t i = start; i < count; ++i) {
//         prev->Append(owner->materialized->Get(i));
//     }
    
//     T result = rule(prev);
//     delete prev;
//     return result;
// }

// template <typename T>
// T LazySequence<T>::DefaultGenerator::GetNext() {
//     static int callCount = 0;
//     callCount++;
    
//     if (callCount % 10000 == 0) {
//         std::cout << "GetNext called " << callCount << " times, arity=" << arity << std::endl;
//     }
    
//     if (arity == 0) {
//         if (callCount % 10000 == 0) {
//             std::cout << "  arity==0, calling rule(nullptr)" << std::endl;
//         }
//         return rule(nullptr);
//     }
    
//     if (callCount % 10000 == 0) {
//         std::cout << "  arity>0, creating prev array..." << std::endl;
//     }
    
//     auto* prev = new ArraySequence<T>();
//     size_t count = owner->materialized->GetCount();
//     size_t start = count > arity ? count - arity : 0;
    
//     if (callCount % 10000 == 0) {
//         std::cout << "    count=" << count << ", start=" << start << ", arity=" << arity << std::endl;
//     }
    
//     for (size_t i = start; i < count; ++i) {
//         prev->Append(owner->materialized->Get(i));
//     }
    
//     T result = rule(prev);
//     delete prev;
    
//     if (callCount % 10000 == 0) {
//         std::cout << "    result=" << result << std::endl;
//     }
    
//     return result;
// }

template <typename T>
T LazySequence<T>::DefaultGenerator::GetNext() {
    static int callCount = 0;
    callCount++;
    
    auto start_total = std::chrono::high_resolution_clock::now();
    
    if (arity == 0) {
        auto start_rule = std::chrono::high_resolution_clock::now();
        T result = rule(nullptr);
        auto end_rule = std::chrono::high_resolution_clock::now();
        auto rule_us = std::chrono::duration_cast<std::chrono::microseconds>(end_rule - start_rule).count();
        
        if (callCount % 10000 == 0) {
            std::cout << "GetNext #" << callCount << " [arity=0] rule took " << rule_us << " us, result=" << result << std::endl;
        }
        return result;
    }
    
    // Для рекуррентных генераторов (arity > 0)
    auto start_prev = std::chrono::high_resolution_clock::now();
    
    auto* prev = new ArraySequence<T>();
    size_t count = owner->materialized->GetCount();
    size_t start = count > arity ? count - arity : 0;
    
    for (size_t i = start; i < count; ++i) {
        prev->Append(owner->materialized->Get(i));
    }
    
    auto end_prev = std::chrono::high_resolution_clock::now();
    auto prev_us = std::chrono::duration_cast<std::chrono::microseconds>(end_prev - start_prev).count();
    
    auto start_rule = std::chrono::high_resolution_clock::now();
    T result = rule(prev);
    auto end_rule = std::chrono::high_resolution_clock::now();
    auto rule_us = std::chrono::duration_cast<std::chrono::microseconds>(end_rule - start_rule).count();
    
    delete prev;
    
    auto end_total = std::chrono::high_resolution_clock::now();
    auto total_us = std::chrono::duration_cast<std::chrono::microseconds>(end_total - start_total).count();
    
    if (callCount % 10000 == 0) {
        std::cout << "GetNext #" << callCount << " [arity=" << arity << "] prev=" << prev_us << " us, rule=" << rule_us << " us, total=" << total_us << " us" << std::endl;
    }
    
    return result;
}



























template <typename T>
bool LazySequence<T>::DefaultGenerator::HasNext() const {
    return true;
}

template <typename T>
Optional<T> LazySequence<T>::DefaultGenerator::TryGetNext() {
    try { 
        return Optional<T>(GetNext()); 
    } catch (...) { 
        return Optional<T>(); 
    }
}

template <typename T>
LazySequence<T>::SkipGenerator::SkipGenerator(
    LazySequence<T>* owner,
    size_t startSkip,
    size_t endSkip,
    LazySequence<T>* parentSeq)
    : owner(owner), startSkip(startSkip), endSkip(endSkip), 
      parentSeq(parentSeq), currentPosition(0) {}

template <typename T>
T LazySequence<T>::SkipGenerator::GetNext() {
    while (currentPosition < startSkip) {
        parentSeq->Get(currentPosition);
        currentPosition++;
    }
    
    if (currentPosition >= endSkip) {
        throw IndexOutOfRangeException("No more elements");
    }
    
    T result = parentSeq->Get(currentPosition);
    currentPosition++;
    return result;
}

template <typename T>
bool LazySequence<T>::SkipGenerator::HasNext() const {
    return currentPosition < endSkip;
}

template <typename T>
Optional<T> LazySequence<T>::SkipGenerator::TryGetNext() {
    try { 
        return Optional<T>(GetNext()); 
    } catch (...) { 
        return Optional<T>(); 
    }
}

template <typename T>
LazySequence<T>::MapGenerator::MapGenerator(
    std::unique_ptr<IGenerator> src,
    std::function<T(const T&)> f)
    : source(std::move(src)), func(f) {}

template <typename T>
T LazySequence<T>::MapGenerator::GetNext() {
    return func(source->GetNext());
}

template <typename T>
bool LazySequence<T>::MapGenerator::HasNext() const {
    return source->HasNext();
}

template <typename T>
Optional<T> LazySequence<T>::MapGenerator::TryGetNext() {
    try { 
        return Optional<T>(GetNext()); 
    } catch (...) { 
        return Optional<T>(); 
    }
}

template <typename T>
void LazySequence<T>::WhereGenerator::FindNext() {
    while (source && source->HasNext() && !nextValue.HasValue()) {
        T val = source->GetNext();
        if (predicate(val)) {
            nextValue.SetValue(val);
        }
    }
}

template <typename T>
LazySequence<T>::WhereGenerator::WhereGenerator(
    std::unique_ptr<IGenerator> src,
    std::function<bool(const T&)> pred)
    : source(std::move(src)), predicate(pred) {}

template <typename T>
T LazySequence<T>::WhereGenerator::GetNext() {
    FindNext();
    if (!nextValue.HasValue()) {
        throw IndexOutOfRangeException("No more elements");
    }
    T result = nextValue.GetValue();
    nextValue.Reset();
    return result;
}

template <typename T>
bool LazySequence<T>::WhereGenerator::HasNext() const {
    const_cast<WhereGenerator*>(this)->FindNext();
    return nextValue.HasValue();
}

template <typename T>
Optional<T> LazySequence<T>::WhereGenerator::TryGetNext() {
    try { 
        return Optional<T>(GetNext()); 
    } catch (...) { 
        return Optional<T>(); 
    }
}

template <typename T>
LazySequence<T>::ConcatGenerator::ConcatGenerator(
    std::unique_ptr<IGenerator> first,
    Sequence<T>* second,
    bool firstInfinite)
    : firstGen(std::move(first))
    , secondSeq(std::make_unique<ArraySequence<T>>())
    , secondIndex(0)
    , firstIsInfinite(firstInfinite)
    , firstFinished(false) {
    
    for (size_t i = 0; i < second->GetCount(); ++i) {
        secondSeq->Append(second->Get(i));
    }
}

template <typename T>
T LazySequence<T>::ConcatGenerator::GetNext() {
    if (!firstFinished) {
        try {
            if (firstGen && firstGen->HasNext()) {
                return firstGen->GetNext();
            }
            firstFinished = true;
        } catch (const IndexOutOfRangeException&) {
            firstFinished = true;
        }
    }
    
    if (firstIsInfinite) {
        throw IndexOutOfRangeException("Infinite sequence never ends");
    }
    
    if (secondIndex < secondSeq->GetCount()) {
        return secondSeq->Get(secondIndex++);
    }
    
    throw IndexOutOfRangeException("No more elements");
}















template <typename T>
bool LazySequence<T>::ConcatGenerator::HasNext() const {
    if (!firstFinished && firstGen && firstGen->HasNext()) {
        return true;
    }
    if (firstIsInfinite) {
        return true;
    }
    return secondIndex < secondSeq->GetCount();
}

template <typename T>
Optional<T> LazySequence<T>::ConcatGenerator::TryGetNext() {
    try {
        return Optional<T>(GetNext());
    } catch (...) {
        return Optional<T>();
    }
}

template <typename T>
IEnumerator<T>* LazySequence<T>::GetEnumerator() const {
    class LazyEnumerator : public IEnumerator<T> {
    private:
        const LazySequence* seq;
        size_t currentIndex;
        mutable T currentValue;
        
    public:
        LazyEnumerator(const LazySequence* sequence) 
            : seq(sequence), currentIndex(0) {}
        
        bool MoveNext() override {
            if (seq->IsInfinite()) {
                currentValue = seq->Get(currentIndex);
                currentIndex++;
                return true;
            }
            if (currentIndex < seq->GetCount()) {
                currentValue = seq->Get(currentIndex);
                currentIndex++;
                return true;
            }
            return false;
        }
        T& Current() override { return currentValue; }
        const T& Current() const override { return currentValue; }
        void Reset() override { currentIndex = 0; }
    };
    return new LazyEnumerator(this);
}

template <typename T>
LazySequence<T>::LazySequence()
    : materialized(std::make_unique<ArraySequence<T>>())
    , generator(nullptr)
    , isFinite(true)
    , finiteSize(0)
    , materializedCount(0) {}

template <typename T>
LazySequence<T>::LazySequence(const T* items, int count)
    : materialized(std::make_unique<ArraySequence<T>>())
    , generator(nullptr)
    , isFinite(true)
    , finiteSize(count)
    , materializedCount(0) {
    for (int i = 0; i < count; ++i) {
        materialized->Append(items[i]);
    }
}

template <typename T>
LazySequence<T>::LazySequence(Sequence<T>* seq)
    : materialized(std::make_unique<ArraySequence<T>>())
    , generator(nullptr)
    , isFinite(true)
    , finiteSize(seq->GetCount())
    , materializedCount(0) {
    for (size_t i = 0; i < seq->GetCount(); ++i) {
        materialized->Append(seq->Get(i));
    }
}

template <typename T>
LazySequence<T>::LazySequence(std::function<T(Sequence<T>*)> rule, 
                               Sequence<T>* firstElements, 
                               size_t arity)
    : materialized(std::make_unique<ArraySequence<T>>())
    , generator(nullptr)
    , isFinite(false)
    , finiteSize(0)
    , materializedCount(0) {
    for (size_t i = 0; i < firstElements->GetCount(); ++i) {
        materialized->Append(firstElements->Get(i));
    }
    generator = std::make_unique<DefaultGenerator>(this, rule, arity);
}

template <typename T>
LazySequence<T>::LazySequence(const LazySequence& other)
    : materialized(std::make_unique<ArraySequence<T>>())
    , generator(nullptr)
    , isFinite(other.isFinite)
    , finiteSize(other.finiteSize)
    , materializedCount(other.materializedCount) {
    for (size_t i = 0; i < other.materialized->GetCount(); ++i) {
        materialized->Append(other.materialized->Get(i));
    }
}

// template <typename T>
// void LazySequence<T>::Materialize(size_t index) {
//     while (materialized->GetCount() <= index && generator && generator->HasNext()) {
//         materialized->Append(generator->GetNext());
//         materializedCount++;
//     }
// }

template <typename T>
void LazySequence<T>::Materialize(size_t index) {
    static long long totalTime = 0;
    static int callCount = 0;
    auto start = std::chrono::high_resolution_clock::now();
    
    while (materialized->GetCount() <= index && generator && generator->HasNext()) {
        materialized->Append(generator->GetNext());
        materializedCount++;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    totalTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    callCount++;
    
    if (callCount % 10000 == 0) {
        std::cout << "Materialize called " << callCount << " times, total time: " << totalTime << " us" << std::endl;
    }
}


















// template <typename T>
// T LazySequence<T>::Get(size_t index) const {
//     if (isFinite && index >= finiteSize) {
//         throw IndexOutOfRangeException("Index out of range");
//     }
//     const_cast<LazySequence*>(this)->Materialize(index);
//     if (index >= materialized->GetCount()) {
//         throw IndexOutOfRangeException("Index out of range");
//     }
//     return materialized->Get(index);
// }

template <typename T>
T LazySequence<T>::Get(size_t index) const {
    if (isFinite && index >= finiteSize) {
        throw IndexOutOfRangeException("Index out of range");
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    const_cast<LazySequence*>(this)->Materialize(index);
    auto end = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    static int callCount = 0;
    callCount++;
    if (callCount % 10000 == 0) {
        std::cout << "Get(" << index << ") Materialize took " << us << " us" << std::endl;
    }
    
    if (index >= materialized->GetCount()) {
        throw IndexOutOfRangeException("Index out of range");
    }
    return materialized->Get(index);
}

template <typename T>
size_t LazySequence<T>::GetCount() const {
    if (isFinite) return finiteSize;
    return 0;
}

template <typename T>
T LazySequence<T>::GetFirst() const {
    return Get(0);
}

template <typename T>
T LazySequence<T>::GetLast() const {
    if (isFinite && finiteSize > 0) {
        return Get(finiteSize - 1);
    }
    throw EmptySequenceException("Cannot get last element of infinite sequence");
}

template <typename T>
LazySequence<T>* LazySequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (startIndex > endIndex) {
        throw IndexOutOfRangeException("startIndex > endIndex");
    }
    auto* result = new LazySequence<T>();
    for (size_t i = startIndex; i < endIndex && i < materialized->GetCount(); ++i) {
        result->materialized->Append(materialized->Get(i));
    }
    if (generator && endIndex > materialized->GetCount()) {
        result->generator = std::make_unique<SkipGenerator>(
            const_cast<LazySequence*>(this), startIndex, endIndex, const_cast<LazySequence*>(this));
        result->isFinite = true;
        result->finiteSize = endIndex - startIndex;
    }
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Append(const T& item) {
    auto* result = new LazySequence<T>();
    for (size_t i = 0; i < materialized->GetCount(); ++i) {
        result->materialized->Append(materialized->Get(i));
    }
    result->materialized->Append(item);
    result->isFinite = isFinite;
    result->finiteSize = isFinite ? finiteSize + 1 : 0;
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Prepend(const T& item) {
    auto* result = new LazySequence<T>();
    result->materialized->Append(item);
    for (size_t i = 0; i < materialized->GetCount(); ++i) {
        result->materialized->Append(materialized->Get(i));
    }
    result->isFinite = isFinite;
    result->finiteSize = isFinite ? finiteSize + 1 : 0;
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::InsertAt(const T& item, size_t index) {
    auto* result = new LazySequence<T>();
    for (size_t i = 0; i < index && i < materialized->GetCount(); ++i) {
        result->materialized->Append(materialized->Get(i));
    }
    result->materialized->Append(item);
    for (size_t i = index; i < materialized->GetCount(); ++i) {
        result->materialized->Append(materialized->Get(i));
    }
    result->isFinite = isFinite;
    result->finiteSize = isFinite ? finiteSize + 1 : 0;
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Concat(LazySequence<T>* list) const {
    auto* result = new LazySequence<T>();
    for (size_t i = 0; i < materialized->GetCount(); ++i) {
        result->materialized->Append(materialized->Get(i));
    }
    for (size_t i = 0; i < list->materialized->GetCount(); ++i) {
        result->materialized->Append(list->materialized->Get(i));
    }
    result->isFinite = isFinite && list->isFinite;
    result->finiteSize = (isFinite ? finiteSize : 0) + (list->isFinite ? list->finiteSize : 0);
    return result;
}

template <typename T>
Sequence<T>* LazySequence<T>::Concat(Sequence<T>* other) const {
    LazySequence<T>* result = new LazySequence<T>();
    for (size_t i = 0; i < materializedCount; ++i) {
        result->materialized->Append(materialized->Get(i));
    }
    result->generator = std::make_unique<ConcatGenerator>(
        generator ? std::unique_ptr<IGenerator>(generator.get()) : nullptr,
        other,
        IsInfinite()
    );
    result->isFinite = !IsInfinite();  
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Map(std::function<T(const T&)> func) const {
    auto* result = new LazySequence<T>();
    for (size_t i = 0; i < materialized->GetCount(); ++i) {
        result->materialized->Append(func(materialized->Get(i)));
    }
    if (generator) {
        result->generator = std::make_unique<MapGenerator>(
            std::unique_ptr<IGenerator>(generator.get()), func);
        result->isFinite = isFinite;
        result->finiteSize = finiteSize;
    } else {
        result->isFinite = isFinite;
        result->finiteSize = finiteSize;
    }
    return result;
}

template <typename T>
T LazySequence<T>::Reduce(std::function<T(const T&, const T&)> func, const T& initial) const {
    if (generator && !isFinite) {
        throw std::logic_error("Cannot reduce infinite sequence");
    }
    T result = initial;
    for (size_t i = 0; i < materialized->GetCount(); ++i) {
        result = func(result, materialized->Get(i));
    }
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Where(std::function<bool(const T&)> predicate) const {
    auto* result = new LazySequence<T>();
    for (size_t i = 0; i < materialized->GetCount(); ++i) {
        if (predicate(materialized->Get(i))) {
            result->materialized->Append(materialized->Get(i));
        }
    }
    if (generator) {
        result->generator = std::make_unique<WhereGenerator>(
            std::unique_ptr<IGenerator>(generator.get()), predicate);
        result->isFinite = false;
        result->finiteSize = 0;
    } else {
        result->isFinite = true;
        result->finiteSize = result->materialized->GetCount();
    }
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Skip(size_t start, size_t end) const {
    auto* result = new LazySequence<T>();
    for (size_t i = start; i < end && i < materialized->GetCount(); ++i) {
        result->materialized->Append(materialized->Get(i));
    }
    if (generator && end > materialized->GetCount()) {
        result->generator = std::make_unique<SkipGenerator>(
            const_cast<LazySequence*>(this), start, end, const_cast<LazySequence*>(this));
        result->isFinite = true;
        result->finiteSize = end - start;
    }
    return result;
}