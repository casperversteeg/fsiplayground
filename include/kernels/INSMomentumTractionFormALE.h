#ifndef INSMomentumTractionFormALE_H
#define INSMomentumTractionFormALE_H

#pragma once

#include "INSMomentumTractionForm.h"

// Forward Declarations
class INSMomentumTractionFormALE;

template <>
InputParameters validParams<INSMomentumTractionFormALE>();

class INSMomentumTractionFormALE : public INSMomentumTractionForm
{
public:
  INSMomentumTractionFormALE(const InputParameters & parameters);

  virtual ~INSMomentumTractionFormALE() {}

protected:
  virtual RealVectorValue convectiveTerm() override;
  virtual RealVectorValue dConvecDUComp(unsigned comp) override;

  const VariableValue & _u_mesh;
  const VariableValue & _v_mesh;
  const VariableValue & _w_mesh;
};

#endif
