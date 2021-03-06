#include "BuildingManager.h"

#include <iostream>
#include <vector>
#include <boost/any.hpp>

#include "SupplyDemandManager.h"
#include "SolverTools.h"
#include "Solver.h"
#include "SolverInterface.h"
#include "CBCSolver.h"

using namespace std;
using boost::any_cast;
using namespace Cyclopts;

// -------------------------------------------------------------------
BuildingManager::BuildingManager(SupplyDemandManager* m) {
  manager_ = m;
}

// -------------------------------------------------------------------
vector<BuildOrder> 
BuildingManager::makeBuildDecision(const Commodity& p, 
                                   double unmet_demand) {
  orders_ = vector<BuildOrder>();
  if (unmet_demand > 0) {
    doMakeBuildDecision(p,unmet_demand);
  }
  return orders_;
}

// -------------------------------------------------------------------
void BuildingManager::doMakeBuildDecision(const Commodity& commodity, 
                                          double unmet_demand) {

  // set up solver and interface
  SolverPtr solver(new CBCSolver());
  SolverInterface csi(solver);

  // set up objective function
  ObjFuncPtr obj(new ObjectiveFunction(ObjectiveFunction::MIN));
  csi.registerObjFunction(obj);

  // set up constraint
  ConstraintPtr c(new Constraint(Constraint::GTEQ,unmet_demand));
  csi.registerConstraint(c);

  // set up variables, constraints, and objective function
  int n = manager_->nProducers(commodity);
  vector<VariablePtr> soln;
  for (int i = 0; i < n; i++) {
    Producer* p = manager_->producer(commodity,i);
    // set up var`iables
    VariablePtr x(new IntegerVariable(0,Variable::INF));
    x->setName(p->name());
    soln.push_back(x);
    csi.registerVariable(x);
    double constraint_modifier = p->capacity();
    csi.addVarToConstraint(x,constraint_modifier,c);
    double obj_modifier = p->cost();
    csi.addVarToObjFunction(x,obj_modifier);
  }
  
  // solve and get solution
  csi.solve();

  // construct the build orders
  for (int i = 0; i < n; i++) {
    int number = any_cast<int>(soln.at(i)->value());
    if (number > 0) {
      orders_.push_back(BuildOrder(number,
                                   manager_->producer(commodity,i)));
    }
  }
}
