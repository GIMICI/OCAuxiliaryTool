#include "dlgdatabase.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgdatabase.h"

extern QTableWidget *tableDatabase;
extern MainWindow *mw_one;
extern Method *mymethod;
extern QString SaveFileName;

dlgDatabase::dlgDatabase(QWidget *parent)
    : QDialog(parent), ui(new Ui::dlgDatabase) {
  ui->setupUi(this);

  ui->editFind->setClearButtonEnabled(true);

  tableDatabase = ui->tableDatabase;
  tableDatabase->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableDatabaseFind->setEditTriggers(QAbstractItemView::NoEditTriggers);

  QTableWidgetItem *id0;

  ui->tableDatabase->setColumnWidth(0, 520);
  id0 = new QTableWidgetItem(tr("Config Database"));
  ui->tableDatabase->setHorizontalHeaderItem(0, id0);

  ui->tableDatabase->setAlternatingRowColors(true);
  tableDatabase->horizontalHeader()->setStretchLastSection(
      true);  //设置充满表宽度
  ui->tableDatabaseFind->horizontalHeader()->setStretchLastSection(true);
  tableDatabase->horizontalHeader()->setHidden(true);
  ui->tableDatabaseFind->horizontalHeader()->setHidden(true);
  ui->tableDatabaseFind->setHidden(true);

  tableDatabase->setSelectionBehavior(
      QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
  ui->tabWidget->setCurrentIndex(0);

  ui->tableKextUrl->setColumnWidth(0, 150);
  ui->tableKextUrl->setColumnWidth(1, 350);
  ui->textEdit->setHidden(true);

  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QFileInfo fi(qfile);
  QString strDef = "https://ghproxy.com/https://github.com/";
  QLocale locale;
  if (fi.exists()) {
    QSettings Reg(qfile, QSettings::IniFormat);

    if (locale.language() == QLocale::Chinese) {
      ui->comboBoxNet->setCurrentText(Reg.value("Net", strDef).toString());
    } else {
      ui->comboBoxNet->setCurrentText(
          Reg.value("Net", "https://github.com/").toString());
    }

  } else {
    if (locale.language() == QLocale::Chinese) {
      ui->comboBoxNet->setCurrentText(strDef);

    } else {
      ui->comboBoxNet->setCurrentText("https://github.com/");
    }
  }
}

dlgDatabase::~dlgDatabase() { delete ui; }
void dlgDatabase::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);
  saveKextUrl();
}

void dlgDatabase::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      close();
      break;

    case Qt::Key_Return:

      break;

    case Qt::Key_Backspace:

      break;

    case Qt::Key_Space:

      break;

    case Qt::Key_F1:

      break;
  }

  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_M) {
      this->setWindowState(Qt::WindowMaximized);
    }
  }
}

void dlgDatabase::on_tableDatabase_cellDoubleClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  mw_one->RefreshAllDatabase = true;

  QFileInfo appInfo(qApp->applicationDirPath());

  QString dirpath = appInfo.filePath() + "/Database/";
  QString file = tableDatabase->currentItem()->text();
  mw_one->openFile(dirpath + file);
  mymethod->on_GenerateEFI();

  mw_one->RefreshAllDatabase = false;
}

void dlgDatabase::on_btnFind_clicked() {
  QString text = ui->editFind->text().trimmed();
  if (text == "") return;

  tableDatabase->setFocus();
  tableDatabase->setSelectionMode(QAbstractItemView::MultiSelection);
  tableDatabase->clearSelection();
  ui->tableDatabaseFind->setRowCount(0);
  int count = 0;
  for (int i = 0; i < tableDatabase->rowCount(); i++) {
    tableDatabase->setCurrentCell(i, 0);
    QString str = tableDatabase->currentItem()->text();
    QFileInfo fi(str);
    if (fi.baseName().toLower().contains(text.toLower())) {
      ui->tableDatabaseFind->setRowCount(count + 1);
      QTableWidgetItem *newItem1;
      newItem1 = new QTableWidgetItem(str);
      ui->tableDatabaseFind->setItem(count, 0, newItem1);

      count++;

    } else {
      tableDatabase->selectRow(i);
    }
  }

  if (count > 0)
    ui->tableDatabaseFind->setHidden(false);
  else
    ui->tableDatabaseFind->setHidden(true);

  ui->editFind->setFocus();

  ui->lblCount->setText(QString::number(count));

  tableDatabase->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void dlgDatabase::on_editFind_textChanged(const QString &arg1) {
  if (arg1 == "") {
    ui->lblCount->setText("0");
    ui->tableDatabaseFind->setHidden(true);
    return;
  }

  on_btnFind_clicked();
}

void dlgDatabase::on_editFind_returnPressed() { on_btnFind_clicked(); }

void dlgDatabase::on_tableDatabaseFind_cellDoubleClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  mw_one->RefreshAllDatabase = true;

  QFileInfo appInfo(qApp->applicationDirPath());

  QString dirpath = appInfo.filePath() + "/Database/";
  QString file = ui->tableDatabaseFind->currentItem()->text();
  mw_one->openFile(dirpath + file);

  mymethod->on_GenerateEFI();

  mw_one->RefreshAllDatabase = false;
}

void dlgDatabase::on_btnRefreshAll_clicked() {
  ui->btnRefreshAll->setEnabled(false);
  this->repaint();

  mw_one->RefreshAllDatabase = true;

  QString bakFile;
  if (!SaveFileName.isEmpty()) {
    bakFile = SaveFileName;
  }

  ui->tableDatabase->setCurrentCell(0, 0);

  QFileInfo appInfo(qApp->applicationDirPath());
  QString dirpath = appInfo.filePath() + "/Database/";

  for (int i = 0; i < ui->tableDatabase->rowCount(); i++) {
    ui->tableDatabase->setCurrentCell(i, 0);
    QString file = ui->tableDatabase->currentItem()->text();
    mw_one->openFile(dirpath + file);
    mw_one->SavePlist(dirpath + file);
  }

  if (!bakFile.isEmpty()) mw_one->openFile(bakFile);

  mw_one->RefreshAllDatabase = false;

  ui->btnRefreshAll->setEnabled(true);
  this->repaint();
}

void dlgDatabase::refreshKextUrl() {
  ui->textEdit->clear();
  ui->textEdit->append("Lilu.kext | https://github.com/acidanthera/Lilu");
  ui->textEdit->append(
      "AppleALC.kext | https://github.com/acidanthera/AppleALC");
  ui->textEdit->append(
      "VoodooPS2Controller.kext | https://github.com/acidanthera/VoodooPS2");
  ui->textEdit->append(
      "WhateverGreen.kext | https://github.com/acidanthera/WhateverGreen");
  ui->textEdit->append(
      "VirtualSMC.kext | https://github.com/acidanthera/VirtualSMC");
  ui->textEdit->append(
      "VoodooI2C.kext | https://github.com/VoodooI2C/VoodooI2C");
  ui->textEdit->append(
      "RestrictEvents.kext | https://github.com/acidanthera/RestrictEvents");

  QTextEdit *txtEdit = new QTextEdit;
  QString txt = mymethod->loadText(mw_one->strConfigPath + "KextUrl.txt");
  txtEdit->setPlainText(txt);
  for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
    QString line = mymethod->getTextEditLineText(txtEdit, i).trimmed();
    bool re = false;
    for (int j = 0; j < ui->textEdit->document()->lineCount(); j++) {
      QString line2 = mymethod->getTextEditLineText(ui->textEdit, j).trimmed();
      if (line == line2) {
        re = true;
      }
    }
    if (!re) ui->textEdit->append(line);
  }

  ui->tableKextUrl->setRowCount(0);
  for (int i = 0; i < ui->textEdit->document()->lineCount(); i++) {
    QStringList list =
        mymethod->getTextEditLineText(ui->textEdit, i).split("|");
    QString str0, str1;
    if (list.count() == 2) {
      str0 = list.at(0);
      str1 = list.at(1);
      int n = ui->tableKextUrl->rowCount();
      ui->tableKextUrl->setRowCount(n + 1);
      ui->tableKextUrl->setCurrentCell(i, 0);
      ui->tableKextUrl->setItem(i, 0, new QTableWidgetItem(str0.trimmed()));
      ui->tableKextUrl->setItem(i, 1, new QTableWidgetItem(str1.trimmed()));
    }
  }

  ui->tableKextUrl->setFocus();
}

void dlgDatabase::on_btnAdd_clicked() {
  int n = ui->tableKextUrl->rowCount();
  ui->tableKextUrl->setRowCount(n + 1);
  ui->tableKextUrl->setCurrentCell(n, 0);
  ui->tableKextUrl->setItem(n, 0, new QTableWidgetItem(""));
  ui->tableKextUrl->setItem(n, 1, new QTableWidgetItem(""));
}

void dlgDatabase::on_btnDel_clicked() {
  if (ui->tableKextUrl->rowCount() == 0) return;
  int n = ui->tableKextUrl->currentRow();
  ui->tableKextUrl->removeRow(n);
  if (n - 1 == -1) n = 1;
  ui->tableKextUrl->setCurrentCell(n - 1, 0);
  ui->tableKextUrl->setFocus();
}

void dlgDatabase::saveKextUrl() {
  ui->textEdit->clear();
  QString str0, str1;
  for (int i = 0; i < ui->tableKextUrl->rowCount(); i++) {
    str0 = ui->tableKextUrl->item(i, 0)->text().trimmed();
    str1 = ui->tableKextUrl->item(i, 1)->text().trimmed();
    if (str0 != "" || str1 != "") ui->textEdit->append(str0 + " | " + str1);
  }
  mymethod->TextEditToFile(ui->textEdit, mw_one->strConfigPath + "KextUrl.txt");
}

void dlgDatabase::on_btnTest_clicked() {
  mw_one->on_actionOnline_Download_Updates_triggered();
}
