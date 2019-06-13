#ifndef FSInterfaceNoSlipBC_H
#define FSInterfaceNoSlipBC_H

#pragma once

#include "NodalBC.h"

class FSInterfaceNoSlipBC;

template <>
InputParameters validParams<FSInterfaceNoSlipBC>();

class FSInterfaceNoSlipBC : public NodalBC
{
public:
  FSInterfaceNoSlipBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  const unsigned int _component;

  const VariableValue & _u_mesh;
  const VariableValue & _v_mesh;
  const VariableValue & _w_mesh;

  const VariableValue & _u_ref;
  const VariableValue & _v_ref;
  const VariableValue & _w_ref;

  const VariableValue & _normal_x;
  const VariableValue & _normal_y;
  const VariableValue & _normal_z;
};

#endif // FSInterfaceNoSlipBC_H
