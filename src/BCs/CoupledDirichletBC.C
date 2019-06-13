#include "ScalarDirichletBC.h"

registerMooseObject("fsiplaygroundApp", ScalarDirichletBC);

template <>
InputParameters
validParams<ScalarDirichletBC>()
{
  InputParameters params = validParams<PresetNodalBC>();
  params.addRequiredCoupledVar("var", "coupled scalar");
  return params;
}

ScalarDirichletBC::ScalarDirichletBC(const InputParameters & parameters)
  : PresetNodalBC(parameters), _val(coupledScalarValue("var"))
{
}

Real
ScalarDirichletBC::computeQpValue()
{
  return _val[0];
}
