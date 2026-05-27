#include "TestingWidget.hpp"
#include "LazySequenceController.hpp"
#include "OnlineStatistics.hpp"
#include "ReadOnlyStream.hpp"
#include "..\third_party\Lab_2\library\include\ArraySequence.hpp"
#include <random>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QRegularExpression>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCoreApplication>
#include <chrono>
#include <QCoreApplication>
#include <QFile>
#include <QDataStream>

void TestingWidget::createDataFile() {
    QString appDir = QCoreApplication::applicationDirPath();
    QString filename1 = appDir + "/large_data_arithmetic.txt";
    QFile file1(filename1);
    if (!file1.exists()) {
        if (!file1.open(QIODevice::WriteOnly | QIODevice::Text)) {
        } else {
            QTextStream out(&file1);
            for (int i = 0; i < 1000000; ++i) {
                out << (i + 1) << "\n";
            }
            file1.close();
        }
    }
    
    QString filename2 = appDir + "/large_data_even.txt";
    QFile file2(filename2);
    if (!file2.exists()) {
        if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        } else {
            QTextStream out(&file2);
            for (int i = 0; i < 1000000; ++i) {
                out << ((i + 1) * 2) << "\n";
            }
            file2.close();
        }
    }
    
    QString filename3 = appDir + "/large_data_random_5m.txt";
    QFile file3(filename3);
    if (!file3.exists()) {
        if (!file3.open(QIODevice::WriteOnly | QIODevice::Text)) {
        } else {
            QTextStream out(&file3);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<long long> dis(1, 50000);
            for (int i = 0; i < 5000000; ++i) {
                out << dis(gen) << "\n";
            }
            file3.close();
        }
    }
}

TestingWidget::TestingWidget(QWidget* parent)
    : QWidget(parent) {
    setupUI();
    createDataFile();
}

TestingWidget::~TestingWidget() {}

void TestingWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* btnLayout = new QHBoxLayout();
    autoTestsBtn = new QPushButton("Автоматические тесты");
    largeDataBtn = new QPushButton("Тест больших данных");
    btnLayout->addWidget(autoTestsBtn);
    btnLayout->addWidget(largeDataBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);
    
    QGroupBox* largeDataGroup = new QGroupBox("Настройка теста больших данных");
    QHBoxLayout* largeDataLayout = new QHBoxLayout(largeDataGroup);
    
    largeDataLayout->addWidget(new QLabel("Тип последовательности:"));
    largeDataTypeCombo = new QComboBox();
    largeDataTypeCombo->addItem("Арифметическая (1, 2, 3, ...)");
    largeDataTypeCombo->addItem("Чётные числа (2, 4, 6, ...)");
    largeDataTypeCombo->addItem("Случайные числа (1-50000)");
    largeDataLayout->addWidget(largeDataTypeCombo);
    
    largeDataLayout->addWidget(new QLabel("Количество элементов:"));
    largeDataSizeSpin = new QSpinBox();
    largeDataSizeSpin->setRange(1, 5000000);
    largeDataSizeSpin->setValue(100000);
    largeDataLayout->addWidget(largeDataSizeSpin);
    
    largeDataLayout->addStretch();
    mainLayout->addWidget(largeDataGroup);
    
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);
    
    QGroupBox* inputGroup = new QGroupBox("Ввод чисел (нажмите Enter после каждого числа)");
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);
    
    QHBoxLayout* inputRow = new QHBoxLayout();
    inputRow->addWidget(new QLabel("Число:"));
    numberInput = new QLineEdit();
    numberInput->setPlaceholderText("Введите число и нажмите Enter...");
    inputRow->addWidget(numberInput);
    addNumberBtn = new QPushButton("Добавить");
    inputRow->addWidget(addNumberBtn);
    clearNumbersBtn = new QPushButton("Очистить все");
    inputRow->addWidget(clearNumbersBtn);
    inputLayout->addLayout(inputRow);
    
    numbersList = new QListWidget();
    numbersList->setMaximumHeight(150);
    inputLayout->addWidget(numbersList);
    
    calcFromListBtn = new QPushButton("Рассчитать статистику");
    calcFromListBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    inputLayout->addWidget(calcFromListBtn);
    
    mainLayout->addWidget(inputGroup);
    
    manualResult = new QTextEdit();
    manualResult->setReadOnly(true);
    manualResult->setMaximumHeight(200);
    mainLayout->addWidget(manualResult);
    
    testTable = new QTableWidget();
    testTable->setColumnCount(11);
    testTable->setHorizontalHeaderLabels({
        "Тест", "Статус",
        "Min", "Max", "Размах",
        "Среднее", "Дисперсия", "Ст. откл.",
        "Среднеквадрат.", "Коэф. вариации", "Время"
    });
    testTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    testTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  
    testTable->setSelectionBehavior(QAbstractItemView::SelectRows);  
    mainLayout->addWidget(testTable);
    
    connect(autoTestsBtn, &QPushButton::clicked, this, &TestingWidget::onRunAutoTests);
    connect(largeDataBtn, &QPushButton::clicked, this, &TestingWidget::onRunLargeData);
    connect(largeDataTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TestingWidget::onDataTypeChanged);
    connect(numberInput, &QLineEdit::returnPressed, this, &TestingWidget::onAddNumber);
    connect(addNumberBtn, &QPushButton::clicked, this, &TestingWidget::onAddNumber);
    connect(clearNumbersBtn, &QPushButton::clicked, this, &TestingWidget::onClearNumbers);
    connect(calcFromListBtn, &QPushButton::clicked, this, &TestingWidget::onCalculateFromList);
}

void TestingWidget::onDataTypeChanged(int index) {
    int maxElements = 0;
    int defaultValue = 0;
    
    switch (index) {
        case 0: 
            maxElements = 1000000;
            defaultValue = 1000000; 
            break;
        case 1: 
            maxElements = 1000000;
            defaultValue = 1000000;  
            break;
        case 2: 
            maxElements = 5000000;
            defaultValue = 5000000;  
            break;
        default:
            maxElements = 1000000;
            defaultValue = 1000000;
    }
    
    largeDataSizeSpin->setMaximum(maxElements);
    largeDataSizeSpin->setValue(defaultValue);
}

void TestingWidget::onRunLargeData() {
    int dataType = largeDataTypeCombo->currentIndex();
    int dataSize = largeDataSizeSpin->value();
    
    QString typeName;
    QString filename;
    
    switch (dataType) {
        case 0:
            typeName = "Арифметическая (1, 2, 3, ...) 1 млн";
            filename = QCoreApplication::applicationDirPath() + "/large_data_arithmetic.txt";
            break;
        case 1:
            typeName = "Чётные числа (2, 4, 6, ...) 1 млн";
            filename = QCoreApplication::applicationDirPath() + "/large_data_even.txt";
            break;
        case 2:
            typeName = "Случайные числа (1-50000) 5 млн";
            filename = QCoreApplication::applicationDirPath() + "/large_data_random_5m.txt";
            break;
        default:
            QMessageBox::warning(this, "Ошибка", "Неизвестный тип данных");
            return;
    }
    
    QFile file(filename);
    if (!file.exists()) {
        QMessageBox::warning(this, "Ошибка", 
            QString("Файл %1 не найден.\n"
                    "Он будет создан при следующем запуске.").arg(filename));
        createDataFile();
        return;
    }
    file.close();
    
    QMessageBox::information(this, "Тест больших данных", 
        QString("Тип: %1\nЧтение %2 элементов через ReadOnlyStream...")
        .arg(typeName).arg(dataSize));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        std::function<long long(const std::string&)> deserializer = [](const std::string& s) -> long long {
            return std::stoll(s);
        };
        
        ReadOnlyStream<long long> stream(filename.toStdString(), deserializer);
        stream.Open();
        
        OnlineStatistics<long long> stats;
        
        for (int i = 0; i < dataSize && !stream.IsEndOfStream(); ++i) {
            stats.Update(stream.Read());
        }
        
        stream.Close();
        
        auto end = std::chrono::high_resolution_clock::now();
        double timeSec = std::chrono::duration<double>(end - start).count();
        
        QString result = QString(
            "Тест ReadOnlyStream (чтение из файла)\n"
            "Тип: %1\n"
            "Обработано: %2 элементов\n"
            "Время: %3 сек\n"
            "Минимум: %4\n"
            "Максимум: %5\n"
            "Размах: %6\n"
            "Среднее: %7\n"
            "Дисперсия: %8\n"
            "Ст. отклонение: %9\n"
            "Среднеквадратическое: %10\n"
            "Коэф. вариации: %11\n")
            .arg(typeName)
            .arg(stats.GetCount())
            .arg(QString::number(timeSec, 'f', 2))
            .arg(stats.GetMin())
            .arg(stats.GetMax())
            .arg(QString::number(stats.GetRange(), 'f', 2))
            .arg(QString::number(stats.GetAverage(), 'f', 2))
            .arg(QString::number(stats.GetVariance(), 'f', 2))
            .arg(QString::number(stats.GetStdDeviation(), 'f', 2))
            .arg(QString::number(stats.GetRMS(), 'f', 2))
            .arg(QString::number(stats.GetCoefficientOfVariation(), 'f', 4));
        
        QMessageBox::information(this, "Результат теста больших данных", result);
        
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void TestingWidget::onRunAutoTests() {
    testTable->setRowCount(0);
    
    struct TestResult {
        QString name;
        bool passed;
        long long actualMin;
        long long actualMax;
        double actualRange;
        double actualAvg;
        double actualVariance;
        double actualStdDev;
        double actualRMS;
        double actualCV;
        long long expectedMin;
        long long expectedMax;
        double expectedRange;
        double expectedAvg;
        double expectedVariance;
        double expectedStdDev;
        double expectedRMS;
        double expectedCV;
        double timeMs;
        QString errorMsg;
    };
    
    QVector<TestResult> results;
    
    {
        TestResult r;
        r.name = "Арифметическая 1..100";
        r.expectedMin = 1;
        r.expectedMax = 100;
        r.expectedRange = 99;
        r.expectedAvg = 50.5;
        r.expectedVariance = 833.25;
        r.expectedStdDev = 28.87;
        r.expectedRMS = 57.74;
        r.expectedCV = 0.5716;
        
        try {
            auto start = std::chrono::high_resolution_clock::now();
            
            LazySequenceController controller;
            controller.setGenerator(LazySequenceController::GEN_ARITHMETIC, 1, 1);
            
            ArraySequence<long long> dataCopy;
            for (int i = 0; i < 100; ++i) {
                dataCopy.Append(controller.getElement(i));
            }
            
            LazySequence<long long> tempSeq(&dataCopy);
            ReadOnlyStream<long long> stream(&tempSeq);
            
            OnlineStatistics<long long> stats;
            stream.Open();
            for (int i = 0; i < 100 && !stream.IsEndOfStream(); ++i) {
                stats.Update(stream.Read());
            }
            stream.Close();
            
            r.actualMin = stats.GetMin();
            r.actualMax = stats.GetMax();
            r.actualRange = stats.GetRange();
            r.actualAvg = stats.GetAverage();
            r.actualVariance = stats.GetVariance();
            r.actualStdDev = stats.GetStdDeviation();
            r.actualRMS = stats.GetRMS();
            r.actualCV = stats.GetCoefficientOfVariation();
            
            r.passed = (r.actualMin == r.expectedMin &&
                        r.actualMax == r.expectedMax &&
                        std::abs(r.actualRange - r.expectedRange) < 0.1 &&
                        std::abs(r.actualAvg - r.expectedAvg) < 0.1 &&
                        std::abs(r.actualVariance - r.expectedVariance) < 1.0 &&
                        std::abs(r.actualStdDev - r.expectedStdDev) < 0.5 &&
                        std::abs(r.actualRMS - r.expectedRMS) < 0.5 &&
                        std::abs(r.actualCV - r.expectedCV) < 0.01);
            
            auto end = std::chrono::high_resolution_clock::now();
            r.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
            
        } catch (const std::exception& e) {
            r.passed = false;
            r.errorMsg = e.what();
        }
        
        results.append(r);
    }
    
    {
        TestResult r;
        r.name = "Постоянные числа (42)";
        r.expectedMin = 42;
        r.expectedMax = 42;
        r.expectedRange = 0;
        r.expectedAvg = 42;
        r.expectedVariance = 0;
        r.expectedStdDev = 0;
        r.expectedRMS = 42;
        r.expectedCV = 0;
        
        try {
            auto start = std::chrono::high_resolution_clock::now();
            
            LazySequenceController controller;
            controller.setGenerator(LazySequenceController::GEN_CONSTANT, 42);
            
            ArraySequence<long long> dataCopy;
            for (int i = 0; i < 50; ++i) {
                dataCopy.Append(controller.getElement(i));
            }
            
            LazySequence<long long> tempSeq(&dataCopy);
            ReadOnlyStream<long long> stream(&tempSeq);
            
            OnlineStatistics<long long> stats;
            stream.Open();
            for (int i = 0; i < 50 && !stream.IsEndOfStream(); ++i) {
                stats.Update(stream.Read());
            }
            stream.Close();
            
            r.actualMin = stats.GetMin();
            r.actualMax = stats.GetMax();
            r.actualRange = stats.GetRange();
            r.actualAvg = stats.GetAverage();
            r.actualVariance = stats.GetVariance();
            r.actualStdDev = stats.GetStdDeviation();
            r.actualRMS = stats.GetRMS();
            r.actualCV = stats.GetCoefficientOfVariation();
            
            r.passed = (r.actualMin == r.expectedMin &&
                        r.actualMax == r.expectedMax &&
                        std::abs(r.actualRange - r.expectedRange) < 0.1 &&
                        std::abs(r.actualAvg - r.expectedAvg) < 0.1 &&
                        std::abs(r.actualVariance - r.expectedVariance) < 0.1 &&
                        std::abs(r.actualStdDev - r.expectedStdDev) < 0.1 &&
                        std::abs(r.actualRMS - r.expectedRMS) < 0.1 &&
                        std::abs(r.actualCV - r.expectedCV) < 0.01);
            
            auto end = std::chrono::high_resolution_clock::now();
            r.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
            
        } catch (const std::exception& e) {
            r.passed = false;
            r.errorMsg = e.what();
        }
        
        results.append(r);
    }
    
    {
    TestResult r;
    r.name = "Факториал (первые 10)";
    r.expectedMin = 1;
    r.expectedMax = 3628800;
    r.expectedRange = 3628799;
    r.expectedAvg = 403791.3;
    r.expectedVariance = 1167104883226.01;  
    r.expectedStdDev = 1080340;
    r.expectedRMS = 1153320;
    r.expectedCV = 2.676;
    
    try {
        auto start = std::chrono::high_resolution_clock::now();
        ArraySequence<long long> factData;
        long long fact = 1;
        for (int i = 1; i <= 10; ++i) {
            fact *= i;
            factData.Append(fact);
        }
        
        LazySequence<long long> tempSeq(&factData);
        ReadOnlyStream<long long> stream(&tempSeq);
        
        OnlineStatistics<long long> stats;
        stream.Open();
        for (int i = 0; i < 10 && !stream.IsEndOfStream(); ++i) {
            stats.Update(stream.Read());
        }
        stream.Close();
        
        r.actualMin = stats.GetMin();
        r.actualMax = stats.GetMax();
        r.actualRange = stats.GetRange();
        r.actualAvg = stats.GetAverage();
        r.actualVariance = stats.GetVariance();
        r.actualStdDev = stats.GetStdDeviation();
        r.actualRMS = stats.GetRMS();
        r.actualCV = stats.GetCoefficientOfVariation();
        
        r.passed = (r.actualMin == r.expectedMin &&
                    r.actualMax == r.expectedMax &&
                    std::abs(r.actualRange - r.expectedRange) < 1.0 &&
                    std::abs(r.actualAvg - r.expectedAvg) < 0.1 &&
                    std::abs(r.actualVariance - r.expectedVariance) < 1e9 &&
                    std::abs(r.actualStdDev - r.expectedStdDev) < 1000 &&
                    std::abs(r.actualRMS - r.expectedRMS) < 1000 &&
                    std::abs(r.actualCV - r.expectedCV) < 0.1);
        
        auto end = std::chrono::high_resolution_clock::now();
        r.timeMs = std::chrono::duration<double, std::milli>(end - start).count(); 
    } catch (const std::exception& e) {
        r.passed = false;
        r.errorMsg = e.what();
    }
    results.append(r);
}
    
    {
    TestResult r;
    r.name = "Фибоначчи (первые 10)";
    r.expectedMin = 0;
    r.expectedMax = 34;
    r.expectedRange = 34;
    r.expectedAvg = 8.8;
    r.expectedVariance = 109.56;  
    r.expectedStdDev = 10.47;      
    r.expectedRMS = 14.05;
    r.expectedCV = 1.19;          
    
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        LazySequenceController controller;
        controller.setGenerator(LazySequenceController::GEN_FIBONACCI);
        
        ArraySequence<long long> dataCopy;
        for (int i = 0; i < 10; ++i) {
            dataCopy.Append(controller.getElement(i));
        }
        
        LazySequence<long long> tempSeq(&dataCopy);
        ReadOnlyStream<long long> stream(&tempSeq);
        
        OnlineStatistics<long long> stats;
        stream.Open();
        for (int i = 0; i < 10 && !stream.IsEndOfStream(); ++i) {
            stats.Update(stream.Read());
        }
        stream.Close();
        
        r.actualMin = stats.GetMin();
        r.actualMax = stats.GetMax();
        r.actualRange = stats.GetRange();
        r.actualAvg = stats.GetAverage();
        r.actualVariance = stats.GetVariance();
        r.actualStdDev = stats.GetStdDeviation();
        r.actualRMS = stats.GetRMS();
        r.actualCV = stats.GetCoefficientOfVariation();
        
        r.passed = (r.actualMin == r.expectedMin &&
                    r.actualMax == r.expectedMax &&
                    std::abs(r.actualRange - r.expectedRange) < 1.0 &&
                    std::abs(r.actualAvg - r.expectedAvg) < 0.5 &&
                    std::abs(r.actualVariance - r.expectedVariance) < 5.0 &&
                    std::abs(r.actualStdDev - r.expectedStdDev) < 1.0 &&
                    std::abs(r.actualRMS - r.expectedRMS) < 1.0 &&
                    std::abs(r.actualCV - r.expectedCV) < 0.5);
        
        auto end = std::chrono::high_resolution_clock::now();
        r.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        
    } catch (const std::exception& e) {
        r.passed = false;
        r.errorMsg = e.what();
    }
    
    results.append(r);
}
    
    testTable->setRowCount(results.size());
    for (int i = 0; i < results.size(); ++i) {
        const TestResult& r = results[i];
        
        testTable->setItem(i, 0, new QTableWidgetItem(r.name));
        
        QString status = r.passed ? " Пройден" : " Провален";
        if (!r.errorMsg.isEmpty()) status += " (" + r.errorMsg + ")";
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        if (!r.passed) {
            statusItem->setBackground(Qt::red);
            statusItem->setForeground(Qt::white);
        }
    testTable->setItem(i, 0, new QTableWidgetItem(r.name));
    testTable->setItem(i, 1, new QTableWidgetItem(status));
    testTable->setItem(i, 2, new QTableWidgetItem(QString::number(r.actualMin)));
    testTable->setItem(i, 3, new QTableWidgetItem(QString::number(r.actualMax)));
    testTable->setItem(i, 4, new QTableWidgetItem(QString::number(r.actualRange, 'f', 2)));
    testTable->setItem(i, 5, new QTableWidgetItem(QString::number(r.actualAvg, 'f', 2)));
    testTable->setItem(i, 6, new QTableWidgetItem(QString::number(r.actualVariance, 'f', 2)));
    testTable->setItem(i, 7, new QTableWidgetItem(QString::number(r.actualStdDev, 'f', 2)));
    testTable->setItem(i, 8, new QTableWidgetItem(QString::number(r.actualRMS, 'f', 2)));
    testTable->setItem(i, 9, new QTableWidgetItem(QString::number(r.actualCV, 'f', 4)));
    testTable->setItem(i, 10, new QTableWidgetItem(QString::number(r.timeMs, 'f', 2) + " ms"));
    }
    
    testTable->resizeColumnsToContents();
}

void TestingWidget::onAddNumber() {
    QString text = numberInput->text().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите число");
        return;
    }
    
    bool ok;
    double value = text.toDouble(&ok);
    
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", 
            QString("'%1' не является числом!").arg(text));
        numberInput->clear();
        numberInput->setFocus();
        return;
    }
    const double MAX_SAFE_VALUE = 400000000.0;  
    if (std::abs(value) > MAX_SAFE_VALUE) {
        QMessageBox::warning(this, "Ошибка", 
            QString("Число %1 слишком большое!\n"
                    "Допустимый диапазон: от -400 000 000 до 400 000 000")
            .arg(value));
        numberInput->clear();
        numberInput->setFocus();
        return;
    }
    
    userNumbers.append(value);
    numbersList->addItem(QString("[%1] = %2").arg(userNumbers.size()).arg(value));
    numbersList->scrollToBottom();
    numberInput->clear();
    numberInput->setFocus();
}

void TestingWidget::onClearNumbers() {
    userNumbers.clear();
    numbersList->clear();
}

void TestingWidget::onCalculateFromList() {
    if (userNumbers.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет введённых чисел");
        return;
    }
    
    ArraySequence<double> data;
    for (double val : userNumbers) {
        data.Append(val);
    }
    
    LazySequence<double> lazySeq(&data);
    ReadOnlyStream<double> stream(&lazySeq);
    
    OnlineStatistics<double> stats;
    stream.Open();
    while (!stream.IsEndOfStream()) {
        stats.Update(stream.Read());
    }
    stream.Close();
    
    QString result = QString(
        "Результаты обработки (%1 элементов):\n"
        "Минимум: %2\n"
        "Максимум: %3\n"
        "Размах: %4\n"
        "Среднее: %5\n"
        "Дисперсия: %6\n"
        "Ст. отклонение: %7\n"
        "Среднеквадратическое: %8\n"
        "Коэф. вариации: %9\n")
        .arg(stats.GetCount())
        .arg(stats.GetMin())
        .arg(stats.GetMax())
        .arg(QString::number(stats.GetRange(), 'f', 4))
        .arg(QString::number(stats.GetAverage(), 'f', 4))
        .arg(QString::number(stats.GetVariance(), 'f', 4))
        .arg(QString::number(stats.GetStdDeviation(), 'f', 4))
        .arg(QString::number(stats.GetRMS(), 'f', 4))
        .arg(QString::number(stats.GetCoefficientOfVariation(), 'f', 6));
    
    manualResult->setText(result);
}