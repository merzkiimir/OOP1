#include "contactdialog.h"

#include <QLineEdit>
#include <QDateEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDate>

#include "contact.h"
#include "vectnmb.h"
#include "number.h"

ContactDialog::ContactDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Контакт"));

    m_nameEdit       = new QLineEdit(this);
    m_surnameEdit    = new QLineEdit(this);
    m_patronymicEdit = new QLineEdit(this);
    m_addressEdit    = new QLineEdit(this);
    m_emailEdit      = new QLineEdit(this);

    m_birthEdit = new QDateEdit(this);
    m_birthEdit->setCalendarPopup(true);
    m_birthEdit->setDisplayFormat("dd.MM.yyyy");
    m_birthEdit->setDate(QDate::currentDate().addYears(-18));
    m_birthEdit->setMaximumDate(QDate::currentDate().addDays(-1));

    m_phoneTypeEdit = new QLineEdit(this);
    m_phoneEdit     = new QLineEdit(this);

    m_phoneTable = new QTableWidget(this);
    m_phoneTable->setColumnCount(2);
    m_phoneTable->setHorizontalHeaderLabels({ tr("Тип"), tr("Номер") });
    m_phoneTable->horizontalHeader()->setStretchLastSection(true);
    m_phoneTable->verticalHeader()->setVisible(false);
    m_phoneTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_phoneTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_phoneTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto *btnAddPhone    = new QPushButton(tr("Добавить номер"), this);
    auto *btnRemovePhone = new QPushButton(tr("Удалить номер"), this);

    auto *phoneEditLayout = new QHBoxLayout;
    phoneEditLayout->addWidget(m_phoneTypeEdit);
    phoneEditLayout->addWidget(m_phoneEdit);
    phoneEditLayout->addWidget(btnAddPhone);
    phoneEditLayout->addWidget(btnRemovePhone);

    auto *formLayout = new QFormLayout;
    formLayout->addRow(tr("Имя *:"),           m_nameEdit);
    formLayout->addRow(tr("Фамилия *:"),       m_surnameEdit);
    formLayout->addRow(tr("Отчество:"),        m_patronymicEdit);
    formLayout->addRow(tr("Адрес:"),           m_addressEdit);
    formLayout->addRow(tr("Дата рождения *:"), m_birthEdit);
    formLayout->addRow(tr("E-mail *:"),        m_emailEdit);
    formLayout->addRow(tr("Тип / номер:"),     phoneEditLayout);
    formLayout->addRow(tr("Телефоны:"),        m_phoneTable);

    auto *btnOk     = new QPushButton(tr("OK"), this);
    auto *btnCancel = new QPushButton(tr("Отмена"), this);

    auto *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(btnOk);
    buttonsLayout->addWidget(btnCancel);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    connect(btnAddPhone,    &QPushButton::clicked, this, &ContactDialog::onAddPhone);
    connect(btnRemovePhone, &QPushButton::clicked, this, &ContactDialog::onRemovePhone);
    connect(btnOk,          &QPushButton::clicked, this, &ContactDialog::onAccept);
    connect(btnCancel,      &QPushButton::clicked, this, &QDialog::reject);
}

void ContactDialog::onAddPhone()
{
    QString type  = m_phoneTypeEdit->text().trimmed();
    QString phone = m_phoneEdit->text().trimmed();

    if (phone.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Введите номер телефона."));
        return;
    }

    static const QRegularExpression phoneRe(
        QStringLiteral(R"(^(\+7|8)\(?\d{3}\)?\d{3}\-?\d{2}\-?\d{2}$)")
        );

    if (!phoneRe.match(phone).hasMatch()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Телефон в неверном формате."));
        return;
    }

    int row = m_phoneTable->rowCount();
    m_phoneTable->insertRow(row);
    m_phoneTable->setItem(row, 0, new QTableWidgetItem(type));
    m_phoneTable->setItem(row, 1, new QTableWidgetItem(phone));

    m_phoneTypeEdit->clear();
    m_phoneEdit->clear();
}

void ContactDialog::onRemovePhone()
{
    int row = m_phoneTable->currentRow();
    if (row < 0)
        row = m_phoneTable->rowCount() - 1;

    if (row >= 0)
        m_phoneTable->removeRow(row);
}

bool ContactDialog::validate(QString &errorMessage) const
{
    const QString name       = m_nameEdit->text().simplified();
    const QString surname    = m_surnameEdit->text().simplified();
    const QString patronymic = m_patronymicEdit->text().simplified();
    const QString email      = m_emailEdit->text().trimmed();

    if (name.isEmpty() || surname.isEmpty()) {
        errorMessage = tr("Имя и фамилия обязательны.");
        return false;
    }

    static const QRegularExpression nameRe(
        QStringLiteral(R"(^[A-Za-zА-Яа-яЁё][A-Za-zА-Яа-яЁё\d\- ]*[A-Za-zА-Яа-яЁё\d]$)")
        );

    auto checkName = [&](const QString &s) -> bool {
        if (s.isEmpty()) return true;
        return nameRe.match(s).hasMatch();
    };

    if (!checkName(name) || !checkName(surname) || !checkName(patronymic)) {
        errorMessage = tr("ФИО: только буквы/цифры/пробел/-, "
                          "начинаться с буквы и не начинаться/заканчиваться на '-'.");
        return false;
    }

    if (email.isEmpty()) {
        errorMessage = tr("E-mail обязателен.");
        return false;
    }

    static const QRegularExpression emailRe(
        QStringLiteral(R"(^[A-Za-z][A-Za-z\d]*@[A-Za-z][A-Za-z\d]*\.[A-Za-z]+$)")
        );
    if (!emailRe.match(email).hasMatch()) {
        errorMessage = tr("E-mail в неверном формате.");
        return false;
    }

    if (m_phoneTable->rowCount() == 0) {
        errorMessage = tr("Должен быть хотя бы один номер телефона.");
        return false;
    }

    const QDate birth = m_birthEdit->date();
    if (!birth.isValid()) {
        errorMessage = tr("Неверная дата рождения.");
        return false;
    }
    if (birth >= QDate::currentDate()) {
        errorMessage = tr("Дата рождения должна быть раньше сегодняшнего дня.");
        return false;
    }

    return true;
}

void ContactDialog::onAccept()
{
    QString error;
    if (!validate(error)) {
        QMessageBox::warning(this, tr("Ошибка"), error);
        return;
    }

    const QString name       = m_nameEdit->text().simplified();
    const QString surname    = m_surnameEdit->text().simplified();
    const QString patronymic = m_patronymicEdit->text().simplified();
    const QString address    = m_addressEdit->text().simplified();
    const QString email      = m_emailEdit->text().trimmed();
    const QDate   birth      = m_birthEdit->date();

    vectorNumbers nums;
    std::vector<std::pair<std::string, std::string>> qtPhones;

    for (int row = 0; row < m_phoneTable->rowCount(); ++row) {
        auto *typeItem   = m_phoneTable->item(row, 0);
        auto *numberItem = m_phoneTable->item(row, 1);
        if (!numberItem) continue;

        std::string val  = numberItem->text().trimmed().toStdString();
        std::string type = typeItem ? typeItem->text().trimmed().toStdString()
                                    : std::string();

        nums.add_number(Number(val, type));
        qtPhones.emplace_back(val, type);
    }

    m_contact = Contact(
        name.toStdString(),
        surname.toStdString(),
        patronymic.toStdString(),
        address.toStdString(),
        birth.toString("dd.MM.yyyy").toStdString(),
        email.toStdString(),
        nums
        );

    m_contact.set_qt_phones(qtPhones);

    accept();
}

void ContactDialog::setContact(const Contact &c)
{
    m_contact = c;

    m_nameEdit->setText(QString::fromStdString(c.get_name()));
    m_surnameEdit->setText(QString::fromStdString(c.get_surname()));
    m_patronymicEdit->setText(QString::fromStdString(c.get_name3()));
    m_addressEdit->setText(QString::fromStdString(c.get_address()));
    m_emailEdit->setText(QString::fromStdString(c.get_email()));

    const QString dateStr = QString::fromStdString(c.get_date());
    const QDate birth = QDate::fromString(dateStr, "dd.MM.yyyy");
    if (birth.isValid())
        m_birthEdit->setDate(birth);

    m_phoneTable->setRowCount(0);


    const auto &phones = c.get_qt_phones();
    for (size_t i = 0; i < phones.size(); ++i) {
        int row = m_phoneTable->rowCount();
        m_phoneTable->insertRow(row);

        m_phoneTable->setItem(row, 0,
                              new QTableWidgetItem(QString::fromStdString(phones[i].second)));
        m_phoneTable->setItem(row, 1,
                              new QTableWidgetItem(QString::fromStdString(phones[i].first)));
    }
}

Contact ContactDialog::contact() const
{
    return m_contact;
}
