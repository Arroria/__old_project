#pragma once

namespace AF
{
	class ASEModel final
	{
	//Affiliated Class
	public:
		struct VertexData
		{
			D3DXVECTOR3	positions;
			D3DXVECTOR3	normal;
			D3DXVECTOR2	uvPos;
			enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
		};
		struct AnimationFrameData
		{
			int	firstFrame;
			int	lastFrame;
			int	frameSpeed;
			int	ticksPerFrame;
			AnimationFrameData() : firstFrame(NULL), lastFrame(NULL), frameSpeed(NULL), ticksPerFrame(NULL) {}
		};
		struct GeometryObjectMaterial
		{
			const LPDIRECT3DTEXTURE9	texture;
			const D3DMATERIAL9			material;
			GeometryObjectMaterial(const LPDIRECT3DTEXTURE9& _texture, const D3DMATERIAL9& _material) : texture(_texture), material(_material) { if (texture) texture->AddRef(); }
			~GeometryObjectMaterial() { if (texture) texture->Release(); }
		};
		class GeometryObject final
		{
		private:
			std::vector<GeometryObject*>	m_child;

			D3DXMATRIX						m_worldMatrix;		//This Matrix temporarily holds the data in the ASE file (NODE_TM)
			D3DXMATRIX						m_localMatrix;

			GeometryObjectMaterial*			m_material;
			std::vector<VertexData>*		m_vertexData;

		public:
			void	AddChild(GeometryObject* child)												{ m_child.push_back(child); }

			void	SetTransformMatrix		(const D3DXMATRIX& transformMatrix)					{ m_worldMatrix = transformMatrix; }
			void	CalculateLocalMatrix	(D3DXMATRIX* parentLocalMatrix = nullptr);

			void	SetMaterial				(GeometryObjectMaterial* geometryObjectMaterial)	{ m_material = geometryObjectMaterial; }
			void	SetVertexData			(std::vector<VertexData>* vertexData)				{ m_vertexData = vertexData; }



		public:
			void	Update(D3DXMATRIX* parentWorldMatrix, const D3DXMATRIX* transformMatrix = nullptr);
			void	Render();

		public:
			GeometryObject();
			~GeometryObject();
		};

	//use cannot enter
	private:
		GeometryObject*							m_topGeometryObject;
		AnimationFrameData						m_animationFrameData;
		std::vector<GeometryObjectMaterial*>	m_geometryObjectMaterial;

	public:
		void	SetAnimationFrameData					(const AnimationFrameData& sceneData)								{ m_animationFrameData = sceneData; }
		void	CreateGeometryObjectMaterialContainer	(const int& size)													{ m_geometryObjectMaterial.resize(size); }
		void	AddGeometryObjectMaterial				(GeometryObjectMaterial* geometryObjectMaterial, const int& index)	{ m_geometryObjectMaterial[index] = geometryObjectMaterial; }
		void	AddGeometryObject						(GeometryObject* geometryObject)									{ if (!m_topGeometryObject) m_topGeometryObject = geometryObject; }
		void	CalculateLocalMatrix					()																	{ m_topGeometryObject->CalculateLocalMatrix(nullptr); }

		GeometryObjectMaterial*	GetGeometryObjectMaterial	(const int& index)	{ return m_geometryObjectMaterial[index]; }

	//for user
	private:
		D3DXMATRIX	m_transformMatrix;

	public:
		void	SetTransform(const D3DXMATRIX& transformMatrix) { m_transformMatrix = transformMatrix; }

		void	Update();
		void	Render();

	public:
		ASEModel();
		~ASEModel();
	};





	class ASELoader
	{
	private:
		std::wstring	m_token;
		FILE*			m_loadingFile;
		std::wstring	m_loadingFileAddress;
		std::wstring	m_loadingFileName;

		AF::ASEModel*	m_loadingModel;
		std::map<std::wstring, AF::ASEModel::GeometryObject*>	m_createdGeometryObject;

	private:
		std::wstring&	GetToken();
		int				GetToken_Int()		{ return _wtoi(GetToken().c_str()); }
		float			GetToken_Float()	{ return static_cast<float>( _wtof(GetToken().c_str()) ); }

	public:
		ASEModel*	LoadAseFile	(const std::wstring& fileAddress);

	private:
		void								AnalysisASE_Scene();
		void								AnalysisASE_MaterialList();
		void									AnalysisASE_Material();
		LPDIRECT3DTEXTURE9							AnalysisASE_MapDiffuse();
		void								AnalysisASE_GeometryObject();
		D3DXMATRIX								AnalysisASE_NodeTransfromMatrix	();
		std::vector<ASEModel::VertexData>*		AnalysisASE_Mesh				(const D3DXMATRIX& transformMatrix);
		void										AnalysisASE_VertexList			(std::vector<D3DXVECTOR3>* position);
		void										AnalysisASE_TextureVertexList	(std::vector<D3DXVECTOR2>* uvPos);
		void										AnalysisASE_FaceList			(std::vector<AF::ASEModel::VertexData>* vertexData, std::vector<D3DXVECTOR3>* position);
		void										AnalysisASE_TextureFaceList		(std::vector<AF::ASEModel::VertexData>* vertexData, std::vector<D3DXVECTOR2>* uvPos);
		void										AnalysisASE_Normal				(std::vector<AF::ASEModel::VertexData>* vertexData);
		void									AnalysisASE_TransformMatrixAnimation	(AF::ASEModel::GeometryObject* geometryObject);
																		 
	public:
		ASELoader();
		~ASELoader();
	};
}

