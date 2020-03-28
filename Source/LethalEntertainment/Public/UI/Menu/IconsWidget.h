// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IconsWidget.generated.h"

class UOverlay;
class UImage;

/**
 * Displays icons (Such as Board/Mouse or Gamepad controls)
 */
UCLASS()
class LETHALENTERTAINMENT_API UIconsWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	/** Allows widgets to be stacked ontop of each other */
	UPROPERTY(meta = (BindWidget))
	UOverlay * Base;

protected:
	virtual bool Initialize() override;
	
};
