#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class menu; }
QT_END_NAMESPACE

class Server;

class menu : public QMainWindow
{
    Q_OBJECT

public:
    menu(QWidget *parent = nullptr);
    ~menu();

private:
    Ui::menu *ui;
    std::unique_ptr<Server>myServer;

};
#endif // MENU_H
