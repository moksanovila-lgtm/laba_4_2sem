#include "TestingWidget.hpp"
#include "LazySequenceController.hpp"
#include "StatisticsController.hpp"
#include "OnlineStatistics.hpp"
#include "ReadOnlyStream.hpp"
#include "..\third_party\Lab_2\library\include\ArraySequence.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QRegularExpression>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QCoreApplication>

// TestingWidget::TestingWidget(QWidget* parent)
//     : QWidget(parent) {
//     setupUI();
// }

// TestingWidget::~TestingWidget() {}

// void TestingWidget::setupUI() {
//     QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
//     QHBoxLayout* btnLayout = new QHBoxLayout();
//     autoTestsBtn = new QPushButton("Автоматические тесты");
//     largeDataBtn = new QPushButton("Тест больших данных (100000)");
//     manualBtn = new QPushButton("Ручной ввод");
//     btnLayout->addWidget(autoTestsBtn);
//     btnLayout->addWidget(largeDataBtn);
//     btnLayout->addWidget(manualBtn);
//     btnLayout->addStretch();
//     mainLayout->addLayout(btnLayout);
    
//     QGroupBox* manualGroup = new QGroupBox("Ручной ввод данных");
//     QVBoxLayout* manualLayout = new QVBoxLayout(manualGroup);
    
//     manualInput = new QTextEdit();
//     manualInput->setPlaceholderText("Введите числа, разделенные пробелом, запятой или переводом строки\nПример: 5 8 3 9 1 7 4 6 2 10");
//     manualInput->setMaximumHeight(100);
//     manualLayout->addWidget(manualInput);
    
//     QPushButton* calcBtn = new QPushButton("Рассчитать");
//     manualLayout->addWidget(calcBtn);
    
//     manualResult = new QTextEdit();
//     manualResult->setReadOnly(true);
//     manualResult->setMaximumHeight(150);
//     manualLayout->addWidget(manualResult);
//     mainLayout->addWidget(manualGroup);
    
//     testTable = new QTableWidget();
//     testTable->setColumnCount(5);
//     testTable->setHorizontalHeaderLabels({"Тест", "Статус", "Результат", "Ожидалось", "Время"});
//     testTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
//     mainLayout->addWidget(testTable);
    
//     connect(autoTestsBtn, &QPushButton::clicked, this, &TestingWidget::onRunAutoTests);
//     connect(largeDataBtn, &QPushButton::clicked, this, &TestingWidget::onRunLargeData);
//     connect(calcBtn, &QPushButton::clicked, this, &TestingWidget::onRunManual);
// }

// void TestingWidget::onRunLargeData() {
//     const int LARGE_SIZE = 100000;
    
//     QMessageBox::information(this, "Тест больших данных", 
//         QString("Запуск теста на %1 элементах...").arg(LARGE_SIZE));
    
//     auto start = std::chrono::high_resolution_clock::now();
    
//     LazySequenceController controller;
//     controller.setGenerator(LazySequenceController::GEN_RANDOM, 0, 1000);
//     ReadOnlyStream<long long>* stream = controller.getStream();
    
//     OnlineStatistics<long long> stats;
//     stats.ProcessStream(*stream, LARGE_SIZE);
    
//     auto end = std::chrono::high_resolution_clock::now();
//     double timeSec = std::chrono::duration<double>(end - start).count();
    
//     QString result = QString::fromUtf8("Обработано %1 элементов\n"
//         "Время: %2 сек\n"
//         "Min: %3\n"
//         "Max: %4\n"
//         "Avg: %5\n"
//         "Median: %6\n"
//         "StdDev: %7")
//         .arg(LARGE_SIZE)
//         .arg(QString::number(timeSec, 'f', 2))
//         .arg(stats.GetMin())
//         .arg(stats.GetMax())
//         .arg(QString::number(stats.GetAverage(), 'f', 2))
//         .arg(QString::number(stats.GetMedian(), 'f', 2))
//         .arg(QString::number(stats.GetStdDeviation(), 'f', 2));
    
//     QMessageBox::information(this, "Результат теста больших данных", result);
// }

// void TestingWidget::onRunAutoTests() {
//     testTable->setRowCount(0);
    
//     struct TestResult {
//         QString name;
//         bool passed;
//         double actual;
//         double expected;
//         double timeMs;
//     };
    
//     QVector<TestResult> results;
    
//     {
//         TestResult r;
//         r.name = "Арифметическая 1..100";
//         auto start = std::chrono::high_resolution_clock::now();
        
//         LazySequenceController controller;
//         controller.setGenerator(LazySequenceController::GEN_ARITHMETIC, 1, 1);
//         ReadOnlyStream<long long>* stream = controller.getStream();
        
//         OnlineStatistics<long long> stats;
//         stats.ProcessStream(*stream, 100);
        
//         r.actual = stats.GetAverage();
//         r.expected = 50.5;
//         r.passed = (std::abs(r.actual - r.expected) < 0.1);
        
//         auto end = std::chrono::high_resolution_clock::now();
//         r.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
//         results.append(r);
//     }
    
//     {
//         TestResult r;
//         r.name = "Чётные числа 2..100";
//         auto start = std::chrono::high_resolution_clock::now();
        
//         LazySequenceController controller;
//         controller.setGenerator(LazySequenceController::GEN_ARITHMETIC, 2, 2);
//         ReadOnlyStream<long long>* stream = controller.getStream();
        
//         OnlineStatistics<long long> stats;
//         stats.ProcessStream(*stream, 50);
        
//         r.actual = stats.GetMedian();
//         r.expected = 51;
//         r.passed = (r.actual == r.expected);
        
//         auto end = std::chrono::high_resolution_clock::now();
//         r.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
//         results.append(r);
//     }
    
//     testTable->setRowCount(results.size());
//     for (int i = 0; i < results.size(); ++i) {
//         const TestResult& r = results[i];
//         testTable->setItem(i, 0, new QTableWidgetItem(r.name));
//         testTable->setItem(i, 1, new QTableWidgetItem(r.passed ? " Пройден" : " Провален"));
//         testTable->setItem(i, 2, new QTableWidgetItem(QString::number(r.actual, 'f', 2)));
//         testTable->setItem(i, 3, new QTableWidgetItem(QString::number(r.expected, 'f', 2)));
//         testTable->setItem(i, 4, new QTableWidgetItem(QString::number(r.timeMs, 'f', 2) + " ms"));
        
//         if (!r.passed) {
//             testTable->item(i, 1)->setBackground(Qt::red);
//             testTable->item(i, 1)->setForeground(Qt::white);
//         }
//     }
//     testTable->resizeColumnsToContents();
// }

// void TestingWidget::onRunManual() {
//     QString input = manualInput->toPlainText().trimmed();
//     if (input.isEmpty()) {
//         QMessageBox::warning(this, "Ошибка", "Введите данные");
//         return;
//     }
    
//     QStringList parts = input.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
//     ArraySequence<int> data;
    
//     for (const QString& part : parts) {
//         bool ok;
//         int val = part.toInt(&ok);
//         if (ok) {
//             data.Append(val);
//         }
//     }
    
//     if (data.GetCount() == 0) {
//         QMessageBox::warning(this, "Ошибка", "Не найдено чисел");
//         return;
//     }
    
//     LazySequence<long long> lazySeq(&data);
//     ReadOnlyStream<long long> stream(&lazySeq);
    
//     OnlineStatistics<long long> stats;
//     stats.ProcessStream(stream);
    
//     manualResult->setText(
//         QString::fromUtf8("Результаты обработки (%1 элементов):\n"
//         "Минимум: %2\n"
//         "Максимум: %3\n"
//         "Среднее: %4\n"
//         "Медиана: %5\n"
//         "Дисперсия: %6\n"
//         "Стандартное отклонение: %7")
//         .arg(stats.GetCount())
//         .arg(stats.GetMin())
//         .arg(stats.GetMax())
//         .arg(QString::number(stats.GetAverage(), 'f', 2))
//         .arg(QString::number(stats.GetMedian(), 'f', 2))
//         .arg(QString::number(stats.GetVariance(), 'f', 2))
//         .arg(QString::number(stats.GetStdDeviation(), 'f', 2)));
// }

TestingWidget::TestingWidget(QWidget* parent)
    : QWidget(parent) {
    setupUI();
}

TestingWidget::~TestingWidget() {}

void TestingWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    autoTestsBtn = new QPushButton("Автоматические тесты");
    largeDataBtn = new QPushButton("Тест больших данных (100000)");
    manualBtn = new QPushButton("Ручной ввод");
    btnLayout->addWidget(autoTestsBtn);
    btnLayout->addWidget(largeDataBtn);
    btnLayout->addWidget(manualBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);
    
    QGroupBox* manualGroup = new QGroupBox("Ручной ввод данных");
    QVBoxLayout* manualLayout = new QVBoxLayout(manualGroup);
    
    manualInput = new QTextEdit();
    manualInput->setPlaceholderText("Введите числа, разделенные пробелом, запятой или переводом строки\nПример: 5 8 3 9 1 7 4 6 2 10");
    manualInput->setMaximumHeight(100);
    manualLayout->addWidget(manualInput);
    
    QPushButton* calcBtn = new QPushButton("Рассчитать");
    manualLayout->addWidget(calcBtn);
    
    manualResult = new QTextEdit();
    manualResult->setReadOnly(true);
    manualResult->setMaximumHeight(150);
    manualLayout->addWidget(manualResult);
    mainLayout->addWidget(manualGroup);
    
    testTable = new QTableWidget();
    testTable->setColumnCount(5);
    testTable->setHorizontalHeaderLabels({"Тест", "Статус", "Результат", "Ожидалось", "Время"});
    testTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    mainLayout->addWidget(testTable);
    
    connect(autoTestsBtn, &QPushButton::clicked, this, &TestingWidget::onRunAutoTests);
    connect(largeDataBtn, &QPushButton::clicked, this, &TestingWidget::onRunLargeData);
    connect(calcBtn, &QPushButton::clicked, this, &TestingWidget::onRunManual);
}

void TestingWidget::onRunLargeData() {
    const int LARGE_SIZE = 100000;
    
    QMessageBox::information(this, "Тест больших данных", 
        QString("Запуск теста на %1 элементах...").arg(LARGE_SIZE));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    LazySequenceController controller;
    controller.setGenerator(LazySequenceController::GEN_RANDOM, 0, 1000);
    ReadOnlyStream<long long>* stream = controller.getStream();
    
    OnlineStatistics<long long> stats;
    stats.ProcessStream(*stream, LARGE_SIZE);
    
    auto end = std::chrono::high_resolution_clock::now();
    double timeSec = std::chrono::duration<double>(end - start).count();
    
    QString result = QString("Обработано %1 элементов\n"
        "Время: %2 сек\n"
        "Min: %3\n"
        "Max: %4\n"
        "Avg: %5\n"
        "Median: %6\n"
        "StdDev: %7")
        .arg(LARGE_SIZE)
        .arg(QString::number(timeSec, 'f', 2))
        .arg(stats.GetMin())
        .arg(stats.GetMax())
        .arg(QString::number(stats.GetAverage(), 'f', 2))
        .arg(QString::number(stats.GetMedian(), 'f', 2))
        .arg(QString::number(stats.GetStdDeviation(), 'f', 2));
    
    QMessageBox::information(this, "Результат теста больших данных", result);
}

void TestingWidget::onRunAutoTests() {
    testTable->setRowCount(0);
    
    struct TestResult {
        QString name;
        bool passed;
        double actual;
        double expected;
        double timeMs;
    };
    
    QVector<TestResult> results;
    
    {
        TestResult r;
        r.name = "Арифметическая 1..100";
        auto start = std::chrono::high_resolution_clock::now();
        
        LazySequenceController controller;
        controller.setGenerator(LazySequenceController::GEN_ARITHMETIC, 1, 1);
        ReadOnlyStream<long long>* stream = controller.getStream();
        
        OnlineStatistics<long long> stats;
        stats.ProcessStream(*stream, 100);
        
        r.actual = stats.GetAverage();
        r.expected = 50.5;
        r.passed = (std::abs(r.actual - r.expected) < 0.1);
        
        auto end = std::chrono::high_resolution_clock::now();
        r.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        results.append(r);
    }
    
    {
        TestResult r;
        r.name = "Чётные числа 2..100";
        auto start = std::chrono::high_resolution_clock::now();
        
        LazySequenceController controller;
        controller.setGenerator(LazySequenceController::GEN_ARITHMETIC, 2, 2);
        ReadOnlyStream<long long>* stream = controller.getStream();
        
        OnlineStatistics<long long> stats;
        stats.ProcessStream(*stream, 50);
        
        r.actual = stats.GetMedian();
        r.expected = 51;
        r.passed = (r.actual == r.expected);
        
        auto end = std::chrono::high_resolution_clock::now();
        r.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        results.append(r);
    }
    
    testTable->setRowCount(results.size());
    for (int i = 0; i < results.size(); ++i) {
        const TestResult& r = results[i];
        testTable->setItem(i, 0, new QTableWidgetItem(r.name));
        testTable->setItem(i, 1, new QTableWidgetItem(r.passed ? " Пройден" : " Провален"));
        testTable->setItem(i, 2, new QTableWidgetItem(QString::number(r.actual, 'f', 2)));
        testTable->setItem(i, 3, new QTableWidgetItem(QString::number(r.expected, 'f', 2)));
        testTable->setItem(i, 4, new QTableWidgetItem(QString::number(r.timeMs, 'f', 2) + " ms"));
        
        if (!r.passed) {
            testTable->item(i, 1)->setBackground(Qt::red);
            testTable->item(i, 1)->setForeground(Qt::white);
        }
    }
    testTable->resizeColumnsToContents();
}

void TestingWidget::onRunManual() {
    QString input = manualInput->toPlainText().trimmed();
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите данные");
        return;
    }
    
    QStringList parts = input.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
    ArraySequence<long long> data;  
    
    for (const QString& part : parts) {
        bool ok;
        long long val = part.toLongLong(&ok);
        if (ok) {
            data.Append(val);
        }
    }
    
    if (data.GetCount() == 0) {
        QMessageBox::warning(this, "Ошибка", "Не найдено чисел");
        return;
    }
    
    LazySequence<long long> lazySeq(&data);
    ReadOnlyStream<long long> stream(&lazySeq);
    
    OnlineStatistics<long long> stats;
    stats.ProcessStream(stream);
    
    manualResult->setText(
        QString("Результаты обработки (%1 элементов):\n"
        "Минимум: %2\n"
        "Максимум: %3\n"
        "Среднее: %4\n"
        "Медиана: %5\n"
        "Дисперсия: %6\n"
        "Стандартное отклонение: %7")
        .arg(stats.GetCount())
        .arg(stats.GetMin())
        .arg(stats.GetMax())
        .arg(QString::number(stats.GetAverage(), 'f', 2))
        .arg(QString::number(stats.GetMedian(), 'f', 2))
        .arg(QString::number(stats.GetVariance(), 'f', 2))
        .arg(QString::number(stats.GetStdDeviation(), 'f', 2)));
}