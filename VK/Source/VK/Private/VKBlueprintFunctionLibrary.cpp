#include "VKPrivatePCH.h"


DEFINE_LOG_CATEGORY_STATIC(LogVK, Log, All);


UVKDelegateHandler *UVKBlueprintFunctionLibrary::GetVKDelegateHandler()
{
	IVK *vkHelper = &IVK::Get();

	if (vkHelper)
	{
		return vkHelper->GetDelegateHandler();
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::GetVKDelegateHandler: VK module is NULL"));
	}

	return nullptr;
}

void UVKBlueprintFunctionLibrary::Login()
{
	IVK *vkHelper = &IVK::Get();

	if (vkHelper)
	{
		vkHelper->Login();
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::Login: VK module is NULL"));
	}
}

void UVKBlueprintFunctionLibrary::Logout()
{
	IVK *vkHelper = &IVK::Get();

	if (vkHelper)
	{
		vkHelper->Logout();
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::Logout: VK module is NULL"));
	}
}

bool UVKBlueprintFunctionLibrary::IsAuthorized()
{
	IVK *vkHelper = &IVK::Get();
	bool bResult = false;

	if (vkHelper)
	{
		bResult = vkHelper->IsLoggedIn();
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::IsAuthorized: VK module is NULL"));
	}

	return bResult;
}

void UVKBlueprintFunctionLibrary::Share(FString message, int32 userID /*= 0*/)
{
	IVK *vkHelper = &IVK::Get();

	if (vkHelper)
	{
		IVK::ImageInfo imageInfo;
		vkHelper->Share(message, imageInfo, userID);
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::Share: VK module is NULL"));
	}
}

void UVKBlueprintFunctionLibrary::PostWithParamters(TArray<FVKRequestPair> pairs)
{
	IVK *vkHelper = &IVK::Get();

	if (vkHelper)
	{
		TMap<FString, FString> params;
		for (auto &pair : pairs)
		{
			params.Add(pair.Key, pair.Value);
		}

		vkHelper->PostWithParamters(params);
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::Share: VK module is NULL"));
	}
}

void UVKBlueprintFunctionLibrary::GetFriendList()
{
	IVK *vkHelper = &IVK::Get();

	if (vkHelper)
	{
		vkHelper->GetFriendList();
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::GetFriendList: VK module is NULL"));
	}
}

void UVKBlueprintFunctionLibrary::InviteFriend(const FString& userID, const FString& message)
{
	IVK *vkHelper = &IVK::Get();

	if (vkHelper)
	{
		vkHelper->InviteFriend(userID, message);
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::InviteFriend: VK module is NULL"));
	}
}

void UVKBlueprintFunctionLibrary::JoinToGroup(const FString& groupID)
{
	IVK *vkHelper = &IVK::Get();

	if (vkHelper)
	{
		vkHelper->JoinToGroup(groupID);
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::JoinToGroup: VK module is NULL"));
	}
}

void UVKBlueprintFunctionLibrary::CustomRequest(const FString& method, TArray<FVKRequestPair> pairs)
{
	IVK *vkHelper = &IVK::Get();

	if (vkHelper)
	{
		TMap<FString, FString> params;
		for (auto &pair : pairs)
		{
			params.Add(pair.Key, pair.Value);
		}

		vkHelper->CustomRequest(method, params);
	}
	else
	{
		UE_LOG(LogVK, Warning, TEXT("UVKBlueprintFunctionLibrary::CustomRequest: VK module is NULL"));
	}
}