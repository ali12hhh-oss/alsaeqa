#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSAEQARuntimeVisualComponent.generated.h"
UENUM(BlueprintType) enum class EALSAEQAVisualRole:uint8{Hero,Companion,Enemy,Boss,Snake,Mount,Weapon};
UCLASS(ClassGroup=(ALSAEQA),meta=(BlueprintSpawnableComponent)) class ALSAEQA_API UALSAEQARuntimeVisualComponent:public UActorComponent
{
 GENERATED_BODY()
public:
 UALSAEQARuntimeVisualComponent();
 UFUNCTION(BlueprintCallable,Category="ALSAEQA|Visual") void RebuildVisual();
 UFUNCTION(BlueprintCallable,Category="ALSAEQA|Visual") void SetVisualRole(EALSAEQAVisualRole NewRole);
 UFUNCTION(BlueprintPure,Category="ALSAEQA|Visual") EALSAEQAVisualRole GetVisualRole()const{return VisualRole;}
protected:
 virtual void BeginPlay()override;
 virtual void TickComponent(float DeltaTime,ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)override;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") EALSAEQAVisualRole VisualRole=EALSAEQAVisualRole::Hero;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") bool bBuildOnBeginPlay=true;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") bool bAddWeapon=true;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ALSAEQA|Visual") bool bAddStormLight=true;
private:
 void ClearVisual(); void AddMesh(const TCHAR* AssetPath,const FVector& RelativeLocation,const FVector& RelativeScale,const FRotator& RelativeRotation); void AddWeaponVisual(); void AddStormVisual();
 UPROPERTY(Transient) TArray<TObjectPtr<USceneComponent>> VisualParts;
 float VisualTime=0.0f;
};
