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
    explicit Sheet(QWidget *parent = nullptr, const QString& sheetName = nullptr);
    ~Sheet();

private slots:
    void on_bAdd_clicked();

private:
    Ui::Sheet *ui;
    QString sheetNameTitle {};
    std::unique_ptr<Table> myTable1 {};
    std::unique_ptr<Table> myTable2 {};

    [[nodiscard]] bool ConfigureTable1() const noexcept;
    [[nodiscard]] bool ConfigureTable2() const noexcept;
    void ShowAccounts() const noexcept;
    void ShowResume() const noexcept;
    const QStringList GetDataAcounts() const noexcept;

};

#endif // SHEET_H
