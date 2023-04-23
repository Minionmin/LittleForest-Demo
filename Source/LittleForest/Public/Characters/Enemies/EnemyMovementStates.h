#pragma once

UENUM(BlueprintType, Category = "Movement State")
enum class EEnemyState : uint8
{
	EES_IsHit UMETA(DisplayName = "IsHit"),
	EES_Idle UMETA(DisplayName = "Idle"),
	EES_IsDead UMETA(DisplayName = "IsDead"),
	EES_IsAttacking UMETA(DisplayName = "IsAttacking"),
	EES_IsMoving UMETA(DisplayName = "IsMoving")
};