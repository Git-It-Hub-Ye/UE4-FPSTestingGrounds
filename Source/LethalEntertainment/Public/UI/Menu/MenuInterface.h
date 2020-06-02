// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

enum class EControlType : uint8;

/**
 * Communicate between game classes and Menus
 */
class LETHALENTERTAINMENT_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PlayGame() = 0;

	virtual void ReturnToMainMenu() = 0;

	virtual void OuitGame() = 0;

	virtual void ResumeGame() = 0;

	virtual void RestartGame() = 0;

	virtual void SetNewUserSettings(float Mouse_Sensitivity, float Mouse_ADS_Sensitivity, float Controller_Sensitivity, float Controller_ADS_Sensitivity, bool Invert_Y) = 0;

	virtual void GetCurrentUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity, bool & Invert_Y) = 0;

	virtual void GetDefaultUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity, bool & Invert_Y) = 0;

	virtual void SetNewControlType(EControlType NewControlType) = 0;
	
};
