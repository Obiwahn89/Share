// Fill out your copyright notice in the Description page of Project Settings.


#include "SubdividedPlaneProvider.h"

UMaterialInterface* USubdividedPlaneProvider::GetPlaneMaterial() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return PlaneMaterial;
}

void USubdividedPlaneProvider::SetPlaneMaterial(UMaterialInterface* InMaterial)
{
	FScopeLock Lock(&PropertySyncRoot);
	PlaneMaterial = InMaterial;
	SetupMaterialSlot(0, FName("Plane Base"), PlaneMaterial);
}

void USubdividedPlaneProvider::DefineSetup(int32 inNx, int32 inNy, float inSizeX, float inSizeY)
{
	FScopeLock Lock(&PropertySyncRoot);
	Nx = inNy;
	Ny = inNy;
	SizeX = inSizeX;
	SizeY = inSizeY;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Nx=%f"), (float)Nx));
}

void USubdividedPlaneProvider::Initialize()
{	
	FRuntimeMeshLODProperties LODProperties;
	LODProperties.ScreenSize = 0.0f;
	ConfigureLODs({ LODProperties });
	SetupMaterialSlot(0, FName("Plane Base"), GetPlaneMaterial());

	FRuntimeMeshSectionProperties Properties;
	Properties.bCastsShadow = true;
	Properties.bIsVisible = true;
	Properties.MaterialSlot = 0;
	Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;
	CreateSection(0, 0, Properties);

	MarkCollisionDirty();
}

FBoxSphereBounds USubdividedPlaneProvider::GetBounds()
{
	FVector Ext(SizeX, SizeY, 10.0);
	FBox Box = FBox(FVector(0.0, 0.0, 200.0), Ext);
	return FBoxSphereBounds(Box);
}

bool USubdividedPlaneProvider::GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData)
{
	check(LODIndex == 0 && SectionId == 0);
	float dx = SizeX / (float)Nx; //change of x between two points
	float dy = SizeY / (float)Ny; //change of y between two points
	for (int32 yindex = 0; yindex < Ny+1; yindex++)
	{
		float yvalue = (float)yindex * dy;
		float yalpha = (float)yindex / (float)(Ny);
		for (int32 xindex = 0; xindex < Nx+1; xindex++)
		{
			float xvalue = (float)xindex * dx;
			float xalpha = (float)xindex / (float)(Nx);

			FVector Position(xvalue, yvalue, 0.0);

			//float dfdx = (CalculateHeightForPoint(xvalue + dx, yvalue) - CalculateHeightForPoint(xvalue - dx, yvalue)) / (2 * dx); //derivative of f over x
			//float dfdy = (CalculateHeightForPoint(xvalue, yvalue + dy) - CalculateHeightForPoint(xvalue, yvalue - dy)) / (2 * dy); //derivative of f over y

			float dfdx = 0.0;
			float dfdy = 0.0;

			FVector Normal(-dfdx, -dfdy, 1);
			Normal.Normalize();
			FVector Tangent(1, 0, dfdx);
			Tangent.Normalize();

			FVector2D UV(xalpha, yalpha);

			MeshData.Positions.Add(Position);
			MeshData.Tangents.Add(Normal, Tangent);
			MeshData.TexCoords.Add(UV);
			MeshData.Colors.Add(FColor::White);

			if (xindex != Nx && yindex != Ny )
			{
				int32 AIndex = xindex + yindex * (Nx + 1);
				int32 BIndex = AIndex + 1;
				int32 CIndex = AIndex + (Nx + 1);
				int32 DIndex = CIndex + 1;
				MeshData.Triangles.AddTriangle(AIndex, CIndex, BIndex);
				MeshData.Triangles.AddTriangle(BIndex, CIndex, DIndex);
			}
		}
	}
	return true;
}

FRuntimeMeshCollisionSettings USubdividedPlaneProvider::GetCollisionSettings()
{
	FRuntimeMeshCollisionSettings Settings;
	Settings.bUseAsyncCooking = false;
	Settings.bUseComplexAsSimple = false;
	Settings.Boxes.Emplace(FVector(0.5*SizeX,0.5*SizeY,0.5), FRotator::ZeroRotator, SizeX, SizeY, 1.0);

	return Settings;
}

bool USubdividedPlaneProvider::HasCollisionMesh()
{
	return true;
}

bool USubdividedPlaneProvider::GetCollisionMesh(FRuntimeMeshCollisionData& CollisionData)
{
	// Add the single collision section
	CollisionData.CollisionSources.Emplace(0, 1, this, 0, ERuntimeMeshCollisionFaceSourceType::Collision);

	FRuntimeMeshCollisionVertexStream& CollisionVertices = CollisionData.Vertices;
	FRuntimeMeshCollisionTriangleStream& CollisionTriangles = CollisionData.Triangles;

	FVector BoxRadiusTemp = FVector(SizeX,SizeY,1.0);

	// Generate verts
	CollisionVertices.Add(FVector(0.0, 0.0, 0.0));
	CollisionVertices.Add(FVector(SizeX, 0.0, 0.0));
	CollisionVertices.Add(FVector(0.0, SizeY, 0.0));
	CollisionVertices.Add(FVector(SizeX, SizeY, 0.0));

	CollisionTriangles.Add(0, 1, 2);
	CollisionTriangles.Add(1, 3, 2);

	return true;
}

bool USubdividedPlaneProvider::IsThreadSafe()
{
	return true;
}