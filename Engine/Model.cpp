#include"Model.h"
#include"Direct3D.h"

namespace Model {
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string filename)
{
	//読み込み
	//既に読み込まれていたら -1
	//初めてなら読み込んで配列に追加
	ModelData* pData = new ModelData;
	pData->filename_ = filename;
	pData->pFbx_ = nullptr;

	for (auto& e : modelList)
	{
		//読み込み済みなら
		if (e->filename_ == filename) {
			pData->pFbx_ = e->pFbx_;
			break;
		}
	}
	//同名のファイルがなかったら
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
			//同じモデルなら
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
