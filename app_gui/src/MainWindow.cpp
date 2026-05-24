#include "MainWindow.hpp"
#include "LazySequenceWidget.hpp"
#include "StatisticsWidget.hpp"
#include "TestingWidget.hpp"
#include <QVBoxLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    
    tabWidget = new QTabWidget(this);
    
    lazyWidget = new LazySequenceWidget(this);
    statsWidget = new StatisticsWidget(this);
    testingWidget = new TestingWidget(this);
    
    tabWidget->addTab(lazyWidget, "LazySequence");
    tabWidget->addTab(statsWidget, "OnlineStatistics");
    tabWidget->addTab(testingWidget, "Тестирование");
    
    mainLayout->addWidget(tabWidget);
    
    statusBar()->showMessage("Готов");
}