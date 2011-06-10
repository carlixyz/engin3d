#ifndef OBJECT_H
#define OBJECT_H
#include "../../Utility/ResourceHandle.h"
#include "../../MathLib/Matrix/Matrix.h"

class cObject
{
public:
  void Init();
  void Deinit(){};
  virtual void Update( float lfTimeStep);
  virtual void Render();
  inline std::string GetName() { return macName;}
  inline void SetName( const std::string &lacName) {macName = lacName; }
  inline void SetWorldMatrix( const cMatrix& lWorld) { mWorldMatrix = lWorld; }
  inline cMatrix GetWorldMatrix ( const cMatrix& lWorld) { return mWorldMatrix; }
  void AddMesh ( cResourceHandle lMeshHandle, cResourceHandle lMaterialHandle );
protected:
  std::string macName;
  cMatrix mWorldMatrix;

  std::vector <cResourceHandle> mMeshHandles;
  std::vector <cResourceHandle> mMaterialHandles;
};

#endif
