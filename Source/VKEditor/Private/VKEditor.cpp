#include "VKEditorPrivatePCH.h"

#include "VKEditor.h"

#include "VKTargetSettingsCustomization.h"

#include "ModuleInterface.h"
#include "ISettingsModule.h"
#include "ModuleManager.h"

#include "VKProjectSettings.h"

#define LOCTEXT_NAMESPACE "VKPlugin"

void FVKEditor::StartupModule()
{
    // register settings detail panel customization
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomClassLayout(
                                             UVKProjectSettings::StaticClass()->GetFName(),
                                             FOnGetDetailCustomizationInstance::CreateStatic(&FVKTargetSettingsCustomization::MakeInstance)
                                             );
    
    PropertyModule.NotifyCustomizationModuleChanged();
    
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if( SettingsModule != nullptr )
    {
        SettingsModule->RegisterSettings(
                                         "Project", "Plugins", "VK",
                                         LOCTEXT( "VKSettingsName", "VK" ),
                                         LOCTEXT( "VKSettingsDescription", "VK settings" ),
                                         GetMutableDefault<UVKProjectSettings>() );
    }
}

void FVKEditor::ShutdownModule()
{
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if( SettingsModule != nullptr )
    {
        SettingsModule->UnregisterSettings( "Project", "Plugins", "VK" );
    }
}

IMPLEMENT_MODULE(FVKEditor, VKEditor)