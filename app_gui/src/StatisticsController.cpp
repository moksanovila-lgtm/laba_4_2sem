#include "StatisticsController.hpp"

StatisticsController::StatisticsController(QObject* parent)
    : QObject(parent) {}

StatisticsController::~StatisticsController() {}

void StatisticsController::updateFromValue(long long value) {
    stats.Update(value);
    emit statsUpdated();
}

void StatisticsController::reset() {
    stats.Reset();
    emit statsUpdated();
}

long long StatisticsController::getMin() const {
    try {
        return stats.GetMin();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0;
    }
}

long long StatisticsController::getMax() const {
    try {
        return stats.GetMax();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0;
    }
}

double StatisticsController::getRange() const {
    try {
        return stats.GetRange();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0.0;
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

double StatisticsController::getRMS() const {
    try {
        return stats.GetRMS();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0.0;
    }
}

double StatisticsController::getCoefficientOfVariation() const {
    try {
        return stats.GetCoefficientOfVariation();
    } catch (const std::exception& e) {
        const_cast<StatisticsController*>(this)->emit error(e.what());
        return 0.0;
    }
}

size_t StatisticsController::getCount() const {
    return stats.GetCount();
}