#include "LazySequenceWidget.hpp"
#include "LazySequenceController.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

LazySequenceWidget::LazySequenceWidget(QWidget* parent)
    : QWidget(parent)
    , controller(new LazySequenceController(this)) {
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
    generatorCombo->addItem("Арифметическая (первый элемент, шаг)");
    generatorCombo->addItem("Случайные числа (min, max)");
    generatorCombo->addItem("Числа Фибоначчи");
    generatorCombo->addItem("Постоянные числа");
    genLayout->addWidget(generatorCombo);
    
    genLayout->addWidget(new QLabel("Параметр 1:"));
    param1Spin = new QSpinBox();
    param1Spin->setRange(-1000000, 1000000);
    param1Spin->setValue(1);
    genLayout->addWidget(param1Spin);
    
    genLayout->addWidget(new QLabel("Параметр 2:"));
    param2Spin = new QSpinBox();
    param2Spin->setRange(-1000000, 1000000);
    param2Spin->setValue(1);
    genLayout->addWidget(param2Spin);
    
    applyGenBtn = new QPushButton("Применить");
    genLayout->addWidget(applyGenBtn);
    genLayout->addStretch();
    mainLayout->addWidget(genGroup);
    
    QGroupBox* matGroup = new QGroupBox("Материализация");
    QHBoxLayout* matLayout = new QHBoxLayout(matGroup);
    
    matLayout->addWidget(new QLabel("Сгенерировать первых:"));
    generateCountSpin = new QSpinBox();
    generateCountSpin->setRange(1, 100000);
    generateCountSpin->setValue(100);
    matLayout->addWidget(generateCountSpin);
    matLayout->addWidget(new QLabel("элементов"));
    
    generateBtn = new QPushButton("Сгенерировать");
    matLayout->addWidget(generateBtn);
    matLayout->addStretch();
    mainLayout->addWidget(matGroup);
    
    infoLabel = new QLabel();
    infoLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 8px; }");
    mainLayout->addWidget(infoLabel);
    
    elementsList = new QListWidget();
    elementsList->setWordWrap(true);
    elementsList->setMaximumHeight(300);
    elementsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mainLayout->addWidget(elementsList);
    
    QLabel* hintLabel = new QLabel(
        "При количестве элементов > 1000 показываются первые 100 и последние 10.\n"
        "При количестве <= 1000 показываются все элементы.\n"
        "Для чисел Фибоначчи максимальное количество - 93 элемента."
    );
    hintLabel->setStyleSheet("color: gray; font-size: 11px;");
    mainLayout->addWidget(hintLabel);

    connect(generatorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &LazySequenceWidget::onGeneratorTypeChanged);
    connect(applyGenBtn, &QPushButton::clicked, this, &LazySequenceWidget::onApplyGenerator);
    connect(generateBtn, &QPushButton::clicked, this, &LazySequenceWidget::onGenerateElements);
}

void LazySequenceWidget::onGeneratorTypeChanged(int index) {
    if (index == 2) {  
        generateCountSpin->setMaximum(93);
        if (generateCountSpin->value() > 93) {
            generateCountSpin->setValue(93);
        }
    } else {
        generateCountSpin->setMaximum(1000000);
    }
}

void LazySequenceWidget::onApplyGenerator() {
    int type = generatorCombo->currentIndex();
    
    switch (type) {
        case 0:
            controller->setGenerator(LazySequenceController::GEN_ARITHMETIC, param1Spin->value(), param2Spin->value());
            break;
        case 1:
            controller->setGenerator(LazySequenceController::GEN_RANDOM, param1Spin->value(), param2Spin->value());
            break;
        case 2:
            controller->setGenerator(LazySequenceController::GEN_FIBONACCI);
            if (generateCountSpin->value() > 93) {
                generateCountSpin->setValue(93);
            }
            break;
        case 3:
            controller->setGenerator(LazySequenceController::GEN_CONSTANT, param1Spin->value());
            break;
    }
}

void LazySequenceWidget::onGenerateElements() {
    int count = generateCountSpin->value();
    controller->generateFirstNElements(count);
}

void LazySequenceWidget::onSequenceChanged() {
    updateDisplay();
}

void LazySequenceWidget::updateDisplay() {
    size_t materialized = controller->getMaterializedCount();
    bool isInfinite = controller->isInfinite();
    
    infoLabel->setText(QString("Состояние: %1 | Материализовано: %2 элементов | Всего: %3")
        .arg(isInfinite ? "Бесконечная" : "Конечная")
        .arg(materialized)
        .arg(isInfinite ? "бесконечно" : QString::number(materialized)));
    
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