// Obiwahn89, 25.11.2020
// RunTimeMeshActor to use the SubdividedPlaneProvider

#include "DiggingActor.h"
#include "SubdividedPlaneProvider.h"
#include "Providers/RuntimeMeshProviderCollision.h"
#include "Providers/RuntimeMeshProviderModifiers.h"
#include "PlaneDeform.h"


// Sets default values
ADiggingActor::ADiggingActor()
{
}

void ADiggingActor::DefineSetup(int32 inNx, int32 inNy, float inSizeX, float inSizeY)
{
	FScopeLock Lock(&PropertySyncRoot);
	Nx = inNy;
	Ny = inNy;
	SizeX = inSizeX;
	SizeY = inSizeY;

	CreateMesh();
}


void ADiggingActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	PlaneProvider = NewObject<USubdividedPlaneProvider>(this, TEXT("RuntimeMeshProvider-Plane"));
	ModifierProvider = NewObject<URuntimeMeshProviderModifiers>(this, TEXT("RuntimeMeshProviderModifiers"));
	PlaneMeshModifier = NewObject<UPlaneDeform>(this, TEXT("RuntimeMeshModifier-Plane"));
	CreateMesh();
}

void ADiggingActor::CreateMesh()
{
	if (PlaneProvider && ModifierProvider)
	{
		PlaneProvider->SetPlaneMaterial(PlaneMaterial);
		PlaneProvider->DefineSetup(Nx, Ny, SizeX, SizeY);
		ModifierProvider->SetChildProvider(PlaneProvider);
		ModifierProvider->AddModifier(PlaneMeshModifier);
		GetRuntimeMeshComponent()->Initialize(ModifierProvider);
	}
}

void ADiggingActor::DeformMesh()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Call to Deform Mesh from Digging Actor"));
	PlaneProvider->MarkAllLODsDirty();
}

