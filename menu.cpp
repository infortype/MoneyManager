#include "menu.h"
#include "ui_menu.h"
#include "server/server.h"

#include <memory>
#include <QMessageBox>
#include <QInputDialog>
#include <QCompleter>
#include <QAbstractButton>

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
    std::unique_ptr<QIcon> myIcon = std::make_unique<QIcon>(":/assets/sheet.png");
    auto data = myTable1->GetAllElements();
    QStringList items {};

    foreach (QStringList p, data)
    {
        auto current = p.at(0);
        items << current;
        ui->listWidget->addItem(CreateSheetItem(current));

    }

   return items;

}

void menu::on_actionNueva_Hoja_triggered()
{

    const QString title     { "Insert" };
    const QString info      { "Write the name of the sheet." };
    const QString example   { "Sheet1" };
    const QString nameSheet { QInputDialog::getText(this, title, info, QLineEdit::Normal, example) };
     if(nameSheet.isEmpty()) { QMessageBox::warning(this, "Error", "Sheet must have a name."); return; }

    auto existsNameSheet = [this](const QString& nameSheet){

         bool exists { false };
         auto data = myTable1->GetAllElements();

         foreach(QStringList p, data){
             if(p.at(0) == nameSheet) { exists = true; }
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

    if(search.isEmpty()) { ShowData(); QMessageBox::warning(this, "Error", "Search can not be empty."); return; }

    //We loop the data.
    for (int i = 0; i <= size; i++){
        const QString currentItem { data.at(i) };
         if(currentItem == search) { find = true; break; } //Here we find the data
         else find = false;
    }

    if(find) {
        //Put the data in the list
        ui->listWidget->clear(); ui->listWidget->addItem(CreateSheetItem(search));
    }else { ShowData(); QMessageBox::critical(this, "Error", "Search has not found."); }

   ui->lineEdit->setText("");

   return;

}


void menu::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

    const QString sheet { item->text() };
    const int button { CreateMessage(sheet) };

    switch(button)
    {
    case 0: break;
    case 1:
    {
            const QString title { "Are you sure?" };
            const QString info { "If you erase the sheet, you will lose all the data." };
            int msg = QMessageBox::question(this,title,info,QMessageBox::No | QMessageBox::Yes, QMessageBox::No );
             if(msg == QMessageBox::Yes){
                EraseSheet(sheet);
                ShowData();
             }
            break;
    }
    case 2: break; //Make this function.
    }

}

int menu::CreateMessage(const QString &title)
{
    QPixmap myPix(":/assets/ask.png");
    myPix = myPix.scaled(50,50);

    QMessageBox msg;
    const QString info { "What action do you want to do with the sheet?" };

    [[maybe_unused]] auto exitButton= msg.addButton("Return", QMessageBox::ActionRole);
    [[maybe_unused]] auto delButton = msg.addButton("Erase", QMessageBox::ActionRole);
                     auto seeButton = msg.addButton("See", QMessageBox::ActionRole);

    msg.setDefaultButton(seeButton);
    msg.setText(title);
    msg.setInformativeText(info);
    msg.setIconPixmap(myPix);
    msg.setStyleSheet("QLabel{min-width: 80;}");

    return msg.exec();

}

void menu::EraseSheet(const QString &sheetName) const noexcept
{
    const QString msg = QString("Sheet '%1' has been erased").arg(sheetName);

     if(!myTable1->Delete(0,sheetName)) QMessageBox::critical(NULL, "Error", "Sheet has not erased");
     else QMessageBox::information(NULL, "Sheet erased", msg);

    return;

}

QListWidgetItem* menu::CreateSheetItem(const QString &sheetName) const noexcept
{

    std::unique_ptr<QIcon> myIcon = std::make_unique<QIcon>(":/assets/sheet.png");
    QListWidgetItem *item = new QListWidgetItem(*myIcon, sheetName); //Why here I can´t delete this pointer?

    return item;

}

