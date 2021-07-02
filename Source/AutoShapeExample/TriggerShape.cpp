// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerShape.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"


ATriggerShape::ATriggerShape()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	BoxCollisionComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetupAttachment(RootComponent);
	//by default
	SphereComponent->SetVisibility(true);
	BoxCollisionComponent->SetVisibility(false);
	CapsuleComponent->SetVisibility(false);
}


void ATriggerShape::BeginPlay()
{
	Super::BeginPlay();
	switch (ShapeType)
	{
	case EShapeType::Sphere:
		{
			SelectedComponent = SphereComponent;
			BoxCollisionComponent->DestroyComponent();
			CapsuleComponent->DestroyComponent();
			break;
		}

	case EShapeType::Box:
		{
			SelectedComponent = BoxCollisionComponent;
			SphereComponent->DestroyComponent();
			CapsuleComponent->DestroyComponent();
			break;
		}

	case EShapeType::Capsule:
		{
			SelectedComponent = CapsuleComponent;
			SphereComponent->DestroyComponent();
			BoxCollisionComponent->DestroyComponent();
			break;
		}
	}
}

// Called every frame
void ATriggerShape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void ATriggerShape::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = (PropertyChangedEvent.Property != nullptr)
		                     ? PropertyChangedEvent.Property->GetFName()
		                     : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ATriggerShape, ShapeType))
	{
		FEnumProperty* EnumProperty = CastField<FEnumProperty>(PropertyChangedEvent.Property);
		if (EnumProperty)
		{
			EShapeType* ShapeValue = EnumProperty->ContainerPtrToValuePtr<EShapeType>(this);
			if (ShapeValue)
			{
				switch (*ShapeValue)
				{
				case EShapeType::Sphere:
					{
						SphereComponent->SetVisibility(true);
						BoxCollisionComponent->SetVisibility(false);
						CapsuleComponent->SetVisibility(false);
						break;
					}

				case EShapeType::Box:
					{
						SphereComponent->SetVisibility(false);
						BoxCollisionComponent->SetVisibility(true);
						CapsuleComponent->SetVisibility(false);
						break;
					}

				case EShapeType::Capsule:
					{
						SphereComponent->SetVisibility(false);
						BoxCollisionComponent->SetVisibility(false);
						CapsuleComponent->SetVisibility(true);
						break;
					}
				}
			}
		}
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(FBoxCollision, X) || PropertyName == GET_MEMBER_NAME_CHECKED(FBoxCollision, Y) || PropertyName == GET_MEMBER_NAME_CHECKED(FBoxCollision, Z))
	{
		// For some reason it's not working with FVector so i have to split in separate floats :S
		BoxCollisionComponent->SetBoxExtent(FVector(BoxCollision.X, BoxCollision.Y, BoxCollision.Z));
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(FSphereCollision, SphereRadius))
	{
		SphereComponent->SetSphereRadius(SphereCollision.SphereRadius);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(FCapsuleCollision, CapsuleHalfHeight) || PropertyName == GET_MEMBER_NAME_CHECKED(FCapsuleCollision, CapsuleRadius))
	{
		CapsuleComponent->SetCapsuleRadius(CapsuleCollision.CapsuleRadius);
		CapsuleComponent->SetCapsuleHalfHeight(CapsuleCollision.CapsuleHalfHeight);
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}


bool ATriggerShape::CanEditChange(const FProperty* InProperty) const
{
	const bool ParentVal = Super::CanEditChange(InProperty);
	bool bCanEdit = true;
	const FName& PropertyName = InProperty->GetFName();
	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ATriggerShape, BoxCollision) || PropertyName == GET_MEMBER_NAME_CHECKED(ATriggerShape, SphereCollision) || PropertyName == GET_MEMBER_NAME_CHECKED(ATriggerShape, CapsuleCollision))
	{
		TArray<FName> DisabledProperties;
		switch (ShapeType)
		{
		case EShapeType::Sphere:
			DisabledProperties = {
				GET_MEMBER_NAME_CHECKED(ATriggerShape, BoxCollision),
                GET_MEMBER_NAME_CHECKED(ATriggerShape, CapsuleCollision)
            };
			break;
                
		case EShapeType::Box:
			DisabledProperties = {
				GET_MEMBER_NAME_CHECKED(ATriggerShape, SphereCollision),
                GET_MEMBER_NAME_CHECKED(ATriggerShape, CapsuleCollision)
            };
			break;
                
		case EShapeType::Capsule:
			DisabledProperties = {
				GET_MEMBER_NAME_CHECKED(ATriggerShape, BoxCollision),
				GET_MEMBER_NAME_CHECKED(ATriggerShape, SphereCollision)
            };
			break;
		}
		bCanEdit = !DisabledProperties.Contains(PropertyName);
	}
	
	return ParentVal && bCanEdit;
}

#endif