// Obiwahn89, 25.11.2020
// RunTimeMeshActor to initialize the SubdividedPlaneProvider

#include "DiggingActor.h"
#include "SubdividedPlaneProvider.h"
#include "Providers/RuntimeMeshProviderCollision.h"
#include "Providers/RuntimeMeshProviderModifiers.h"
#include "PlaneDeform.h"
#include "Modifiers/RuntimeMeshModifierNormals.h"

// Initialize the plane from BP, with Nx being the number of 'cells' in x and SizeX the length in x
void ADiggingActor::DefineSetup(int32 inNx, int32 inNy, float inSizeX, float inSizeY)
{
	FScopeLock Lock(&PropertySyncRoot);
	Nx = inNy;
	Ny = inNy;
	SizeX = inSizeX;
	SizeY = inSizeY;

	CreateMesh();
}

// Create the modifiers, call 'CreateMesh'
void ADiggingActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	PlaneProvider = NewObject<USubdividedPlaneProvider>(this, TEXT("RuntimeMeshProvider-Plane"));
	CollisionProvider = NewObject<URuntimeMeshProviderCollision>(this, TEXT("RuntimeMeshProvider-Collision"));
	ModifierProvider = NewObject<URuntimeMeshProviderModifiers>(this, TEXT("RuntimeMeshProviderModifiers"));
	PlaneMeshModifier = NewObject<UPlaneDeform>(this, TEXT("RuntimeMeshModifier-Plane"));
	NormalModifier = NewObject<URuntimeMeshModifierNormals>(this, TEXT("RuntimeNormalModifier"));
	CreateMesh();
}

// Initialize the PlaneProvider and the Modifier with the same parameters, initialize the mesh
void ADiggingActor::CreateMesh()
{
	if (PlaneProvider && CollisionProvider && ModifierProvider && PlaneMeshModifier && NormalModifier)
	{
		// define the plane provider and set is as child of the ModifierProvider
		PlaneProvider->SetPlaneMaterial(PlaneMaterial);
		PlaneProvider->DefineSetup(Nx, Ny, SizeX, SizeY);
		PlaneMeshModifier->DefineSetup(SizeX, SizeY);
		ModifierProvider->SetChildProvider(PlaneProvider);
		
		// set up the ModifierProvider and set is as child of the CollisionProvider
		ModifierProvider->AddModifier(PlaneMeshModifier);
		NormalModifier->bComputeSmoothNormals = true; // smooth shading
		ModifierProvider->AddModifier(NormalModifier);
		CollisionProvider->SetChildProvider(ModifierProvider);
		
		// collision settings
		CollisionProvider->SetRenderableLODForCollision(0);
		CollisionProvider->SetRenderableSectionAffectsCollision(0, true);
		FRuntimeMeshCollisionSettings CollisionSettings;
		CollisionSettings.bUseComplexAsSimple = true;
		CollisionSettings.bUseAsyncCooking = true;
		CollisionProvider->SetCollisionSettings(CollisionSettings);

		GetRuntimeMeshComponent()->Initialize(CollisionProvider);
	}
}

// Deform the mesh using the 'PlaneDeform', update the mesh
void ADiggingActor::DeformMesh(float inScaleZ)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Call to Deform Mesh from Digging Actor"));
	PlaneMeshModifier->SetScaleZ(inScaleZ);
	ModifierProvider->MarkAllLODsDirty();
	ModifierProvider->MarkCollisionDirty();
}

