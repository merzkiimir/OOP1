#pragma once

#include <QDialog>
#include <unordered_map>
#include <string>

class QLineEdit;

class SearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SearchDialog(QWidget *parent = nullptr);

    std::unordered_map<std::string, std::string> query() const;

private:
    QLineEdit *m_nameEdit;
    QLineEdit *m_surnameEdit;
    QLineEdit *m_emailEdit;
    QLineEdit *m_addressEdit;
    QLineEdit *m_dateEdit;
};
