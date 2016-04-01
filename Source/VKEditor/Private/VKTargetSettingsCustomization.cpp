// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "VKEditorPrivatePCH.h"
#include "VKTargetSettingsCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "PropertyEditing.h"

#include "ObjectEditorUtils.h"
#include "IDocumentation.h"
#include "OutputDevice.h"


TSharedRef<IDetailCustomization> FVKTargetSettingsCustomization::MakeInstance()
{
	return MakeShareable(new FVKTargetSettingsCustomization);
}

void FVKTargetSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	IDetailCategoryBuilder& IosCategory = DetailLayout.EditCategory(TEXT("Setup"), FText::GetEmpty(), ECategoryPriority::Variable);

}

#undef LOCTEXT_NAMESPACE