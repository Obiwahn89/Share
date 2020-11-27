// Obiwahn89, 25.11.2020
// RunTimeMeshActor to initialize the SubdividedPlaneProvider

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "DiggingActor.generated.h"

UCLASS()
class FISHERSISLE_API ADiggingActor : public ARuntimeMeshActor
{
	GENERATED_BODY()

private:
	mutable FCriticalSection PropertySyncRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|RuntimeMesh", AllowPrivateAccess = "true"))
		class USubdividedPlaneProvider* PlaneProvider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|RuntimeMesh", AllowPrivateAccess = "true"))
		class URuntimeMeshProviderModifiers* ModifierProvider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|RuntimeMesh", AllowPrivateAccess = "true"))
		class UPlaneDeform* PlaneMeshModifier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|RuntimeMesh", AllowPrivateAccess = "true"))
		class URuntimeMeshModifierNormals* NormalModifier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|RuntimeMesh", AllowPrivateAccess = "true"))
		class URuntimeMeshProviderCollision* CollisionProvider;

	int32 Nx, Ny;
	float SizeX, SizeY;
public:
	UFUNCTION(BlueprintCallable)
		void DefineSetup(int32 inNx, int32 inNy, float inSizeX, float inSizeY);
	UFUNCTION(BlueprintCallable, Meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|RuntimeMesh"))
		void DeformMesh(float inScaleZ);
	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlaneMaterial;

public:

	void OnConstruction(const FTransform& Transform) override;

protected:
	void CreateMesh();
};
