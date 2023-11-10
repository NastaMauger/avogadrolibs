/******************************************************************************
  This source file is part of the Avogadro project.
  This source code is released under the 3-Clause BSD License, (see "LICENSE").
******************************************************************************/

#ifndef AVOGADRO_CALC_AMOEBA_H
#define AVOGADRO_CALC_AMOEBA_H

#include "avogadrocalcexport.h"

#include <avogadro/calc/energycalculator.h>

namespace Avogadro {
namespace Core {
class Molecule;
class UnitCell;
} // namespace Core

namespace Calc {

class AVOGADROCALC_EXPORT AMOEBA : public EnergyCalculator
{
public:
  AMOEBA();
  ~AMOEBA();

  AMOEBA* newInstance() const override { return new AMOEBA; }

  std::string identifier() const override { return "AMOEBA"; }

  std::string name() const override { return "AMOEBA"; }

  std::string description() const override
  {
    return "AMOEBA";
  }

  bool acceptsUnitCell() const override { return true; }

  Core::Molecule::ElementMask elements() const override { return (m_elements); }

  Real value(const Eigen::VectorXd& x) override;
  void gradient(const Eigen::VectorXd& x, Eigen::VectorXd& grad) override;

  /**
   * Called when the current molecule changes.
   */
  void setMolecule(Core::Molecule* mol) override;

protected:
  Core::Molecule* m_molecule;
  Core::UnitCell* m_cell;
  Eigen::MatrixXd m_radii;
  bool m_vdw;
  Real m_depth;
  int m_exponent;

  Core::Molecule::ElementMask m_elements;
};

} // namespace Calc
} // namespace Avogadro

#endif // AVOGADRO_CALC_AMOEBA_H
