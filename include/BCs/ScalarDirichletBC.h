/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef ScalarDirichletBC_H
#define ScalarDirichletBC_H

#pragma once

#include "PresetNodalBC.h"

// Forward Declarations
class ScalarDirichletBC;

template <>
InputParameters validParams<ScalarDirichletBC>();

/**
 * Defines a boundary condition that forces the value to be a user specified
 * function at the boundary.
 */
class ScalarDirichletBC : public PresetNodalBC
{
public:
  ScalarDirichletBC(const InputParameters & parameters);

protected:
  /**
   * Evaluate the function at the current quadrature point and timestep.
   */
  virtual Real computeQpValue() override;

  /// Function being used for evaluation of this BC
  const VariableValue & _val;
};

#endif // ScalarDirichletBC_H
