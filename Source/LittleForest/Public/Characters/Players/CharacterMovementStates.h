#pragma once

UENUM(BlueprintType, Category = "MovementState")
enum class EMovementState : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_RunRight UMETA(DisplayName = "RunRight"),
	EMS_RunLeft UMETA(DisplayName = "RunLeft"),
	EMS_Jump UMETA(DisplayName = "Jump"),
	EMS_Attacking UMETA(DisplayName = "IsAttacking")
};