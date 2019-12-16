[GlobalParams]
  gravity = '0 0 0'
  integrate_p_by_parts = true
  laplace = true
  convective_term = true
  transient_term = true
  pspg = false
  displacements = 'disp_x disp_y'
  order = 'SECOND'
[]

[Mesh]
  type = FileMesh
  file = mesh/domain.msh
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
    order = FIRST
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./vel_x_solid]
    block = 'solid'
  [../]
  [./vel_y_solid]
    block = 'solid'
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
  [./vel_x_mesh]
    type = INSConvectedMesh
    disp_x = disp_x
    disp_y = disp_y
    variable = vel_x
    block = 'fluid'
    use_displaced_mesh = true
  [../]
  [./vel_y_mesh]
    type = INSConvectedMesh
    disp_x = disp_x
    disp_y = disp_y
    variable = vel_y
    block = 'fluid'
    use_displaced_mesh = true
  [../]
  [./disp_x_fluid]
    type = Diffusion
    variable = disp_x
    block = 'fluid'
  [../]
  [./disp_y_fluid]
    type = Diffusion
    variable = disp_y
    block = 'fluid'
    use_displaced_mesh = true
  [../]
  [./accel_tensor_x]
    type = CoupledTimeDerivative
    variable = disp_x
    v = vel_x_solid
    block = 'solid'
    use_displaced_mesh = true
  [../]
  [./accel_tensor_y]
    type = CoupledTimeDerivative
    variable = disp_y
    v = vel_y_solid
    block = 'solid'
    use_displaced_mesh = true
  [../]
  [./vxs_time_derivative_term]
    type = CoupledTimeDerivative
    variable = vel_x_solid
    v = disp_x
    block = 'solid'
    use_displaced_mesh = true
  [../]
  [./vys_time_derivative_term]
    type = CoupledTimeDerivative
    variable = vel_y_solid
    v = disp_y
    block = 'solid'
    use_displaced_mesh = true
  [../]
  [./source_vxs]
    type = MatReaction
    variable = vel_x_solid
    block = 'solid'
    mob_name = 1
    use_displaced_mesh = true
  [../]
  [./source_vys]
    type = MatReaction
    variable = vel_y_solid
    block = 'solid'
    mob_name = 1
    use_displaced_mesh = true
  [../]
[]

[InterfaceKernels]
  [./penalty_interface_x]
    type = CoupledPenaltyInterfaceDiffusion
    variable = vel_x
    neighbor_var = disp_x
    slave_coupled_var = vel_x_solid
    boundary = 'dam_left dam_top dam_right'
    penalty = 1e6
    use_displaced_mesh = true
  [../]
  [./penalty_interface_y]
    type = CoupledPenaltyInterfaceDiffusion
    variable = vel_y
    neighbor_var = disp_y
    slave_coupled_var = vel_y_solid
    boundary = 'dam_left dam_top dam_right'
    penalty = 1e6
    use_displaced_mesh = true
  [../]
[]

[Modules/TensorMechanics/Master]
  [./solid_domain]
    strain = FINITE
    # incremental = false
    # generate_output = 'strain_xx strain_yy strain_zz' ## Not at all necessary, but nice
    block = 'solid'
    use_displaced_mesh = true
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1e2
    poissons_ratio = 0.3
    block = 'solid'
  [../]
  [./finite_stress]
    type = ComputeFiniteStrainElasticStress
    block = 'solid'
  [../]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1e2'
    block = 'solid'
  [../]
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
    boundary = 'no_slip'
    value = 0.0
    use_displaced_mesh = true
  [../]
  [./fluid_y_no_slip]
    type = DirichletBC
    variable = vel_y
    boundary = 'no_slip'
    value = 0.0
    use_displaced_mesh = true
  [../]
  [./x_inlet]
    type = FunctionDirichletBC
    variable = vel_x
    boundary = 'inlet'
    function = '1'
    use_displaced_mesh = true
  [../]
  [./outlet]
    type = FunctionDirichletBC
    variable = p
    boundary = 'outlet'
    function = '0'
    use_displaced_mesh = true
  [../]
  [./no_disp_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'fixed no_slip inlet outlet'
    value = 0
    use_displaced_mesh = true
  [../]
  [./no_disp_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'fixed no_slip inlet outlet'
    value = 0
    use_displaced_mesh = true
  [../]
  [./solid_x_no_slip]
    type = DirichletBC
    variable = vel_x_solid
    boundary = 'fixed'
    value = 0.0
    use_displaced_mesh = true
  [../]
  [./solid_y_no_slip]
    type = DirichletBC
    variable = vel_y_solid
    boundary = 'fixed'
    value = 0.0
    use_displaced_mesh = true
  [../]
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
  nl_abs_tol = 1e-8
  solve_type = 'PJFNK'
  petsc_options = '-snes_converged_reason -ksp_converged_reason'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  line_search = none
[]

[Outputs]
  [./out]
    type = Exodus
    file_base = output/fsi_test_out
  [../]
[]
