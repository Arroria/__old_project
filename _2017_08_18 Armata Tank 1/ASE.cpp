#include "MainHeader.h"
#include "ASE.h"

namespace AF
{
	ASEModel::GeometryObject::GeometryObject()
		: m_material(nullptr)
		, m_vertexData(nullptr)
	{
		D3DXMatrixIdentity(&m_worldMatrix);
		D3DXMatrixIdentity(&m_localMatrix);
	}
	ASEModel::GeometryObject::~GeometryObject()
	{
		SAFE_DELETE(m_vertexData);
	}



	void ASEModel::GeometryObject::Update(D3DXMATRIX* parentWorldMatrix, const D3DXMATRIX* transformMatrix)
	{
		D3DXMATRIX rotMat, posMat;
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&posMat);

		rotMat = m_localMatrix;
		rotMat._41 = 0;
		rotMat._42 = 0;
		rotMat._43 = 0;

		posMat._41 = m_localMatrix._41;
		posMat._42 = m_localMatrix._42;
		posMat._43 = m_localMatrix._43;

		m_worldMatrix = parentWorldMatrix ?
			m_localMatrix * (*parentWorldMatrix) :
			m_localMatrix * rotMat * posMat;

		if (transformMatrix)
			m_worldMatrix *= *transformMatrix;

		for (auto& iter : m_child)
			iter->Update(&m_worldMatrix);
	}

	void ASEModel::GeometryObject::Render()
	{
		if (m_material)
		{
			AF::g_processManager->GetDevice()->SetFVF(AF::ASEModel::VertexData::FVF);
			if (m_material->texture)
				AF::g_processManager->GetDevice()->SetTexture(0, m_material->texture);
			AF::g_processManager->GetDevice()->SetMaterial(&m_material->material);
			AF::g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &m_worldMatrix);
			AF::g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vertexData->size() / 3, &((*m_vertexData)[0]), sizeof(AF::ASEModel::VertexData));
		}
		for (auto& iter : m_child)
			iter->Render();


	}



	void ASEModel::GeometryObject::CalculateLocalMatrix(D3DXMATRIX * parentLocalMatrix)
	{
		if (parentLocalMatrix)
		{
			D3DXMATRIX parentInverseLocalMatrix;
			D3DXMatrixInverse(&parentInverseLocalMatrix, 0, parentLocalMatrix);
			m_localMatrix = m_worldMatrix * parentInverseLocalMatrix;
		}
		else
			m_localMatrix = m_worldMatrix;

		for (auto& iter : m_child)
			iter->CalculateLocalMatrix(&m_worldMatrix);
	}










	ASEModel::ASEModel()
		: m_topGeometryObject(nullptr)
	{
		D3DXMatrixIdentity(&m_transformMatrix);
		ZeroMemory(&m_animationFrameData, sizeof(m_animationFrameData));
	}
	ASEModel::~ASEModel()
	{
		SAFE_DELETE(m_topGeometryObject);
		for (auto& iter : m_geometryObjectMaterial)
			SAFE_DELETE(iter);
		m_geometryObjectMaterial.clear();
	}



	void ASEModel::Update()
	{
		m_topGeometryObject->Update(nullptr, &m_transformMatrix);
	}

	void ASEModel::Render()
	{
		m_topGeometryObject->Render();
	}










	ASELoader::ASELoader()
		: m_loadingFile(nullptr)
		, m_loadingModel(nullptr)
	{
	}
	ASELoader::~ASELoader()
	{
	}



	std::wstring & ASELoader::GetToken()
	{
		m_token.clear();
		if (m_loadingFile)
		{
			char text = NULL;
			bool isString = false;

			while (!feof(m_loadingFile))
			{
				text = fgetc(m_loadingFile);

				if (text == '\"')
				{
					if (isString)
						break;
					isString = true;
					continue;
				}

				if (!isString && text <= 32)
				{
					if (m_token.empty())
						continue;
					break;
				}

				m_token += text;
			}
		}
		return m_token;
	}

	ASEModel * ASELoader::LoadAseFile(const std::wstring & fileAddress)
	{
		//Address analysis
		m_loadingFileAddress.clear();
		m_loadingFileName.clear();
		for (auto& iter : fileAddress)
		{
			m_loadingFileName += iter;
			if (iter == '/')
			{
				m_loadingFileAddress += m_loadingFileName;
				m_loadingFileName.clear();
			}
		}

		m_loadingFile = _wfopen(fileAddress.c_str(), L"r");
		if (!m_loadingFile)
			return nullptr;


		if (m_loadingModel = new AF::ASEModel())
		{
			while (!GetToken().empty())
			{
					 if (m_token == L"*SCENE")			AnalysisASE_Scene();
				else if (m_token == L"*MATERIAL_LIST")	AnalysisASE_MaterialList();
				else if (m_token == L"*GEOMOBJECT")		AnalysisASE_GeometryObject();
			}

			m_loadingModel->CalculateLocalMatrix();

		}


		fclose(m_loadingFile);
		return m_loadingModel;
	}

	void ASELoader::AnalysisASE_Scene()
	{
		AF::ASEModel::AnimationFrameData animationFrameData;

		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}")	{ if (--areaDepth == 0) break; }
			
			else if (m_token == L"*SCENE_FIRSTFRAME")		animationFrameData.firstFrame		= GetToken_Int();
			else if (m_token == L"*SCENE_LASTFRAME")		animationFrameData.lastFrame		= GetToken_Int();
			else if (m_token == L"*SCENE_FRAMESPEED")		animationFrameData.frameSpeed		= GetToken_Int();
			else if (m_token == L"*SCENE_TICKSPERFRAME")	animationFrameData.ticksPerFrame	= GetToken_Int();
		}

		m_loadingModel->SetAnimationFrameData(animationFrameData);
	}

	void ASELoader::AnalysisASE_MaterialList()
	{
		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }

			else if (m_token == L"*MATERIAL_COUNT")	m_loadingModel->CreateGeometryObjectMaterialContainer(GetToken_Int());
			else if (m_token == L"*MATERIAL")		AnalysisASE_Material();
		}
	}

	void ASELoader::AnalysisASE_Material()
	{
		int					index = GetToken_Int();
		LPDIRECT3DTEXTURE9	texture = nullptr;
		D3DMATERIAL9		material;
		ZeroMemory(&material, sizeof(material));

		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}")	{ if (--areaDepth == 0) break; }
			
			else if (m_token == L"*MATERIAL_AMBIENT")
			{
				material.Ambient.r = GetToken_Float();
				material.Ambient.g = GetToken_Float();
				material.Ambient.b = GetToken_Float();
				material.Ambient.a = 1;
			}
			else if (m_token == L"*MATERIAL_DIFFUSE")
			{
				material.Diffuse.r = GetToken_Float();
				material.Diffuse.g = GetToken_Float();
				material.Diffuse.b = GetToken_Float();
				material.Diffuse.a = 1;
			}
			else if (m_token == L"*MATERIAL_SPECULAR")
			{
				material.Specular.r = GetToken_Float();
				material.Specular.g = GetToken_Float();
				material.Specular.b = GetToken_Float();
				material.Specular.a = 1;
			}
			else if (m_token == L"*MAP_DIFFUSE")		texture = AnalysisASE_MapDiffuse();
		}
		m_loadingModel->AddGeometryObjectMaterial(new ASEModel::GeometryObjectMaterial(texture, material), index);
	}

	LPDIRECT3DTEXTURE9 ASELoader::AnalysisASE_MapDiffuse()
	{
		LPDIRECT3DTEXTURE9 texture = nullptr;

		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }

			else if (m_token == L"*BITMAP")		D3DXCreateTextureFromFileExW(AF::g_processManager->GetDevice(), (m_loadingFileAddress + GetToken()).c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &texture);
		}
		return texture;
	}

	void ASELoader::AnalysisASE_GeometryObject()
	{
		ASEModel::GeometryObject* geometryObject = new ASEModel::GeometryObject();
		D3DXMATRIX geometryObjectTransfromMatrix;

		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }
			
			else if (m_token == L"*NODE_NAME")		m_createdGeometryObject.insert(std::make_pair( GetToken(), geometryObject ));
			else if (m_token == L"*NODE_PARENT")	m_createdGeometryObject[ GetToken() ]->AddChild( geometryObject );
			else if (m_token == L"*NODE_TM")		{ geometryObjectTransfromMatrix = AnalysisASE_NodeTransfromMatrix(); geometryObject->SetTransformMatrix( geometryObjectTransfromMatrix ); }
			else if (m_token == L"*MESH")			geometryObject->SetVertexData( AnalysisASE_Mesh( geometryObjectTransfromMatrix ) );
			else if (m_token == L"*TM_ANIMATION")	;
			else if (m_token == L"*MATERIAL_REF")	geometryObject->SetMaterial( m_loadingModel->GetGeometryObjectMaterial( GetToken_Int() ) );
		}

		m_loadingModel->AddGeometryObject(geometryObject);
	}

	D3DXMATRIX ASELoader::AnalysisASE_NodeTransfromMatrix()
	{
		D3DXMATRIX transfromMatrix;
		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }
			
			else if (m_token == L"*TM_ROW0")	{ transfromMatrix._11 = GetToken_Float(); transfromMatrix._13 = GetToken_Float(); transfromMatrix._12 = GetToken_Float(); transfromMatrix._14 = 0; }
			else if (m_token == L"*TM_ROW1")	{ transfromMatrix._31 = GetToken_Float(); transfromMatrix._33 = GetToken_Float(); transfromMatrix._32 = GetToken_Float(); transfromMatrix._34 = 0; }
			else if (m_token == L"*TM_ROW2")	{ transfromMatrix._21 = GetToken_Float(); transfromMatrix._23 = GetToken_Float(); transfromMatrix._22 = GetToken_Float(); transfromMatrix._24 = 0; }
			else if (m_token == L"*TM_ROW3")	{ transfromMatrix._41 = GetToken_Float(); transfromMatrix._43 = GetToken_Float(); transfromMatrix._42 = GetToken_Float(); transfromMatrix._44 = 1; }
		}

		return transfromMatrix;
	}

	std::vector<ASEModel::VertexData>* ASELoader::AnalysisASE_Mesh(const D3DXMATRIX& transformMatrix)
	{
		std::vector<D3DXVECTOR3>	posData;
		std::vector<D3DXVECTOR2>	uvData;
		std::vector<ASEModel::VertexData>*	vertexData = new std::vector<ASEModel::VertexData>;

		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }
			
			else if (m_token == L"*MESH_NUMVERTEX")		posData		.resize( GetToken_Int() );
			else if (m_token == L"*MESH_NUMTVERTEX")	uvData		.resize( GetToken_Int() );
			else if (m_token == L"*MESH_NUMFACES")		vertexData ->resize( GetToken_Int() * 3 );
			else if (m_token == L"*MESH_VERTEX_LIST")	AnalysisASE_VertexList			( &posData );
			else if (m_token == L"*MESH_TVERTLIST")		AnalysisASE_TextureVertexList	( &uvData );
			else if (m_token == L"*MESH_FACE_LIST")		AnalysisASE_FaceList			( vertexData, &posData );
			else if (m_token == L"*MESH_TFACELIST")		AnalysisASE_TextureFaceList		( vertexData, &uvData );
			else if (m_token == L"*MESH_NORMALS")		AnalysisASE_Normal				( vertexData );
		}

		D3DXMATRIX inversedTransfromMatrix;
		D3DXMatrixInverse(&inversedTransfromMatrix, NULL, &transformMatrix);
		for (auto& iter : *vertexData)
		{
			D3DXVec3TransformCoord(&iter.positions, &iter.positions, &inversedTransfromMatrix);
			D3DXVec3TransformNormal(&iter.normal, &iter.normal, &inversedTransfromMatrix);
		}

		return vertexData;
	}

	void ASELoader::AnalysisASE_VertexList(std::vector<D3DXVECTOR3>* position)
	{
		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }
			
			else if (m_token == L"*MESH_VERTEX")
			{
				D3DXVECTOR3& pos = (*position)[ GetToken_Int() ];
				pos.x = GetToken_Float();
				pos.z = GetToken_Float();
				pos.y = GetToken_Float();
				int a = 6;
			}
		}
	}

	void ASELoader::AnalysisASE_TextureVertexList(std::vector<D3DXVECTOR2>* uvPos)
	{
		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }

			else if (m_token == L"*MESH_TVERT")
			{
				D3DXVECTOR2& uvPss = (*uvPos)[ GetToken_Int() ];
				uvPss.x = GetToken_Float();
				uvPss.y = 1.f - GetToken_Float();
			}
		}
	}

	void ASELoader::AnalysisASE_FaceList(std::vector<AF::ASEModel::VertexData>* vertexData, std::vector<D3DXVECTOR3>* position)
	{
		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }

			else if (m_token == L"*MESH_FACE")
			{
				int index = GetToken_Int() * 3;
				GetToken();	(*vertexData)[ index     ].positions = (*position)[ GetToken_Int() ];
				GetToken();	(*vertexData)[ index + 2 ].positions = (*position)[ GetToken_Int() ];
				GetToken();	(*vertexData)[ index + 1 ].positions = (*position)[ GetToken_Int() ];
			}
		}
	}

	void ASELoader::AnalysisASE_TextureFaceList(std::vector<AF::ASEModel::VertexData>* vertexData, std::vector<D3DXVECTOR2>* uvPos)
	{
		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }
			
			else if (m_token == L"*MESH_TFACE")
			{
				int index = GetToken_Int() * 3;
				(*vertexData)[ index     ].uvPos = (*uvPos)[ GetToken_Int() ];
				(*vertexData)[ index + 2 ].uvPos = (*uvPos)[ GetToken_Int() ];
				(*vertexData)[ index + 1 ].uvPos = (*uvPos)[ GetToken_Int() ];
			}
		}
	}

	void ASELoader::AnalysisASE_Normal(std::vector<AF::ASEModel::VertexData>* vertexData)
	{
		int	index = NULL;
		int	readCount = 0;
		const int temp[3] = { 0, 2, 1 };

		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }

			else if (m_token == L"*MESH_FACENORMAL")
			{	
				index = GetToken_Int() * 3;
				readCount = 0;
			}
			else if (m_token == L"*MESH_VERTEXNORMAL")
			{
				D3DXVECTOR3& normal = (*vertexData)[ index + temp[readCount] ].normal;
				GetToken();

				normal.x = GetToken_Float();
				normal.z = GetToken_Float();
				normal.y = GetToken_Float();

				readCount++;
			}
		}
	}

	void ASELoader::AnalysisASE_TransformMatrixAnimation(AF::ASEModel::GeometryObject* geometryObject)
	{
		int areaDepth = 0;
		while (true)
		{
			GetToken();
				 if (m_token == L"{")	areaDepth++;
			else if (m_token == L"}") { if (--areaDepth == 0) break; }
		}
	}
}
