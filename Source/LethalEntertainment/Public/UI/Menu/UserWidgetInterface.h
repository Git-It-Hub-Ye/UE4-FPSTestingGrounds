// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UserWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUserWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Allows communication between parent and child C++ UUserWidgets
 */
class LETHALENTERTAINMENT_API IUserWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void RequestReturnToParentWidget() = 0;

	virtual void OnNavUpToParent() = 0;

	virtual void OnNavDownToParent() = 0;

};
