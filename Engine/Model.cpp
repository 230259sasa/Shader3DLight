#include"Model.h"
#include"Direct3D.h"

namespace Model {
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string filename)
{
	//�ǂݍ���
	//���ɓǂݍ��܂�Ă����� -1
	//���߂ĂȂ�ǂݍ���Ŕz��ɒǉ�
	ModelData* pData = new ModelData;
	pData->filename_ = filename;
	pData->pFbx_ = nullptr;

	for (auto& e : modelList)
	{
		//�ǂݍ��ݍς݂Ȃ�
		if (e->filename_ == filename) {
			pData->pFbx_ = e->pFbx_;
			break;
		}
	}
	//�����̃t�@�C�����Ȃ�������
	if (pData->pFbx_ == nullptr) {
		pData->pFbx_ = new FBX;
		pData->pFbx_->Load(filename);
	}
	modelList.push_back(pData);
	
	return(modelList.size()-1);
}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList[hModel]->transform_ = transform;
}

void Model::Draw(int hModel)
{
	modelList[hModel]->pFbx_->Draw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false;
	for (int j = 0; j < modelList.size(); j++) {
		for (int i = j+1; i < modelList.size(); i++) {
			//�������f���Ȃ�
			if (modelList[j]->pFbx_ == modelList[i]->pFbx_) {
				isReffered = true;
				break;
			}
		}
		if (!isReffered) {
			SAFE_DELETE(modelList[j]->pFbx_);
		}
		SAFE_DELETE(modelList[j]);
	}
	modelList.clear();
}
