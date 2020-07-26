#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec2		 	vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexTextureColor
	{
		_vec3	vPos;
		_ulong	dwColor;
		_vec2	vTexUV;
	}VTXCOLTEX;

	const _ulong FVF_COLTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTex;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagScreenVertex
	{
		_vec4				vPos;
		_vec2				vTexUV;

	}VTXSCREEN;

	const _ulong		FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef	struct tagVertexPointTexture
	{
		_vec3			vPos;
		_float			fPSize;
		_vec2			vTexUV;

	}VTXPTTEX;

	typedef	struct tagVertexMatrix
	{
		union 
		{
			struct  
			{
				_vec4		vRight;
				_vec4		vUp;
				_vec4		vLook;
				_vec4		vPos;
			};
			_matrix		Matrix;
		};
	}VTXMATRIX;

	typedef	struct tagIndex16Line
	{
		_ushort	_0, _1;

	}INDEX16_LINE;

	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32Line
	{
		_ulong	_0, _1;

	}INDEX32_LINE;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;

	typedef struct tagTextureInfo
	{
		IDirect3DBaseTexture9*	pTexture;
		D3DXIMAGE_INFO			tImgInfo;
	}TEX_INFO, *PTEX_INFO;

	typedef struct tagMeshTextureInfo
	{
		//LPDIRECT3DTEXTURE9 pTexture[MTT_END];
		class CTexture*			pTexture[MTT_END];
	}MESH_TEXTURES, *PMESH_TEXTURES;

	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix				CombinedTransformationMatrix;

	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		//LPDIRECT3DTEXTURE9*		ppTexture;
		MESH_TEXTURES*			pMeshTexture;
		LPD3DXMESH				pOriMesh;				// 애니메이션 스키닝을 위한 원본 메쉬 정보를 보관하기 위함(최초의 메쉬의 상태 값을 갖고 있는 메쉬 정보 객체(값은 불변))
		_ulong					dwNumBones;				// 뼈의 개수
		_matrix*				pFrameOffsetMatrix;		// 최초의 뼈대들이 갖고 있는 행렬(애니메이션을 포함한 행렬)
		_matrix**				ppFrameCombinedMatrix;	// 뼈들이 갖고 있는 CombinedTransformationMatrix의 주소값을 보관하기 위한 이중 포인터(매 프레임마다 갱신을 해야하기 때문에 매번 갱신 시점마다 행렬을 얻어오는 행위는 비 합리적이다. 그래서 한꺼번에 보관하려 하는 것이다.)
		_matrix*				pRenderingMatrix;		// 결국 최종 뼈의 상태 값을 보관하는 행렬(pFrameOffsetMatrix * ppFrameCombinedMatrix[i])
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagLine2D 
	{
		_float a, b, c;
	}LINE2D, *PLINE2D;

	typedef struct tagLine
	{
		tagLine()
			: x1(0.f), y1(0.f), z1(0.f)
			, x2(0.f), y2(0.f), z2(0.f)
		{

		}
		union 
		{
			struct  
			{
				_float x1, y1, z1;
			};
			_vec3 vPos1;
		};
		union 
		{
			struct 
			{
				_float x2, y2, z2;
			};
			_vec3 vPos2;
		};
	}LINE, *PLINE;

	typedef struct tagRectangle
	{
		tagRectangle()
			: vPos(0.f, 0.f, 0.f), fWidth(0.f), fHeight(0.f)
		{
		}
		_vec3 vPos;
		_float fWidth, fHeight;
	}RECTANGLE;

	typedef struct tagLine3D
	{
		tagLine3D()
			: dx(0.f), dy(0.f), dz(0.f)
			, px(0.f), py(0.f), pz(0.f)
		{

		}
		union
		{
			_vec3 vDir;
			struct
			{
				_float dx, dy, dz;
			};
			struct
			{
				_float a, b, c;
			};
		};

		union 
		{
			_vec3 vPos;
			struct
			{
				_float px, py, pz;
			};
		};
	}LINE3D, *PLINE3D;

	typedef struct tagSphere
	{
	public:
		tagSphere()
			: vPos(0.f, 0.f, 0.f)
			, fRadius(0.f)
		{

		}

		union 
		{
			_vec3	vPos;
			struct { _float	x, y, z; };
		};
		_float	fRadius;
	}SPHERE, *PSPHERE;

	typedef struct tagCube
	{
		union 
		{
			_vec3	vPos;
			_float	x, y, z;
		};
		union 
		{
			_vec3	vSize;
			_float	Width, Height, Depth;
		};
		union 
		{
			_vec3	vAngle;
			_float	alpha,	Beta,	Gamma;
		};
	}MY_CUBE, *PMY_CUBE;
}

#endif // Engine_Struct_h__
