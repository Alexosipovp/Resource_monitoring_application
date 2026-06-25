#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>

class MyWindow : public QWidget {
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr) : QWidget(parent);

private:
    void updateOutput(QCheckBox *switchWidget, QLineEdit *inputLine, QLabel *outputLabel);
};