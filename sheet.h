#ifndef SHEET_H
#define SHEET_H

#include <QMainWindow>
#include <memory>

class Table;

namespace Ui {

class Sheet;
}

class Sheet : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sheet(QWidget *parent = nullptr, QString sheet = nullptr);
    ~Sheet();

private:
    Ui::Sheet *ui;
    std::unique_ptr<Table>myTable1;
    QString sheetNameTitle {};

    [[nodiscard]] bool configureTable1() const noexcept;
};

#endif // SHEET_H
