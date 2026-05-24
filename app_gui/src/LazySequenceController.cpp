#include "LazySequenceController.hpp"
#include <random>
#include <chrono>

LazySequenceController::LazySequenceController(QObject* parent)
    : QObject(parent)
    , currentType(GEN_ARITHMETIC)
    , param1(1)
    , param2(1) {
    rebuildSequence();
}

LazySequenceController::~LazySequenceController() {}

void LazySequenceController::setGenerator(GeneratorType type, int p1, int p2) {
    currentType = type;
    param1 = p1;
    param2 = p2;
    rebuildSequence();
    emit sequenceChanged();
}

void LazySequenceController::rebuildSequence() {
    ArraySequence<int> empty;
    
    switch (currentType) {
        case GEN_ARITHMETIC: {
            struct ArithmeticFunctor {
                int start;
                int step;
                mutable size_t counter = 0;
                
                int operator()(Sequence<int>* prev) const {
                    (void)prev;
                    int result = start + static_cast<int>(counter) * step;
                    counter++;
                    return result;
                }
            };
            
            ArithmeticFunctor func{param1, param2, 0};
            auto lambda = [func](Sequence<int>* prev) mutable -> int {
                return func(prev);
            };
            
            lazySeq = std::make_unique<LazySequence<int>>(lambda, &empty, 0);
            break;
        }
        case GEN_RANDOM: {
            struct RandomFunctor {
                int minVal;
                int maxVal;
                mutable std::mt19937 gen;
                mutable std::uniform_int_distribution<int> dis;
                
                RandomFunctor(int minV, int maxV) 
                    : minVal(minV), maxVal(maxV)
                    , gen(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()))
                    , dis(minV, maxV) {}
                
                int operator()(Sequence<int>* prev) const {
                    (void)prev;
                    return dis(gen);
                }
            };
            
            RandomFunctor func(param1, param2);
            auto lambda = [func](Sequence<int>* prev) mutable -> int {
                return func(prev);
            };
            
            lazySeq = std::make_unique<LazySequence<int>>(lambda, &empty, 0);
            break;
        }
        case GEN_FIBONACCI: {
            struct FibonacciFunctor {
                mutable int a = 0;
                mutable int b = 1;
                mutable int counter = 0;
                
                int operator()(Sequence<int>* prev) const {
                    (void)prev;
                    if (counter == 0) {
                        counter++;
                        return 0;
                    }
                    if (counter == 1) {
                        counter++;
                        return 1;
                    }
                    int next = a + b;
                    a = b;
                    b = next;
                    counter++;
                    return next;
                }
            };
            
            FibonacciFunctor func;
            auto lambda = [func](Sequence<int>* prev) mutable -> int {
                return func(prev);
            };
            
            ArraySequence<int> fibStart = {0, 1};
            lazySeq = std::make_unique<LazySequence<int>>(lambda, &fibStart, 2);
            break;
        }
        case GEN_CONSTANT: {
            int value = param1;
            auto lambda = [value](Sequence<int>* prev) -> int {
                (void)prev;
                return value;
            };
            lazySeq = std::make_unique<LazySequence<int>>(lambda, &empty, 0);
            break;
        }
    }
    
    stream = std::make_unique<ReadOnlyStream<int>>(lazySeq.get());
}

void LazySequenceController::generateFirstNElements(int n) {
    for (int i = 0; i < n; ++i) {
        lazySeq->Get(i);
    }
    emit materializedCountChanged(lazySeq->GetMaterializedCount());
}

int LazySequenceController::getElement(size_t index) {
    try {
        return lazySeq->Get(index);
    } catch (const std::exception& e) {
        emit error(e.what());
        return 0;
    }
}

size_t LazySequenceController::getMaterializedCount() const {
    return lazySeq ? lazySeq->GetMaterializedCount() : 0;
}

bool LazySequenceController::isInfinite() const {
    return lazySeq ? lazySeq->IsInfinite() : true;
}

QString LazySequenceController::getGeneratorDescription() const {
    switch (currentType) {
        case GEN_ARITHMETIC:
            return QString("Арифметическая: %1, %2, %3, ...")
                .arg(param1).arg(param1 + param2).arg(param1 + 2 * param2);
        case GEN_RANDOM:
            return QString("Случайные [%1..%2]").arg(param1).arg(param2);
        case GEN_FIBONACCI:
            return "Фибоначчи: 0, 1, 1, 2, 3, 5, ...";
        case GEN_CONSTANT:
            return QString("Постоянные: %1, %1, %1, ...").arg(param1);
        default:
            return "Неизвестный генератор";
    }
}