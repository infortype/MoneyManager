#include "menu.h"
#include "ui_menu.h"
#include "server/server.h"

#include <memory>
#include <QMessageBox>
#include <QInputDialog>
#include <QCompleter>

menu::menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::menu)
{
    ui->setupUi(this);

    myServer = std::make_unique<Server>("MoneyManager.sqlite");
    myServer->ConnectToBase();

    myTable1 = std::make_unique<Table>("Sheets");
     if(!ConfigureTable1()) { QMessageBox::critical(this, "Error", "Table error"); return; };

    ShowData();
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

void menu::ShowData() noexcept
{
    ui->listWidget->clear();

    const QStringList items = GetData();

    auto CompleterLine = [this](const QStringList items){
        myCompleter = std::make_unique<QCompleter>(items);
        ui->lineEdit->setCompleter(myCompleter.get());
        return;

    };

    CompleterLine(items);

    return;

}

const QStringList menu::GetData() const noexcept
{
    const short RowTable1 { 0 };
    auto data = myTable1->GetAllElements();
    QStringList items {};

    for(int i = 0; i <= data.size() - 1; i++)
    {
        auto elem = data.at(i).at(RowTable1);
        items << elem;
        ui->listWidget->addItem(elem);

    }

   return items;

}

void menu::on_actionNueva_Hoja_triggered()
{

    const QString title     { "Insertar" };
    const QString info      { "Escriba el nombre de la hoja." };
    const QString example   { "Sheet1" };
    const QString nameSheet { QInputDialog::getText(this, title, info, QLineEdit::Normal, example) };
     if(nameSheet.isEmpty()) { QMessageBox::warning(this, "Error", "Sheet must have a name."); return; }

    auto existsNameSheet = [this](const QString& nameSheet){

         bool exists { false };
         const int rowTable1 { 0 };
         auto data = myTable1->GetAllElements();

         for(int i = 0; i <= data.size() - 1; i++){
             if (data.at(i).at(rowTable1) == nameSheet) { exists = true; }
         }

         return exists;

    };

     if(existsNameSheet(nameSheet))
     {
         QMessageBox::warning(this, "Error", "Sheet must have a different name.");
         return;
     }else
     {
         myTable1->Add({nameSheet});
         QMessageBox::information(this, "Good", "Sheet was created.");
         ShowData();
     }

    return;
}

void menu::on_actionTodos_triggered()
{
    ShowData();
    return;
}


void menu::on_searchButton_clicked()
{

    bool find { false };
    const QString search { ui->lineEdit->text() };
    const QStringList data { GetData() };
    const int size = data.size() - 1;

    //We loop the data.
    for (int i = 0; i <= size; i++){
        const QString currentItem { data.at(i) };
         if(currentItem == search) { find = true; break; } //Here we find the data
         else find = false;
    }

    if(find) { ui->listWidget->clear(); ui->listWidget->addItem(search); } //Put the data in the list
    else { ShowData(); QMessageBox::critical(this, "Error", "Search has not found."); }

   ui->lineEdit->setText("");

   return;

}

