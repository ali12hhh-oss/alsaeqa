#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSAEQAMountTypes.h"
#include "ALSAEQAMountComponent.generated.h"

delegate void FALSAEQAMountStateChanged(EALSAEQAMountState NewState);

delegate void FALSAEQAMountChanged(FName MountId, bool bMounted);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(ALSAEQA), meta=(BlueprintSpawnableComponent))
class ALSAEQA_API UALSAEQAMountComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UALSAEQAMountComponent();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Mount")
    bool TameMount(const FALSAEQAMountProfile& Profile);

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Mount")
    bool Mount();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Mount")
    bool Dismount();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Mount")
    bool ReleaseMount();

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Mount")
    bool HasTamedMount() const { return MountState == EALSAEQAMountState::Tamed || MountState == EALSAEQAMountState::Mounted; }

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Mount")
    bool IsMounted() const { return MountState == EALSAEQAMountState::Mounted; }

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Mount")
    EALSAEQAMountState GetState() const { return MountState; }

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Mount")
    FALSAEQAMountProfile GetMountProfile() const { return MountProfile; }

    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Mount")
    FALSAEQAMountStateChanged OnStateChanged;

    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Mount")
    FALSAEQAMountChanged OnMountChanged;

private:
    void SetState(EALSAEQAMountState NewState);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame, Category="ALSAEQA|Mount", meta=(AllowPrivateAccess="true"))
    EALSAEQAMountState MountState = EALSAEQAMountState::Wild;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame, Category="ALSAEQA|Mount", meta=(AllowPrivateAccess="true"))
    FALSAEQAMountProfile MountProfile;
};
