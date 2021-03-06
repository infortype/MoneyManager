#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <memory>
#include <QCompleter>
#include <QStringList>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class menu; }
QT_END_NAMESPACE

class Server;
class Table;

class menu : public QMainWindow
{
    Q_OBJECT

public:
    menu(QWidget *parent = nullptr);
    ~menu();

private slots:
    void on_actionNueva_Hoja_triggered();

    void on_actionTodos_triggered();

    void on_searchButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_actionAgregar_cuenta_triggered();

    void on_actionEliminar_cuenta_triggered();

private:
    Ui::menu *ui;
    std::unique_ptr<Server>myServer;
    std::unique_ptr<Table>myTable1;
    std::unique_ptr<Table>myTable2;
    std::unique_ptr<QCompleter> myCompleter;
    std::unique_ptr<QListWidgetItem> myItem;

    [[nodiscard]] bool ConfigureTable1() const noexcept;
    [[nodiscard]] bool ConfigureTable2() const noexcept;
                  void ShowData() noexcept;
    [[nodiscard]] const QStringList GetData() const noexcept;
    [[nodiscard]] int CreateMessage(const QString& title);
                  void EraseSheet(const QString& sheetName) const noexcept;
                  void EraseAccount(const QString& account) const noexcept;
    QListWidgetItem *CreateSheetItem(const QString& sheetName) const noexcept;

};
#endif // MENU_H
