#ifndef INSOpenBC_H
#define INSOpenBC_H

#pragma once

#include "IntegratedBC.h"

class INSOpenBC;

template <>
InputParameters validParams<INSOpenBC>();

class INSOpenBC : public IntegratedBC
{
public:
  INSOpenBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  /// Value of grad(u) on the boundary.
  const unsigned int _component;
  const VariableValue & _p_old;
};

#endif // INSOpenBC_H
