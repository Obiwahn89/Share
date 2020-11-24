// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshProvider.h"
#include "DirtHillActor.generated.h"


UCLASS(HideCategories = Object, BlueprintType)
class FISHERSISLE_API UDirtHillActor : public URuntimeMeshProvider
{
	GENERATED_BODY()
private:
	mutable FCriticalSection PropertySyncRoot;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetPlaneMaterial, BlueprintSetter = SetPlaneMaterial)
		UMaterialInterface* PlaneMaterial;

	int32 Nx, Ny;
	float SizeX, SizeY, SizeZ;
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
	bool HasCollisionMesh() override;
	bool GetCollisionMesh(FRuntimeMeshCollisionData& CollisionData) override;
	bool IsThreadSafe() override;
};
