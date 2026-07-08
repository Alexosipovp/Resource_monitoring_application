#include "./../headers/mainwindow.h"
#include <QtWidgets>  
SystemMonitorWindow::SystemMonitorWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();

    api = PlatformFactory::create();
    api->followApp("firefox");

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &SystemMonitorWindow::updateData);
    updateTimer->start(500);
}

void SystemMonitorWindow::updateData()
{
    if (monitoringCheckBox->isChecked()) {
        api->updateData();
    
        cpuSystemProgress->setValue(api->getSystemCpuUsagePercent());
        cpuAppProgress->setValue(api->getAppCpuUsagePercent());
        memorySystemProgress->setValue(api->getSystemMemoryUsagePercent());
        memoryAppValue->setNum(double(api->getAppMemoryUsageMB()));
        processesCountValue->setNum(double(api->getRunningTasksCount()));
        threadsValue->setNum(double(api->getAppThreadsCount()));
        readValue->setText(QString("%1 Mb/c").arg(api->getDiskReadSpeedMBps()));
        writeValue->setText(QString("%1 Mb/c").arg(api->getDiskWriteSpeedMBps()));
    }
}

SystemMonitorWindow::~SystemMonitorWindow()
{
}

bool SystemMonitorWindow::getCheckbox() {
    return monitoringCheckBox->checkState();
}

void SystemMonitorWindow::setupUI()
{
    setWindowTitle("Монитор ресурсов");
    setFixedSize(450, 600);
    
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 10, 15, 10);
    
    // Чекбокс
    monitoringCheckBox = new QCheckBox("Включить мониторинг", this);
    monitoringCheckBox->setChecked(true);
    mainLayout->addWidget(monitoringCheckBox);
    
    // Секции (пока пустые)
    cpuGroup = new QGroupBox("ЦП", this);
    memoryGroup = new QGroupBox("Память", this);
    processGroup = new QGroupBox("Процессы", this);
    diskGroup = new QGroupBox("Диск", this);
    createCPUGroup();
    createMemoryGroup(); 
    createProcessGroup();
    createDiskGroup(); 
    mainLayout->addWidget(cpuGroup);
    mainLayout->addWidget(memoryGroup);
    mainLayout->addWidget(processGroup);
    mainLayout->addWidget(diskGroup);
    
    setCentralWidget(centralWidget);
}
void SystemMonitorWindow::createCPUGroup()
{
    QGridLayout* layout = new QGridLayout(cpuGroup);
    
    // Строка 1: Система
    cpuSystemLabel = new QLabel("Система", this);
    
    
    cpuSystemProgress = new QProgressBar(this);
    cpuSystemProgress->setRange(0, 100);
    cpuSystemProgress->setValue(0);
    cpuSystemProgress->setTextVisible(true);  
    cpuSystemProgress->setFormat("%p%"); 
    cpuSystemProgress->setFixedHeight(21);
    
    // Строка 2: Приложение
    cpuAppLabel = new QLabel("Приложение", this);
    cpuAppProgress = new QProgressBar(this);
    cpuAppProgress->setRange(0, 100);
    cpuAppProgress->setValue(0);
    cpuAppProgress->setTextVisible(true);
    cpuAppProgress->setFormat("%p%"); 
    cpuAppProgress->setFixedHeight(21);
    
    // Добавляем в сетку
    layout->addWidget(cpuSystemLabel, 0, 0);
    layout->addWidget(cpuSystemProgress, 0, 2);
    
    layout->addWidget(cpuAppLabel, 1, 0);
    layout->addWidget(cpuAppProgress, 1, 2);
}

void SystemMonitorWindow::createMemoryGroup()
{
    QGridLayout* layout = new QGridLayout(memoryGroup);
    
    // Строка 1: Система
    memorySystemLabel = new QLabel("Система", this);
    memorySystemProgress = new QProgressBar(this);
    memorySystemProgress->setRange(0, 100);
    memorySystemProgress->setValue(0);
    memorySystemProgress->setTextVisible(true);
    memorySystemProgress->setFormat("%p%"); 
    memorySystemProgress->setFixedHeight(21);
    
    // Строка 2: Приложение
    memoryAppLabel = new QLabel("Приложение", this);
    memoryAppValue = new QLabel("299 МБ", this);
    memoryAppValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    // Добавляем в сетку
    layout->addWidget(memorySystemLabel, 0, 0);
    layout->addWidget(memorySystemProgress, 0, 2);
    layout->addWidget(memoryAppLabel, 1, 0);
    layout->addWidget(memoryAppValue, 1, 2);
    
}
void SystemMonitorWindow::createProcessGroup()
{
    QGridLayout* layout = new QGridLayout(processGroup);
    // Строка 1: Запущенных задач
    processesCountLabel = new QLabel("Запущенных задач", this);
    processesCountValue = new QLabel("0", this);
    processesCountValue->setAlignment(Qt::AlignRight);
    
    // Строка 2: Потоков приложения
    threadsLabel = new QLabel("Потоков приложения", this);
    threadsValue = new QLabel("29", this);
    threadsValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    // Добавляем в сетку
    layout->addWidget(processesCountLabel, 0, 0);
    layout->addWidget(processesCountValue, 0, 1);
    layout->addWidget(threadsLabel, 1, 0);
    layout->addWidget(threadsValue, 1, 1);
    
}
void SystemMonitorWindow::createDiskGroup()
{
    
    QGridLayout* layout = new QGridLayout(diskGroup);
    
    // Чтение
    readLabel = new QLabel("Чтение", this);
    readValue = new QLabel("0 МБ/с", this);
    readValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    // Запись
    writeLabel = new QLabel("Запись", this);
    writeValue = new QLabel("0 МБ/с", this);
    writeValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    // Добавляем в layout
    layout->addWidget(readLabel, 0, 0);
    layout->addWidget(readValue, 0, 1);
    
    layout->addWidget(writeLabel, 1, 0);
    layout->addWidget(writeValue, 1, 1);
}