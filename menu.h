#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <memory>
#include <QCompleter>
#include <QStringList>

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

private:
    Ui::menu *ui;
    std::unique_ptr<Server>myServer;
    std::unique_ptr<Table>myTable1;
    std::unique_ptr<QCompleter> myCompleter;

    [[nodiscard]] bool ConfigureTable1() const noexcept;
                  void ShowData()              noexcept;
    [[nodiscard]] const QStringList GetData() const noexcept;

};
#endif // MENU_H
