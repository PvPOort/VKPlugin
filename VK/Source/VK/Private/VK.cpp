// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "VKPrivatePCH.h"

class FVK : public IVK
{
	PlatformVKHelper *VKHelper;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:

	virtual void Login() override;
	virtual void Logout() override;
	virtual bool IsLoggedIn() override;
	virtual void Share(FString message, IVK::ImageInfo imageInfo, int32 userID = 0) override;
	virtual void GetFriendList() override;
	virtual void InviteFriend(const FString& userID, const FString& message) override;
	virtual void JoinToGroup(const FString& groupID) override;
	virtual void CustomRequest(const FString& method, TMap<FString, FString> params) override;
	virtual void PostWithParamters(TMap<FString, FString> params) override;
};


IMPLEMENT_MODULE( FVK, VK )

void FVK::StartupModule()
{
	IVK::StartupModule();

	DelegateHandler = NewObject<UVKDelegateHandler>();

	UE_LOG(LogClass, Log, TEXT("FVK::StartupModule"));

	VKHelper = new PlatformVKHelper(this);
}

void FVK::ShutdownModule()
{
	IVK::ShutdownModule();

	UE_LOG(LogClass, Log, TEXT("FVK::ShutdownModule"));
}

void FVK::Login()
{
	if (VKHelper)
	{
		VKHelper->Login();
	}
}

void FVK::Logout()
{
	if (VKHelper)
	{
		VKHelper->Logout();
	}
}

bool FVK::IsLoggedIn()
{
	if (VKHelper)
	{
		return VKHelper->IsLoggedIn();
	}

	return false;
}

void FVK::Share(FString message, IVK::ImageInfo imageInfo, int32 userID /*= 0*/)
{
	if (VKHelper)
	{
		VKHelper->Share(message, imageInfo, userID);
	}
}

void FVK::GetFriendList()
{
	if (VKHelper)
	{
		VKHelper->GetFriendList();
	}
}

void FVK::InviteFriend(const FString& userID, const FString& message)
{
	if (VKHelper)
	{
		VKHelper->InviteFriend(userID, message);
	}
}

void FVK::JoinToGroup(const FString& groupID)
{
	if (VKHelper)
	{
		VKHelper->JoinToGroup(groupID);
	}
}

void FVK::CustomRequest(const FString& method, TMap<FString, FString> params)
{
	if (VKHelper)
	{
		VKHelper->CustomRequest(method, params);
	}
}

void FVK::PostWithParamters(TMap<FString, FString> params)
{
	if (VKHelper)
	{
		VKHelper->PostWithParamters(params);
	}
}