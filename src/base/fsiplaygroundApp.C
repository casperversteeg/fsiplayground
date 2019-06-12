#include "fsiplaygroundApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<fsiplaygroundApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

fsiplaygroundApp::fsiplaygroundApp(InputParameters parameters) : MooseApp(parameters)
{
  fsiplaygroundApp::registerAll(_factory, _action_factory, _syntax);
}

fsiplaygroundApp::~fsiplaygroundApp() {}

void
fsiplaygroundApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"fsiplaygroundApp"});
  Registry::registerActionsTo(af, {"fsiplaygroundApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
fsiplaygroundApp::registerApps()
{
  registerApp(fsiplaygroundApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
fsiplaygroundApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  fsiplaygroundApp::registerAll(f, af, s);
}
extern "C" void
fsiplaygroundApp__registerApps()
{
  fsiplaygroundApp::registerApps();
}
