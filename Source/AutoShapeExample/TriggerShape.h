// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerShape.generated.h"

class UCapsuleComponent;
class USphereComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EShapeType : uint8
{
    Sphere,
    Capsule,
    Box,
};

USTRUCT(BlueprintType)
struct FBoxCollision
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category=Shape)
    float X = 32.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category=Shape)
    float Y = 32.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category=Shape)
    float Z = 32.0f;
};

USTRUCT(BlueprintType)
struct FSphereCollision
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category=Shape)
    float SphereRadius = 32.0f;
};

USTRUCT(BlueprintType)
struct FCapsuleCollision
{
    GENERATED_BODY()

    /** 
    *	Half-height, from center of capsule to the end of top or bottom hemisphere.  
    *	This cannot be less than CapsuleRadius.
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category=Shape, meta=(ClampMin="0", UIMin="0"))
    float CapsuleHalfHeight = 44.0f;

    /** 
    *	Radius of cap hemispheres and center cylinder. 
    *	This cannot be more than CapsuleHalfHeight.
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category=Shape, meta=(ClampMin="0", UIMin="0"))
    float CapsuleRadius = 22.0f;
};

UCLASS()
class AUTOSHAPEEXAMPLE_API ATriggerShape : public AActor
{
    GENERATED_BODY()

public:

    ATriggerShape();
    virtual void Tick(float DeltaTime) override;
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif // WITH_EDITOR


    UPROPERTY(EditAnywhere, Category = TriggerShape)
    EShapeType ShapeType;

    UPROPERTY(EditAnywhere, Category = Collision)
    FBoxCollision BoxCollision;

    UPROPERTY(EditAnywhere, Category = Collision)
    FSphereCollision SphereCollision;

    UPROPERTY(EditAnywhere, Category = Collision)
    FCapsuleCollision CapsuleCollision;
protected:

    virtual void BeginPlay() override;
private:
    UPROPERTY()
    UBoxComponent* BoxCollisionComponent;

    UPROPERTY()
    USphereComponent* SphereComponent;

    UPROPERTY()
    UCapsuleComponent* CapsuleComponent;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

    UPROPERTY()
    UShapeComponent* SelectedComponent;
};
