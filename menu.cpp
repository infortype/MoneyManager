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
    const short RowTable1 { 0 };
    auto data = myTable1->GetAllElements();
    QStringList items {};

    for(int i = 0; i <= data.size() - 1; i++)
    {
        auto elem = data.at(i).at(RowTable1);
        items << elem;
        ui->listWidget->addItem(elem);

    }

    auto CompleterLine = [this](QStringList& items){
        myCompleter = std::make_unique<QCompleter>(items);
        ui->lineEdit->setCompleter(myCompleter.get());
        return;

    };

    CompleterLine(items);
    return;

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


void menu::on_pushButton_clicked()
{
    const QString search { ui->lineEdit->text() };
    const int size = ui->listWidget->count() - 1;

     if(search.isEmpty()) { QMessageBox::warning(this, "Error", "Search is empty"); return; };

     for(int i = 0; i <= size; i++){
         auto currentElem = ui->listWidget->item(i)->text();
         qDebug() << currentElem << " " << search;
          if(currentElem == search){
              ui->listWidget->clear();
              ui->listWidget->addItem(currentElem);
          }//else { QMessageBox::critical(this, "Error", "Search has not found."); return; };

     }

}




