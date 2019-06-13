#ifndef CoupledTractionBC_H
#define CoupledTractionBC_H

#pragma once

#include "IntegratedBC.h"
#include "Function.h"

class CoupledTractionBC;

template <>
InputParameters validParams<CoupledTractionBC>();

class CoupledTractionBC : public IntegratedBC
{
public:
  CoupledTractionBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  const VariableValue & _traction;
  const Real _factor;
  const Function * _function;
  const PostprocessorValue * const _postprocessor;
};

#endif // CoupledTractionBC_H
