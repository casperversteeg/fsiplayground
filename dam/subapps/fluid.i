## Header comments

# Global parameters that will be set for all kernels in the simulation
[GlobalParams]
  u = vel_x
  v = vel_y
  p = pressure
  integrate_p_by_parts = true
  supg = true
  laplace = false
[]

# Load or build mesh file for this problem.
[Mesh]
  type = FileMesh
  file = ../mesh/fluid.msh
  dim = 2 # Must be supplied for GMSH generated meshes
  # uniform_refine = 1 # Use for refining mesh
[]

# Set material parameters based on mesh regions
[Materials]
  # Define fluid domain material parameters by density and viscosity
  [./fluid]
    type = GenericConstantMaterial
    prop_names = 'rho mu'
    prop_values = '1e0 1e-1'
    block = 'fluid'
  [../]
[]

# Variables in the problem's governing equations which must be solved
[Variables]
  # Set x-direction velocity as variable to solve for
  [./vel_x]
    order = SECOND
    family = LAGRANGE
  [../]
  # Set y-direction velocity as variable to solve for
  [./vel_y]
    order = SECOND
    family = LAGRANGE
  [../]
  # Set pressure as variable to solve for
  [./pressure]
    order = FIRST
    family = LAGRANGE
  [../]
[]

# Auxiliary variables used for postprocessing and passing data between apps
[AuxVariables]
  # These are the boundary-only versions of the fluid stresses above
  [./sigma_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./sigma_y]
    order = CONSTANT
    family = MONOMIAL
  [../]

  # Compute velocity magnitude at every point
  [./norm_u]
    family = LAGRANGE
    order = SECOND
  [../]
[]

# All the terms in the weak form that need to be solved in this simulation
[Kernels]
  [./mass]
    type = INSMass
    variable = pressure
  [../]
  # Add unsteady term of fluid field to residual (rho * du/dt)
  [./unsteady_x]
    type = INSMomentumTimeDerivative
    variable = vel_x
  [../]
  [./unsteady_y]
    type = INSMomentumTimeDerivative
    variable = vel_y
  [../]
  # Add ALE momentum equation kernels, including convective, traction and body terms
  [./momentum_x]
    type = INSMomentumTractionForm
    variable = vel_x
    component = 0
  [../]
  [./momentum_y]
    type = INSMomentumTractionForm
    variable = vel_y
    component = 1
  [../]
[]

# Operations defined on auxiliary variables that will be computed at end
[AuxKernels]
  [./fluid_traction_x_INS]
    type = INSStressComponentAux
    variable = sigma_x
    boundary = 'dam_left dam_top dam_right'
    comp = 0
  [../]
  [./fluid_traction_y_INS]
    type = INSStressComponentAux
    variable = sigma_y
    boundary = 'dam_left dam_top dam_right'
    comp = 1
  [../]

  [./velocity_magnitude]
    type = VectorMagnitudeAux
    variable = norm_u
    x = vel_x
    y = vel_y
  [../]
[]

# Model boundary conditions that need to be enforced
[BCs]
  [./inlet]
    type = FunctionDirichletBC
    variable = pressure
    boundary = 'inlet'
    # value = 0.0 #1e0
    # function = '100 * (-exp(-1e3 *t) + 1)'
    function = '100'
  [../]
  [./outlet]
    type = DirichletBC
    variable = pressure
    boundary = 'outlet'
    value = 0.0
  [../]
  [./x_noslip]
    type = DirichletBC
    variable = vel_x
    boundary = 'no_slip dam_left dam_top dam_right'
    value = 0.0
  [../]
  [./y_noslip]
    type = DirichletBC
    variable = vel_y
    boundary = 'no_slip dam_left dam_top dam_right'
    value = 0.0
  [../]
[]

# Define postprocessor operations that can be used for viewing data/statistics
[Postprocessors]
  # Postprocess for optimal timestep based on Courant number
  [./Timestep_from_Courant]
    type = INSExplicitTimestepSelector
    beta = 1
    vel_mag = norm_u
  [../]
[]

# Set up matrix preconditioner to improve convergence
[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

# Type of algorithm and convergence parameters used to solve the matrix problem
[Executioner]
  [./TimeStepper]
    type = PostprocessorDT
    postprocessor = Timestep_from_Courant
    dt = 1e-5
  [../]
  # Set solver to transient, with Newton-Raphson nonlinear solver
  type = Transient
  solve_type = NEWTON

  # Nonlinear solver parameters for nonlinear iterations and linear subiterations
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-10
  nl_max_its = 30
  l_tol = 1e-6
  l_max_its = 300

  # PETSc solver options
  # petsc_options = '-snes_converged_reason -ksp_converged_reason'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package '
  petsc_options_value = 'lu       superlu_dist'
[]

# Output files for viewing after model finishes
[Outputs]
  # Do not print linear residuals to stdout
  print_linear_residuals = false
  # Write results to exodus .e file
  [./exodus]
    type = Exodus
    # Set output folder and filename
    file_base = output/fluid
  [../]
  [./console]
    type = Console
    execute_postprocessors_on = none
    # max_rows = 2
  [../]
[]
