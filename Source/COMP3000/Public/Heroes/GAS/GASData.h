// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerAbilityInputID : uint8
{
	// 0 None
	None,
	// 1 Confirm
	Confirm,
	// 2 Cancel
	Cancel,
	// 3 LShift
	Dash,
	// 4 E
	Ability1,
	// 5 R
	Ability2,
	// 6 F
	Ability3,
	// 7 Q
	Ultimate
};