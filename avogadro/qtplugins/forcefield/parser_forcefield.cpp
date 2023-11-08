/******************************************************************************
  This source file is part of the Avogadro project.
  This source code is released under the 3-Clause BSD License, (see "LICENSE").
******************************************************************************/

#include "forcefielddialog.h"
#include "ui_forcefielddialog.h"
#include "parser_forcefield.h"
#include "readprm.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDir>   
#include <QtCore/QtGlobal>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QMessageBox>

namespace Avogadro {
namespace QtPlugins {

QString ParserForceField::loadAndParseFile(QString& fileName) {
    fileName = QFileDialog::getOpenFileName(nullptr, "Select Parameter File", QDir::homePath(), "Parameter Files (*.prm *.txt *.out);;All Files (*)");
    QString fileContent;

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                fileContent.append(line);
                qDebug() << line;

                // Check if the line contains the word 'amoeba'
                if (line.contains("amoeba", Qt::CaseInsensitive)) {
                    // Found 'amoeba' in the file
                    QString result = amoebaReadPrm(fileContent);
                    break;
                } else {
                    QMessageBox msgBox;
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText("Only AMOEBA parameter file can be read");
                    msgBox.setWindowTitle("Error");
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.exec();
                    msgBox.close();
                    file.close();  
                    return fileContent;
                }
            }
            file.close();
        } else {
            // File open error
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Error opening the file.");
            msgBox.setWindowTitle("Error");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    return fileContent;
}



} // namespace QtPlugins
} // namespace Avogadro
