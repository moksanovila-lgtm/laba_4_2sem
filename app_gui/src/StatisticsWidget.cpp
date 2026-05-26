#include "StatisticsWidget.hpp"
#include "OnlineStatistics.hpp"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

StatisticsWidget::StatisticsWidget(ReadOnlyStream<long long>* stream, int count, 
                                   const QString& title, QWidget* parent)
    : QWidget(parent)
    , sourceStream(stream)
    , elementCount(count)
    , minVal(0)
    , maxVal(0)
    , range(0.0)
    , average(0.0)
    , variance(0.0)
    , stdDeviation(0.0)
    , rms(0.0)
    , cv(0.0)
    , count(0) {
    setupUI();
    setWindowTitle(title);
    collectStatistics();
    updateDisplay();
}

StatisticsWidget::~StatisticsWidget() {}

void StatisticsWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QGroupBox* resultGroup = new QGroupBox("Результаты статистики");
    QGridLayout* resultLayout = new QGridLayout(resultGroup);
    
    resultLayout->addWidget(new QLabel("Минимум:"), 0, 0);
    minLabel = new QLabel("-");
    resultLayout->addWidget(minLabel, 0, 1);
    
    resultLayout->addWidget(new QLabel("Максимум:"), 1, 0);
    maxLabel = new QLabel("-");
    resultLayout->addWidget(maxLabel, 1, 1);
    
    resultLayout->addWidget(new QLabel("Размах:"), 2, 0);
    rangeLabel = new QLabel("-");
    resultLayout->addWidget(rangeLabel, 2, 1);
    
    resultLayout->addWidget(new QLabel("Количество:"), 3, 0);
    countLabel = new QLabel("-");
    resultLayout->addWidget(countLabel, 3, 1);
    
    resultLayout->addWidget(new QLabel("Среднее:"), 0, 2);
    avgLabel = new QLabel("-");
    resultLayout->addWidget(avgLabel, 0, 3);
    
    resultLayout->addWidget(new QLabel("Дисперсия:"), 1, 2);
    varianceLabel = new QLabel("-");
    resultLayout->addWidget(varianceLabel, 1, 3);
    
    resultLayout->addWidget(new QLabel("Ст. отклонение:"), 2, 2);
    stdDevLabel = new QLabel("-");
    resultLayout->addWidget(stdDevLabel, 2, 3);
    
    resultLayout->addWidget(new QLabel("Среднеквадратическое (RMS):"), 3, 2);
    rmsLabel = new QLabel("-");
    resultLayout->addWidget(rmsLabel, 3, 3);
    
    resultLayout->addWidget(new QLabel("Коэф. вариации (CV):"), 4, 2);
    cvLabel = new QLabel("-");
    resultLayout->addWidget(cvLabel, 4, 3);
    
    resultLayout->setColumnStretch(1, 1);
    resultLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(resultGroup);
}

void StatisticsWidget::collectStatistics() {
    if (!sourceStream) {
        return;
    }
    
    try {
        OnlineStatistics<long long> stats;
        sourceStream->Open();
        for (int i = 0; i < elementCount && !sourceStream->IsEndOfStream(); ++i) {
            stats.Update(sourceStream->Read());
        }
        sourceStream->Close();
        
        minVal = stats.GetMin();
        maxVal = stats.GetMax();
        range = stats.GetRange();
        average = stats.GetAverage();
        variance = stats.GetVariance();
        stdDeviation = stats.GetStdDeviation();
        rms = stats.GetRMS();
        cv = stats.GetCoefficientOfVariation();
        count = stats.GetCount();
        
    } catch (const std::exception& e) {
        minVal = 0;
        maxVal = 0;
        range = 0;
        average = 0;
        variance = 0;
        stdDeviation = 0;
        rms = 0;
        cv = 0;
        count = 0;
    }
}

void StatisticsWidget::updateDisplay() {
    if (count == 0) {
        minLabel->setText("-");
        maxLabel->setText("-");
        rangeLabel->setText("-");
        countLabel->setText("0");
        avgLabel->setText("-");
        varianceLabel->setText("-");
        stdDevLabel->setText("-");
        rmsLabel->setText("-");
        cvLabel->setText("-");
        return;
    }
    
    minLabel->setText(QString::number(minVal));
    maxLabel->setText(QString::number(maxVal));
    rangeLabel->setText(QString::number(range, 'f', 2));
    countLabel->setText(QString::number(count));
    avgLabel->setText(QString::number(average, 'f', 2));
    varianceLabel->setText(QString::number(variance, 'f', 2));
    stdDevLabel->setText(QString::number(stdDeviation, 'f', 2));
    rmsLabel->setText(QString::number(rms, 'f', 2));
    cvLabel->setText(QString::number(cv, 'f', 4));
}