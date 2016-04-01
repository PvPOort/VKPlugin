#include "VKPrivatePCH.h"

#if PLATFORM_IOS
#include "IOSAppDelegate.h"

static NSMutableArray *SCOPE = nil;

static void ListenVKOpenURL(UIApplication* application, NSURL* url, NSString* sourceApplication, id annotation)
{
	[VKSdk processOpenURL : url fromApplication : sourceApplication];
}

@implementation IOSVKHelper

+(id)initWithContext: (IVK*)context
{
	IOSVKHelper *instancePtr = [[IOSVKHelper alloc] init];

	if (instancePtr)
	{
		FIOSCoreDelegates::OnOpenURL.AddStatic(&ListenVKOpenURL);

		TArray<FString> scopes;
		GConfig->GetArray(TEXT("/Script/VKEditor.VKProjectSettings"), TEXT("Scopes"), scopes, GEngineIni);

		for (int32 i = 0; i < scopes.Num(); i++)
		{
			[SCOPE addObject : [NSString stringWithFString:scopes[i]]];
		}

		instancePtr->VKContext = context;

		NSBundle* mainBundle = [NSBundle mainBundle];
		NSString *appID = [mainBundle objectForInfoDictionaryKey : @"VKAppID"];

		[[VKSdk initializeWithAppId : appID] registerDelegate : instancePtr];
		[[VKSdk instance] setUiDelegate:instancePtr];
	}

	return instancePtr;
}

-(void)Login
{
	TArray<FString> loginPermissions;
	GConfig->GetArray(TEXT("/Script/VKEditor.VKProjectSettings"), TEXT("LoginPermissions"), loginPermissions, GEngineIni);

	[VKSdk wakeUpSession : SCOPE completeBlock : ^ (VKAuthorizationState state, NSError *error)
	{
		if (state == VKAuthorizationAuthorized)
		{
			FString fields = TEXT("");
			for (int32 i = 0; i < loginPermissions.Num(); i++)
			{
				fields += (i == 0 ? TEXT("") : TEXT(",")) + loginPermissions[i];
			}

			VKRequest *request = [VKRequest requestWithMethod : @"users.get" parameters:@{VK_API_FIELDS:[NSString stringWithFString:fields]}];

			[request executeWithResultBlock : ^ (VKResponse *response)
			{
				NSString *jsonResponse = [NSString stringWithFormat : @"%@", response.json];

				if (VKContext && VKContext->GetDelegateHandler())
				{
					VKContext->GetDelegateHandler()->OnLoggedIn(FString(jsonResponse));
				}
			}
			errorBlock: ^ (NSError *requestError)
			{
				NSLog(@"VK Log:  Error: %@", requestError);

				if (VKContext && VKContext->GetDelegateHandler())
				{
					VKContext->GetDelegateHandler()->OnLoginFailed(FString([requestError localizedDescription]));
				}
			}];
		}
		else if (error)
		{
			NSLog(@"VK Log: Error of authorization %@", error);
		}
		else
		{
			[VKSdk authorize : SCOPE];
		}
	}];
}

-(void)Logout
{
	[VKSdk forceLogout];
	[[UIApplication sharedApplication].keyWindow.rootViewController.navigationController popToRootViewControllerAnimated : YES];
}

-(bool)IsLoggedIn
{
	return
		[VKSdk accessToken] != nil;
}

-(void)Share:(NSString*)message withImage : (UIImage*)image withUserID : (NSInteger)userID
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		if (image == nil)
		{
			NSLog(@"VK: cannot share post with image: image is nil");

			[self Post : @{VK_API_MESSAGE : message}];
			return;
		}

		VKRequest *request = [VKApi uploadWallPhotoRequest : image parameters : [VKImageParameters pngImage] userId : userID groupId : 0];
		[request executeWithResultBlock : ^ (VKResponse *response)
		{
			NSLog(@"Photo: %@", response.json);
			VKPhoto *photoInfo = [(VKPhotoArray *)response.parsedModel objectAtIndex : 0];
			NSString *photoAttachment = [NSString stringWithFormat : @"photo%@_%@", photoInfo.owner_id, photoInfo.id];

			[self Post : @{VK_API_ATTACHMENTS : photoAttachment, VK_API_MESSAGE : message }];
		}
		errorBlock: ^ (NSError *error)
		{
			NSLog(@"request Error: %@", error);

			if (VKContext && VKContext->GetDelegateHandler())
			{
				VKContext->GetDelegateHandler()->OnShareCompleted(false, FString([error localizedDescription]));
			}
		}];
	});
}

-(void)Post:(NSDictionary*)parameters
{
	VKRequest *post = [[VKApi wall] post:parameters];

	[post executeWithResultBlock : ^ (VKResponse *postResponse)
	{
		NSString *jsonResponse = [NSString stringWithFormat : @"%@", postResponse.json];

		NSLog(@"Result: %@", postResponse);

		if (VKContext && VKContext->GetDelegateHandler())
		{
			VKContext->GetDelegateHandler()->OnShareCompleted(true, FString(jsonResponse));
		}
	}
	errorBlock: ^ (NSError *error)
	{
		NSLog(@"post Error: %@", error);

		if (VKContext && VKContext->GetDelegateHandler())
		{
			VKContext->GetDelegateHandler()->OnShareCompleted(false, FString([error localizedDescription]));
		}
	}];
}

-(void)vkSdkReceivedNewToken:(VKAccessToken*)newToken
{
	NSLog(@"VK Log:  vkSdkReceivedNewToken Authorized");
}

-(void)vkSdkUserDeniedAccess : (VKError*)authorizationError
{
	NSLog(@"VK Log:  vkSdkUserDeniedAccess Error of authorization");
}


-(void)vkSdkShouldPresentViewController:(UIViewController *)controller
{
	[[UIApplication sharedApplication].keyWindow.rootViewController presentViewController : controller animated : YES completion : nil];
}


-(void)vkSdkAccessAuthorizationFinishedWithResult:(VKAuthorizationResult *)result
{
	VKRequest *request = [VKRequest requestWithMethod : @"users.get" parameters:@{VK_API_FIELDS:@"sex, bdate"}];

	[request executeWithResultBlock : ^ (VKResponse *response)
	{
		NSString *jsonResponse = [NSString stringWithFormat:@"%@", response.json];
		//NSLog(@"Result: %@", jsonResponse);

		if (VKContext && VKContext->GetDelegateHandler())
		{
			VKContext->GetDelegateHandler()->OnLoggedIn(FString(jsonResponse));
		}
	}
	errorBlock: ^ (NSError *requestError)
	{
		NSLog(@"VK Log:  Error: %@", requestError);

		if (VKContext && VKContext->GetDelegateHandler())
		{
			VKContext->GetDelegateHandler()->OnLoginFailed(FString([requestError localizedDescription]));
		}
	}];
}

-(void)vkSdkUserAuthorizationFailed
{
	if (VKContext && VKContext->GetDelegateHandler())
	{
		VKContext->GetDelegateHandler()->OnLoginFailed(TEXT("Unknown Error"));
	}
}

-(void)vkSdkNeedCaptchaEnter:(VKError *)captchaError
{

}

-(void)GetFriendList
{
	// TODO: BWF: remove logs
	dispatch_async(dispatch_get_main_queue(), ^
	{
		VKRequest *request = [VKRequest requestWithMethod : @"apps.getFriendsList" parameters:@{VK_API_COUNT : @0}];

		[request executeWithResultBlock : ^ (VKResponse *response)
		{
			if (response.responseString == nil)
			{
				if (VKContext && VKContext->GetDelegateHandler())
				{
					TArray<FVKUserInfo> friends;
					VKContext->GetDelegateHandler()->OnGotFriends(false, friends, TEXT("response.responseString == nil"));
				}
				return;
			}

			NSError *jsonError = nil;
			NSDictionary *json = [NSJSONSerialization JSONObjectWithData : [response.responseString dataUsingEncoding : NSUTF8StringEncoding] options : 0 error : &jsonError];

			if (jsonError)
			{
				NSLog(@"VK: Json error : %@",[jsonError localizedDescription]);
				if (VKContext && VKContext->GetDelegateHandler())
				{
					TArray<FVKUserInfo> friends;
					VKContext->GetDelegateHandler()->OnGotFriends(false, friends, FString([jsonError localizedDescription]));
				}
				return;
			}

			NSDictionary *responseData = [json objectForKey : @"response"];
			NSNumber *friendsCount = [responseData objectForKey : @"count"];

			NSLog(@"Friends count: %@", friendsCount);

			if (friendsCount <= 0)
			{
				NSLog(@"VK: Cannot invite friends, friends count : %@", friendsCount);
				if (VKContext && VKContext->GetDelegateHandler())
				{
					TArray<FVKUserInfo> friends;
					VKContext->GetDelegateHandler()->OnGotFriends(false, friends, TEXT("Not enough friends."));
				}
				return;
			}

			NSLog(@"Request Result: %@",[NSString stringWithFormat : @"%@", response.responseString]);

			VKRequest *requestFriends = [VKRequest requestWithMethod : @"apps.getFriendsList" parameters:@{VK_API_COUNT : friendsCount, VK_API_EXTENDED : @1, @"type" : @"request"}];

			[requestFriends executeWithResultBlock : ^ (VKResponse *friendsResponse)
			{
				NSString *jsonResponse = [NSString stringWithFormat : @"%@", friendsResponse.responseString];
				NSLog(@"VK: Request Result : %@", jsonResponse);

				if (VKContext && VKContext->GetDelegateHandler())
				{
					TArray<FVKUserInfo> friends;

					NSError *jsonErrorFriends = nil;
					NSDictionary *jsonFriends = [NSJSONSerialization JSONObjectWithData : [jsonResponse dataUsingEncoding : NSUTF8StringEncoding] options : 0 error : &jsonErrorFriends];

					if (jsonErrorFriends)
					{
						VKContext->GetDelegateHandler()->OnGotFriends(false, friends, FString([jsonErrorFriends localizedDescription]));
						return;
					}

					if (jsonFriends != nil)
					{
						NSDictionary *responseDict = [jsonFriends objectForKey : @"response"];

						if (responseDict != nil)
						{
							NSArray *items = [responseDict objectForKey : @"items"];

							for (NSDictionary *item in items)
							{
								NSString *userID = [item objectForKey : @"id"];
								NSString *firstName = [item objectForKey : @"first_name"];
								NSString *lastName = [item objectForKey : @"last_name"];

								if (userID != nil)
								{
									FString ueUserID = FString([NSString stringWithFormat:@"%@", userID]);
									FString ueFirstName = TEXT("");
									FString ueLastName = TEXT("");

									if (firstName)
									{
										ueFirstName = FString([NSString stringWithFormat : @"%@", firstName]);
									}
									if (lastName)
									{
										ueLastName = FString([NSString stringWithFormat : @"%@", lastName]);
									}

									friends.Add(FVKUserInfo(ueUserID, ueFirstName, ueLastName));
								}
							}
						}
					}

					if (friends.Num())
					{
						VKContext->GetDelegateHandler()->OnGotFriends(true, friends, TEXT(""));
					}
					else
					{
						VKContext->GetDelegateHandler()->OnGotFriends(false, friends, TEXT("Not enough friends."));
					}
				}
			}
			errorBlock: ^ (NSError *error)
			{
				NSLog(@"VK requestFriends error : %@", error);
				if (VKContext && VKContext->GetDelegateHandler())
				{
					TArray<FVKUserInfo> friends;
					VKContext->GetDelegateHandler()->OnGotFriends(false, friends, FString([error localizedDescription]));
				}
			}];
		}
		errorBlock: ^ (NSError *error)
		{
			NSLog(@"VK requestFriends error : %@", error);
			if (VKContext && VKContext->GetDelegateHandler())
			{
				TArray<FVKUserInfo> friends;
				VKContext->GetDelegateHandler()->OnGotFriends(false, friends, FString([error localizedDescription]));
			}
		}];
	});
}

-(void)InviteFriend:(NSString*)userID withMessage : (NSString*)message
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		VKRequest *request = [VKRequest requestWithMethod : @"apps.sendRequest" parameters:@{VK_API_USER_ID : userID, @"text" : message}];

		[request executeWithResultBlock : ^ (VKResponse *response)
		{
			NSString *jsonResponse = [NSString stringWithFormat : @"%@", response.responseString];
			NSLog(@"Request Result: %@", jsonResponse);

			if (VKContext && VKContext->GetDelegateHandler())
			{
				VKContext->GetDelegateHandler()->OnFriendWasInvited(true, FString(jsonResponse));
			}
		}
		errorBlock: ^ (NSError *error)
		{
			NSLog(@"VK request error : %@", error);
			if (VKContext && VKContext->GetDelegateHandler())
			{
				VKContext->GetDelegateHandler()->OnFriendWasInvited(false, FString([error localizedDescription]));
			}
		}];
	});
}

@end

#endif // PLATFORM_IOS

PlatformVKHelper::PlatformVKHelper(IVK* context)
{
#if PLATFORM_IOS
	VKHelper = [IOSVKHelper initWithContext: context];
#endif // PLATFORM_IOS
}

PlatformVKHelper::~PlatformVKHelper()
{

}

void PlatformVKHelper::Login()
{
#if PLATFORM_IOS
	if(VKHelper != nil)
	{
		[VKHelper Login];
	}
#endif // PLATFORM_IOS
}

void PlatformVKHelper::Logout()
{
#if PLATFORM_IOS
	if(VKHelper != nil)
	{
		[VKHelper Logout];
	}
#endif // PLATFORM_IOS
}

bool PlatformVKHelper::IsLoggedIn()
{
#if PLATFORM_IOS
	if (VKHelper != nil)
	{
		return 
			[VKHelper IsLoggedIn];
	}
#endif // PLATFORM_IOS

	return false;
}

void PlatformVKHelper::Share(FString message, IVK::ImageInfo imageInfo, int32 userID)
{
#if PLATFORM_IOS
	if (VKHelper != nil)
	{
		UIImage *finalImage = nil;

		if (imageInfo.Data != nullptr)
		{
			int dataSize = imageInfo.Size.X * imageInfo.Size.Y * imageInfo.ChannelsNum;
			for (int i = 0; i < dataSize; i += 4)
			{
				uint8 r = imageInfo.Data[i];
				imageInfo.Data[i] = imageInfo.Data[i + 2]; // red
				imageInfo.Data[i + 2] = r; // blue
			}

			CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, imageInfo.Data, dataSize, NULL);

			int bitsPerComponent = 8;
			int bitsPerPixel = 32;
			int bytesPerRow = 4 * imageInfo.Size.X;
			CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
			CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
			CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;

			CGImageRef imageRef =
				CGImageCreate(imageInfo.Size.X, imageInfo.Size.Y, 8, 32, 4 * imageInfo.Size.X, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);

			UIImage* flippedImage = [UIImage imageWithCGImage : imageRef];

			finalImage = [UIImage imageWithData :
			UIImagePNGRepresentation([UIImage imageWithCGImage : flippedImage.CGImage scale : flippedImage.scale orientation : UIImageOrientationDown])];
		}

		[VKHelper Share : [NSString stringWithFString : message] withImage : finalImage withUserID : (NSInteger)userID];
	}
#endif // PLATFORM_IOS
}

void PlatformVKHelper::GetFriendList()
{
#if PLATFORM_IOS
	if (VKHelper != nil)
	{
		[VKHelper GetFriendList];
	}
#endif // PLATFORM_IOS
}

void PlatformVKHelper::InviteFriend(const FString& userID, const FString& message)
{
#if PLATFORM_IOS
	if (VKHelper != nil)
	{
		[VKHelper InviteFriend : [NSString stringWithFString : userID] withMessage : [NSString stringWithFString : message]];
	}
#endif // PLATFORM_IOS
}

void PlatformVKHelper::JoinToGroup(const FString& groupID)
{
#if PLATFORM_IOS
	NSString *vkGroupID = [NSString stringWithFString : groupID];

	dispatch_async(dispatch_get_main_queue(), ^
	{
		VKRequest *request = [VKRequest requestWithMethod : @"groups.join" parameters:@{@"group_id" : vkGroupID}];

		[request executeWithResultBlock : ^ (VKResponse *response)
		{
			NSString *jsonResponse = [NSString stringWithFormat : @"%@", response.responseString];
			NSLog(@"Request Result: %@", jsonResponse);

		}
		errorBlock: ^ (NSError *error)
		{
			NSLog(@"VK request error : %@", error);

		}];
	});
#endif // PLATFORM_IOS
}

void PlatformVKHelper::CustomRequest(const FString& method, TMap<FString, FString> params)
{
#if PLATFORM_IOS
	NSString *convertedMethod = [NSString stringWithFString : method];
	NSMutableDictionary *convertedParameters = [[NSMutableDictionary alloc] init];

	for (auto &elem : params)
	{
		NSString *key = [NSString stringWithFString : elem.Key];
		NSString *value = [NSString stringWithFString : elem.Value];

		[convertedParameters setValue : key forKey : value];
	}

	dispatch_async(dispatch_get_main_queue(), ^
	{
		VKRequest *request = [VKRequest requestWithMethod : convertedMethod parameters: convertedParameters];

		[request executeWithResultBlock : ^ (VKResponse *response)
		{
			NSString *jsonResponse = [NSString stringWithFormat : @"%@", response.responseString];
			NSLog(@"Request Result: %@", jsonResponse);

			IVK *moduleInstance = &IVK::Get();
			if(moduleInstance && moduleInstance->GetDelegateHandler())
			{
				moduleInstance->GetDelegateHandler()->OnCustomRequestCompleted(true, FString(convertedMethod), FString(jsonResponse), TEXT(""));
			}
		}
		errorBlock: ^ (NSError *error)
		{
			NSLog(@"VK request error : %@", error);

			IVK *moduleInstance = &IVK::Get();
			if (moduleInstance && moduleInstance->GetDelegateHandler())
			{
				moduleInstance->GetDelegateHandler()->OnCustomRequestCompleted(false, FString(convertedMethod), TEXT(""), FString([error localizedDescription]));
			}
		}];
	});
#endif // PLATFORM_IOS
}

void PlatformVKHelper::PostWithParamters(TMap<FString, FString> params)
{
#if PLATFORM_IOS
	NSMutableDictionary *convertedParameters = [[NSMutableDictionary alloc] init];

	for (auto &elem : params)
	{
		NSString *key = [NSString stringWithFString : elem.Key];
		NSString *value = [NSString stringWithFString : elem.Value];

		[convertedParameters setValue : key forKey : value];
	}

	dispatch_async(dispatch_get_main_queue(), ^
	{
		if (VKHelper)
		{
			[VKHelper Post : convertedParameters];
		}
	});
#endif // PLATFORM_IOS
}
