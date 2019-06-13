#ifndef FSIFluidTraction_H
#define FSIFluidTraction_H

#pragma once

#include "AuxKernel.h"

class FSIFluidTraction;

template <>
InputParameters validParams<FSIFluidTraction>();

class FSIFluidTraction : public AuxKernel
{
public:
  FSIFluidTraction(const InputParameters & parameters);
  virtual ~FSIFluidTraction() {}

protected:
  virtual Real computeValue();

  const VariableValue & _p;
  const VariableGradient & _grad_vel;
  const unsigned int _component;
  const MaterialProperty<Real> & _mu;
  const MooseArray<Point> & _normals;
};

#endif // FSIFluidTraction_H
