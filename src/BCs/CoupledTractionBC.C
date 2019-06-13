#include "CoupledTractionBC.h"

registerMooseObject("fsiplaygroundApp", CoupledTractionBC);

template <>
InputParameters
validParams<CoupledTractionBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredCoupledVar("traction", "variable that stores traction");
  params.addParam<Real>("factor", 1.0, "The magnitude to use in computing the traction");
  params.addParam<FunctionName>("function", "The function that describes the traction");
  params.addParam<PostprocessorName>("postprocessor",
                                     "Postprocessor that will supply the traction value");
  return params;
}

CoupledTractionBC::CoupledTractionBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    _traction(coupledValue("traction")),
    _factor(getParam<Real>("factor")),
    _function(isParamValid("function") ? &getFunction("function") : NULL),
    _postprocessor(isParamValid("postprocessor") ? &getPostprocessorValue("postprocessor") : NULL)
{
}

Real
CoupledTractionBC::computeQpResidual()
{
  Real factor = _factor;

  if (_function)
    factor *= _function->value(_t, _q_point[_qp]);

  if (_postprocessor)
    factor *= *_postprocessor;

  return -factor * _test[_i][_qp] * _traction[_qp];
}
