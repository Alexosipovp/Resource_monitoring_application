#include "./../headers/test.h"

class MyWindow : public QWidget {
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // 1. Создаём виджеты
        QCheckBox *switchWidget = new QCheckBox("Включить режим", this);
        QLineEdit *inputLine = new QLineEdit(this);
        QLabel *outputLabel = new QLabel("Здесь будет результат", this);

        // Настройка внешнего вида (опционально)
        inputLine->setPlaceholderText("Введите текст...");
        outputLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

        // 2. Располагаем виджеты в окне
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(switchWidget);
        mainLayout->addWidget(inputLine);
        mainLayout->addWidget(outputLabel);

        // 3. Подключаем сигналы к слотам (главная логика)
        // При изменении состояния переключателя или текста ввода обновляем вывод
        connect(switchWidget, &QCheckBox::toggled, this, [=]() {
            updateOutput(switchWidget, inputLine, outputLabel);
        });
        connect(inputLine, &QLineEdit::textChanged, this, [=]() {
            updateOutput(switchWidget, inputLine, outputLabel);
        });

        // Устанавливаем начальное состояние
        updateOutput(switchWidget, inputLine, outputLabel);
    }

private:
    void updateOutput(QCheckBox *switchWidget, QLineEdit *inputLine, QLabel *outputLabel) {
        QString inputText = inputLine->text();
        if (switchWidget->isChecked()) {
            // Если переключатель включён — выводим текст в верхнем регистре
            outputLabel->setText("Режим включён: " + inputText.toUpper());
        } else {
            // Если выключен — выводим как есть (или можно добавить префикс)
            outputLabel->setText("Режим выключен: " + inputText);
        }
    }
};