[GlobalParams]
  gravity = '0 0 0'
  integrate_p_by_parts = true
  laplace = true
  convective_term = true
  transient_term = true
  pspg = true
[]

[Mesh]
  type = FileMesh
  file = mesh/fluid.msh
  dim = 2
[]

[Variables]
  [./vel_x]
    block = 'fluid'
  [../]
  [./vel_y]
    block = 'fluid'
  [../]
  [./p]
    block = 'fluid'
  [../]
[]

[Kernels]
  [./vel_x_time]
    type = INSMomentumTimeDerivative
    variable = vel_x
    block = 'fluid'
    use_displaced_mesh = true
  [../]
  [./vel_y_time]
    type = INSMomentumTimeDerivative
    variable = vel_y
    block = 'fluid'
    use_displaced_mesh = true
  [../]
  [./mass]
    type = INSMass
    variable = p
    u = vel_x
    v = vel_y
    p = p
    block = 'fluid'
    use_displaced_mesh = true
  [../]
  [./x_momentum_space]
    type = INSMomentumLaplaceForm
    variable = vel_x
    u = vel_x
    v = vel_y
    p = p
    component = 0
    block = 'fluid'
    use_displaced_mesh = true
  [../]
  [./y_momentum_space]
    type = INSMomentumLaplaceForm
    variable = vel_y
    u = vel_x
    v = vel_y
    p = p
    component = 1
    block = 'fluid'
    use_displaced_mesh = true
  [../]
[]

[Materials]
  [./const]
    type = GenericConstantMaterial
    block = 'fluid'
    prop_names = 'rho mu'
    prop_values = '1  1'
  [../]
[]

[BCs]
  [./fluid_x_no_slip]
    type = DirichletBC
    variable = vel_x
    boundary = 'no_slip dam_left dam_top dam_right'
    value = 0.0
  [../]
  [./fluid_y_no_slip]
    type = DirichletBC
    variable = vel_y
    boundary = 'no_slip dam_left dam_top dam_right'
    value = 0.0
  [../]
  [./x_inlet]
    type = FunctionDirichletBC
    variable = vel_x
    boundary = 'inlet'
    function = '1'
  [../]
  [./outlet]
    type = FunctionDirichletBC
    variable = p
    boundary = 'outlet'
    function = '0'
  []
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 5
  # num_steps = 60
  dt = 0.1
  dtmin = 0.1
  nl_abs_tol = 1e-10
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  line_search = none
[]

[Outputs]
  [./out]
    type = Exodus
    file_base = output/fluid_test_out
  [../]
[]
