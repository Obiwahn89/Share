// Obiwahn89, 25.11.2020
// RunTimeMeshModifier to modify a subdivided plane

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshModifier.h"
#include "PlaneDeform.generated.h"

UCLASS()
class FISHERSISLE_API UPlaneDeform : public URuntimeMeshModifier
{
	GENERATED_BODY()

private:
	mutable FCriticalSection PropertySyncRoot;
	float SizeX, SizeY, ScaleZ;

public:
	void ApplyToMesh_Implementation(FRuntimeMeshRenderableMeshData& MeshData) override;

	UFUNCTION(BlueprintCallable)
		void DefineSetup(float inSizeX, float inSizeY);
	UFUNCTION(BlueprintCallable)
		void SetScaleZ(float inScaleZ);
};
