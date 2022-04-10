#include "menu.h"
#include "ui_menu.h"
#include "server/server.h"

#include <memory>

menu::menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::menu)
{
    ui->setupUi(this);
    myServer = std::make_unique<Server>("MoneyManager.sqlite");
    myServer->ConnectToBase();
}

menu::~menu()
{
    delete ui;
}

