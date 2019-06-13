#include "KineticEnergy.h"
#include "MooseMesh.h"
#include "SubProblem.h"

registerMooseObject("fsiplaygroundApp", KineticEnergy);

template <>
InputParameters
validParams<KineticEnergy>()
{
  InputParameters params = validParams<NodalVariablePostprocessor>();
  params.set<bool>("unique_node_execute") = true;
  params.addRequiredCoupledVar("disp_x", "displacement in x direction");
  params.addCoupledVar("disp_y", "displacement in y direction");
  params.addCoupledVar("disp_z", "displacement in z direction");
  return params;
}

KineticEnergy::KineticEnergy(const InputParameters & parameters)
  : NodalVariablePostprocessor(parameters),
    MaterialPropertyInterface(this, blockIDs(), Moose::EMPTY_BOUNDARY_IDS),
    _sum(0),
    _density(getMaterialPropertyByName<Real>("density")),
    _disp_x(coupledValue("disp_x")),
    _disp_y(isParamValid("disp_y") ? coupledValue("disp_y") : _zero),
    _disp_z(isParamValid("disp_z") ? coupledValue("disp_z") : _zero),
    _disp_x_old(coupledValueOld("disp_x")),
    _disp_y_old(isParamValid("disp_y") ? coupledValueOld("disp_y") : _zero),
    _disp_z_old(isParamValid("disp_z") ? coupledValueOld("disp_z") : _zero)
{
}

void
KineticEnergy::initialize()
{
  _sum = 0;
}

void
KineticEnergy::execute()
{
  Real vel_x = (_disp_x[_qp] - _disp_x_old[_qp]) / _dt;
  Real vel_y = (_disp_y[_qp] - _disp_y_old[_qp]) / _dt;
  Real vel_z = (_disp_z[_qp] - _disp_z_old[_qp]) / _dt;
  RealVectorValue vel(vel_x, vel_y, vel_z);
  _sum += 0.5 * _density[_qp] * vel * vel;
}

Real
KineticEnergy::getValue()
{
  gatherSum(_sum);

  return _sum;
}

void
KineticEnergy::threadJoin(const UserObject & y)
{
  const KineticEnergy & pps = static_cast<const KineticEnergy &>(y);
  _sum += pps._sum;
}
