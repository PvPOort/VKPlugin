#include "VKPrivatePCH.h"


UVKDelegateHandler::UVKDelegateHandler()
{

}

void UVKDelegateHandler::OnLoggedIn(FString userID)
{
#if PLATFORM_IOS
	[FIOSAsyncTask CreateTaskWithBlock : ^ bool(void) {
#endif
		VKOnLoggedIn.Broadcast(userID);
#if PLATFORM_IOS
		return true;
	}];
#endif
}

void UVKDelegateHandler::OnLoginFailed(FString error)
{
#if PLATFORM_IOS
	[FIOSAsyncTask CreateTaskWithBlock : ^ bool(void) {
#endif
		VKOnLoginFailed.Broadcast(error);
#if PLATFORM_IOS
		return true;
	}];
#endif
}

void UVKDelegateHandler::OnShareCompleted(bool bWasSuccessfull, FString result)
{
#if PLATFORM_IOS
	[FIOSAsyncTask CreateTaskWithBlock : ^ bool(void) {
#endif
		VKOnShareCompleted.Broadcast(bWasSuccessfull, result);
#if PLATFORM_IOS
		return true;
	}];
#endif
}

void UVKDelegateHandler::OnGotFriends(bool bWasSuccessfull, TArray<FVKUserInfo> friends, FString errorCode)
{
#if PLATFORM_IOS
	[FIOSAsyncTask CreateTaskWithBlock : ^ bool(void) {
#endif
		VKOnGotFriends.Broadcast(bWasSuccessfull, friends, errorCode);
#if PLATFORM_IOS
		return true;
	}];
#endif
}

void UVKDelegateHandler::OnFriendWasInvited(bool bWasSuccessfull, FString jsonResponse)
{
#if PLATFORM_IOS
	[FIOSAsyncTask CreateTaskWithBlock : ^ bool(void) {
#endif
		VKOnFriendWasInvited.Broadcast(bWasSuccessfull, jsonResponse);
#if PLATFORM_IOS
		return true;
	}];
#endif
}

void UVKDelegateHandler::OnCustomRequestCompleted(bool bWasSuccessfull, FString method, FString jsonResponse, FString error)
{
#if PLATFORM_IOS
	[FIOSAsyncTask CreateTaskWithBlock : ^ bool(void) {
#endif
		VKOnCustomRequestCompleted.Broadcast(bWasSuccessfull, method, jsonResponse, error);
#if PLATFORM_IOS
		return true;
	}];
#endif
}


