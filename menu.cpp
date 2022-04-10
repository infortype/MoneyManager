#include "menu.h"
#include "ui_menu.h"
#include "server/server.h"

#include <memory>
#include <QMessageBox>
#include <QInputDialog>

menu::menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::menu)
{
    ui->setupUi(this);

    myServer = std::make_unique<Server>("MoneyManager.sqlite");
    myServer->ConnectToBase();

    myTable1 = std::make_unique<Table>("Sheets");
     if(!ConfigureTable1()) { QMessageBox::critical(this, "Error", "Table error"); return; };
}

menu::~menu()
{
    delete ui;
}

bool menu::ConfigureTable1() const noexcept
{

    myTable1->AddRow("NameSheet", Server::Text, "Primary Key unique");
    return myTable1->Create();

}

void menu::on_actionNueva_Hoja_triggered()
{

    const QString title     { "Insertar" };
    const QString info      { "Escriba el nombre de la hoja." };
    const QString example   { "Sheet1" };
    const QString nameSheet { QInputDialog::getText(this, title, info, QLineEdit::Normal, example) };

}

