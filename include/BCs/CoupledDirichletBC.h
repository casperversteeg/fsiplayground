#ifndef CoupledDirichletBC_H
#define CoupledDirichletBC_H

#pragma once

#include "PresetNodalBC.h"

class CoupledDirichletBC;

template <>
InputParameters validParams<CoupledDirichletBC>();

class CoupledDirichletBC : public PresetNodalBC
{
public:
  CoupledDirichletBC(const InputParameters & parameters);

protected:
  virtual Real computeQpValue() override;

  const VariableValue & _val;
};

#endif // CoupledDirichletBC_H
