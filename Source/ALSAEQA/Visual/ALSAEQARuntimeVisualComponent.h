#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSAEQARuntimeVisualComponent.generated.h"

class UALSAEQAArtProfile;
class UAnimInstance;
class UAnimMontage;
class USkeletalMesh;
class USkeletalMeshComponent;

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
 UFUNCTION(BlueprintPure,Category="ALSAEQA|Visual") bool IsUsingImportedArt() const{return bUsingImportedArt;}
 UFUNCTION(BlueprintCallable,Category="ALSAEQA|Animation") bool PlayLightAttackAnimation();
 UFUNCTION(BlueprintCallable,Category="ALSAEQA|Animation") bool PlayHeavyAttackAnimation();
 UFUNCTION(BlueprintCallable,Category="ALSAEQA|Animation") bool PlayHitReactionAnimation();
 UFUNCTION(BlueprintCallable,Category="ALSAEQA|Animation") bool PlayDeathAnimation();
 UFUNCTION(BlueprintCallable,Category="ALSAEQA|Animation") bool PlaySpecialAbilityAnimation();
protected:
 virtual void BeginPlay() override;
 virtual void TickComponent(float Delta,ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") EALSAEQAVisualRole VisualRole=EALSAEQAVisualRole::Hero;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") bool bBuildOnBeginPlay=true;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") bool bAddWeapon=true;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") bool bAddStormLight=true;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Real Art",meta=(ToolTip="Preferred source of truth for final imported 3D art. Assign a UALSAEQAArtProfile asset after importing the real mesh and animations.")) TSoftObjectPtr<UALSAEQAArtProfile> ArtProfile;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Real Art",meta=(ToolTip="Direct imported skeletal mesh override. Used when ArtProfile is empty.")) TSoftObjectPtr<USkeletalMesh> ImportedSkeletalMesh;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Real Art",meta=(ToolTip="Optional animation blueprint class. Leave empty to use the mesh's configured Anim Class.")) TSubclassOf<UAnimInstance> ImportedAnimClass;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Real Art") FVector ImportedRelativeLocation=FVector::ZeroVector;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Real Art") FRotator ImportedRelativeRotation=FRotator::ZeroRotator;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Real Art") FVector ImportedRelativeScale=FVector(1.f);
 UPROPERTY(Transient,VisibleInstanceOnly,BlueprintReadOnly,Category="ALSAEQA|Real Art") bool bUsingImportedArt=false;
private:
 void ClearVisual();
 bool TryBuildImportedArt();
 bool PlayProfileMontage(TSoftObjectPtr<UAnimMontage> MontageAsset);
 void AddMeshPart(const TArray<FVector>&V,const TArray<int32>&T,const FVector&Loc,const FRotator&Rot,const FVector&Scale,const FLinearColor&Color,bool Animate=false);
 void AddEllipsoid(const FVector&Loc,const FVector&Scale,const FLinearColor&Color,bool Animate=false,int32 Seg=14,int32 Rings=7);
 void AddTube(const FVector&Loc,float R0,float R1,float H,const FVector&Scale,const FLinearColor&Color,const FRotator&Rot=FRotator::ZeroRotator,bool Animate=false,int32 Seg=12);
 void AddWeaponVisual(); void AddStormVisual();
 UPROPERTY(Transient) TObjectPtr<USkeletalMeshComponent> ImportedMeshComponent;
 UPROPERTY(Transient) TArray<TObjectPtr<USceneComponent>> VisualParts;
 UPROPERTY(Transient) TArray<TObjectPtr<USceneComponent>> AnimatedParts;
 float AnimTime=0.f;
};
