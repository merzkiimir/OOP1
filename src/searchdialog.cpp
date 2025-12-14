#include "searchdialog.h"

#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>

SearchDialog::SearchDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Поиск контактов"));

    m_nameEdit    = new QLineEdit(this);
    m_surnameEdit = new QLineEdit(this);
    m_emailEdit   = new QLineEdit(this);
    m_addressEdit = new QLineEdit(this);
    m_dateEdit    = new QLineEdit(this);

    m_dateEdit->setPlaceholderText(tr("dd.MM.yyyy"));

    auto *form = new QFormLayout;
    form->addRow(tr("Имя:"),           m_nameEdit);
    form->addRow(tr("Фамилия:"),       m_surnameEdit);
    form->addRow(tr("E-mail:"),        m_emailEdit);
    form->addRow(tr("Адрес:"),         m_addressEdit);
    form->addRow(tr("Дата рождения:"), m_dateEdit);

    auto *btnOk     = new QPushButton(tr("Искать"), this);
    auto *btnCancel = new QPushButton(tr("Отмена"), this);

    auto *buttons = new QHBoxLayout;
    buttons->addStretch();
    buttons->addWidget(btnOk);
    buttons->addWidget(btnCancel);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addLayout(buttons);
    setLayout(mainLayout);

    connect(btnOk,     &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

std::unordered_map<std::string, std::string> SearchDialog::query() const
{
    std::unordered_map<std::string, std::string> q;

    auto put = [&q](const char *key, const QString &value) {
        QString v = value.trimmed();
        if (!v.isEmpty())
            q[key] = v.toStdString();
    };

    put("name",    m_nameEdit->text());
    put("surname", m_surnameEdit->text());
    put("email",   m_emailEdit->text());
    put("address", m_addressEdit->text());
    put("date",    m_dateEdit->text());

    return q;
}
