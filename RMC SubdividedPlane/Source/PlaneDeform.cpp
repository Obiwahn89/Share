// Obiwahn89, 25.11.2020
// RunTimeMeshModifier to modify a subdivided plane


#include "PlaneDeform.h"

// Initialized by the DiggingActor, SizeX is the length in x
void UPlaneDeform::DefineSetup(float inSizeX, float inSizeY)
{
	FScopeLock Lock(&PropertySyncRoot);
	SizeX = inSizeX;
	SizeY = inSizeY;
	ScaleZ = 0.0;
}

void UPlaneDeform::SetScaleZ(float inScaleZ)
{
	FScopeLock Lock(&PropertySyncRoot);
	ScaleZ = inScaleZ;
}

// Deform the mesh by changing the z-coordinate
void UPlaneDeform::ApplyToMesh_Implementation(FRuntimeMeshRenderableMeshData& MeshData)
{
	FVector currentPos;
	float newheight;

	for (int i = 0 ; i < MeshData.Positions.Num(); i++)
	{
		currentPos = MeshData.Positions.GetPosition(i);
		newheight = ScaleZ * (FMath::Min(1.0, 0.00005 * (powf(currentPos.X - 0.5 * SizeX, 2.0) + powf(currentPos.Y - 0.5 * SizeY, 2.0))) - 1.0) ;
		MeshData.Positions.SetPosition(i, FVector(currentPos.X, currentPos.Y, newheight));
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("x=%f y=%f z=%f"), MeshData.Positions.GetPosition(i).X, MeshData.Positions.GetPosition(i).Y, MeshData.Positions.GetPosition(i).Z));
	}

	return;
}