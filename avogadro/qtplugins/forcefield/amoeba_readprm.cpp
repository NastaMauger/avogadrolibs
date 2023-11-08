/******************************************************************************
  This source file is part of the Avogadro project.
  This source code is released under the 3-Clause BSD License, (see "LICENSE").
******************************************************************************/

#include "forcefielddialog.h"
#include "ui_forcefielddialog.h"
#include "readprm.h"

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

QString amoebaReadPrm(const QString& fileContent) 
{
 qDebug () << "OK  ";
 return "";
}


} // namespace QtPlugins
} // namespace Avogadro
