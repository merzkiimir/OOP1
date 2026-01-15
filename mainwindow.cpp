#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QInputDialog>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


#include "src/contactdialog.h"
#include "src/searchdialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("Телефонный справочник"));
    setFixedSize(size());

    setStyleSheet(R"(

        QMainWindow, QDialog {
            background-color: #1e1e2e;
            color: #f8f8f2;
        }


        QWidget#centralwidget {
            background-color: #1e1e2e;
        }

        QLabel {
            color: #f8f8f2;
        }


        QLineEdit, QDateEdit {
            background-color: #242438;
            color: #f8f8f2;
            border: 1px solid #44475a;
            border-radius: 4px;
            padding: 2px 6px;
        }

        QLineEdit:focus, QDateEdit:focus {
            border: 1px solid #6272a4;
        }


        QTableWidget {
            background-color: #242438;
            color: #f8f8f2;
            gridline-color: #44475a;
            selection-background-color: #6272a4;
            selection-color: #ffffff;
            alternate-background-color: #1a1a2a;
        }


        QHeaderView::section {
            background-color: #44475a;
            color: #f8f8f2;
            padding: 4px;
            border: 1px solid #282a36;
        }


        QPushButton {
            background-color: #6272a4;
            color: #ffffff;
            border-radius: 6px;
            padding: 4px 10px;
            border: 1px solid #44475a;
        }

        QPushButton:hover {
            background-color: #7083c4;
        }

        QPushButton:pressed {
            background-color: #4b5a8b;
        }


        QScrollBar:vertical, QScrollBar:horizontal {
            background: #1e1e2e;
            border: 1px solid #282a36;
        }
        QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
            background: #44475a;
            min-height: 20px;
            min-width: 20px;
        }
        QScrollBar::handle:vertical:hover, QScrollBar::handle:horizontal:hover {
            background: #6272a4;
        }
    )");


    ui->tableContacts->setColumnCount(5);
    ui->tableContacts->setHorizontalHeaderLabels(
        { "Имя", "Фамилия", "Email", "Дата рождения", "Телефон" });
    ui->tableContacts->verticalHeader()->setVisible(false);
    ui->tableContacts->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableContacts->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableContacts->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableContacts->setAlternatingRowColors(true);


    auto *header = ui->tableContacts->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setMinimumSectionSize(80);



    header->setSortIndicatorShown(true);

    connect(header, &QHeaderView::sectionClicked,
            this, &MainWindow::onHeaderSectionClicked);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatetable()
{


    QTableWidget *table = ui->tableContacts;

    const std::vector<Contact> &all = searchActive
                                          ? searchResult.get_all()
                                          : contacts.get_all();
    int rowCount = static_cast<int>(all.size());

    table->clearContents();
    table->setRowCount(rowCount);

    for (int row = 0; row < rowCount; ++row) {
        const Contact &c = all[static_cast<size_t>(row)];


        auto *itemName = new QTableWidgetItem(
            QString::fromStdString(c.get_name()));
        auto *itemSurname = new QTableWidgetItem(
            QString::fromStdString(c.get_surname()));
        auto *itemEmail = new QTableWidgetItem(
            QString::fromStdString(c.get_email()));
        auto *itemDate = new QTableWidgetItem(
            QString::fromStdString(c.get_date()));

        table->setItem(row, 0, itemName);
        table->setItem(row, 1, itemSurname);
        table->setItem(row, 2, itemEmail);
        table->setItem(row, 3, itemDate);


        std::string phoneStr = c.get_first_qt_phone();
        auto *itemPhone = new QTableWidgetItem(
            QString::fromStdString(phoneStr));
        table->setItem(row, 4, itemPhone);
    }

}




void MainWindow::on_btnadd_clicked()
{
    ContactDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        contacts.add_contact(dlg.contact());
        updatetable();
    }
}


void MainWindow::on_btnrem_clicked()
{
    int row = ui->tableContacts->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите контакт для удаления.");
        return;
    }

    if (searchActive) {
        QMessageBox::warning(this, "Внимание",
                             "Сбросьте поиск (reset), чтобы удалить запись.");
        return;
    }

    contacts.remove_contact(static_cast<size_t>(row) + 1);
    updatetable();
}

void MainWindow::on_btnedit_clicked()
{
    int row = ui->tableContacts->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите контакт для редактирования.");
        return;
    }

    if (searchActive) {
        QMessageBox::warning(this, "Внимание",
                             "Сбросьте поиск (reset), чтобы удалить запись.");
        return;
    }

    auto &all = contacts.get_all();
    ContactDialog dlg(this);
    dlg.setContact(all[static_cast<size_t>(row)]);

    if (dlg.exec() == QDialog::Accepted) {
        all[static_cast<size_t>(row)] = dlg.contact();
        updatetable();
    }
}

void MainWindow::on_btnsave_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
        this, "Сохранить в файл", "", "Text Files (*.txt)");
    if (filename.isEmpty()) {
        return;
    }
    contacts.savetofile(filename.toLocal8Bit().toStdString());
    QMessageBox::information(this, "Сохранено", "Данные успешно сохранены!");
}

void MainWindow::on_btnload_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this, "Открыть файл", "", "Text Files (*.txt)");
    if (!filename.isEmpty()) {
        contacts.loadfromfile(filename.toStdString());
        updatetable();
        QMessageBox::information(this, "Загружено", "Данные успешно загружены!");
    }
}


void MainWindow::on_btnsearch_clicked()
{
    SearchDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    auto query = dlg.query();
    if (query.empty()) {
        QMessageBox::information(
            this,
            tr("Поиск"),
            tr("Вы не заполнили ни одного поля.")
            );
        return;
    }

    searchResult = find_cntct_gui(contacts, query);
    searchActive = true;
    updatetable();

    QMessageBox::information(
        this,
        tr("Поиск"),
        tr("Найдено записей: %1").arg(searchResult.get_all().size())
        );
}


void MainWindow::on_btnreset_clicked()
{
    searchActive = false;
    searchResult = vectorContact();
    updatetable();
}


void MainWindow::onHeaderSectionClicked(int logicalIndex)
{
    if (logicalIndex < 0 || logicalIndex > 4)
        return;

    std::vector<Contact> &vec = searchActive
                                    ? searchResult.get_all()
                                    : contacts.get_all();

    if (vec.empty())
        return;

    if (m_lastSortColumn == logicalIndex) {
        m_sortAscending = !m_sortAscending;
    } else {
        m_lastSortColumn = logicalIndex;
        m_sortAscending = true;
    }


    auto keyFor = [logicalIndex](const Contact &c) -> std::string {
        switch (logicalIndex) {
        case 0: return c.get_name();
        case 1: return c.get_surname();
        case 2: return c.get_email();
        case 3: return c.get_date();
        case 4: return c.get_first_qt_phone();
        default: return "";
        }
    };

    std::sort(vec.begin(), vec.end(),
              [&](const Contact &a, const Contact &b) {
                  std::string ka = keyFor(a);
                  std::string kb = keyFor(b);
                  if (m_sortAscending)
                      return ka < kb;
                  else
                      return kb < ka;
              });


    auto *header = ui->tableContacts->horizontalHeader();
    header->setSortIndicator(
        logicalIndex,
        m_sortAscending ? Qt::AscendingOrder : Qt::DescendingOrder
        );


    updatetable();
}


bool MainWindow::initDb()
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName("127.0.0.1");
    m_db.setPort(5432);
    m_db.setDatabaseName("phonebook");
    m_db.setUserName("postgres");
    m_db.setPassword("...");


    QSqlQuery q(m_db);

    if (!q.exec(
            "CREATE TABLE IF NOT EXISTS contacts ("
            "  id SERIAL PRIMARY KEY,"
            "  name       TEXT NOT NULL,"
            "  surname    TEXT NOT NULL,"
            "  patronymic TEXT,"
            "  address    TEXT,"
            "  birth_date TEXT NOT NULL,"
            "  email      TEXT NOT NULL"
            ");"
            )) {
        qWarning() << "Create contacts failed:" << q.lastError().text();
        return false;
    }

    if (!q.exec(
            "CREATE TABLE IF NOT EXISTS phones ("
            "  id SERIAL PRIMARY KEY,"
            "  contact_id INTEGER NOT NULL "
            "      REFERENCES contacts(id) ON DELETE CASCADE,"
            "  phone_type   TEXT,"
            "  phone_number TEXT NOT NULL"
            ");"
            )) {
        qWarning() << "Create phones failed:" << q.lastError().text();
        return false;
    }

    return true;
}



void MainWindow::on_btnSaveDb_clicked()
{
    if (!m_db.isOpen()) {
        QMessageBox::warning(this, tr("База данных"),
                             tr("Соединение с БД не установлено."));
        return;
    }

    const auto &all = contacts.get_all();
    if (all.empty()) {
        QMessageBox::information(this, tr("База данных"),
                                 tr("Нет данных для сохранения."));
        return;
    }

    if (!m_db.transaction()) {
        QMessageBox::warning(this, tr("База данных"),
                             m_db.lastError().text());
        return;
    }

    QSqlQuery q(m_db);

    if (!q.exec("TRUNCATE TABLE phones RESTART IDENTITY CASCADE")) {
        m_db.rollback();
        QMessageBox::warning(this, tr("База данных"),
                             q.lastError().text());
        return;
    }
    if (!q.exec("TRUNCATE TABLE contacts RESTART IDENTITY CASCADE")) {
        m_db.rollback();
        QMessageBox::warning(this, tr("База данных"),
                             q.lastError().text());
        return;
    }

    for (const Contact &c : all) {
        QSqlQuery qc(m_db);
        qc.prepare(
            "INSERT INTO contacts("
            "  name, surname, patronymic, address, birth_date, email"
            ") VALUES (:name, :surname, :patronymic, :address, :birth, :email)"
            " RETURNING id"
            );

        qc.bindValue(":name",       QString::fromStdString(c.get_name()));
        qc.bindValue(":surname",    QString::fromStdString(c.get_surname()));
        qc.bindValue(":patronymic", QString::fromStdString(c.get_name3()));
        qc.bindValue(":address",    QString::fromStdString(c.get_address()));
        qc.bindValue(":birth",      QString::fromStdString(c.get_date()));
        qc.bindValue(":email",      QString::fromStdString(c.get_email()));

        if (!qc.exec() || !qc.next()) {
            m_db.rollback();
            QMessageBox::warning(this, tr("База данных"),
                                 tr("Ошибка вставки контакта: %1")
                                     .arg(qc.lastError().text()));
            return;
        }

        int contactId = qc.value(0).toInt();

        QSqlQuery qp(m_db);
        qp.prepare(
            "INSERT INTO phones(contact_id, phone_type, phone_number) "
            "VALUES(:cid, :type, :num)"
            );

        const auto &phones = c.get_qt_phones();
        for (const auto &p : phones) {
            qp.bindValue(":cid",  contactId);
            qp.bindValue(":type", QString::fromStdString(p.second)); // тип
            qp.bindValue(":num",  QString::fromStdString(p.first));  // номер

            if (!qp.exec()) {
                m_db.rollback();
                QMessageBox::warning(this, tr("База данных"),
                                     tr("Ошибка вставки телефона: %1")
                                         .arg(qp.lastError().text()));
                return;
            }
        }
    }

    m_db.commit();
    QMessageBox::information(this, tr("База данных"),
                             tr("Данные сохранены в PostgreSQL."));
}




void MainWindow::on_btnLoadDb_clicked()
{
    if (!m_db.isOpen()) {
        QMessageBox::warning(this, tr("База данных"),
                             tr("Соединение с БД не установлено."));
        return;
    }

    contacts = vectorContact();
    searchActive = false;
    searchResult = vectorContact();

    QSqlQuery qc(m_db);
    if (!qc.exec(
            "SELECT id, name, surname, patronymic, address, birth_date, email "
            "FROM contacts ORDER BY id"
            )) {
        QMessageBox::warning(this, tr("База данных"),
                             tr("Ошибка чтения контактов: %1")
                                 .arg(qc.lastError().text()));
        return;
    }

    while (qc.next()) {
        int id = qc.value("id").toInt();
        std::string name       = qc.value("name").toString().toStdString();
        std::string surname    = qc.value("surname").toString().toStdString();
        std::string patronymic = qc.value("patronymic").toString().toStdString();
        std::string address    = qc.value("address").toString().toStdString();
        std::string birth      = qc.value("birth_date").toString().toStdString();
        std::string email      = qc.value("email").toString().toStdString();

        vectorNumbers nums;
        std::vector<std::pair<std::string, std::string>> qtPhones;

        QSqlQuery qp(m_db);
        qp.prepare(
            "SELECT phone_number, phone_type "
            "FROM phones WHERE contact_id = :cid"
            );
        qp.bindValue(":cid", id);

        if (!qp.exec()) {
            QMessageBox::warning(this, tr("База данных"),
                                 tr("Ошибка чтения телефонов: %1")
                                     .arg(qp.lastError().text()));
            return;
        }

        while (qp.next()) {
            std::string num  = qp.value("phone_number").toString().toStdString();
            std::string type = qp.value("phone_type").toString().toStdString();
            nums.add_number(Number(num, type));
            qtPhones.emplace_back(num, type);
        }

        Contact c(name, surname, patronymic, address, birth, email, nums);
        c.set_qt_phones(qtPhones);
        contacts.add_contact(c);
    }

    updatetable();
    QMessageBox::information(this, tr("База данных"),
                             tr("Данные загружены из PostgreSQL."));
}
