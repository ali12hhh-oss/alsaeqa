#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSAEQARuntimeVisualComponent.generated.h"

UENUM(BlueprintType)
enum class EALSAEQAVisualRole : uint8 { Hero, Companion, Enemy, Boss, Snake, Mount, Weapon };

UCLASS(ClassGroup=(ALSAEQA), meta=(BlueprintSpawnableComponent))
class ALSAEQA_API UALSAEQARuntimeVisualComponent : public UActorComponent
{
 GENERATED_BODY()
public:
 UALSAEQARuntimeVisualComponent();
 UFUNCTION(BlueprintCallable,Category="ALSAEQA|Visual") void RebuildVisual();
 UFUNCTION(BlueprintCallable,Category="ALSAEQA|Visual") void SetVisualRole(EALSAEQAVisualRole NewRole);
 UFUNCTION(BlueprintPure,Category="ALSAEQA|Visual") EALSAEQAVisualRole GetVisualRole() const{return VisualRole;}
protected:
 virtual void BeginPlay() override; virtual void TickComponent(float Delta,ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") EALSAEQAVisualRole VisualRole=EALSAEQAVisualRole::Hero;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") bool bBuildOnBeginPlay=true;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") bool bAddWeapon=true;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") bool bAddStormLight=true;
private:
 void ClearVisual(); void AddMeshPart(const TArray<FVector>&V,const TArray<int32>&T,const FVector&Loc,const FRotator&Rot,const FVector&Scale,const FLinearColor&Color,bool Animate=false);
 void AddEllipsoid(const FVector&Loc,const FVector&Scale,const FLinearColor&Color,bool Animate=false,int32 Seg=14,int32 Rings=7);
 void AddTube(const FVector&Loc,float R0,float R1,float H,const FVector&Scale,const FLinearColor&Color,const FRotator&Rot=FRotator::ZeroRotator,bool Animate=false,int32 Seg=12);
 void AddWeaponVisual(); void AddStormVisual();
 UPROPERTY(Transient) TArray<TObjectPtr<USceneComponent>> VisualParts; UPROPERTY(Transient) TArray<TObjectPtr<USceneComponent>> AnimatedParts;
 float AnimTime=0.f;
};
