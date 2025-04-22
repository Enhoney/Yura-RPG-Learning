// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "YuraInputConfig.h"
#include "YuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// 模板函数，允许绑定按下、按住、抬起三种输入
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UYuraInputConfig* InputConfig, UserClass* Object,
		PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UYuraInputComponent::BindAbilityActions(const UYuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FYuraInputAction& InputAction : InputConfig->AbilityInputActions)
	{
		if (PressedFunc)
		{
			// 这个函数是接收可变参数的，在回调函数后面的参数将会作为参数传递到回调函数中，所以这种回调函数的设计会稍有不同
			BindAction(InputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputAction.InputTag);
		}

		if (ReleasedFunc)
		{
			// 这个函数是接收可变参数的，在回调函数后面的参数将会作为参数传递到回调函数中，所以这种回调函数的设计会稍有不同
			BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.InputTag);
		}

		if (HeldFunc)
		{
			// 这个函数是接收可变参数的，在回调函数后面的参数将会作为参数传递到回调函数中，所以这种回调函数的设计会稍有不同
			BindAction(InputAction.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputAction.InputTag);
		}
	}
}
