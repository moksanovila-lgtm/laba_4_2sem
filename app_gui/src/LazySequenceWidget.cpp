#include "LazySequenceWidget.hpp"
#include "StatisticsWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

LazySequenceWidget::LazySequenceWidget(QTabWidget* tabWidget, QWidget* parent)
    : QWidget(parent)
    , controller(new LazySequenceController(this))
    , parentTabWidget(tabWidget)
    , statsTabCounter(1) {
    setupUI();
    
    connect(controller, &LazySequenceController::sequenceChanged,
            this, &LazySequenceWidget::onSequenceChanged);
    connect(controller, &LazySequenceController::materializedCountChanged,  
            this, &LazySequenceWidget::onMaterializedCountChanged);     
    connect(controller, &LazySequenceController::error,
            this, &LazySequenceWidget::onError);
    
    onApplyGenerator();
}

LazySequenceWidget::~LazySequenceWidget() {}

void LazySequenceWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QGroupBox* genGroup = new QGroupBox("Генератор последовательности");
    QHBoxLayout* genLayout = new QHBoxLayout(genGroup);
    
    genLayout->addWidget(new QLabel("Тип:"));
    generatorCombo = new QComboBox();
    generatorCombo->addItem("Арифметическая (1 парам - первый элемент, 2 парам - шаг)");
    generatorCombo->addItem("Случайные числа (1 парам - min, 2 парам - max)");
    generatorCombo->addItem("Постоянные числа (1 парам - число)");
    genLayout->addWidget(generatorCombo);
    
    genLayout->addWidget(new QLabel("Параметр 1:"));
    param1Spin = new QSpinBox();
    param1Spin->setRange(-100000, 100000);
    param1Spin->setValue(1);
    genLayout->addWidget(param1Spin);
    
    genLayout->addWidget(new QLabel("Параметр 2:"));
    param2Spin = new QSpinBox();
    param2Spin->setRange(-100000, 100000);
    param2Spin->setValue(1);
    genLayout->addWidget(param2Spin);
    
    applyGenBtn = new QPushButton("Применить");
    genLayout->addWidget(applyGenBtn);
    genLayout->addStretch();
    mainLayout->addWidget(genGroup);
    
    QGroupBox* matGroup = new QGroupBox("Материализация");
    QHBoxLayout* matLayout = new QHBoxLayout(matGroup);
    
    matLayout->addWidget(new QLabel("Сгенерировать:"));
    generateCountSpin = new QSpinBox();
    generateCountSpin->setRange(1, 20000);
    generateCountSpin->setValue(100);
    matLayout->addWidget(generateCountSpin);
    matLayout->addWidget(new QLabel("элементов"));

    generateBtn = new QPushButton("Сгенерировать");
    matLayout->addWidget(generateBtn);
    matLayout->addStretch();
    mainLayout->addWidget(matGroup);
    
    QGroupBox* statsGroup = new QGroupBox("Сбор статистики");
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);
    
    QHBoxLayout* firstLayout = new QHBoxLayout();
    firstRadio = new QRadioButton("Первые");
    firstRadio->setChecked(true);
    firstLayout->addWidget(firstRadio);
    firstCountSpin = new QSpinBox();
    firstCountSpin->setRange(1, 100000);
    firstCountSpin->setValue(10);
    firstLayout->addWidget(firstCountSpin);
    firstLayout->addWidget(new QLabel("элементов"));
    firstLayout->addStretch();
    statsLayout->addLayout(firstLayout);
    
    QHBoxLayout* rangeLayout = new QHBoxLayout();
    rangeRadio = new QRadioButton("Диапазон");
    rangeLayout->addWidget(rangeRadio);
    rangeFromSpin = new QSpinBox();
    rangeFromSpin->setRange(1, 100000);
    rangeFromSpin->setValue(1);
    rangeLayout->addWidget(rangeFromSpin);
    rangeLayout->addWidget(new QLabel("до"));
    rangeToSpin = new QSpinBox();
    rangeToSpin->setRange(1, 100000);
    rangeToSpin->setValue(10);
    rangeLayout->addWidget(rangeToSpin);
    rangeLayout->addWidget(new QLabel("(включительно)"));
    rangeLayout->addStretch();
    statsLayout->addLayout(rangeLayout);
    
    QHBoxLayout* lastLayout = new QHBoxLayout();
    lastRadio = new QRadioButton("Последние");
    lastLayout->addWidget(lastRadio);
    lastCountSpin = new QSpinBox();
    lastCountSpin->setRange(1, 100000);
    lastCountSpin->setValue(10);
    lastLayout->addWidget(lastCountSpin);
    lastLayout->addWidget(new QLabel("элементов"));
    lastLayout->addStretch();
    statsLayout->addLayout(lastLayout);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    collectStatsBtn = new QPushButton("Собрать статистику");
    collectStatsBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    btnLayout->addStretch();
    btnLayout->addWidget(collectStatsBtn);
    btnLayout->addStretch();
    statsLayout->addLayout(btnLayout);
    
    mainLayout->addWidget(statsGroup);
    
    infoLabel = new QLabel();
    infoLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 8px; }");
    mainLayout->addWidget(infoLabel);
    
    elementsList = new QListWidget();
    elementsList->setWordWrap(true);
    elementsList->setMaximumHeight(300);
    elementsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mainLayout->addWidget(elementsList);  
    
    QLabel* hintLabel = new QLabel(
        "Подсказки:\n"
        " При количестве элементов > 1000 показываются первые 100 и последние 10\n"
        " Статистику можно собирать с любых элементов последовательности\n"
        " Результаты открываются в новой вкладке"
    );
    hintLabel->setStyleSheet("color: gray; font-size: 11px;");
    mainLayout->addWidget(hintLabel);
    
    connect(generatorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LazySequenceWidget::onGeneratorTypeChanged);
    connect(applyGenBtn, &QPushButton::clicked, this, &LazySequenceWidget::onApplyGenerator);
    connect(generateBtn, &QPushButton::clicked, this, &LazySequenceWidget::onGenerateElements);
    connect(collectStatsBtn, &QPushButton::clicked, this, &LazySequenceWidget::onCollectStatistics);
    
    connect(firstRadio, &QRadioButton::toggled, this, &LazySequenceWidget::onStatsModeChanged);
    connect(rangeRadio, &QRadioButton::toggled, this, &LazySequenceWidget::onStatsModeChanged);
    connect(lastRadio, &QRadioButton::toggled, this, &LazySequenceWidget::onStatsModeChanged);
    
    onStatsModeChanged();
}

void LazySequenceWidget::onStatsModeChanged() {
    bool firstMode = firstRadio->isChecked();
    bool rangeMode = rangeRadio->isChecked();
    bool lastMode = lastRadio->isChecked();
    
    firstCountSpin->setEnabled(firstMode);
    rangeFromSpin->setEnabled(rangeMode);
    rangeToSpin->setEnabled(rangeMode);
    lastCountSpin->setEnabled(lastMode);
    
    if (firstMode && firstCountSpin->value() < 1) {
        firstCountSpin->setValue(1);
    }
    
    if (rangeMode) {
        if (rangeFromSpin->value() < 1) {
            rangeFromSpin->setValue(1);
        }
        if (rangeToSpin->value() < 1) {
            rangeToSpin->setValue(1);
        }
        if (rangeFromSpin->value() > rangeToSpin->value()) {
            rangeToSpin->setValue(rangeFromSpin->value());
        }
    }
    if (lastMode && lastCountSpin->value() < 1) {
        lastCountSpin->setValue(1);
    }
}

void LazySequenceWidget::onGeneratorTypeChanged(int index) {
    int maxVal = 100000;
    
    generateCountSpin->setMaximum(maxVal);
    firstCountSpin->setMaximum(maxVal);
    rangeFromSpin->setMaximum(maxVal - 1);
    rangeToSpin->setMaximum(maxVal);
    lastCountSpin->setMaximum(maxVal);
    
    if (generateCountSpin->value() > maxVal) {
        generateCountSpin->setValue(maxVal);
    }
    if (firstCountSpin->value() > maxVal) {
        firstCountSpin->setValue(maxVal);
    }
    if (lastCountSpin->value() > maxVal) {
        lastCountSpin->setValue(maxVal);
    }
}

void LazySequenceWidget::onApplyGenerator() {
    int type = generatorCombo->currentIndex();
    if (type == 1) {
        int minVal = param1Spin->value();
        int maxVal = param2Spin->value();
        
        if (minVal >= maxVal) {
            QMessageBox::warning(this, "Ошибка", 
                "Для случайных чисел максимальное значение (Параметр 2)\n"
                "должно быть больше минимального (Параметр 1).\n\n"
                "Пожалуйста, исправьте значения и нажмите 'Применить' снова.");
            return;  
        }
    }
    switch (type) {
        case 0:
            controller->setGenerator(LazySequenceController::GEN_ARITHMETIC, 
                                     param1Spin->value(), param2Spin->value());
            break;
        case 1:
            controller->setGenerator(LazySequenceController::GEN_RANDOM,
                                     param1Spin->value(), param2Spin->value());
            break;
        case 2:
            controller->setGenerator(LazySequenceController::GEN_CONSTANT,
                                     param1Spin->value());
            break;
    }
}

void LazySequenceWidget::onGenerateElements() {
    int count = generateCountSpin->value();
    controller->generateFirstNElements(count);
}

void LazySequenceWidget::collectStatisticsForFirst(int count) {
    if (count <= 0) {
        QMessageBox::warning(this, "Ошибка", "Количество элементов должно быть больше 0");
        return;
    }
    
    size_t materializedCount = controller->getMaterializedCount();
    if (static_cast<size_t>(count) > materializedCount) {
        count = static_cast<int>(materializedCount);
    }
    
    ArraySequence<long long>* dataCopy = new ArraySequence<long long>();
    for (int i = 0; i < count; ++i) {
        dataCopy->Append(controller->getElement(i));
    }
    
    LazySequence<long long>* tempSeq = new LazySequence<long long>(dataCopy);
    ReadOnlyStream<long long>* tempStream = new ReadOnlyStream<long long>(tempSeq);
    
    QString tabTitle = QString("Статистика #%1 (первые %2)").arg(statsTabCounter++).arg(count);
    
    StatisticsWidget* statsWidget = new StatisticsWidget(tempStream, count, tabTitle, this);
    parentTabWidget->addTab(statsWidget, tabTitle);
    parentTabWidget->setCurrentWidget(statsWidget);
}

void LazySequenceWidget::collectStatisticsForRange(int start, int end) {
    int startIdx = start - 1;
    int endIdx = end - 1;
    
    if (startIdx < 0 || endIdx < startIdx) {
        QMessageBox::warning(this, "Ошибка", 
            "Некорректный диапазон: начало должно быть меньше или равно концу");
        return;
    }
    
    size_t materializedCount = controller->getMaterializedCount();

    if (static_cast<size_t>(endIdx) >= materializedCount) {
        endIdx = static_cast<int>(materializedCount - 1);
        QMessageBox::information(this, "Корректировка", 
            QString("Конечный индекс скорректирован до %1 (максимум %2)")
            .arg(endIdx + 1) 
            .arg(materializedCount));
    }
    if (static_cast<size_t>(startIdx) >= materializedCount) {
        startIdx = static_cast<int>(materializedCount - 1);
        QMessageBox::information(this, "Корректировка", 
            QString("Начальный индекс скорректирован до %1")
            .arg(startIdx + 1));
    }
    
    int count = endIdx - startIdx + 1;
    
    ArraySequence<long long>* dataCopy = new ArraySequence<long long>();
    for (int i = startIdx; i <= endIdx; ++i) {
        dataCopy->Append(controller->getElement(i));
    }
    
    LazySequence<long long>* tempSeq = new LazySequence<long long>(dataCopy);
    ReadOnlyStream<long long>* tempStream = new ReadOnlyStream<long long>(tempSeq);
    
    QString tabTitle = QString("Статистика #%1 [%2..%3]")
        .arg(statsTabCounter++)
        .arg(start)   
        .arg(end);    
    
    StatisticsWidget* statsWidget = new StatisticsWidget(tempStream, count, tabTitle, this);
    parentTabWidget->addTab(statsWidget, tabTitle);
    parentTabWidget->setCurrentWidget(statsWidget);
}

void LazySequenceWidget::collectStatisticsForLast(int count) {
    if (count <= 0) {
        QMessageBox::warning(this, "Ошибка", "Количество элементов должно быть больше 0");
        return;
    }
    
    size_t materializedCount = controller->getMaterializedCount();
    if (static_cast<size_t>(count) > materializedCount) {
        count = static_cast<int>(materializedCount);
    }
    
    int start = static_cast<int>(materializedCount) - count;
    
    ArraySequence<long long>* dataCopy = new ArraySequence<long long>();
    for (int i = start; i < static_cast<int>(materializedCount); ++i) {
        dataCopy->Append(controller->getElement(i));
    }
    
    LazySequence<long long>* tempSeq = new LazySequence<long long>(dataCopy);
    ReadOnlyStream<long long>* tempStream = new ReadOnlyStream<long long>(tempSeq);
    
    QString tabTitle = QString("Статистика #%1 (последние %2)").arg(statsTabCounter++).arg(count);
    
    StatisticsWidget* statsWidget = new StatisticsWidget(tempStream, count, tabTitle, this);
    parentTabWidget->addTab(statsWidget, tabTitle);
    parentTabWidget->setCurrentWidget(statsWidget);
}

void LazySequenceWidget::onCollectStatistics() {
    size_t materializedCount = controller->getMaterializedCount();
    
    if (materializedCount == 0) {
        QMessageBox::warning(this, "Ошибка", 
            "Нет данных для сбора статистики!\n"
            "Сначала нажмите 'Сгенерировать'.");
        return;
    }
    
    if (firstRadio->isChecked()) {
        collectStatisticsForFirst(firstCountSpin->value());
    } else if (rangeRadio->isChecked()) {
        collectStatisticsForRange(rangeFromSpin->value(), rangeToSpin->value());
    } else if (lastRadio->isChecked()) {
        collectStatisticsForLast(lastCountSpin->value());
    }
}

void LazySequenceWidget::onSequenceChanged() {
    updateDisplay();
}

void LazySequenceWidget::updateDisplay() {
    size_t materialized = controller->getMaterializedCount();
    bool isInfinite = controller->isInfinite();
    
    infoLabel->setText(QString("Состояние: %1 | Вычислено: %2 элементов | Индексы: 1..%3 | Всего: %4")
        .arg(isInfinite ? "Бесконечная" : "Конечная")
        .arg(materialized)
        .arg(materialized)
        .arg(isInfinite ? "бесконечно" : QString::number(materialized)));
    
    int maxVal = static_cast<int>(materialized);
    if (maxVal == 0) maxVal = 1;
    
    firstCountSpin->setMaximum(maxVal);
    rangeFromSpin->setMaximum(maxVal);
    rangeToSpin->setMaximum(maxVal);
    lastCountSpin->setMaximum(maxVal);
    
    elementsList->clear();
    
    if (materialized == 0) {
        elementsList->addItem("[]");
        return;
    }
    
    const size_t FIRST_COUNT = 100;
    const size_t LAST_COUNT = 10;
    const size_t LIMIT = 1000;
    
    QStringList elements;
    
    if (materialized <= LIMIT) {
        for (size_t i = 0; i < materialized; ++i) {
            elements << QString::number(controller->getElement(i));
        }
    } else {
        for (size_t i = 0; i < FIRST_COUNT; ++i) {
            elements << QString::number(controller->getElement(i));
        }
        elements << "...........................";
        for (size_t i = materialized - LAST_COUNT; i < materialized; ++i) {
            elements << QString::number(controller->getElement(i));
        }
    }
    
    elementsList->addItem("[" + elements.join(", ") + "]");
    
    if (materialized > LIMIT) {
        elementsList->addItem(QString("Показано %1 элементов: первые %2 и последние %3")
            .arg(FIRST_COUNT + LAST_COUNT)
            .arg(FIRST_COUNT)
            .arg(LAST_COUNT));
    }
}

void LazySequenceWidget::onError(const QString& msg) {
    QMessageBox::warning(this, "Ошибка", msg);
}

void LazySequenceWidget::onMaterializedCountChanged(size_t count) {
    updateDisplay();
}
