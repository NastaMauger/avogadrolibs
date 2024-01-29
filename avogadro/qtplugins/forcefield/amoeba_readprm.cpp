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
    
    // ATOM TYPE PARAMETERS
    if (line.contains("atom")) {
      QStringList parts = line.split(QRegExp("\\s+"));

      if (parts.size() >= 6) {
        QString AtomTypeValue1 = parts[1]; // Atom Multipole Value Reference
        QString AtomTypeValue2 = parts[2]; // Atom Vdw Value Reference
        QString AtomTypeValue3 = parts[3]; // Atom Type
        QString AtomTypeValue4 = parts[4] + " " + parts[5] + " " + parts[6];

        // Check if the extracted values are numeric
        bool conversionSuccess = false;
        double Multipole_Reference_Atom = AtomTypeValue1.toDouble(&conversionSuccess);
        double Vdw_Reference_Atom = conversionSuccess ? AtomTypeValue2.toDouble(&conversionSuccess) : 0.0;

        if (conversionSuccess) {
          // Debug output to verify the extracted values
          qDebug() << "Atom Type Definitions:";
          qDebug() << "Atom Type: " << AtomTypeValue4; 
          qDebug() << "Atom Name: " << AtomTypeValue3; 
        }
      }
    }

    // VDW PARAMETERS 
    if (line.contains("vdw")) {                                                                                                             
      QStringList parts = line.split(QRegExp("\\s+"));

      if (parts.size() >= 4) {                                                                                                                
        QString VDWValue1 = parts[1]; // Atom Type 
        QString VDWValue2 = parts[2]; // vdW Diameter 
        QString VDWValue3 = parts[3]; // vdW Epsilon
        QString VDWValue4; // vdW Reduction (initialize it as an empty string)

        // Check if the extracted values are numeric
        bool conversionSuccess = false;
        double vdWType = VDWValue1.toDouble(&conversionSuccess);
        double vdWDiameter = conversionSuccess ? VDWValue2.toDouble(&conversionSuccess) : 0.0;    
        double vdWeps = conversionSuccess ? VDWValue3.toDouble(&conversionSuccess) : 0.0;

        // Check if there is a 5th part (vdW Reduction)
        if (parts.size() >= 5) {
          VDWValue4 = parts[4]; // vdW Reduction 
        }   

        // Only try to convert if VDWValue4 is not empty and parts size is greater than or equal to 5
        if (!VDWValue4.isEmpty() && parts.size() >= 5) {
          double vdWReduc = conversionSuccess ? VDWValue4.toDouble(&conversionSuccess) : 0.0;
          // Debug output to verify the extracted values
          qDebug() << "vdW Values of Atom Type" << vdWType;
          qDebug() << "vdW Parameter " << vdWDiameter; 
          qDebug() << "vdW Epsilon: " <<  vdWeps;
          qDebug() << "vdW Reduction: " << vdWReduc;
        }
        else {
          qDebug() << "vdW Values of Atom Type" << vdWType;
          qDebug() << "vdW Parameter " << vdWDiameter; 
          qDebug() << "vdW Epsilon: " <<  vdWeps;
        }
      }   
    }


    //BOND STRETCHING PARAMETERS
    if (line.contains("bond")) {
      QStringList parts = line.split(QRegExp("\\s+"));

      if (parts.size() >= 5) {
        QString BondStretchValue1 = parts[1]; // Atom Type 1
        QString BondStretchValue2 = parts[2]; // Atom Type 2
        QString BondStretchValue3 = parts[3]; // Bond Force constant
        QString BondStretchValue4 = parts[4]; // Bond Length between Atom1 and Atom2

        // Check if the extracted values are numeric
        bool conversionSuccess = false;
        double atom1BondName = BondStretchValue1.toDouble(&conversionSuccess);
        double atom2BondName = conversionSuccess ? BondStretchValue2.toDouble(&conversionSuccess) : 0.0;
        double BondForceConstant = conversionSuccess ? BondStretchValue3.toDouble(&conversionSuccess) : 0.0;
        double BondLength = conversionSuccess ? BondStretchValue4.toDouble(&conversionSuccess) : 0.0;

        if (conversionSuccess) {
          // Debug output to verify the extracted values
          qDebug() << "Bond Values between Atom Type" << atom1BondName << "and Atom Type" << atom2BondName << ":";
          qDebug() << "Bond Force Constant:" << BondForceConstant;
          qDebug() << "Bond Length between Atom Type" << atom1BondName << "and" << atom2BondName << ":" << BondLength;
        }
      }
    }

    //ANGLE BENDING PARAMETERS//
    if (line.contains("angle")) {
      QStringList parts = line.split(QRegExp("\\s+"));

      if (parts.size() >= 6) {
        QString AngleBendValue1 = parts[1]; // Atom Type 1
        QString AngleBendValue2 = parts[2]; // Atom Type 2
        QString AngleBendValue3 = parts[3]; // Atom Type 3
        QString AngleBendValue4 = parts[4]; // Angle Force constant
        QString AngleBendValue5 = parts[5]; // Atom Type 1 - Atom Type 2 - Atom Type 3 angle

        // Check if the extracted values are numeric
        bool conversionSuccess = false;
        double atom1AngleName = AngleBendValue1.toDouble(&conversionSuccess);
        double atom2AngleName = conversionSuccess ? AngleBendValue2.toDouble(&conversionSuccess) : 0.0;
        double atom3AngleName = conversionSuccess ? AngleBendValue3.toDouble(&conversionSuccess) : 0.0;
        double AngleForceConstant = conversionSuccess ? AngleBendValue4.toDouble(&conversionSuccess) : 0.0;
        double AngleLength = conversionSuccess ? AngleBendValue5.toDouble(&conversionSuccess) : 0.0;

        if (conversionSuccess) {
          // Debug output to verify the extracted values
          qDebug() << "Angle Values between Atom Type" << atom1AngleName <<","<< atom2AngleName << "and" <<atom3AngleName << ":";
          qDebug() << "Angle Force Constant:" << AngleForceConstant;
          qDebug() << "Angle between Atom Type" <<  atom1AngleName <<","<< atom2AngleName << "and" << atom3AngleName << "is :" <<AngleLength;
        }
      }
    }

    //UREY-BRADLEY PARAMETERS
    if (line.contains("ureybrad")) {
      QStringList parts = line.split(QRegExp("\\s+"));

      if (parts.size() >= 6) {
        QString UBValue1 = parts[1]; // Atom Type 1
        QString UBValue2 = parts[2]; // Atom Type 2
        QString UBValue3 = parts[3]; // Atom Type 3
        QString UBValue4 = parts[4]; // UB Atom Type 1 and Atom Type 3 length 
        QString UBValue5 = parts[5]; // UB Force constant 

        // Check if the extracted values are numeric
        bool conversionSuccess = false;
        double atom1UBName = UBValue1.toDouble(&conversionSuccess);
        double atom2UBName = conversionSuccess ? UBValue2.toDouble(&conversionSuccess) : 0.0;
        double atom3UBName = conversionSuccess ? UBValue3.toDouble(&conversionSuccess) : 0.0;
        double UBLength = conversionSuccess ? UBValue4.toDouble(&conversionSuccess) : 0.0;
        double UBForceConstant = conversionSuccess ? UBValue5.toDouble(&conversionSuccess) : 0.0;

        if (conversionSuccess) {
          // Debug output to verify the extracted values
          qDebug() << "UB Values between Atom Type" << atom1UBName <<","<< atom2UBName << "and" <<atom3UBName << ":";
          qDebug() << "UB Length between Atom Type" <<  atom1UBName << "and" << atom3UBName << "is ;" <<UBLength;
          qDebug() << "UB Force Constant: " << UBForceConstant;
        }
      }
    }

    //DIPOLE POLARIZABILITY PARAMETER
    if (line.contains("polarize")) {
      QStringList parts = line.split(QRegExp("\\s+"));

      if (parts.size() <= 6) {
        QString PolarizeValue1 = parts[1]; // Atom Type 1
        QString PolarizeValue2 = parts[2]; // Polarization of Atom Type 1
        QString PolarizeValue3 = parts[3]; // Mutual dampimg factor
        QString PolarizeValue4 = parts[4]; // Atome Type 2

        // Check if the extracted values are numeric
        bool conversionSuccess = false;
        double atom1PolarizeName= PolarizeValue1.toDouble(&conversionSuccess);
        double PolarizeValue = conversionSuccess ? PolarizeValue2.toDouble(&conversionSuccess) : 0.0;
        double DampingFactor = conversionSuccess ? PolarizeValue3.toDouble(&conversionSuccess) : 0.0;
        double atom2PolarizeName = conversionSuccess ? PolarizeValue4.toDouble(&conversionSuccess) : 0.0;

        if (conversionSuccess) {
          // Debug output to verify the extracted values
          qDebug() << "Dipole Polarizability Values of Atom Type" << atom1PolarizeName ;
          qDebug() << "Polarizability" <<  PolarizeValue ;
          qDebug() << "Damping Factor: " << DampingFactor;
          qDebug() << "with Atom Type: " << atom2PolarizeName;
      
        }
      }
    }


    // ATOMIC MULTIPOLE PARAMETERS
    if (line.contains("multipole")) {
      QStringList parts = line.split(QRegExp("\\s+"));
    
      if (parts.size() <= 6) {
        // Monopole parameters
        QString MultipoleValue1 = parts[1]; // Atom Type 1
        QString MultipoleValue2 = parts[2]; // Atom Type 2
        QString MultipoleValue3 = parts[3]; // Atom Type 3
        QString MultipoleValue4 = parts[4]; // Monopole Atom Type 1
    
        // Check if the extracted values are numeric
        bool monopoleConversionSuccess = false;
        double atom1MultipoleName = MultipoleValue1.toDouble(&monopoleConversionSuccess);
        double atom2MultipoleName = monopoleConversionSuccess ? qAbs(MultipoleValue2.toDouble(&monopoleConversionSuccess)) : 0.0;
        double atom3MultipoleName = monopoleConversionSuccess ? qAbs(MultipoleValue3.toDouble(&monopoleConversionSuccess)) : 0.0;
        double Monopole = monopoleConversionSuccess ? MultipoleValue4.toDouble(&monopoleConversionSuccess) : 0.0;
    
        if (monopoleConversionSuccess) {
          // Debug output to verify the extracted values
          qDebug() << "Multipole Values of Atom Type" << atom1MultipoleName;
          qDebug() << "Monopole:" << Monopole;
        }
      }
    
      // Read Dipole Line
      QString dipoleLine = in.readLine();
      QStringList dipoleParts = dipoleLine.split(QRegExp("\\s+"));
    
      QString MultipoleValue5 = dipoleParts[1]; // Dipole X Atom Type 1
      QString MultipoleValue6 = dipoleParts[2]; // Dipole Y Atom Type 1
      QString MultipoleValue7 = dipoleParts[3]; // Dipole Z Atom Type 1
    
      // Check if the extracted values are numeric
      bool dipoleConversionSuccess = false;
      double DipoleX = MultipoleValue5.toDouble(&dipoleConversionSuccess);
      double DipoleY = dipoleConversionSuccess ? MultipoleValue6.toDouble(&dipoleConversionSuccess) : 0.0;
      double DipoleZ = dipoleConversionSuccess ? MultipoleValue7.toDouble(&dipoleConversionSuccess) : 0.0;
    
      if (dipoleConversionSuccess) {
        // Debug output to verify the extracted values
        qDebug() << "Dipole X:" << DipoleX;
        qDebug() << "Dipole Y:" << DipoleY;
        qDebug() << "Dipole Z:" << DipoleZ;
      }
    
      // Read Quadrupole Matrix (Line 1)
      QString quadrupoleLine1 = in.readLine();
      QStringList quadrupoleParts1 = quadrupoleLine1.split(QRegExp("\\s+"));
    
      QString MultipoleValue8 = quadrupoleParts1[1]; // Quadrupole XX Atom Type 1
    
      // Check if the extracted values are numeric
      bool quadrupoleConversionSuccess1 = false;
      double QuadrupoleXX = MultipoleValue8.toDouble(&quadrupoleConversionSuccess1);
    
      if (quadrupoleConversionSuccess1) {
        // Debug output to verify the extracted values
        qDebug() << "Quadrupole XX:" << QuadrupoleXX;
      }

      // Read Quadrupole Matrix (Line 2)
      QString quadrupoleLine2 = in.readLine();
      QStringList quadrupoleParts2 = quadrupoleLine2.split(QRegExp("\\s+"));

      QString MultipoleValue9 = quadrupoleParts2[1]; // Quadrupole XY Atom Type 1
      QString MultipoleValue10 = quadrupoleParts2[2]; // Quadrupole YY Atom Type 1

      // Check if the extracted values are numeric
      bool quadrupoleConversionSuccess2 = false;
      double QuadrupoleXY = MultipoleValue9.toDouble(&quadrupoleConversionSuccess2);
      double QuadrupoleYY = quadrupoleConversionSuccess2 ? MultipoleValue10.toDouble(&quadrupoleConversionSuccess2) : 0.0;

      if (quadrupoleConversionSuccess2) {
        // Debug output to verify the extracted values
        qDebug() << "Quadrupole XY:" << QuadrupoleXY;
        qDebug() << "Quadrupole YY:" << QuadrupoleYY;
      }

      // Read Quadrupole Matrix (Line 3)
      QString quadrupoleLine3 = in.readLine();
      QStringList quadrupoleParts3 = quadrupoleLine3.split(QRegExp("\\s+"));

      QString MultipoleValue11 = quadrupoleParts3[1]; // Quadrupole XZ Atom Type 1
      QString MultipoleValue12 = quadrupoleParts3[2]; // Quadrupole YZ Atom Type 1
      QString MultipoleValue13 = quadrupoleParts3[3]; // Quadrupole ZZ Atom Type 1

      // Check if the extracted values are numeric
      double QuadrupoleXZ = MultipoleValue11.toDouble();
      double QuadrupoleYZ = MultipoleValue12.toDouble();
      double QuadrupoleZZ = MultipoleValue13.toDouble();

     // Debug output to verify the extracted values
     qDebug() << "Quadrupole XZ:" << QuadrupoleXZ;
     qDebug() << "Quadrupole YZ:" << QuadrupoleYZ;
     qDebug() << "Quadrupole ZZ:" << QuadrupoleZZ;
    }
  }






file.close();
}   
return result;
}


} // namespace QtPlugins
} // namespace Avogadro