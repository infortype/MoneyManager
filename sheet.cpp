#include "sheet.h"
#include "server/server.h"
#include "ui_sheet.h"

#include <memory>
#include <QMessageBox>
#include <QDebug>

Sheet::Sheet(QWidget *parent, const QString& sheetName) :
    QMainWindow(parent),
    ui(new Ui::Sheet),
    sheetNameTitle(sheetName)
{
    ui->setupUi(this);

    myTable1 = std::make_unique<Table>(sheetNameTitle);
     if(!ConfigureTable1()) { QMessageBox::critical(this, "Error", "Table error"); return; };

    myTable2 = std::make_unique<Table>("Accounts");
     if(!ConfigureTable2()) { QMessageBox::critical(this, "Error", "Table error"); return; };
    ShowAccounts();
}

Sheet::~Sheet()
{
    delete ui;
}

bool Sheet::ConfigureTable1() const noexcept
{

    myTable1->AddRow("Event", Server::Text, "Primary Key unique");
    myTable1->AddRow("Amount", Server::Float);
    myTable1->AddRow("Account", Server::Text);
    myTable1->AddRow("Paid", Server::Text);
    return myTable1->Create();

}

bool Sheet::ConfigureTable2() const noexcept
{

    myTable2->AddRow("Account", Server::Text, "Primary Key unique");
    return myTable2->Create();

}

void Sheet::ShowAccounts() const noexcept
{

    QStringList items {};
    auto data = myTable2->GetAllElements();

    foreach(QStringList p, data){
        QString item = p.at(0);
        items << item;
    }

    ui->account->addItems(items);

    return;

}


