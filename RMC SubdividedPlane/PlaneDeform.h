// Obiwahn89, 25.11.2020
// RunTimeMeshModifier to use the modify a subdivided plane

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshModifier.h"
#include "PlaneDeform.generated.h"

UCLASS()
class FISHERSISLE_API UPlaneDeform : public URuntimeMeshModifier
{
	GENERATED_BODY()

public:
	void ApplyToMesh(FRuntimeMeshRenderableMeshData& MeshData);
	
};
