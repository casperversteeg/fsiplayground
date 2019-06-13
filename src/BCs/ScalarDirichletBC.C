#include "CoupledDirichletBC.h"

registerMooseObject("fsiplaygroundApp", CoupledDirichletBC);

template <>
InputParameters
validParams<CoupledDirichletBC>()
{
  InputParameters params = validParams<PresetNodalBC>();
  params.addRequiredCoupledVar("var", "coupled variable");
  return params;
}

CoupledDirichletBC::CoupledDirichletBC(const InputParameters & parameters)
  : PresetNodalBC(parameters), _val(coupledValue("var"))
{
}

Real
CoupledDirichletBC::computeQpValue()
{
  return _val[_qp];
}
