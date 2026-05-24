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
    generatorCombo->addItem("Арифметическая (start, step)");
    generatorCombo->addItem("Случайные числа (min, max)");
    generatorCombo->addItem("Фибоначчи");
    generatorCombo->addItem("Постоянные числа");
    genLayout->addWidget(generatorCombo);
    
    genLayout->addWidget(new QLabel("Параметр 1:"));
    param1Spin = new QSpinBox();
    param1Spin->setRange(-1000, 1000);
    param1Spin->setValue(1);
    genLayout->addWidget(param1Spin);
    
    genLayout->addWidget(new QLabel("Параметр 2:"));
    param2Spin = new QSpinBox();
    param2Spin->setRange(-1000, 1000);
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
    generateCountSpin->setRange(1, 10000);
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
    mainLayout->addWidget(elementsList);
    
    connect(applyGenBtn, &QPushButton::clicked, this, &LazySequenceWidget::onApplyGenerator);
    connect(generateBtn, &QPushButton::clicked, this, &LazySequenceWidget::onGenerateElements);
}

void LazySequenceWidget::onApplyGenerator() {
    int type = generatorCombo->currentIndex();
    
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
            controller->setGenerator(LazySequenceController::GEN_FIBONACCI);
            break;
        case 3:
            controller->setGenerator(LazySequenceController::GEN_CONSTANT,
                                     param1Spin->value());
            break;
    }
}

void LazySequenceWidget::onGenerateElements() {
    controller->generateFirstNElements(generateCountSpin->value());
}

void LazySequenceWidget::onSequenceChanged() {
    updateDisplay();
}

void LazySequenceWidget::updateDisplay() {
    size_t materialized = controller->getMaterializedCount();
    
    infoLabel->setText(QString("Состояние: %1 | Материализовано: %2 элементов")
        .arg(controller->isInfinite() ? "Бесконечная" : "Конечная")
        .arg(materialized));
    
    elementsList->clear();
    
    size_t showCount = std::min(materialized, size_t(50));
    QStringList elements;
    for (size_t i = 0; i < showCount; ++i) {
        elements << QString::number(controller->getElement(i));
    }
    
    elementsList->addItem("[" + elements.join(", ") + 
                          (materialized > 50 ? ", ..." : "") + "]");
}

void LazySequenceWidget::onError(const QString& msg) {
    QMessageBox::warning(this, "Ошибка", msg);
}

void LazySequenceWidget::onMaterializedCountChanged(size_t count) {
    infoLabel->setText(QString("Состояние: %1 | Материализовано: %2 элементов")
        .arg(controller->isInfinite() ? "Бесконечная" : "Конечная")
        .arg(count));
    updateDisplay();
}