#include "FSInterfaceNoSlipBC.h"

registerMooseObject("fsiplaygroundApp", FSInterfaceNoSlipBC);

template <>
InputParameters
validParams<FSInterfaceNoSlipBC>()
{
  InputParameters params = validParams<NodalBC>();
  params.addRequiredParam<unsigned int>("component", "component of velocity vector");

  params.addRequiredCoupledVar("mesh_disp_x", "x component of mesh displacement");
  params.addCoupledVar("mesh_disp_y", 0, "y component of mesh displacement");
  params.addCoupledVar("mesh_disp_z", 0, "z component of mesh  displacement");

  params.addCoupledVar("ref_vel_x", 0, "x component of velocity of reference frame");
  params.addCoupledVar("ref_vel_y", 0, "y component of velocity of reference frame");
  params.addCoupledVar("ref_vel_z", 0, "z component of velocity of reference frame");

  params.addRequiredCoupledVar("nodal_normal_x", "x component of nodal normal vector");
  params.addCoupledVar("nodal_normal_y", 0, "y component of nodal normal vector");
  params.addCoupledVar("nodal_normal_z", 0, "z component of nodal normal vector");
  return params;
}

FSInterfaceNoSlipBC::FSInterfaceNoSlipBC(const InputParameters & parameters)
  : NodalBC(parameters),
    _component(getParam<unsigned int>("component")),

    _u_mesh(coupledDot("mesh_disp_x")),
    _v_mesh(coupledDot("mesh_disp_y")),
    _w_mesh(coupledDot("mesh_disp_z")),

    _u_ref(coupledValue("ref_vel_x")),
    _v_ref(coupledValue("ref_vel_y")),
    _w_ref(coupledValue("ref_vel_z")),

    _normal_x(coupledValue("nodal_normal_x")),
    _normal_y(coupledValue("nodal_normal_y")),
    _normal_z(coupledValue("nodal_normal_z"))
{
}

Real
FSInterfaceNoSlipBC::computeQpResidual()
{
  RealVectorValue N(_normal_x[_qp], _normal_y[_qp], _normal_z[_qp]);
  RealVectorValue U_mesh(_u_mesh[_qp], _v_mesh[_qp], _w_mesh[_qp]);
  RealVectorValue U_ref(_u_ref[_qp], _v_ref[_qp], _w_ref[_qp]);
  RealVectorValue U_base = U_mesh + U_ref;
  RealVectorValue U_base_tangential = U_base - (U_base * N) * N;

  return _u[_qp] - U_base_tangential(_component);
}
