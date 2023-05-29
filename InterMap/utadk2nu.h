#ifndef uta_DK2NU_H
#define uta_DK2NU_H

#include "TROOT.h"
#include "TObject.h"

#include <vector>
#include <string>

#define UTADK2NUVER 1   // KEEP THIS UP-TO-DATE!  increment for each change
#define KEEP_ANCESTOR_PPRODPXYZ 1

namespace uta {
   
  class NuRay{
  public:
    Double_t px;     ///< px for nu at location
    Double_t py;     ///< py for nu at location
    Double_t pz;     ///< pz for nu at location
    Double_t E;      ///< E for nu at location
    Double_t wgt;    ///< weight for nu at location
    
  public:
    NuRay();
    NuRay(double px, double py, double pz, double E, double wgt);
    virtual     ~NuRay();
    void        clear(const std::string &opt = "");    ///< reset everything
    
  private:
    ClassDef(uta::NuRay,UTADK2NUVER)
  };  // end-of-class uta::NuRay

  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   *  Decay Data:
   *  Core information about the neutrino and the decay that gave rise to it.
   *  % = necessary for reweighting
   */
  class Decay{
  public:
   Int_t    norig;        ///< not used?
   Int_t    ndecay;       ///< decay process (see dkproc_t)
   Int_t    ntype;        ///< % neutrino flavor (PDG? code)

   Double_t vx;           ///< % neutrino production vertex x
   Double_t vy;           ///< % neutrino production vertex y
   Double_t vz;           ///< % neutrino production vertex z
   Double_t pdpx;         ///< % px momentum of nu parent at (vx,vy,vz)
   Double_t pdpy;         ///< % py momentum of nu parent at (vx,vy,vz)
   Double_t pdpz;         ///< % pz momentum of nu parent at (vx,vy,vz)

   /**  these are used in muon decay case? */
   Double_t ppdxdz;       ///< % direction of nu parent at its production point
   Double_t ppdydz;       ///< % direction of nu parent at its production point
   Double_t pppz;         ///< % z momentum of nu parent at its production point
   Double_t ppenergy;     ///< % energy of nu parent at its production point

   Int_t    ppmedium;     ///< material nu parent was produced in
   Int_t    ptype;        ///< % nu parent species (PDG? code)

   /** momentum and energy of nu grandparent at
       muons:    grandparent decay point
       hadrons:  grandparent production point
       Huh?  this needs better documentation
       Marco DT says: One should look at parent type ptype. 
                     If ptype is a muon, then muparpx,y,z,e 
                     are momentum and energy of the neutrino 
                     grandparent (muon parent) at its decay point. 
                     Otherwise (for all other values of ptype), 
                     muparpx,y,z,e refer to neutrino grandparent
                     (a hadron in this case) production point.
                     If the Ancestor List is on , then
                     these variables are superfluous.
    */
   Double_t muparpx;      ///< %
   Double_t muparpy;      ///< %
   Double_t muparpz;      ///< %
   Double_t mupare;       ///< % energy of nu grandparent

   Double_t necm;         ///< % nu energy in center-of-mass frame
   Double_t nimpwt;       ///< % cumulative importance weight prod to decay

  public:
    Decay();
    virtual     ~Decay();
    void        clear(const std::string &opt = "");    ///< reset everything
    
  private:
    ClassDef(uta::Decay,UTADK2NUVER)
  };  // end-of-class uta::Decay

  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   *  Ancestor Data:
   *  Information about the chain of particles from the initial proton (indx=0)
   *  to the final neutrino.
   */
  class Ancestor{
  public:
    Int_t    pdg;      ///< ancestor species
    
    Double_t startx;   ///< particle x initial position
    Double_t starty;   ///< particle y initial position
    Double_t startz;   ///< particle z initial position
    Double_t startt;   ///< particle initial time

    Double_t startpx;  ///< particle x initial momentum
    Double_t startpy;  ///< particle y initial momentum
    Double_t startpz;  ///< particle z initial momentum
    
    Double_t stoppx;   ///< particle x final momentum
    Double_t stoppy;   ///< particle y final momentum
    Double_t stoppz;   ///< particle z final momentum
    
    Double_t polx;     ///< x component of polarization
    Double_t poly;     ///< y component of polarization
    Double_t polz;     ///< z component of polarization
    
    // what are these ... somehow different from stoppx[-1]?
    // Marco DT says: Yes, they can be different. Nu parent first entry can
    //                be different than [-1]. Exapmle: it could be [-2], then [-1] 
    //                contains a parent elastic interaction.
    //                I'm gettint rid of these variables and I'll add a
    //                parIndex instead, see following.
#ifdef KEEP_ANCESTOR_PPRODPXYZ
    Double_t pprodpx;  ///< parent x momentum when producing this particle
    Double_t pprodpy;  ///< parent y momentum when producing this particle
    Double_t pprodpz;  ///< parent z momentum when producing this particle
#endif
    
    Int_t    nucleus;  ///< nucleus (PDG) type causing the scatter

    // Marco DT is adding parIndex    
    Int_t    parIndex; ///< particle index, from nu (0), parent (1) ... to proton (n)
 
    std::string proc;  ///< name of the process that creates this particle
    std::string ivol;  ///< name of the volume where the particle starts
    std::string imat;  ///< name of the material where the particle starts

  public:
    Ancestor();
    virtual     ~Ancestor();
    void        clear(const std::string &opt = "");    ///< reset everything
    
    /// set triplets
    void        SetStartXYZT(Double_t x, Double_t y, Double_t z, Double_t t);
    void        SetStartP(Double_t px, Double_t py, Double_t pz);
    void        SetStopP(Double_t px, Double_t py, Double_t pz);
    void        SetPProdP(Double_t px, Double_t py, Double_t pz);

    /// helper functions
    Double_t    r() const;
    Double_t    startpt() const;
    Double_t    startp() const;
    Double_t    stoppt() const;
    Double_t    stopp() const;

    // helper functions added by Marco
    bool        IsInTarget();

#ifdef KEEP_ANCESTOR_PPRODPXYZ
    Double_t    pprodpt() const;
    Double_t    pprodp() const;
#endif

  private:
    ClassDef(uta::Ancestor,UTADK2NUVER)
  };  // end-of-class uta::Ancestor

  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   * these ancestors are possibly, but not necessarily, the direct nu parent
   * DK2NU: can these be removed in favor of cascade info (ancestor above)?
   * 2012-11-08: for now keep these
   */
  class TgtExit{
  public:
    Double_t tvx;          ///< x position of nu ancestor as it exits target
    Double_t tvy;          ///< y position of nu ancestor as it exits target
    Double_t tvz;          ///< z position of nu ancestor as it exits target
    Double_t tpx;          ///< x momentum of nu ancestor as it exits target
    Double_t tpy;          ///< y momentum of nu ancestor as it exits target
    Double_t tpz;          ///< z momentum of nu ancestor as it exits target
    Int_t    tptype;       ///< species of ancestor exiting the target
    Int_t    tgen;         ///< nu parent generation in cascade:
                           ///<   1=primary proton
                           ///<   2=particles produced by proton interaction
                           ///<   etc
  public:
    TgtExit();
    virtual     ~TgtExit();
    void        clear(const std::string &opt = "");    ///< reset everything
    
  private:
    ClassDef(uta::TgtExit,UTADK2NUVER)
  };  // end-of-class TgtExit
  
  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   * Track points stored at special locations for plotting trajectories
   */
  class Traj{
  public:
    Double_t trkx;
    Double_t trky;
    Double_t trkz;
    Double_t trkpx;
    Double_t trkpy;
    Double_t trkpz;

  public:
    Traj();
    virtual     ~Traj();
    void        clear(const std::string &opt = "");    ///< reset everything
    
  private:
    ClassDef(uta::Traj,UTADK2NUVER)
  };  // end-of-class uta::Traj
  
  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   * This is the structure that is the basis for the flux ntuple
   */
  class Dk2Nu{
  public:
   Int_t job;                        ///< identifying job #
   Int_t potnum;                     ///< proton # processed by simulation
   uta::Decay decay;                ///< basic decay information
   std::vector<uta::NuRay> nuray;   ///< rays through detector fixed points
   std::vector<uta::Ancestor> ancestor;  ///< chain from proton to neutrino

   /**
    * These are ancestor.vx[size-2]  kept, for now, for convenience
    */
   Double_t ppvx;                   ///< production vertex x of nu parent
   Double_t ppvy;                   ///< production vertex y of nu parent
   Double_t ppvz;                   ///< production vertex z of nu parent

   uta::TgtExit tgtexit;           ///< info about leaving the target
   std::vector<uta::Traj> traj;    ///< [optional] trajectory points

   /**
    *=======================================================================
    *  Special Info:
    */
   Int_t    flagbits;      ///< bits signify non-std setting such as
                           ///< Geant vs. PDG codes, mm vs. cm, Mev vs. GeV
                           ///< bit00 = ancestore overflow (carry over from g4minerva)
   std::vector<Int_t>    vint;    ///< user defined vector of integers
   std::vector<Double_t> vdbl;    ///< user defined vector of doubles

  public:
    /**
     *   Public methods for constructing/destruction and resetting the data
     */
    Dk2Nu();
    virtual     ~Dk2Nu();
    void        clear(const std::string &opt = "");    ///< reset everything

    size_t      indxnu() const;    ///< ancestor index of nu ancestor.size()-1
    size_t      indxp() const;     ///< ancestor index of parent ancestor.size()-2
    size_t      indxgp() const;    ///< ancestor index of grandparent ancestor.size()-3

  private:
    ClassDef(uta::Dk2Nu,UTADK2NUVER)
   
 };

} // end-of-namespace "uta"


#endif  // uta_DK2NU_H
