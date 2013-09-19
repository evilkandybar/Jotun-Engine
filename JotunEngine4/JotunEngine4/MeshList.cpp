#include "MeshList.h"

aiMesh* MeshList::loadMesh(std::string mName) {
	aiMesh *ret = meshes.at(mName);
	if (ret != NULL) {
		return ret;
	}
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(mName, aiProcessPreset_TargetRealtime_Fast);
	if (scene == NULL || !scene->HasMeshes()) {
		std::cout << "Error importing mesh. Do not rely on anything it does";
		return;
	}
	ret = scene->mMeshes[0];
	meshes.insert(std::pair<std::string, aiMesh*>(mName, ret));
	return ret;
}