#include "INSOpenBC.h"

registerMooseObject("fsiplaygroundApp", INSOpenBC);

template <>
InputParameters
validParams<INSOpenBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addClassDescription("This class implements a form of the Neumann boundary condition in "
                             "which the boundary term is treated 'implicitly'.");
  params.addRequiredParam<unsigned int>("component", "0 for x, 1 for y, 2 for z");
  params.addRequiredCoupledVar("p", "pressure variable");
  return params;
}

INSOpenBC::INSOpenBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    _component(getParam<unsigned int>("component")),
    _p_old(coupledValueOld("p"))
{
}

Real
INSOpenBC::computeQpResidual()
{
  RealVectorValue residual = _p_old[_qp] * _normals[_qp] * _test[_i][_qp];
  return residual(_component);
}
