#include "World/ALSAEQARuntimeWorldBuilder.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AALSAEQARuntimeWorldBuilder::AALSAEQARuntimeWorldBuilder()
{
    PrimaryActorTick.bCanEverTick = false;
    LoadPrimitiveMeshes();
}

void AALSAEQARuntimeWorldBuilder::BeginPlay()
{
    Super::BeginPlay();
    if (bGenerateOnBeginPlay)
    {
        GenerateWorld();
    }
}

void AALSAEQARuntimeWorldBuilder::LoadPrimitiveMeshes()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cone(TEXT("/Engine/BasicShapes/Cone.Cone"));

    CubeMesh = Cube.Succeeded() ? Cube.Object : nullptr;
    SphereMesh = Sphere.Succeeded() ? Sphere.Object : nullptr;
    CylinderMesh = Cylinder.Succeeded() ? Cylinder.Object : nullptr;
    ConeMesh = Cone.Succeeded() ? Cone.Object : nullptr;
}

UStaticMeshComponent* AALSAEQARuntimeWorldBuilder::AddPrimitive(UStaticMesh* Mesh, const FVector& Location, const FVector& Scale, const FRotator& Rotation)
{
    if (!Mesh)
    {
        return nullptr;
    }

    UStaticMeshComponent* Component = NewObject<UStaticMeshComponent>(this);
    if (!Component)
    {
        return nullptr;
    }

    Component->SetStaticMesh(Mesh);
    Component->SetWorldLocation(Location);
    Component->SetWorldScale3D(Scale);
    Component->SetWorldRotation(Rotation);
    Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Component->SetMobility(EComponentMobility::Static);
    Component->RegisterComponent();
    GeneratedComponents.Add(Component);
    return Component;
}

void AALSAEQARuntimeWorldBuilder::ClearGeneratedWorld()
{
    for (UStaticMeshComponent* Component : GeneratedComponents)
    {
        if (IsValid(Component))
        {
            Component->DestroyComponent();
        }
    }
    GeneratedComponents.Reset();
}

void AALSAEQARuntimeWorldBuilder::GenerateWorld()
{
    ClearGeneratedWorld();

    FRandomStream Random(Seed);
    BuildGround(Random);
    BuildCliffsAndRocks(Random);
    BuildForest(Random);
    BuildAncientRuins(Random);
    BuildTraversalSetPieces(Random);
}

void AALSAEQARuntimeWorldBuilder::BuildGround(FRandomStream& Random)
{
    const int32 Half = TileCount / 2;
    for (int32 X = -Half; X <= Half; ++X)
    {
        for (int32 Y = -Half; Y <= Half; ++Y)
        {
            const float Height = Random.FRandRange(-45.0f, 45.0f);
            AddPrimitive(CubeMesh,
                FVector(X * TileSize, Y * TileSize, Height),
                FVector(TileSize / 200.0f, TileSize / 200.0f, 0.25f),
                FRotator::ZeroRotator);
        }
    }
}

void AALSAEQARuntimeWorldBuilder::BuildForest(FRandomStream& Random)
{
    const float Radius = TileCount * TileSize * 0.43f;
    for (int32 Index = 0; Index < TileCount * TileCount * 2; ++Index)
    {
        const FVector Location(
            Random.FRandRange(-Radius, Radius),
            Random.FRandRange(-Radius, Radius),
            180.0f);

        const float Height = Random.FRandRange(2.5f, 5.5f);
        AddPrimitive(CylinderMesh, Location, FVector(0.45f, 0.45f, Height), FRotator::ZeroRotator);
        AddPrimitive(SphereMesh, Location + FVector(0, 0, 420.0f * Height), FVector(2.2f, 2.2f, 2.8f), FRotator::ZeroRotator);
    }
}

void AALSAEQARuntimeWorldBuilder::BuildCliffsAndRocks(FRandomStream& Random)
{
    const float Radius = TileCount * TileSize * 0.5f;
    for (int32 Index = 0; Index < TileCount * 5; ++Index)
    {
        const FVector Location(
            Random.FRandRange(-Radius, Radius),
            Random.FRandRange(-Radius, Radius),
            Random.FRandRange(100.0f, 650.0f));
        const float Size = Random.FRandRange(1.0f, 4.5f);
        AddPrimitive(SphereMesh, Location, FVector(Size, Size * Random.FRandRange(0.7f, 1.3f), Size * Random.FRandRange(0.6f, 1.8f)),
            FRotator(0, Random.FRandRange(0, 360), Random.FRandRange(-12, 12)));
    }
}

void AALSAEQARuntimeWorldBuilder::BuildAncientRuins(FRandomStream& Random)
{
    const FVector Center(0, 0, 260);
    for (int32 Side = 0; Side < 4; ++Side)
    {
        const float Angle = Side * 90.0f;
        const FVector Offset = FRotator(0, Angle, 0).RotateVector(FVector(1400, 0, 0));
        AddPrimitive(CylinderMesh, Center + Offset, FVector(1.1f, 1.1f, 5.0f), FRotator::ZeroRotator);
    }

    for (int32 Beam = 0; Beam < 4; ++Beam)
    {
        const float Angle = Beam * 90.0f;
        const FVector Offset = FRotator(0, Angle, 0).RotateVector(FVector(700, 0, 850));
        AddPrimitive(CubeMesh, Center + Offset, FVector(7.5f, 0.8f, 0.7f), FRotator(0, Angle, 0));
    }

    AddPrimitive(CylinderMesh, Center + FVector(0, 0, 120), FVector(5.0f, 5.0f, 0.4f), FRotator::ZeroRotator);
    AddPrimitive(ConeMesh, Center + FVector(0, 0, 980), FVector(3.0f, 3.0f, 2.0f), FRotator::ZeroRotator);
    (void)Random;
}

void AALSAEQARuntimeWorldBuilder::BuildTraversalSetPieces(FRandomStream& Random)
{
    const float Radius = TileCount * TileSize * 0.35f;

    // A broken bridge: a traversal gap that later powers can solve in different ways.
    for (int32 Segment = -3; Segment <= 3; ++Segment)
    {
        if (Segment == 0)
        {
            continue;
        }
        AddPrimitive(CubeMesh,
            FVector(Segment * 450.0f, 2200.0f, 620.0f + FMath::Abs(Segment) * 35.0f),
            FVector(2.0f, 0.45f, 0.18f),
            FRotator(0, Random.FRandRange(-4, 4), Random.FRandRange(-6, 6)));
    }

    // Stepping stones and a vertical climb route.
    for (int32 Step = 0; Step < 9; ++Step)
    {
        AddPrimitive(CubeMesh,
            FVector(-2600.0f + Step * 520.0f, -1800.0f + Step * 120.0f, 160.0f + Step * 145.0f),
            FVector(1.1f, 1.1f, 0.22f),
            FRotator::ZeroRotator);
    }

    // A remote thunder altar that becomes meaningful once the staged abilities reach it.
    AddPrimitive(CylinderMesh, FVector(Radius, -Radius, 320), FVector(2.8f, 2.8f, 1.2f), FRotator::ZeroRotator);
    AddPrimitive(ConeMesh, FVector(Radius, -Radius, 1150), FVector(1.4f, 1.4f, 2.8f), FRotator::ZeroRotator);
}
