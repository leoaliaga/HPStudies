#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace uta;

#pragma link C++ class uta::NuRay+;
#pragma link C++ class uta::Decay+;
#pragma link C++ class uta::Ancestor+;
#pragma link C++ class uta::TgtExit+;
#pragma link C++ class uta::Traj+;
#pragma link C++ class uta::Dk2Nu+;

#pragma link C++ class std::vector<uta::NuRay>+;
#pragma link C++ class std::vector<uta::Ancestor>+;
#pragma link C++ class std::vector<uta::Traj>+;

#pragma link C++ class uta::Location+;
#pragma link C++ class uta::DkMeta+;

#pragma link C++ class std::vector<uta::Location>+;

#endif
