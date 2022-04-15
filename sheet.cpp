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
    ShowResume();
    ShowData();
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

    ui->account->addItems(GetDataAcounts());

    return;

}

void Sheet::ShowResume() const noexcept
{
    ui->resumeList->clear();

    const short rowAccount { 2 };
    const short rowAmount { 1 };

    QVector<double>amounts{};
    const QStringList dataAccounts { GetDataAcounts() };
    amounts.resize(dataAccounts.size());
    const auto data = myTable1->GetAllElements();

     for(int i = 0; i<= data.size() - 1; i++){

         for(int j = 0; j <= dataAccounts.size() - 1; j++){
             const QStringList currentData = data.at(i);
             const QString currentAccount = currentData.at(rowAccount);
             const QString currentAmount = currentData.at(rowAmount);
              if(currentAccount == dataAccounts.at(j)) amounts[j] += currentAmount.toDouble();
         }

     }//Esto va a petar

     for(int i = 0; i <= dataAccounts.size() - 1; i++){
         const QString msg { QString("%1: %2 Euros").arg(dataAccounts.at(i),
                                                        QString::number(amounts.at(i))) };
         ui->resumeList->addItem(msg);
     }

    return;

}

void Sheet::ShowData() const noexcept
{
    ui->list->clear();
    const auto data = myTable1->GetAllElements();

    foreach(QStringList p, data){
        ui->list->addItem(p.at(0));
    }

    return;

}

const QStringList Sheet::GetDataAcounts() const noexcept
{

    QStringList items {};
    const auto data = myTable2->GetAllElements();

    foreach(QStringList p, data){
        QString item = p.at(0);
        items << item;
    }

   return items;

}

void Sheet::on_bAdd_clicked()
{
    const QString event { ui->event->text() };
    const QString account { ui->account->currentText() };
    const double amount { ui->amount->value() };
    bool paidBool = ui->paid;
    QString paid;
     if(paidBool) paid = "True";
     else paid = "False";

     if(event.isEmpty() || amount == 0.0 ){
         QMessageBox::critical(this, "Error", "Event requires a text and the amount must be > 0.");
         ui->event->setFocus();
     }else{
         myTable1->Add({event,QString::number(amount),account, paid});
         ShowResume();
     }

    return;

}


void Sheet::on_list_itemClicked(QListWidgetItem *item)
{
    const QStringList data = (myTable1->GetElement(0, item->text())).at(0);

    QString paid {};
     if(data.at(3) == "True") paid = "Pagado";
     else paid = "No pagado";

    const QString msg = QString("Evento: %1\nCantidad: %2€\nCuenta: %3\nEstado: %4")
            .arg(data.at(0), data.at(1), data.at(2), paid);

    const int returnedValue { CreateMessage(item->text(), msg) };

    switch (returnedValue) {
    case 0: break;
    case 1:
    {
        const QString title { "Are you sure?" };
        const QString info { "If you erase the item, you will lose all the data." };
        int msg = QMessageBox::question(this,title,info,QMessageBox::No | QMessageBox::Yes, QMessageBox::No );
         if(msg == QMessageBox::Yes){
            EraseItem(item->text());
            ShowResume();
            ShowData();
         }
        break;
    }
    case 2: //modificar
        break;

    }

    return;

}

int Sheet::CreateMessage(const QString &title, const QString &myMsg) const noexcept
{
    QPixmap myPix(":/assets/info.png");
    myPix = myPix.scaled(50,50);

    QMessageBox msg;

    [[maybe_unused]] auto exitButton= msg.addButton("Return", QMessageBox::ActionRole);
    [[maybe_unused]] auto delButton = msg.addButton("Erase", QMessageBox::ActionRole);
                     auto ChangeButton = msg.addButton("Change", QMessageBox::ActionRole);

    msg.setDefaultButton(ChangeButton);
    msg.setText(title);
    msg.setInformativeText(myMsg);
    msg.setIconPixmap(myPix);
    msg.setStyleSheet("QLabel{min-width: 80;}");

    return msg.exec();

}

void Sheet::EraseItem(const QString &item) const noexcept
{
    const QString msg = QString("Item '%1' has been erased").arg(item);

     if(!myTable1->Delete(0,item)) QMessageBox::critical(NULL, "Error", "Item has not erased");
     else QMessageBox::information(NULL, "Item erased", msg);

    return;

}

