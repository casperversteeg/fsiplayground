//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef ExplicitDynamicsTimeStep_H
#define ExplicitDynamicsTimeStep_H

#pragma once

#include "ElementPostprocessor.h"
#include "RankTwoTensor.h"

// Forward Declarations
class ExplicitDynamicsTimeStep;

template <>
InputParameters validParams<ExplicitDynamicsTimeStep>();

/**
 * This postprocessor computes an average element size (h) for the whole domain.
 */
class ExplicitDynamicsTimeStep : public ElementPostprocessor
{
public:
  ExplicitDynamicsTimeStep(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;

  virtual Real getValue() override;
  virtual void threadJoin(const UserObject & y) override;

protected:
  virtual Real computeQpIntegral();
  virtual Real computeIntegral();
  unsigned int _qp;

  Real _time_step;
  const std::string _base_name;
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;
  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _stress_old;
  const MaterialProperty<RankTwoTensor> & _strain;
  const MaterialProperty<RankTwoTensor> & _strain_old;
  const MaterialProperty<Real> & _density;
};

#endif // ExplicitDynamicsTimeStep_H
