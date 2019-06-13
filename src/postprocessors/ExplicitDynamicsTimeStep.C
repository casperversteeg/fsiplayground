//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ExplicitDynamicsTimeStep.h"

registerMooseObject("fsiplaygroundApp", ExplicitDynamicsTimeStep);

template <>
InputParameters
validParams<ExplicitDynamicsTimeStep>()
{
  InputParameters params = validParams<ElementPostprocessor>();
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  return params;
}

ExplicitDynamicsTimeStep::ExplicitDynamicsTimeStep(const InputParameters & parameters)
  : ElementPostprocessor(parameters),
    _qp(0),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _elasticity_tensor(getMaterialPropertyByName<RankFourTensor>(_base_name + "elasticity_tensor")),
    _stress(getMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
    _stress_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "stress")),
    _strain(getMaterialPropertyByName<RankTwoTensor>(_base_name + "mechanical_strain")),
    _strain_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "mechanical_strain")),
    _density(getMaterialPropertyByName<Real>("density"))
{
}

void
ExplicitDynamicsTimeStep::initialize()
{
  _time_step = 1.0e30;
}

void
ExplicitDynamicsTimeStep::execute()
{
  _time_step = std::min(_time_step, computeIntegral());
}

Real
ExplicitDynamicsTimeStep::computeQpIntegral()
{
  Real lambda, mu;
  if (_t_step == 0)
  {
    lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
    mu = _elasticity_tensor[_qp](0, 1, 0, 1);
  }
  else
  {
    // change in volumetric stress
    Real delta_stress_vol = (_stress[_qp].trace() - _stress_old[_qp].trace()) / 3.0;
    // change in volumetric strain
    Real delta_strain_vol = (_strain[_qp].trace() - _strain_old[_qp].trace()) / 3.0;
    // change in deviatoric stress
    RankTwoTensor delta_stress_dev = _stress[_qp].deviatoric() - _stress_old[_qp].deviatoric();
    // change in deviatoric strain
    RankTwoTensor delta_strain_dev = _strain[_qp].deviatoric() - _strain_old[_qp].deviatoric();
    // assuming a hypoelastic constitutive relation
    mu = 0.5 * (delta_stress_dev.doubleContraction(delta_strain_dev)) /
         (delta_strain_dev.doubleContraction(delta_strain_dev));
    lambda = delta_stress_vol / delta_strain_vol - 2.0 / 3.0 * mu;
  }
  Real wave_speed = std::sqrt((lambda + 2.0 * mu) / _density[_qp]);
  Real current_elem_time_step =
      0.5 * _current_elem->hmin() / wave_speed / std::sqrt(_mesh.dimension());

  return current_elem_time_step / _current_elem->volume();
}

Real
ExplicitDynamicsTimeStep::getValue()
{
  gatherMin(_time_step);
  return _time_step;
}

void
ExplicitDynamicsTimeStep::threadJoin(const UserObject & y)
{
  const ExplicitDynamicsTimeStep & pps = static_cast<const ExplicitDynamicsTimeStep &>(y);
  _time_step = std::min(_time_step, pps._time_step);
}

Real
ExplicitDynamicsTimeStep::computeIntegral()
{
  Real sum = 0;

  for (_qp = 0; _qp < _qrule->n_points(); _qp++)
    sum += _JxW[_qp] * _coord[_qp] * computeQpIntegral();

  return sum;
}
