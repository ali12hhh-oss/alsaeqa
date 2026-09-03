#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALSAEQARuntimeWorldBuilder.generated.h"

class UStaticMesh;
class UStaticMeshComponent;

/** Runtime procedural first-playable world builder using Unreal built-in primitives. */
UCLASS()
class ALSAEQA_API AALSAEQARuntimeWorldBuilder : public AActor
{
    GENERATED_BODY()

public:
    AALSAEQARuntimeWorldBuilder();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|World")
    void GenerateWorld();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|World")
    void ClearGeneratedWorld();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|World")
    int32 Seed = 20260903;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|World", meta=(ClampMin="2", ClampMax="20"))
    int32 TileCount = 8;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|World", meta=(ClampMin="400.0", ClampMax="5000.0"))
    float TileSize = 1800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|World")
    bool bGenerateOnBeginPlay = true;

private:
    UPROPERTY(Transient)
    TArray<TObjectPtr<UStaticMeshComponent>> GeneratedComponents;

    TObjectPtr<UStaticMesh> CubeMesh;
    TObjectPtr<UStaticMesh> SphereMesh;
    TObjectPtr<UStaticMesh> CylinderMesh;
    TObjectPtr<UStaticMesh> ConeMesh;

    void LoadPrimitiveMeshes();
    UStaticMeshComponent* AddPrimitive(UStaticMesh* Mesh, const FVector& Location, const FVector& Scale, const FRotator& Rotation);
    void BuildGround(FRandomStream& Random);
    void BuildForest(FRandomStream& Random);
    void BuildCliffsAndRocks(FRandomStream& Random);
    void BuildAncientRuins(FRandomStream& Random);
    void BuildTraversalSetPieces(FRandomStream& Random);
};
