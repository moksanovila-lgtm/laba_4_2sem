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

// void TestingWidget::createDataFile() {
//     QString filename = QCoreApplication::applicationDirPath() + "/large_data.txt";
    
//     QFile file(filename);
//     if (file.exists()) {
//         std::cout << "Файл уже существует: " << filename.toStdString() << std::endl;
//         return;
//     }
    
//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//         std::cout << "Не удалось создать файл" << std::endl;
//         return;
//     }
    
//     std::cout << "Создание файла с 1 000 000 чисел..." << std::endl;
    
//     QTextStream out(&file);
//     for (int i = 0; i < 1000000; ++i) {
//         out << (i + 1) << "\n";
//     }
    
//     file.close();
//     std::cout << "Файл создан: " << filename.toStdString() << std::endl;
// }

void TestingWidget::createDataFile() {
    QString appDir = QCoreApplication::applicationDirPath();
    
    // ========== Файл 1: Арифметическая прогрессия (1 млн) ==========
    QString filename1 = appDir + "/large_data_arithmetic.txt";
    QFile file1(filename1);
    if (!file1.exists()) {
        if (!file1.open(QIODevice::WriteOnly | QIODevice::Text)) {
            std::cout << "Не удалось создать файл: " << filename1.toStdString() << std::endl;
        } else {
            std::cout << "Создание файла: " << filename1.toStdString() << " (1 млн чисел)..." << std::endl;
            QTextStream out(&file1);
            for (int i = 0; i < 1000000; ++i) {
                out << (i + 1) << "\n";
            }
            file1.close();
            std::cout << "Файл создан" << std::endl;
        }
    }
    
    // ========== Файл 2: Чётные числа (1 млн) ==========
    QString filename2 = appDir + "/large_data_even.txt";
    QFile file2(filename2);
    if (!file2.exists()) {
        if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
            std::cout << "Не удалось создать файл: " << filename2.toStdString() << std::endl;
        } else {
            std::cout << "Создание файла: " << filename2.toStdString() << " (1 млн чётных чисел)..." << std::endl;
            QTextStream out(&file2);
            for (int i = 0; i < 1000000; ++i) {
                out << ((i + 1) * 2) << "\n";
            }
            file2.close();
            std::cout << "Файл создан" << std::endl;
        }
    }
    
    // ========== Файл 3: Случайные числа (5 млн) ==========
    QString filename3 = appDir + "/large_data_random_5m.txt";
    QFile file3(filename3);
    if (!file3.exists()) {
        if (!file3.open(QIODevice::WriteOnly | QIODevice::Text)) {
            std::cout << "Не удалось создать файл: " << filename3.toStdString() << std::endl;
        } else {
            std::cout << "Создание файла: " << filename3.toStdString() << " (5 млн случайных чисел 1-50000)..." << std::endl;
            QTextStream out(&file3);
            
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<long long> dis(1, 50000);
            
            for (int i = 0; i < 5000000; ++i) {
                out << dis(gen) << "\n";
            }
            file3.close();
            std::cout << "Файл создан" << std::endl;
        }
    }
}



































TestingWidget::TestingWidget(QWidget* parent)
    : QWidget(parent) {
    setupUI();
    createDataFile();
}

TestingWidget::~TestingWidget() {}

// void TestingWidget::setupUI() {
//     QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
//     QHBoxLayout* btnLayout = new QHBoxLayout();
//     autoTestsBtn = new QPushButton("Автоматические тесты");
//     largeDataBtn = new QPushButton("Тест больших данных");
//     btnLayout->addWidget(autoTestsBtn);
//     btnLayout->addWidget(largeDataBtn);
//     btnLayout->addStretch();
//     mainLayout->addLayout(btnLayout);
    
//     QGroupBox* largeDataGroup = new QGroupBox("Настройка теста больших данных");
//     QHBoxLayout* largeDataLayout = new QHBoxLayout(largeDataGroup);

//     largeDataTypeCombo = new QComboBox();
//     largeDataTypeCombo->addItem("Арифметическая (1, 2, 3, ...)");
//     largeDataTypeCombo->addItem("Чётные числа (2, 4, 6, ...)");
//     largeDataTypeCombo->addItem("Случайные числа (1-50000)");
    
//     largeDataLayout->addWidget(new QLabel("Тип последовательности:"));
//     largeDataTypeCombo = new QComboBox();
//     // largeDataTypeCombo->addItem("Арифметическая (1, 2, 3, ...)");
//     // largeDataTypeCombo->addItem("Случайные числа");
//     // largeDataTypeCombo->addItem("Постоянные числа");
//     largeDataLayout->addWidget(largeDataTypeCombo);
    
//     largeDataLayout->addWidget(new QLabel("Количество элементов:"));
//     largeDataSizeSpin = new QSpinBox();
//     largeDataSizeSpin->setRange(1, 1000000);
//     largeDataSizeSpin->setValue(1000000);
//     largeDataLayout->addWidget(largeDataSizeSpin);
    
//     largeDataLayout->addStretch();
//     mainLayout->addWidget(largeDataGroup);
    
//     progressBar = new QProgressBar();
//     progressBar->setVisible(false);
//     mainLayout->addWidget(progressBar);
    
//     QGroupBox* manualGroup = new QGroupBox("Ручной ввод данных");
//     manualGroup->setMaximumHeight(250);
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
//     testTable->setColumnCount(11);
//     testTable->setHorizontalHeaderLabels({
//         "Тест", "Статус",
//         "Min", "Max", "Range",
//         "Среднее", "Дисперсия", "Ст. откл.",
//         "RMS", "CV", "Время"
//     });
//     testTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
//     testTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  
//     testTable->setSelectionBehavior(QAbstractItemView::SelectRows);  
//     mainLayout->addWidget(testTable);
    
//     connect(autoTestsBtn, &QPushButton::clicked, this, &TestingWidget::onRunAutoTests);
//     connect(largeDataBtn, &QPushButton::clicked, this, &TestingWidget::onRunLargeData);
//     connect(calcBtn, &QPushButton::clicked, this, &TestingWidget::onRunManual);
// }


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
    
    QGroupBox* manualGroup = new QGroupBox("Ручной ввод данных");
    manualGroup->setMaximumHeight(250);
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
    testTable->setColumnCount(11);
    testTable->setHorizontalHeaderLabels({
        "Тест", "Статус",
        "Min", "Max", "Range",
        "Среднее", "Дисперсия", "Ст. откл.",
        "RMS", "CV", "Время"
    });
    testTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    testTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  
    testTable->setSelectionBehavior(QAbstractItemView::SelectRows);  
    mainLayout->addWidget(testTable);
    
    connect(autoTestsBtn, &QPushButton::clicked, this, &TestingWidget::onRunAutoTests);
    connect(largeDataBtn, &QPushButton::clicked, this, &TestingWidget::onRunLargeData);
    connect(calcBtn, &QPushButton::clicked, this, &TestingWidget::onRunManual);
     connect(largeDataTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TestingWidget::onDataTypeChanged);
}

// void TestingWidget::onDataTypeChanged(int index) {
//     int maxElements = 0;
    
//     switch (index) {
//         case 0:  // Арифметическая
//             maxElements = 1000000;  // 1 млн
//             break;
//         case 1:  // Чётные числа
//             maxElements = 1000000;  // 1 млн
//             break;
//         case 2:  // Случайные числа
//             maxElements = 5000000;  // 5 млн
//             break;
//         default:
//             maxElements = 1000000;
//     }
    
//     largeDataSizeSpin->setMaximum(maxElements);
    
//     // Если текущее значение больше нового максимума, уменьшаем
//     if (largeDataSizeSpin->value() > maxElements) {
//         largeDataSizeSpin->setValue(maxElements);
//     }
// }


void TestingWidget::onDataTypeChanged(int index) {
    int maxElements = 0;
    int defaultValue = 0;
    
    switch (index) {
        case 0:  // Арифметическая
            maxElements = 1000000;
            defaultValue = 1000000;  // по умолчанию 1 млн
            break;
        case 1:  // Чётные числа
            maxElements = 1000000;
            defaultValue = 1000000;  // по умолчанию 1 млн
            break;
        case 2:  // Случайные числа
            maxElements = 5000000;
            defaultValue = 5000000;  // по умолчанию 5 млн
            break;
        default:
            maxElements = 1000000;
            defaultValue = 1000000;
    }
    
    largeDataSizeSpin->setMaximum(maxElements);
    largeDataSizeSpin->setValue(defaultValue);
}

































































// void TestingWidget::onRunLargeData() {
//     int dataType = largeDataTypeCombo->currentIndex();
//     int dataSize = largeDataSizeSpin->value();
    
//     if (dataType == 2) {  
//         QMessageBox::information(this, "Информация", 
//             "Для теста больших данных доступны только:\n"
//             "- Арифметическая\n"
//             "- Случайные числа\n"
//             "- Постоянные числа");
//         return;
//     }
    
//     QString typeName;
//     switch (dataType) {
//         case 0: typeName = "Арифметическая"; break;
//         case 1: typeName = "Случайные числа"; break;
//         case 3: typeName = "Постоянные числа"; break;
//         default: return;
//     }
    
//     if (dataSize > 100000) {
//         dataSize = 100000;
//         largeDataSizeSpin->setValue(100000);
//         int answer = QMessageBox::question(this, "Предупреждение", 
//             QString("Обработка %1 элементов может занять время.\nПродолжить?").arg(dataSize),
//             QMessageBox::Yes | QMessageBox::No);
//         if (answer != QMessageBox::Yes) {
//             return;
//         }
//     }
    
//     QMessageBox::information(this, "Тест больших данных", 
//         QString("Запуск теста на %1 элементов (%2)...").arg(dataSize).arg(typeName));
    
//     auto start = std::chrono::high_resolution_clock::now();
    
//     try {
//         LazySequenceController testController;
        
//         switch (dataType) {
//             case 0:
//                 testController.setGenerator(LazySequenceController::GEN_ARITHMETIC, 1, 1);
//                 break;
//             case 1:
//                 testController.setGenerator(LazySequenceController::GEN_RANDOM, 0, 1000);
//                 break;
//             case 3:
//                 testController.setGenerator(LazySequenceController::GEN_CONSTANT, 42);
//                 break;
//         }
        
//         OnlineStatistics<long long> stats;
        
//         for (int i = 0; i < dataSize; ++i) {
//             stats.Update(testController.getElement(i));
//         }
        
//         auto end = std::chrono::high_resolution_clock::now();
//         double timeSec = std::chrono::duration<double>(end - start).count();
        
//         QString result = QString(
//             "Тип: %1\n"
//             "Размер: %2 элементов\n"
//             "Время: %3 сек\n"
//             "Минимум: %4\n"
//             "Максимум: %5\n"
//             "Размах: %6\n"
//             "Среднее: %7\n"
//             "Дисперсия: %8\n"
//             "Ст. отклонение: %9\n"
//             "Среднеквадратическое: %10\n"
//             "Коэф. вариации: %11\n")
//             .arg(typeName)
//             .arg(stats.GetCount())
//             .arg(QString::number(timeSec, 'f', 2))
//             .arg(stats.GetMin())
//             .arg(stats.GetMax())
//             .arg(QString::number(stats.GetRange(), 'f', 2))
//             .arg(QString::number(stats.GetAverage(), 'f', 2))
//             .arg(QString::number(stats.GetVariance(), 'f', 2))
//             .arg(QString::number(stats.GetStdDeviation(), 'f', 2))
//             .arg(QString::number(stats.GetRMS(), 'f', 2))
//             .arg(QString::number(stats.GetCoefficientOfVariation(), 'f', 4));
        
//         QMessageBox::information(this, "Результат теста больших данных", result);
        
//     } catch (const std::exception& e) {
//         QMessageBox::warning(this, "Ошибка", e.what());
//     }
// }

// void TestingWidget::onRunLargeData() {
//     int dataType = largeDataTypeCombo->currentIndex();
//     int dataSize = largeDataSizeSpin->value();
    
//     // Ограничиваем только 3 типа для теста больших данных
//     if (dataType == 2) {
//         QMessageBox::information(this, "Информация", 
//             "Для теста больших данных доступны только:\n"
//             "- Арифметическая\n"
//             "- Случайные числа\n"
//             "- Постоянные числа");
//         return;
//     }
    
//     if (dataSize > 100000) {
//         dataSize = 100000;
//         largeDataSizeSpin->setValue(100000);
//         int answer = QMessageBox::question(this, "Предупреждение", 
//             QString("Обработка %1 элементов может занять время.\nПродолжить?").arg(dataSize),
//             QMessageBox::Yes | QMessageBox::No);
//         if (answer != QMessageBox::Yes) {
//             return;
//         }
//     }
    
//     try {
//         LazySequenceController testController;
//         testController.setGenerator(LazySequenceController::GEN_ARITHMETIC, 1, 1);
        
//         // ========== ЗАМЕР 1: только getElement ==========
//         auto t1 = std::chrono::high_resolution_clock::now();
//         for (int i = 0; i < dataSize; ++i) {
//             testController.getElement(i);
//         }
//         auto t2 = std::chrono::high_resolution_clock::now();
//         double getElement_time = std::chrono::duration<double, std::milli>(t2 - t1).count();
        
//         // ========== ЗАМЕР 2: только Update с простыми числами ==========
//         OnlineStatistics<long long> stats2;
//         auto t3 = std::chrono::high_resolution_clock::now();
//         for (int i = 0; i < dataSize; ++i) {
//             stats2.Update(i + 1);
//         }
//         auto t4 = std::chrono::high_resolution_clock::now();
//         double update_time = std::chrono::duration<double, std::milli>(t4 - t3).count();
        
//         // ========== ЗАМЕР 3: вместе (как было) ==========
//         LazySequenceController testController2;
//         testController2.setGenerator(LazySequenceController::GEN_ARITHMETIC, 1, 1);
//         OnlineStatistics<long long> stats3;
        
//         auto t5 = std::chrono::high_resolution_clock::now();
//         for (int i = 0; i < dataSize; ++i) {
//             stats3.Update(testController2.getElement(i));
//         }
//         auto t6 = std::chrono::high_resolution_clock::now();
//         double together_time = std::chrono::duration<double, std::milli>(t6 - t5).count();
        
//         QString result = QString(
//             "═══════════════════════════════════════════════════════════════════\n"
//             "Размер: %1 элементов\n"
//             "═══════════════════════════════════════════════════════════════════\n"
//             "getElement() только: %2 мс\n"
//             "Update() только (простые числа): %3 мс\n"
//             "Вместе (getElement + Update): %4 мс\n"
//             "═══════════════════════════════════════════════════════════════════\n"
//             "Если getElement ~17 сек, проблема в LazySequence\n"
//             "Если Update ~17 сек, проблема в OnlineStatistics\n"
//             "Если вместе ~17 сек, а по отдельности мало - проблема в их совместной работе")
//             .arg(dataSize)
//             .arg(getElement_time, 0, 'f', 2)
//             .arg(update_time, 0, 'f', 2)
//             .arg(together_time, 0, 'f', 2);
        
//         QMessageBox::information(this, "Результаты замеров", result);
        
//     } catch (const std::exception& e) {
//         QMessageBox::warning(this, "Ошибка", e.what());
//     }
// }





// void TestingWidget::onRunLargeData() {
//     int dataType = largeDataTypeCombo->currentIndex();
//     int dataSize = largeDataSizeSpin->value();
    
//     // Только для арифметической прогрессии
//     if (dataType != 0) {
//         QMessageBox::information(this, "Информация", 
//             "Тест больших данных работает с арифметической прогрессией.\n"
//             "Данные читаются из файла large_data.txt (1 000 000 чисел).");
//         return;
//     }
    
//     QString filename = QCoreApplication::applicationDirPath() + "/large_data.txt";
//     QFile file(filename);
    
//     if (!file.exists()) {
//         QMessageBox::warning(this, "Ошибка", 
//             "Файл large_data.txt не найден.\n"
//             "Он будет создан при следующем запуске.");
//         createDataFile();
//         return;
//     }
    
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
//         return;
//     }
    
//     QMessageBox::information(this, "Тест больших данных", 
//         QString("Чтение %1 элементов из файла large_data.txt...").arg(dataSize));
    
//     auto start = std::chrono::high_resolution_clock::now();
    
//     try {
//         OnlineStatistics<long long> stats;
//         QTextStream in(&file);
//         long long value;
//         int count = 0;
        
//         while (count < dataSize && !in.atEnd()) {
//             in >> value;
//             stats.Update(value);
//             count++;
//         }
        
//         file.close();
        
//         auto end = std::chrono::high_resolution_clock::now();
//         double timeSec = std::chrono::duration<double>(end - start).count();
        
//         QString result = QString(
//             "═══════════════════════════════════════════════════════════════════\n"
//             "Тест чтения из файла (large_data.txt)\n"
//             "Размер: %1 элементов\n"
//             "Время: %2 сек\n"
//             "═══════════════════════════════════════════════════════════════════\n"
//             "Минимум: %3\n"
//             "Максимум: %4\n"
//             "Размах: %5\n"
//             "Среднее: %6\n"
//             "Дисперсия: %7\n"
//             "Ст. отклонение: %8\n"
//             "Среднеквадратическое: %9\n"
//             "Коэф. вариации: %10\n"
//             "═══════════════════════════════════════════════════════════════════")
//             .arg(stats.GetCount())
//             .arg(QString::number(timeSec, 'f', 2))
//             .arg(stats.GetMin())
//             .arg(stats.GetMax())
//             .arg(QString::number(stats.GetRange(), 'f', 2))
//             .arg(QString::number(stats.GetAverage(), 'f', 2))
//             .arg(QString::number(stats.GetVariance(), 'f', 2))
//             .arg(QString::number(stats.GetStdDeviation(), 'f', 2))
//             .arg(QString::number(stats.GetRMS(), 'f', 2))
//             .arg(QString::number(stats.GetCoefficientOfVariation(), 'f', 4));
        
//         QMessageBox::information(this, "Результат теста больших данных", result);
        
//     } catch (const std::exception& e) {
//         QMessageBox::warning(this, "Ошибка", e.what());
//         file.close();
//     }
// }


// void TestingWidget::onRunLargeData() {
//     int dataType = largeDataTypeCombo->currentIndex();
//     int dataSize = largeDataSizeSpin->value();
    
//     if (dataType != 0) {
//         QMessageBox::information(this, "Информация", 
//             "Тест больших данных работает с арифметической прогрессией.\n"
//             "Данные читаются из файла large_data.txt");
//         return;
//     }
    
//     QString filename = QCoreApplication::applicationDirPath() + "/large_data.txt";
//     QFile file(filename);
    
//     if (!file.exists()) {
//         QMessageBox::warning(this, "Ошибка", 
//             "Файл large_data.txt не найден.\n"
//             "Он будет создан при следующем запуске.");
//         createDataFile();
//         return;
//     }
    
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
//         return;
//     }
    
//     QMessageBox::information(this, "Тест больших данных", 
//         QString("Чтение %1 элементов из файла large_data.txt...").arg(dataSize));
    
//     auto start = std::chrono::high_resolution_clock::now();
    
//     try {
//         OnlineStatistics<long long> stats;
//         QTextStream in(&file);
//         long long value;
//         int count = 0;
//         int errorLine = 0;
//         QString errorToken;
        
//         while (count < dataSize && !in.atEnd()) {
//             errorLine = count + 1;
//             in >> value;
            
           
//             if (in.status() != QTextStream::Ok) {
//                 in >> errorToken;  
//                 file.close();
//                 QMessageBox::warning(this, "Ошибка в файле", 
//                     QString("Ошибка чтения на строке %1\n"
//                             "Найдено: '%2'\n"
//                             "Ожидалось: число")
//                     .arg(errorLine).arg(errorToken));
//                 return;
//             }
            
//             stats.Update(value);
//             count++;
//         }
        
//         file.close();
        
//         if (count == 0) {
//             QMessageBox::warning(this, "Ошибка", "Файл пуст или не содержит чисел");
//             return;
//         }
        
//         auto end = std::chrono::high_resolution_clock::now();
//         double timeSec = std::chrono::duration<double>(end - start).count();
        
//         QString result = QString(
//             "Тест чтения из файла (large_data.txt)\n"
//             "Обработано: %1 элементов\n"
//             "Время: %2 сек\n"
//             "Минимум: %3\n"
//             "Максимум: %4\n"
//             "Размах: %5\n"
//             "Среднее: %6\n"
//             "Дисперсия: %7\n"
//             "Ст. отклонение: %8\n"
//             "Среднеквадратическое: %9\n"
//             "Коэф. вариации: %10\n")
//             .arg(stats.GetCount())
//             .arg(QString::number(timeSec, 'f', 2))
//             .arg(stats.GetMin())
//             .arg(stats.GetMax())
//             .arg(QString::number(stats.GetRange(), 'f', 2))
//             .arg(QString::number(stats.GetAverage(), 'f', 2))
//             .arg(QString::number(stats.GetVariance(), 'f', 2))
//             .arg(QString::number(stats.GetStdDeviation(), 'f', 2))
//             .arg(QString::number(stats.GetRMS(), 'f', 2))
//             .arg(QString::number(stats.GetCoefficientOfVariation(), 'f', 4));
        
//         QMessageBox::information(this, "Результат теста больших данных", result);
        
//     } catch (const std::exception& e) {
//         QMessageBox::warning(this, "Ошибка", e.what());
//         file.close();
//     }
// }

// void TestingWidget::onRunLargeData() {
//     int dataType = largeDataTypeCombo->currentIndex();
//     int dataSize = largeDataSizeSpin->value();
    
//     QString typeName;
//     QString filename;
    
//     switch (dataType) {
//         case 0:
//             typeName = "Арифметическая (1, 2, 3, ...)";
//             filename = QCoreApplication::applicationDirPath() + "/large_data_arithmetic.txt";
//             break;
//         case 1:
//             typeName = "Чётные числа (2, 4, 6, ...)";
//             filename = QCoreApplication::applicationDirPath() + "/large_data_even.txt";
//             break;
//         case 2:
//             typeName = "Случайные числа (1-50000)";
//             filename = QCoreApplication::applicationDirPath() + "/large_data_random_5m.txt";
//             break;
//         default:
//             QMessageBox::warning(this, "Ошибка", "Неизвестный тип данных");
//             return;
//     }
    
//     QFile file(filename);
    
//     if (!file.exists()) {
//         QMessageBox::warning(this, "Ошибка", 
//             QString("Файл %1 не найден.\n"
//                     "Он будет создан при следующем запуске программы.\n"
//                     "Нажмите OK и перезапустите программу.").arg(filename));
//         createDataFile();
//         return;
//     }
    
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
//         return;
//     }
    
//     QMessageBox::information(this, "Тест больших данных", 
//         QString("Тип: %1\n"
//                 "Файл: %2\n"
//                 "Чтение %3 элементов...")
//         .arg(typeName)
//         .arg(filename)
//         .arg(dataSize));
    
//     auto start = std::chrono::high_resolution_clock::now();
    
//     try {
//         OnlineStatistics<long long> stats;
//         QTextStream in(&file);
//         long long value;
//         int count = 0;
//         int errorLine = 0;
//         QString errorToken;
        
//         while (count < dataSize && !in.atEnd()) {
//             errorLine = count + 1;
//             in >> value;
            
//             if (in.status() != QTextStream::Ok) {
//                 in >> errorToken;
//                 file.close();
//                 QMessageBox::warning(this, "Ошибка в файле", 
//                     QString("Ошибка чтения на строке %1\n"
//                             "Найдено: '%2'\n"
//                             "Ожидалось: число")
//                     .arg(errorLine).arg(errorToken));
//                 return;
//             }
            
//             stats.Update(value);
//             count++;
//         }
        
//         file.close();
        
//         if (count == 0) {
//             QMessageBox::warning(this, "Ошибка", "Файл пуст или не содержит чисел");
//             return;
//         }
        
//         auto end = std::chrono::high_resolution_clock::now();
//         double timeSec = std::chrono::duration<double>(end - start).count();
        
//         QString result = QString(
//             "Тип: %1\n"
//             "Файл: %2\n"
//             "Обработано: %3 элементов\n"
//             "Время: %4 сек\n"
//             "Минимум: %5\n"
//             "Максимум: %6\n"
//             "Размах: %7\n"
//             "Среднее: %8\n"
//             "Дисперсия: %9\n"
//             "Ст. отклонение: %10\n"
//             "Среднеквадратическое: %11\n"
//             "Коэф. вариации: %12\n")
//             .arg(typeName)
//             .arg(filename)
//             .arg(stats.GetCount())
//             .arg(QString::number(timeSec, 'f', 2))
//             .arg(stats.GetMin())
//             .arg(stats.GetMax())
//             .arg(QString::number(stats.GetRange(), 'f', 2))
//             .arg(QString::number(stats.GetAverage(), 'f', 2))
//             .arg(QString::number(stats.GetVariance(), 'f', 2))
//             .arg(QString::number(stats.GetStdDeviation(), 'f', 2))
//             .arg(QString::number(stats.GetRMS(), 'f', 2))
//             .arg(QString::number(stats.GetCoefficientOfVariation(), 'f', 4));
        
//         QMessageBox::information(this, "Результат теста больших данных", result);
        
//     } catch (const std::exception& e) {
//         QMessageBox::warning(this, "Ошибка", e.what());
//         file.close();
//     }
// }

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
            "═══════════════════════════════════════════════════════════════════\n"
            "Тест ReadOnlyStream (чтение из файла)\n"
            "Тип: %1\n"
            "═══════════════════════════════════════════════════════════════════\n"
            "Обработано: %2 элементов\n"
            "Время: %3 сек\n"
            "═══════════════════════════════════════════════════════════════════\n"
            "Минимум: %4\n"
            "Максимум: %5\n"
            "Размах: %6\n"
            "Среднее: %7\n"
            "Дисперсия: %8\n"
            "Ст. отклонение: %9\n"
            "Среднеквадратическое: %10\n"
            "Коэф. вариации: %11\n"
            "═══════════════════════════════════════════════════════════════════")
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
        r.name = "Случайные числа (0..1000)";
        r.expectedMin = 0;
        r.expectedMax = 1000;
        
        try {
            auto start = std::chrono::high_resolution_clock::now();
            
            LazySequenceController controller;
            controller.setGenerator(LazySequenceController::GEN_RANDOM, 0, 1000);
            
            ArraySequence<long long> dataCopy;
            for (int i = 0; i < 1000; ++i) {
                dataCopy.Append(controller.getElement(i));
            }
            
            LazySequence<long long> tempSeq(&dataCopy);
            ReadOnlyStream<long long> stream(&tempSeq);
            
            OnlineStatistics<long long> stats;
            stream.Open();
            for (int i = 0; i < 1000 && !stream.IsEndOfStream(); ++i) {
                stats.Update(stream.Read());
            }
            stream.Close();
            
            r.actualMin = stats.GetMin();
            r.actualMax = stats.GetMax();
            
            r.passed = (r.actualMin >= r.expectedMin && r.actualMax <= r.expectedMax);
            
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
    stream.Open();
    while (!stream.IsEndOfStream()) {
        stats.Update(stream.Read());
    }
    stream.Close();
    
    manualResult->setText(
        QString("Результаты обработки (%1 элементов):\n"
        "Минимум: %2\n"
        "Максимум: %3\n"
        "Среднее: %4\n"
        "Дисперсия: %5\n"
        "Стандартное отклонение: %6\n")
        .arg(stats.GetCount())
        .arg(stats.GetMin())
        .arg(stats.GetMax())
        .arg(QString::number(stats.GetAverage(), 'f', 2))
        .arg(QString::number(stats.GetVariance(), 'f', 2))
        .arg(QString::number(stats.GetStdDeviation(), 'f', 2)));
}



