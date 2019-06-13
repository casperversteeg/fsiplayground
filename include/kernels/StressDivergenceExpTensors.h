/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef STRESSDIVERGENCEEXPTENSORS_H
#define STRESSDIVERGENCEEXPTENSORS_H

#pragma once

#include "StressDivergenceTensors.h"
#include "Material.h"

/**
 * This class computes the off-diagonal Jacobian component of stress divergence residual system
 * Contribution from damage order parameter c
 * Residual calculated in StressDivergenceTensors
 * Useful if user wants to add the off diagonal Jacobian term
 */

class StressDivergenceExpTensors;

template <>
InputParameters validParams<StressDivergenceExpTensors>();

class StressDivergenceExpTensors : public StressDivergenceTensors
{
public:
  StressDivergenceExpTensors(const InputParameters & parameters);

protected:
  const MaterialProperty<RankTwoTensor> & _stress_old;

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
};

#endif // STRESSDIVERGENCEEXPTENSORS_H
