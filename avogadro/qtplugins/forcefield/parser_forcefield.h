/******************************************************************************
  This source file is part of the Avogadro project.
  This source code is released under the 3-Clause BSD License, (see "LICENSE").
******************************************************************************/

#ifndef PARSER_FORCEFIELD_H
#define PARSER_FORCEFIEL_H

#include <QDialog>

namespace Avogadro {
namespace QtPlugins {

class ParserForceField {
public:
    static QString loadAndParseFile(const QString& fileName); 
    static QString amoebaReadPrm(const QString& fileName); 
};


} // namespace QtPlugins
} // namespace Avogadro

#endif // PARSER_FORCE_FIELD_H

