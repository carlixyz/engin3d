#include "Game.h"

#include "InputConfiguration.h"
#include "../Input/InputManager.h"
#include "../Lua/LuaManager.h"
#include "../Character/CharacterManager.h"
#include "../Graphics/Textures/TextureManager.h"
#include "../Graphics/Materials/MaterialManager.h"
#include "../Graphics/Skeletal/SkeletalManager.h"
#include "../Graphics/Effects/EffectManager.h"
#include "../Graphics/Meshes/MeshManager.h"
#include "../Gameplay/Scene/SceneManager.h"

//#include "../Utility/Debug.h"

#ifdef _WIN32
  #include <windows.h>
  #include "../Window/Window.h" 
#endif

//Method to initialize the game
bool cGame::Init()
{
  extern tActionMapping kaActionMapping[];
 
  //Init time and exit attribute
  mbFinish = false;
  mfTimeElapsed = 0.0f; 

    // window Init & Creation
  cApplicationProperties lProperties;
  if ( !lProperties.LoadXML("./Src/Data/XML/Config.xml") ) OutputDebugString("\n Config XML Load: FAILED\n");

  bool lbResult  =  cWindow::Get().Init( lProperties );
  if (lbResult) //Init OpenGL
  {
    lbResult = cGraphicManager::Get().Init( &cWindow::Get() );
    if (!lbResult) cWindow::Get().Deinit();     //If something failed kill the window
  }

	// 3D Camera	
  m3DCamera.Init();
  float lfAspect = (float)lProperties.muiWidth/(float)lProperties.muiHeight;
  m3DCamera.SetPerspective(45.0f, lfAspect, 0.1f, 100.0f);
//  m3DCamera.SetLookAt( cVec3(0.001f, 10.0f, 0.0f), cVec3(0.0f, 0.0f, 0.0f) ); // Camera Used for IA
  m3DCamera.SetLookAt( cVec3(3.0f, 3.0f, 3.0f), cVec3(0.0f , 0.0f, 0.0f) );		// Camera Used for Jesus 

   // 2D Camera 
  float lfRight = (float) lProperties.muiWidth / 2.0f;
  float lfLeft = -lfRight;
  float lfTop = (float) lProperties.muiHeight  / 2.0f;
  float lfBottom = -lfTop;
  m2DCamera.Init();
  m2DCamera.SetOrtho(lfLeft,lfRight,lfBottom,lfTop, 0.1f, 100.0f);
  m2DCamera.SetLookAt( cVec3(0.0f,0.0f,1.0f), cVec3(0.0f, 0.f, 0.f) );

  cInputManager::Get().Init(kaActionMapping, eIA_Count);

  // Init Texture manager
  cMaterialManager::Get().Init( 20 );
  cTextureManager::Get().Init( 20 ); 
  cFontManager::Get().Init( 1);
  cEffectManager::Get().Init( 5 );
  cMeshManager::Get().Init(20);
  cSceneManager::Get().Init(5); // some conflict with SkeletalManager
  cSkeletalManager::Get().Init(5);

  // Character & Behaviour Manager initialization
  cCharacterManager::Get().Init();
  cBehaviourManager::Get().Init() ;

  cLuaManager::Get().Init();
  cLuaManager::Get().DoFile( "./Src/Data/Scripts/CreatePatrol.lua"  );

  // Multiple Resources Loading
  mFontHandle = cFontManager::Get().LoadResource("Font1");  //same as LoadResource("Font1","./Src/Data/Fonts/Test1.fnt" );

  mScene = cSceneManager::Get().LoadResource("Dragon"); // some conflict with SkeletalManager
  //mScene = cSceneManager::Get().LoadResource("Dragon", "./Src/Data/Scene/dragonsmall.DAE");


	//  just more testing code, forget it later
// ------------------------------------ For testing porpouses ------------------------------------
  cSkeletalManager::Get().LoadResource("Skeleton", "./Src/Data/Skeletal/SkeletonModel.xml");
  mSkeletalMesh = cMeshManager::Get().LoadResource("Skeleton1", "Skeleton", kuiSkeletalMesh);
  cSkeletalMesh *lpSkeletonMesh = (cSkeletalMesh *) mSkeletalMesh.GetResource();
  lpSkeletonMesh->PlayAnim("Idle", 1.0f, 1.0f);

  cResourceHandle lMaterial = cMaterialManager::Get().LoadResource( "Skeleton", "./Src/Data/Material/SkeletonMaterial.xml");
  assert(lMaterial.IsValidHandle() );
  mObject.AddMesh(mSkeletalMesh, lMaterial);
  cMatrix lMatrix;
  lMatrix.LoadScale(0.01f);
  mObject.SetWorldMatrix(lMatrix);
// ------------------------------------ For testing porpouses ------------------------------------
 
  return lbResult;
}

void cGame::Update(float lfTimeStep)
{
  cWindow::Get().Update();

    //Update time
  mfTimeElapsed += lfTimeStep;

  cInputManager::Get().Update(lfTimeStep);
  cCharacterManager::Get().Update(lfTimeStep);

  // ------------------------------------ For testing porpouses ------------------------------------
  cSkeletalMesh *lpSkeletonMesh = (cSkeletalMesh *) mSkeletalMesh.GetResource();
  lpSkeletonMesh->Update(lfTimeStep);
  mObject.Update(lfTimeStep);

  static bool mbJogging = false;
  if (BecomePressed( eIA_PlayJog ) && !mbJogging)
  {
	  mbJogging = true;
	  lpSkeletonMesh->PlayAnim("Jog", 1.0f, 0.1f);
	  lpSkeletonMesh->StopAnim("Idle", 0.1f);
  }
  else if( IsReleased( eIA_PlayJog ) && mbJogging)
  {
	  mbJogging = false;
	  lpSkeletonMesh->PlayAnim("Idle", 1.0f, 0.1f);
	  lpSkeletonMesh->StopAnim("Jog", 0.1f);
  }

    if (BecomePressed( eIA_PlaySalute ))
  {
	  lpSkeletonMesh->PlayAnim("Wave", 1.0f, 0.1f);
  }
  else if( IsReleased( eIA_PlaySalute ) )
  {
	  lpSkeletonMesh->StopAnim("Wave", 0.1f);
  }
// ------------------------------------ For testing porpouses ------------------------------------

 
	mbFinish = mbFinish || cWindow::Get().GetCloseApplication() || IsPressed(eIA_CloseApplication);
	if (mbFinish)
	{ 
		return;
	}
}

//Method to render the game 
void cGame::Render()
{
 	// 1) Clear buffers
	// ---------------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	// 2) Activate the 3D Camera
  	// ---------------------------------------------------------------------------------------
		cGraphicManager::Get().ActivateCamera( &m3DCamera );

  // 3) Render Solid 3D
  	// ---------------------------------------------------------------------------------------
	// Set the world matrix
		cMatrix lWorld;
		lWorld.LoadIdentity();
		cGraphicManager::Get().SetWorldMatrix(lWorld);
  
	//Render Debug Lines
		cGraphicManager::Get().DrawGrid();
		cGraphicManager::Get().DrawAxis(); 
	
	 //Render scene
		((cScene *)mScene.GetResource())->Render(); // some conflict with SkeletalManager

	// Character Rendering  
 		cCharacterManager::Get().Render();

// ------------------------------------ For testing porpouses ------------------------------------
	mObject.Render();
	cSkeletalMesh *lpSkeletonMesh = (cSkeletalMesh *) mSkeletalMesh.GetResource();
	lpSkeletonMesh->RenderSkeleton();
// ------------------------------------ For testing porpouses ------------------------------------


	// 4) Render 3D with transparency
	// ---------------------------------------------------------------------------------------

	// 5) Activate 2D Camera
	// ---------------------------------------------------------------------------------------
		cGraphicManager::Get().ActivateCamera( &m2DCamera ); // comment this for IA
		lWorld.LoadIdentity();
		cGraphicManager::Get().SetWorldMatrix(lWorld);

	// 6) Render 2D Elements
	// ---------------------------------------------------------------------------------------
		// Draw some strings
  
	//	cFont * lpFont = (cFont*)mFontHandle.GetResource();
		cFont * lpFont = (cFont*)cFontManager::Get().SearchResource("Font1").GetResource();
		lpFont->SetColour( 1.0f, 0.0f, 0.0f);
		lpFont->Write(0, 200,0, "Año Totó pingüino() !¡¿?", 0, FONT_ALIGN_CENTER);

		lpFont->SetColour( 0.0f, 1.0f, 0.0f );
		lpFont->WriteBox(150, 200, 0 ,100 , " Esto es un test \n MultiLinea", 0, FONT_ALIGN_CENTER );	
		

	// 7) Postprocessing
	// ---------------------------------------------------------------------------------------

  
	// 8) Swap Buffers
	// ---------------------------------------------------------------------------------------
		cGraphicManager::Get().SwapBuffer();
}

//Method to deinitialize the game
bool cGame::Deinit()
{
  //Deinit all Managers
  cInputManager::Get().Deinit();

  cCharacterManager::Get().Deinit();

  cBehaviourManager::Get().Deinit() ;

  cLuaManager::Get().Deinit();

  cFontManager::Get().Deinit();

  cEffectManager::Get().Deinit();

  cMaterialManager::Get().Deinit();

  cTextureManager::Get().Deinit();

  cSkeletalManager::Get().Deinit();

  cSceneManager::Get().Deinit(); // some conflict with SkeletalManager

  //Deinit graphic manager
  bool lbResult = cGraphicManager::Get().Deinit();

  //Deinit window
	lbResult =  lbResult && cWindow::Get().Deinit();
  return lbResult;
}
 
void cGame::SetFinished(bool lbFinish)
{
  mbFinish = lbFinish;
}