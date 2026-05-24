#include "StatisticsWidget.hpp"
#include "StatisticsController.hpp"
#include "ReadOnlyStream.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QCoreApplication>

StatisticsWidget::StatisticsWidget(QWidget* parent)
    : QWidget(parent)
    , controller(new StatisticsController(this))
    , currentStream(nullptr) {
    setupUI();
    
    connect(controller, &StatisticsController::statsUpdated,
            this, &StatisticsWidget::onStatsUpdated);
    connect(controller, &StatisticsController::processingProgress,
            this, &StatisticsWidget::onProgress);
    connect(controller, &StatisticsController::error,
            this, &StatisticsWidget::onError);
}

StatisticsWidget::~StatisticsWidget() {}

void StatisticsWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QGroupBox* controlGroup = new QGroupBox("Сбор статистики");
    QHBoxLayout* controlLayout = new QHBoxLayout(controlGroup);
    
    controlLayout->addWidget(new QLabel("Обработать элементов:"));
    countSpin = new QSpinBox();
    countSpin->setRange(1, 1000000);
    countSpin->setValue(100);
    controlLayout->addWidget(countSpin);
    
    collectBtn = new QPushButton("Собрать статистику");
    controlLayout->addWidget(collectBtn);
    
    resetBtn = new QPushButton("Сброс");
    controlLayout->addWidget(resetBtn);
    
    controlLayout->addStretch();
    mainLayout->addWidget(controlGroup);
    
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);
    
    QGroupBox* resultGroup = new QGroupBox("Результаты");
    QGridLayout* resultLayout = new QGridLayout(resultGroup);
    
    resultLayout->addWidget(new QLabel("Минимум:"), 0, 0);
    minLabel = new QLabel("-");
    resultLayout->addWidget(minLabel, 0, 1);
    
    resultLayout->addWidget(new QLabel("Максимум:"), 0, 2);
    maxLabel = new QLabel("-");
    resultLayout->addWidget(maxLabel, 0, 3);
    
    resultLayout->addWidget(new QLabel("Среднее:"), 1, 0);
    avgLabel = new QLabel("-");
    resultLayout->addWidget(avgLabel, 1, 1);
    
    resultLayout->addWidget(new QLabel("Медиана:"), 1, 2);
    medianLabel = new QLabel("-");
    resultLayout->addWidget(medianLabel, 1, 3);
    
    resultLayout->addWidget(new QLabel("Дисперсия:"), 2, 0);
    varianceLabel = new QLabel("-");
    resultLayout->addWidget(varianceLabel, 2, 1);
    
    resultLayout->addWidget(new QLabel("Ст. отклонение:"), 2, 2);
    stdDevLabel = new QLabel("-");
    resultLayout->addWidget(stdDevLabel, 2, 3);
    
    resultLayout->addWidget(new QLabel("Количество:"), 3, 0);
    countLabel = new QLabel("-");
    resultLayout->addWidget(countLabel, 3, 1);
    
    mainLayout->addWidget(resultGroup);
    
    // Connect
    connect(collectBtn, &QPushButton::clicked, this, &StatisticsWidget::onCollect);
    connect(resetBtn, &QPushButton::clicked, this, &StatisticsWidget::onReset);
}

void StatisticsWidget::setStream(ReadOnlyStream<int>* stream) {
    currentStream = stream;
}

void StatisticsWidget::onCollect() {
    if (!currentStream) {
        QMessageBox::warning(this, "Ошибка", "Нет источника данных");
        return;
    }
    
    int count = countSpin->value();
    
    progressBar->setVisible(true);
    progressBar->setValue(0);
    collectBtn->setEnabled(false);
    
    controller->reset();
    
    try {
        currentStream->Open();
        
        for (int i = 0; i < count && !currentStream->IsEndOfStream(); ++i) {
            int value = currentStream->Read();
            controller->updateFromValue(value);
            
            if (i % (count / 100 + 1) == 0) {
                progressBar->setValue(static_cast<int>(100.0 * i / count));
                QCoreApplication::processEvents();
            }
        }
        
        currentStream->Close();
        progressBar->setValue(100);
        
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
    
    progressBar->setVisible(false);
    collectBtn->setEnabled(true);
}

void StatisticsWidget::onReset() {
    controller->reset();
    updateDisplay();
}

void StatisticsWidget::onStatsUpdated() {
    updateDisplay();
}

void StatisticsWidget::onProgress(int percent) {
    progressBar->setValue(percent);
}

void StatisticsWidget::updateDisplay() {
    minLabel->setText(QString::number(controller->getMin()));
    maxLabel->setText(QString::number(controller->getMax()));
    avgLabel->setText(QString::number(controller->getAverage(), 'f', 2));
    medianLabel->setText(QString::number(controller->getMedian(), 'f', 2));
    varianceLabel->setText(QString::number(controller->getVariance(), 'f', 2));
    stdDevLabel->setText(QString::number(controller->getStdDeviation(), 'f', 2));
    countLabel->setText(QString::number(controller->getCount()));
}

void StatisticsWidget::onError(const QString& msg) {
    QMessageBox::warning(this, "Ошибка", msg);
}