/******************************************************************************
  This source file is part of the Avogadro project.
  This source code is released under the 3-Clause BSD License, (see "LICENSE").
******************************************************************************/

#ifndef PARSER_AMOEBA_H
#define PARSER_AMOEBA_H

#include <QDialog>

namespace Avogadro {
namespace QtPlugins {

class ParserForceField {
public:
    static QString loadAndParseFile(QString& fileName); 
};

} // namespace QtPlugins
} // namespace Avogadro

#endif // PARSER_AMOEBA_H

