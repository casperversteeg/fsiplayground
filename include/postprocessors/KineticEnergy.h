#ifndef KineticEnergy_H
#define KineticEnergy_H

#pragma once

#include "NodalVariablePostprocessor.h"
#include "MaterialPropertyInterface.h"

class KineticEnergy;

template <>
InputParameters validParams<KineticEnergy>();

class KineticEnergy : public NodalVariablePostprocessor, public MaterialPropertyInterface
{
public:
  KineticEnergy(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() override;

  void threadJoin(const UserObject & y) override;

protected:
  Real _sum;
  const MaterialProperty<Real> & _density;
  const VariableValue & _disp_x;
  const VariableValue & _disp_y;
  const VariableValue & _disp_z;
  const VariableValue & _disp_x_old;
  const VariableValue & _disp_y_old;
  const VariableValue & _disp_z_old;
};

#endif // KineticEnergy_H
