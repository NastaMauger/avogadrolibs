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
            }
            file.close();
        }
    }

    return fileContent;
}

} // namespace QtPlugins
} // namespace Avogadro
