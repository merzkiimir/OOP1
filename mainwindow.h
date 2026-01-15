#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

#include "src/vctcontact.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnadd_clicked();
    void on_btnrem_clicked();
    void on_btnedit_clicked();
    void on_btnload_clicked();
    void on_btnsave_clicked();
    void on_btnsearch_clicked();
    void on_btnreset_clicked();
    void onHeaderSectionClicked(int logicalIndex);
    void on_btnSaveDb_clicked();    // НОВЫЙ слот: сохранить в БД
    void on_btnLoadDb_clicked();

private:
    Ui::MainWindow *ui;
    vectorContact contacts;
    vectorContact searchResult;
    bool searchActive = false;

    QSqlDatabase m_db;

    int  m_lastSortColumn = -1;
    bool m_sortAscending  = true;

    void updatetable();
    bool initDb();
};
#endif // MAINWINDOW_H
