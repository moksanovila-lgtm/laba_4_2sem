#include "StatisticsController.hpp"

StatisticsController::StatisticsController(QObject* parent)
    : QObject(parent)
    , isProcessing(false) {}

StatisticsController::~StatisticsController() {}

void StatisticsController::updateFromValue(int value) {
    stats.Update(value);
    emit statsUpdated();
}

void StatisticsController::reset() {
    stats.Reset();
    emit statsUpdated();
}

int StatisticsController::getMin() const {
    try {
        return stats.GetMin();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0;
    }
}

int StatisticsController::getMax() const {
    try {
        return stats.GetMax();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0;
    }
}

double StatisticsController::getAverage() const {
    try {
        return stats.GetAverage();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0.0;
    }
}

double StatisticsController::getVariance() const {
    try {
        return stats.GetVariance();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0.0;
    }
}

double StatisticsController::getStdDeviation() const {
    try {
        return stats.GetStdDeviation();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0.0;
    }
}

double StatisticsController::getMedian() {
    try {
        return stats.GetMedian();
    } catch (const std::exception& e) {
        emit error(e.what());
        return 0.0;
    }
}

size_t StatisticsController::getCount() const {
    return stats.GetCount();
}

QString StatisticsController::getStatsAsString() const {
    if (stats.GetCount() == 0) {
        return "Нет данных";
    }
    
    try {
        return QString("Min: %1, Max: %2, Avg: %3, Median: %4, Count: %5")
            .arg(stats.GetMin())
            .arg(stats.GetMax())
            .arg(stats.GetAverage(), 0, 'f', 2)
            .arg(const_cast<StatisticsController*>(this)->getMedian(), 0, 'f', 2)
            .arg(stats.GetCount());
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return "Ошибка получения статистики";
    }
}