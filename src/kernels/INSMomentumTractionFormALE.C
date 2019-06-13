#include "INSMomentumTractionFormALE.h"

registerMooseObject("fsiplaygroundApp", INSMomentumTractionFormALE);

template <>
InputParameters
validParams<INSMomentumTractionFormALE>()
{
  InputParameters params = validParams<INSMomentumTractionForm>();
  params.addClassDescription("This class computes momentum equation residual and Jacobian viscous "
                             "contributions for the 'traction' form of the governing equations in "
                             "ALE form on the referential configuration.");
  params.addRequiredCoupledVar("mesh_disp_x", "x component of mesh displacement");
  params.addCoupledVar("mesh_disp_y", 0, "y component of mesh displacement");
  params.addCoupledVar("mesh_disp_z", 0, "z component of mesh displacement");
  params.set<bool>("use_displaced_mesh") = true;
  return params;
}

INSMomentumTractionFormALE::INSMomentumTractionFormALE(const InputParameters & parameters)
  : INSMomentumTractionForm(parameters),
    _u_mesh(coupledDot("mesh_disp_x")),
    _v_mesh(coupledDot("mesh_disp_y")),
    _w_mesh(coupledDot("mesh_disp_z"))
{
}

RealVectorValue
INSMomentumTractionFormALE::convectiveTerm()
{
  RealVectorValue U_vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
  RealVectorValue U_mesh(_u_mesh[_qp], _v_mesh[_qp], _w_mesh[_qp]);
  RealVectorValue U = U_vel - U_mesh;

  return _rho[_qp] *
         RealVectorValue(U * _grad_u_vel[_qp], U * _grad_v_vel[_qp], U * _grad_w_vel[_qp]);
}

RealVectorValue
INSMomentumTractionFormALE::dConvecDUComp(unsigned comp)
{
  RealVectorValue U_vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
  RealVectorValue U_mesh(_u_mesh[_qp], _v_mesh[_qp], _w_mesh[_qp]);
  RealVectorValue U = U_vel - U_mesh;

  RealVectorValue d_U_d_comp(0, 0, 0);
  d_U_d_comp(comp) = _phi[_j][_qp];

  RealVectorValue convective_term = _rho[_qp] * RealVectorValue(d_U_d_comp * _grad_u_vel[_qp],
                                                                d_U_d_comp * _grad_v_vel[_qp],
                                                                d_U_d_comp * _grad_w_vel[_qp]);
  convective_term(comp) += _rho[_qp] * U * _grad_phi[_j][_qp];

  return convective_term;
}
