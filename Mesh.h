#pragma once
#include"Error.h"
#include"input.h"
#include <wrl.h>
#include"Vector3.h"

#include <d3d12.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <DirectXmath.h>
#include <d3dcompiler.h>

#include<wrl.h>

#include "DirectXCommon.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler")

#include <string.h>

using namespace std;

using namespace DirectX;







//struct Object3d {
//	ComPtr<ID3D12Resource> constBuffB0;
//	ComPtr<ID3D12Resource> constBuffB1;
//
//	ConstBufferDataTransform* constMapTransform = nullptr;
//
//	XMFLOAT3 scale = { 1,1,1 };
//	XMFLOAT3 rotation = { 0,0,0 };
//	XMFLOAT3 position = { 0,0,0 };
//
//	XMMATRIX matWorld{};
//
//	Object3d* parent = nullptr;
//
//
//};






class Mesh
{
public:
	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//�A���r�G���g�W��
		float pad1;			//�p�f�B���O
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�W��
		float pad2;			//�p�f�B���O
		XMFLOAT3 specular;	//�X�y�L�����[�W��
		float alpha;		//�A���t�@
	};
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	//�}�e���A��
	struct Material {
		std::string name;	//�}�e���A����
		XMFLOAT3 ambient;	//�A���r�G���g�e���x
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;	//�X�y�L�����[�e���x
		float alpha;		//�A���t�@
		std::string textureFilename;	//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

private:
	
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	

	//�f�o�C�X
	static ID3D12Device* device;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short>indices;
	//�}�e���A��
	Material material;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	// �萔�o�b�t�@(�}�e���A��)
	ComPtr<ID3D12Resource> constBuffB1;



	
public:
	//�A�N�Z�b�T
	static void SetDevice(ID3D12Device* device);

public:

	Mesh();
	~Mesh();

	void StaticInit(ID3D12Device* DXDevice);
	//void Init();
	void Update( Input* input);
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);


	void LoadModel(const char* fileName);	//LoadFromObj�Ɉڍs
	void LoadModel();

	//�o�b�t�@�̐���
	void CreateBuffer();
	//�ŃX�N���v�^�q�[�v�̏�����
	void InitializeDescriptorHeap();
	//�}�e���A���ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	//�e�N�X�`���ǂݍ���
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	//���f���ǂݍ��݊֘A
	void LoadFromObjInternal(const std::string& modelname);
	static Mesh* LoadFromOBJ(const std::string& modelname);
};
