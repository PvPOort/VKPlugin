// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"


/**
 * The public interface to this module.  In most cases, this interface is only public to sibling modules 
 * within this plugin.
 */
class IVK : public IModuleInterface
{
protected:
	class UVKDelegateHandler *DelegateHandler;

public:
	struct ImageInfo
	{
		uint8 *Data;
		FVector2D Size;
		uint8 ChannelsNum;

		ImageInfo() : Data(nullptr)
		{

		}
	};

	virtual void StartupModule()
	{

	}

	virtual void ShutdownModule()
	{

	}

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IVK& Get()
	{
		return FModuleManager::LoadModuleChecked< IVK >("VK");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("VK");
	}

	class UVKDelegateHandler *GetDelegateHandler()
	{
		return DelegateHandler;
	}

	virtual void Login()
	{

	}

	virtual void Logout()
	{

	}

	virtual bool IsLoggedIn()
	{
		return false;
	}

	virtual void Share(FString message, IVK::ImageInfo imageInfo, int32 userID = 0)
	{

	}

	virtual void GetFriendList()
	{

	}

	virtual void InviteFriend(const FString& userID, const FString& message)
	{

	}

	virtual void JoinToGroup(const FString& groupID)
	{

	}

	virtual void CustomRequest(const FString& method, TMap<FString, FString> params)
	{

	}

	virtual void PostWithParamters(TMap<FString, FString> params)
	{

	}
};

