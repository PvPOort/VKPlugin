#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VKBlueprintFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct VK_API FVKRequestPair
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VK Social Network")
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VK Social Network")
	FString Value;

	FVKRequestPair()
	{

	}

	FVKRequestPair(const FString& key, const FString& value) :
		Key(key),
		Value(value)
	{

	}
};

UCLASS()
class VK_API UVKBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static UVKDelegateHandler *GetVKDelegateHandler();

	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static void Login();

	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static void Logout();

	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static bool IsAuthorized();

	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static void Share(FString message, int32 userID = 0);

	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static void PostWithParamters(TArray<FVKRequestPair> pairs);

	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static void GetFriendList();

	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static void InviteFriend(const FString& userID, const FString& message);

	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static void JoinToGroup(const FString& groupID);

	UFUNCTION(BlueprintCallable, Category = "VK Social Network")
	static void CustomRequest(const FString& method, TArray<FVKRequestPair> pairs);
};


