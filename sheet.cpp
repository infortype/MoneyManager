#include "sheet.h"
#include "server/server.h"
#include "ui_sheet.h"

#include <memory>
#include <QMessageBox>

Sheet::Sheet(QWidget *parent, QString sheet) :
    QMainWindow(parent),
    ui(new Ui::Sheet),
    sheetNameTitle(sheet)
{
    ui->setupUi(this);

    myTable1 = std::make_unique<Table>(sheetNameTitle);
     if(!configureTable1()) { QMessageBox::critical(this, "Error", "Table error"); return; };
}

Sheet::~Sheet()
{
    delete ui;
}

bool Sheet::configureTable1() const noexcept
{
    myTable1->AddRow("Event", Server::Text, "Primary Key unique");
    myTable1->AddRow("Amount", Server::Float);
    myTable1->AddRow("Account", Server::Text);
    myTable1->AddRow("Paid", Server::Text);
    return myTable1->Create();
}
