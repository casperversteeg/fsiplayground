//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "fsiplaygroundTestApp.h"
#include "fsiplaygroundApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

template <>
InputParameters
validParams<fsiplaygroundTestApp>()
{
  InputParameters params = validParams<fsiplaygroundApp>();
  return params;
}

fsiplaygroundTestApp::fsiplaygroundTestApp(InputParameters parameters) : MooseApp(parameters)
{
  fsiplaygroundTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

fsiplaygroundTestApp::~fsiplaygroundTestApp() {}

void
fsiplaygroundTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  fsiplaygroundApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"fsiplaygroundTestApp"});
    Registry::registerActionsTo(af, {"fsiplaygroundTestApp"});
  }
}

void
fsiplaygroundTestApp::registerApps()
{
  registerApp(fsiplaygroundApp);
  registerApp(fsiplaygroundTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
fsiplaygroundTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  fsiplaygroundTestApp::registerAll(f, af, s);
}
extern "C" void
fsiplaygroundTestApp__registerApps()
{
  fsiplaygroundTestApp::registerApps();
}
