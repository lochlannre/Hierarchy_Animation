#ifndef APPLICATION_H
#define APPLICATION_H

#define WIN32_LEAN_AND_MEAN

#include <assert.h>

#include <stdio.h>
#include <windows.h>
#include <d3d11.h>

#include "CommonApp.h"
#include "CommonMesh.h"

class Aeroplane;
class AeroplaneMeshes;
class HeightMap;
class Skeleton;
class Animation;


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class Application : public CommonApp
{
  public:
	static Application* s_pApp;

  protected:
	bool HandleStart();
	void HandleStop();
	void HandleUpdate();
	void HandleRender();

  private:
	float m_frameCount = 0;
	bool m_reload;
	float m_rotationAngle;
	float m_rotationSpeed;
	float m_cameraZ;
	bool m_bWireframe;

	Shader m_drawShadowCasterShader;
	Shader m_drawHeightMapShader;
	int m_heightMapShaderShadowTexture;
	int m_heightMapShaderShadowSampler;

	struct DrawHeightMapShaderConstants
	{
		ID3D11Buffer *pCB;
		int slot;
		int shadowViewMatrix;
		int shadowColour;
	};

	DrawHeightMapShaderConstants m_drawHeightMapShaderVSConstants;
	DrawHeightMapShaderConstants m_drawHeightMapShaderPSConstants;

	XMMATRIX m_shadowViewMtx;

	int m_cameraState;

	Aeroplane* m_pAeroplane;
	AeroplaneMeshes *m_pAeroplaneDefaultMeshes;
	AeroplaneMeshes *m_pAeroplaneShadowMeshes;

	HeightMap* m_pHeightMap;

	CommonMesh *m_pShadowCastingLightMesh;
	XMFLOAT3 m_shadowCastingLightPosition;
	XMFLOAT4 m_shadowColour;

	ID3D11Texture2D *m_pRenderTargetColourTexture;
	ID3D11Texture2D *m_pRenderTargetDepthStencilTexture;
	ID3D11RenderTargetView *m_pRenderTargetColourTargetView;
	ID3D11ShaderResourceView *m_pRenderTargetColourTextureView;
	ID3D11DepthStencilView *m_pRenderTargetDepthStencilTargetView;

	ID3D11Buffer *m_pRenderTargetDebugDisplayBuffer;
	ID3D11SamplerState *m_pShadowSamplerState;

	bool CreateRenderTarget();
	void RenderShadow();

	void Render3D();
	void Render2D();

	Skeleton* p_Robot;
	Skeleton* p_Robot2;

	std::string HierarchyFilePath;

	std::vector<Animation*> Animations;
	std::string AnimationSetupPath;

	bool SlowMode;
	float slowCount;

	//void ReloadShaders(); //I don't think this is needed
};

#endif
