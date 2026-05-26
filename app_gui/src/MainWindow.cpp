#include "MainWindow.hpp"
#include "LazySequenceWidget.hpp"
#include "TestingWidget.hpp"
#include <QVBoxLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    
    setStatusBar(nullptr); 
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);  
    
    lazyWidget = new LazySequenceWidget(tabWidget, this);
    testingWidget = new TestingWidget(this);
    
    int lazyIndex = tabWidget->addTab(lazyWidget, "LazySequence");
    int testIndex = tabWidget->addTab(testingWidget, "Тестирование");
    
    tabWidget->tabBar()->setTabButton(lazyIndex, QTabBar::RightSide, nullptr);
    tabWidget->tabBar()->setTabButton(testIndex, QTabBar::RightSide, nullptr);
    
    mainLayout->addWidget(tabWidget);

    connect(tabWidget, &QTabWidget::tabCloseRequested, 
            this, &MainWindow::onTabClose);
}

void MainWindow::onTabClose(int index) {
    QString tabText = tabWidget->tabText(index);
    if (tabText.startsWith("Статистика ")) {
        QWidget* widget = tabWidget->widget(index);
        tabWidget->removeTab(index);
        delete widget;
    }
}