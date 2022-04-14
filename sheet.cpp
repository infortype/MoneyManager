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


