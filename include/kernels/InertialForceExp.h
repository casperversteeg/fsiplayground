#ifndef INERTIALFORCEEXP_H
#define INERTIALFORCEEXP_H

#pragma once

#include "Kernel.h"

// Forward Declarations
class InertialForceExp;

template <>
InputParameters validParams<InertialForceExp>();

class InertialForceExp : public Kernel
{
public:
  InertialForceExp(const InputParameters & parameters);

  virtual void computeJacobian() override;

protected:
  virtual Real computeQpResidual() override;

  virtual Real computeQpJacobian() override;

private:
  const MaterialProperty<Real> & _density;
  Real _eta;
  const VariableValue & _u_old;
  const VariableValue & _u_older;
};

#endif // INERTIALFORCEEXP_H
