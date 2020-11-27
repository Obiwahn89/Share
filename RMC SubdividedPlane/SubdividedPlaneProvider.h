// Obiwahn89, 25.11.2020
// RunTimeMeshProvider to create a subdivided plane

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshProvider.h"
#include "SubdividedPlaneProvider.generated.h"


UCLASS(HideCategories = Object, BlueprintType)
class FISHERSISLE_API USubdividedPlaneProvider : public URuntimeMeshProvider
{
	GENERATED_BODY()

private:
	mutable FCriticalSection PropertySyncRoot;
	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetPlaneMaterial, BlueprintSetter = SetPlaneMaterial)
		UMaterialInterface* PlaneMaterial;

	int32 Nx, Ny;
	float SizeX, SizeY;

public:
	UFUNCTION(BlueprintCallable)
		UMaterialInterface* GetPlaneMaterial() const;
	UFUNCTION(BlueprintCallable)
		void SetPlaneMaterial(UMaterialInterface* InMaterial);
	UFUNCTION(BlueprintCallable)
		void DefineSetup(int32 inNx, int32 inNy, float inSizeX, float inSizeY);

protected:
	void Initialize() override;
	FBoxSphereBounds GetBounds() override;
	bool GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData) override;
	FRuntimeMeshCollisionSettings GetCollisionSettings() override;
	bool IsThreadSafe() override;
};
