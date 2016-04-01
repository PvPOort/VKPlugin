#pragma once

#include "VKPrivatePCH.h"

#if PLATFORM_IOS
#import "Foundation/Foundation.h"
#import "UIKit/UIKit.h"
#import "CoreFoundation/CoreFoundation.h"
#include "VKSdk/VKSdk.h"

@interface IOSVKHelper : NSObject <VKSdkDelegate, VKSdkUIDelegate>
{
	IVK *VKContext;
}

+(id)initWithContext:(IVK*) context;

-(void)Login;
-(void)Logout;
-(bool)IsLoggedIn;
-(void)Share:(NSString*)message withImage : (UIImage*)image withUserID:(NSInteger) userID;
-(void)GetFriendList;
-(void)InviteFriend:(NSString*)userID withMessage : (NSString*)message;
-(void)Post:(NSDictionary*)parameters;

@end
#endif // PLATFORM_IOS


class PlatformVKHelper
{
protected:
#if PLATFORM_IOS
	IOSVKHelper *VKHelper;
#endif // PLATFORM_IOS

public:
	PlatformVKHelper(IVK* context);
	virtual ~PlatformVKHelper();

	void Login();
	void Logout();
	bool IsLoggedIn();
	void Share(FString message, IVK::ImageInfo imageInfo, int32 userID);
	void GetFriendList();
	void InviteFriend(const FString& userID, const FString& message);
	void JoinToGroup(const FString& groupID);
	void CustomRequest(const FString& method, TMap<FString, FString> params);
	void PostWithParamters(TMap<FString, FString> params);
};