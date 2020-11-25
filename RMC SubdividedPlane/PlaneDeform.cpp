// Obiwahn89, 25.11.2020
// RunTimeMeshModifier to use the modify a subdivided plane


#include "PlaneDeform.h"

void UPlaneDeform::ApplyToMesh(FRuntimeMeshRenderableMeshData& MeshData)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ApplyToMesh"));
	float SizeX, SizeY;
	int32 Nx, Ny;
	SizeX = 400;
	SizeY = 400;
	Nx = 2;
	Ny = 2;


	float dx = SizeX / (float)Nx; //change of x between two points
	float dy = SizeY / (float)Ny; //change of y between two points
	for (int32 yindex = 0; yindex < Ny + 1; yindex++)
	{
		float yvalue = (float)yindex * dy;
		float yalpha = (float)yindex / (float)(Ny);
		for (int32 xindex = 0; xindex < Nx + 1; xindex++)
		{
			float xvalue = (float)xindex * dx;
			float xalpha = (float)xindex / (float)(Nx);

			FVector Position(xvalue, yvalue, 0.0);
			if (xvalue > 0.0 && xvalue < SizeX)
			{
				Position = FVector(xvalue, yvalue, -100.0);
			}

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

			if (xindex != Nx && yindex != Ny)
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
	return;
}