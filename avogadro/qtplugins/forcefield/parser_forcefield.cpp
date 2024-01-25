/******************************************************************************
  This source file is part of the Avogadro project.
  This source code is released under the 3-Clause BSD License, (see "LICENSE").
******************************************************************************/

#include "forcefielddialog.h"
#include "ui_forcefielddialog.h"
#include "parser_forcefield.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDir>   
#include <QtCore/QtGlobal>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

namespace Avogadro {
namespace QtPlugins {


QString ParserForceField::loadAndParseFile(const QString& fileName)
{
 QString fileContent;

 if (!fileName.isEmpty()) {
  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    QString firstLine = in.readLine();
    // Check if the first line contains AMOEBA'
    //We might want to add new readprm function depending on waht people need/want
    if (firstLine.trimmed().toUpper().contains("AMOEBA")) {
      QString result = amoebaReadPrm(fileName);
    } 
    else {
      QMessageBox msgBox;
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.setText("Only AMOEBA parameter file can be read");
      msgBox.setWindowTitle("Error");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
      msgBox.close();
      file.close();
    }
  } 
  else {
    // File open error
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("Error opening the file.");
    msgBox.setWindowTitle("Error");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    file.close();
  }
 }
return fileContent;
}



} // namespace QtPlugins
} // namespace Avogadro
