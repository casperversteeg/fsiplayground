## Header comments

# These are the apps that need to be linked together
[MultiApps]
  [./Solid]
    type = TransientMultiApp
    app_type = fsiplaygroundApp
    input_files = subapps/structure.i
    positions = '0.0 0.0 0.0'
    execute_on = 'timestep_begin'
    catch_up = true
  [../]
  [./Fluid]
    type = TransientMultiApp
    app_type = fsiplaygroundApp
    input_files = subapps/fluid.i
    positions = '0.0 0.0 0.0'
    execute_on = 'timestep_end'
    sub_cycling = true
    output_sub_cycles = true
  [../]
[]

# Which variables need to be transferred to and from where, and when/how
[Transfers]
  # Solve the fluid tractions along boundary, send from fluid to solid
  [./take_traction_from_fluid_x]
    type = MultiAppCopyTransfer
    direction = from_multiapp
    multi_app = Fluid
    source_variable = sigma_x
    # variable = fluid_bc_store_x
    variable = sigma_x
    execute_on = 'timestep_end'
  [../]
  [./take_traction_from_fluid_y]
    type = MultiAppCopyTransfer
    direction = from_multiapp
    multi_app = Fluid
    source_variable = sigma_y
    # variable = fluid_bc_store_y
    variable = sigma_y
    execute_on = 'timestep_end'
  [../]
  [./send_traction_to_solid_x]
    type = MultiAppInterpolationTransfer
    direction = to_multiapp
    multi_app = Solid
    # source_variable = fluid_bc_store_x
    source_variable = sigma_x
    variable = sigma_x
    execute_on = 'timestep_end'
  [../]
  [./send_traction_to_solid_y]
    type = MultiAppInterpolationTransfer
    direction = to_multiapp
    multi_app = Solid
    # source_variable = fluid_bc_store_y
    source_variable = sigma_y
    variable = sigma_y
    execute_on = 'timestep_end'
  [../]
[]

# Auxiliary variables used for postprocessing and passing data between apps
[AuxVariables]
  # [./fluid_bc_store_x]
  #   family = MONOMIAL
  #   order = CONSTANT
  # [../]
  # [./fluid_bc_store_y]
  #   family = MONOMIAL
  #   order = CONSTANT
  # [../]
  [./sigma_x]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./sigma_y]
    family = MONOMIAL
    order = CONSTANT
  [../]
[]

## Everything below here will be done in between transfers (mesh regularizer)
# Global parameters that will be set for all kernels in the simulation
[GlobalParams]
  # use_displaced_mesh = true
  displacements = 'mesh_disp_x mesh_disp_y'
[]

# Load or build mesh file for this problem.
[Mesh]
  type = FileMesh
  file = mesh/fluid.msh
[]

# Set material parameters based on mesh regions
[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0
  [../]
  [./_elastic_stress1]
    type = ComputeLinearElasticStress
  [../]
[]

# Variables in the problem's governing equations which must be solved
[Variables]
[]

# All the terms in the weak form that need to be solved in this simulation
[Modules/TensorMechanics/Master]
  [./solid]
    strain = SMALL
    add_variables = true
    block = 'fluid'
  [../]
[]

# Operations defined on auxiliary variables that will be computed at end
[AuxKernels]
[]

# Model boundary conditions that need to be enforced
[BCs]
[]

# Define postprocessor operations that can be used for viewing data/statistics
[Postprocessors]
[]

# Set up matrix preconditioner to improve convergence
[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

# Type of algorithm and convergence parameters used to solve the matrix problem
[Executioner]
  # Set solver to transient, with Newton-Raphson nonlinear solver
  type = Transient
  solve_type = NEWTON

  # Nonlinear solver parameters for nonlinear iterations and linear subiterations
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-7
  nl_max_its = 15
  l_tol = 1e-6
  l_max_its = 300
  end_time = 1e-2

  # PETSc solver options
  # petsc_options = '-snes_converged_reason -ksp_converged_reason'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package '
  petsc_options_value = 'lu       superlu_dist'
[]

# Output files for viewing after model finishes
[Outputs]
  interval = 1
  print_linear_residuals = false
  perf_graph = true
[]
