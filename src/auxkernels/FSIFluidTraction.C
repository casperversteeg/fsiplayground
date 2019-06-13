#include "FSIFluidTraction.h"

registerMooseObject("fsiplaygroundApp", FSIFluidTraction);

template <>
InputParameters
validParams<FSIFluidTraction>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("p", "pressure");
  params.addRequiredCoupledVar(
      "vel", "corresponding component of the velocity, i.e. provide vel_x if component is 0");
  params.addRequiredParam<unsigned int>("component", "0 for x, 1 for y, 2 for z");
  params.addParam<MaterialPropertyName>("mu_name", "mu", "The name of the dynamic viscosity");
  return params;
}

FSIFluidTraction::FSIFluidTraction(const InputParameters & parameters)
  : AuxKernel(parameters),
    _p(coupledValue("p")),
    _grad_vel(coupledGradient("vel")),
    _component(getParam<unsigned int>("component")),
    _mu(getMaterialProperty<Real>("mu_name")),
    _normals(_assembly.normals())
{
  if (_mesh.dimension() == 3 && !isParamValid("vel_z"))
    mooseError("missing z component of the velocity");
}

Real
FSIFluidTraction::computeValue()
{
  if (isNodal())
    mooseError("must run on an elemental variable");

  RealVectorValue sigma_row = 2.0 * _mu[_qp] * _grad_vel[_qp];
  sigma_row(_component) = sigma_row(_component) - _p[_qp];
  return sigma_row * _normals[_qp];
}
