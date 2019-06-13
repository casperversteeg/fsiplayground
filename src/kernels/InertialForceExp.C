#include "InertialForceExp.h"

registerMooseObject("fsiplaygroundApp", InertialForceExp);

template <>
InputParameters
validParams<InertialForceExp>()
{
  InputParameters params = validParams<Kernel>();
  params.set<bool>("use_displaced_mesh") = true;
  params.addParam<Real>("eta", 0.0, "damping coefficient for Rayleigh damping");
  return params;
}

InertialForceExp::InertialForceExp(const InputParameters & parameters)
  : Kernel(parameters),
    _density(getMaterialProperty<Real>("density")),
    _eta(getParam<Real>("eta")),
    _u_old(valueOld()),
    _u_older(valueOlder())
{
}

Real
InertialForceExp::computeQpResidual()
{

  Real accel = 1 / _dt / _dt * (_u[_qp] - _u_old[_qp] * 2.0 + _u_older[_qp]);
  Real vel = 1 / _dt * (_u[_qp] - _u_old[_qp]);

  return _test[_i][_qp] * _density[_qp] * accel + _test[_i][_qp] * _eta * _density[_qp] * vel;
}

Real
InertialForceExp::computeQpJacobian()
{
  return _test[_i][_qp] * _density[_qp] / _dt / _dt * _phi[_j][_qp] +
         _test[_i][_qp] * _eta * _density[_qp] / _dt * _phi[_j][_qp];
}

void
InertialForceExp::computeJacobian()
{
  Kernel::computeJacobian();
}
