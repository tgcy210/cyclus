
#include <gtest/gtest.h>

#include "IsoVector.h"
#include "Material.h"
#include "ResourceBuff.h"
#include "CycException.h"
#include "Logger.h"

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
class ResourceBuffTest : public ::testing::Test {
protected:
  int oxygen, u235, u238, pu240;
  double m_oxygen, m_u235, m_u238, m_pu240;
  IsoVector vect1_, vect2_;
  
  rsrc_ptr mat1_, mat2_;
  double mass1, mass2;
  Manifest mats;
  
  ResourceBuff store_; // default constructed mat store
  ResourceBuff filled_store_;
  
  double neg_cap, zero_cap, cap, low_cap;
  double exact_qty; // mass in filled_store_
  double exact_qty_under; // mass in filled_store - 0.9*STORE_EPS
  double exact_qty_over; // mass in filled_store + 0.9*STORE_EPS
  double over_qty;  // mass in filled_store - 1.1*STORE_EPS
  double under_qty; // mass in filled_store + 1.1*STORE_EPS
  double overeps, undereps;

  virtual void SetUp() {
    try {
      // composition
      CompMapPtr comp;
      oxygen = 8001;
      u235 = 92235;
      u238 = 92238;
      pu240 = 94240;
      m_oxygen = 1;
      m_u235 = 10;
      m_u238 = 100;
      m_pu240 = 100;

      // vectors
      vect1_ = IsoVector();
      comp = vect1_.comp();
      (*comp)[oxygen] = m_oxygen;
      (*comp)[u235] = m_u235;
      (*comp)[u238] = m_u238;
      vect2_ = IsoVector();
      comp = vect2_.comp();
      (*comp)[oxygen] = m_oxygen;
      (*comp)[u235] = m_u235;
      (*comp)[pu240] = m_pu240;

      // materials
      mass1 = 111;
      mat1_ = rsrc_ptr(new Material(vect1_));
      mat1_->setQuantity(mass1);
      mass2 = 222;
      mat2_ = rsrc_ptr(new Material(vect2_));
      mat2_->setQuantity(mass2);
      mats.push_back(mat1_);
      mats.push_back(mat2_);

      neg_cap = -1;
      zero_cap = 0;
      cap = mat1_->quantity()+mat2_->quantity()+1; // should be higher than mat1+mat2 masses
      low_cap = mat1_->quantity()+mat2_->quantity()-1; // should be lower than mat1_mat2 masses

      undereps = 0.9 * STORE_EPS;
      overeps = 1.1 * STORE_EPS;
      exact_qty = mat1_->quantity();
      exact_qty_under = exact_qty - undereps;
      exact_qty_over = exact_qty + undereps;
      under_qty = exact_qty - overeps;
      over_qty = exact_qty + overeps;

      filled_store_.setCapacity(cap);
      filled_store_.pushOne(mat1_);
      filled_store_.pushOne(mat2_);
    } catch (std::exception err) {
      LOG(LEV_ERROR, "MSTest") << err.what();
      FAIL() << "An exception was thrown in the fixture SetUp.";
    }
  }
};
