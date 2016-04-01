#pragma once

#include "VKDelegateHandler.generated.h"

USTRUCT()
struct FVKUserInfo
{
	GENERATED_USTRUCT_BODY();

	FString ID;
	FString FirstName;
	FString LastName;

	FVKUserInfo()
	{

	};

	FVKUserInfo(const FString& id, const FString& firstName, const FString& lastName) :
		ID(id),
		FirstName(firstName),
		LastName(lastName)
	{

	};
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVKOnLoggedIn, FString, userID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVKOnLoginFailed, FString, error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVKOnShareCompleted, bool, bWasSuccessfull, FString, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FVKOnGotFriends, bool, bWasSuccessfull, TArray<FVKUserInfo>, friends, FString, errorCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVKOnFriendWasInvited, bool, bWasSuccessfull, FString, jsonResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FVKOnCustomRequestCompleted, bool, bWasSuccessfull, FString, method, FString, jsonResponse, FString, error);


UCLASS()
class VK_API UVKDelegateHandler : public UObject
{
	GENERATED_BODY()

public:
	UVKDelegateHandler();

	UPROPERTY(BlueprintAssignable, Category = "VK Social Network")
	FVKOnLoggedIn VKOnLoggedIn;

	UPROPERTY(BlueprintAssignable, Category = "VK Social Network")
	FVKOnLoginFailed VKOnLoginFailed;

	UPROPERTY(BlueprintAssignable, Category = "VK Social Network")
	FVKOnShareCompleted VKOnShareCompleted;

	UPROPERTY(BlueprintAssignable, Category = "VK Social Network")
	FVKOnGotFriends VKOnGotFriends;

	UPROPERTY(BlueprintAssignable, Category = "VK Social Network")
	FVKOnFriendWasInvited VKOnFriendWasInvited;

	UPROPERTY(BlueprintAssignable, Category = "VK Social Network")
	FVKOnCustomRequestCompleted VKOnCustomRequestCompleted;

	virtual void OnLoggedIn(FString userID);
	virtual void OnLoginFailed(FString error);
	virtual void OnShareCompleted(bool bWasSuccessfull, FString result);
	virtual void OnGotFriends(bool bWasSuccessfull, TArray<FVKUserInfo> friends, FString errorCode);
	virtual void OnFriendWasInvited(bool bWasSuccessfull, FString jsonResponse);
	virtual void OnCustomRequestCompleted(bool bWasSuccessfull, FString method, FString jsonResponse, FString error);
};


