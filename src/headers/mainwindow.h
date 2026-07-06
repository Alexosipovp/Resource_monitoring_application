#include <QtWidgets>  


/**
 * @brief Главное окно монитора ресурсов
 */
class SystemMonitorWindow : public QMainWindow
{
    Q_OBJECT

public:
    SystemMonitorWindow(QWidget *parent = nullptr);
    ~SystemMonitorWindow();

    bool getCheckbox();

private:
    void setupUI();                 // Настройка интерфейса

    void createCPUGroup();          // Создание секции CPU
    QLabel* cpuSystemLabel;
    QProgressBar* cpuSystemProgress;
    QLabel* cpuAppLabel;
    QProgressBar* cpuAppProgress;
    QCheckBox* monitoringCheckBox;  // Включить мониторинг
    QGroupBox* cpuGroup;            // Секция CPU
    QGroupBox* memoryGroup;         // Секция памяти
    QGroupBox* processGroup;        // Секция процессов
    QGroupBox* diskGroup;           // Секция диска
    
    void createMemoryGroup();       // Создание секции памяти
    QLabel* memorySystemLabel;
    QProgressBar* memorySystemProgress;
    QLabel* memoryAppLabel;
    QLabel* memoryAppValue;

    void createProcessGroup();      // Создание секции процессов
    QLabel* processesCountLabel;
    QLabel* processesCountValue;
    QLabel* threadsLabel;
    QLabel* threadsValue;

    void createDiskGroup();         // Создание секции диска
    
    QLabel* readLabel;
    QLabel* readValue;
    QLabel* writeLabel;
    QLabel* writeValue;
};