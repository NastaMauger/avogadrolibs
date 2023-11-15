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
            
            // Check for specific keywords or patterns and extract values as needed
            if (line.contains("bond")) {
                QStringList parts = line.split(QRegExp("\\s+"));

                // Assuming you want to extract the next 4 values after "bond"
                if (parts.size() >= 5) {
                    QString bondValue1 = parts[1]; // The first value after "bond"
                    QString bondValue2 = parts[2]; // The second value
                    QString bondValue3 = parts[3]; // The third value
                    QString bondValue4 = parts[4]; // The fourth value

                    // Check if the extracted values are numeric
                    bool conversionSuccess = false;
                    double value1 = bondValue1.toDouble(&conversionSuccess);
                    if (conversionSuccess) {
                        double value2 = bondValue2.toDouble(&conversionSuccess);
                        if (conversionSuccess) {
                            double value3 = bondValue3.toDouble(&conversionSuccess);
                            if (conversionSuccess) {
                                double value4 = bondValue4.toDouble(&conversionSuccess);
                                if (conversionSuccess) {
                                    // Debug output to verify the extracted values
                                    qDebug() << "Bond Values:";
                                    qDebug() << "Value 1: " << value1;
                                    qDebug() << "Value 2: " << value2;
                                    qDebug() << "Value 3: " << value3;
                                    qDebug() << "Value 4: " << value4;
                                }
                            }
                        }
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
