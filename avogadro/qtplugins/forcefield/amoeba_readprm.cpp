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

QString ParserForceField::amoebaReadPrm(const QString& fileName)
{
 QString result;
 QFile file(fileName);
 if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    
    //BOND STRETCHING PARAMETERS//
    if (line.contains("bond")) {
      QStringList parts = line.split(QRegExp("\\s+"));

      if (parts.size() >= 5) {
        QString BondStretchValue1 = parts[1]; // Atom 1
        QString BondStretchValue2 = parts[2]; // Atom 2
        QString BondStretchValue3 = parts[3]; // Bond Force constant
        QString BondStretchValue4 = parts[4]; // Bond Length between Atom1 and Atom2

        // Check if the extracted values are numeric
        bool conversionSuccess = false;
        double atom1Name = BondStretchValue1.toDouble(&conversionSuccess);
        double atom2Name = conversionSuccess ? BondStretchValue2.toDouble(&conversionSuccess) : 0.0;
        double BondForceConstant = conversionSuccess ? BondStretchValue3.toDouble(&conversionSuccess) : 0.0;
        double BondLength = conversionSuccess ? BondStretchValue4.toDouble(&conversionSuccess) : 0.0;

        if (conversionSuccess) {
          // Debug output to verify the extracted values
          qDebug() << "Bond Values between Atom" << atom1Name << "and Atom" << atom2Name << ";";
          qDebug() << "Atom 1 Name: " << atom1Name;
          qDebug() << "Atom 2 Name: " << atom2Name;
          qDebug() << "Bond Force Constant: " << BondForceConstant;
          qDebug() << "Bond Length between Atom" << atom1Name << "and" << atom2Name << ":" << BondLength;
        }
      }
    }
  }
file.close();
}   
return result;
}


} // namespace QtPlugins
} // namespace Avogadro
