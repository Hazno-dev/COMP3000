#include "CustomEditorModule.h"

#include "Modules/ModuleManager.h"
#include "../../COMP3000/Public/DungeonGeneration/Prefabinator.h"
#include "PlacementMode/Public/IPlacementModeModule.h"
#include "Templates/SharedPointer.h"

IMPLEMENT_MODULE(FCustomEditorModule, VarianceEditor)


void FCustomEditorModule::StartupModule()
{
	const FPlacementCategoryInfo Info(
		INVTEXT("Variance Custom"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "PlacementBrowser.Icons.Basic"),
		TEXT("VariCustom"),
		TEXT("VariCustom"),
		100
	);

	IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
	PlacementModeModule.RegisterPlacementCategory(Info);

	// Add actor classes to the category
	PlacementModeModule.RegisterPlaceableItem(Info.UniqueHandle,
		   MakeShareable(new FPlaceableItem(nullptr, FAssetData(APrefabinator::StaticClass()))));
}

void FCustomEditorModule::ShutdownModule()
{
	// Unregister the custom category when the module is shut down
	if (IPlacementModeModule::IsAvailable())
	{
		IPlacementModeModule::Get().UnregisterPlacementCategory("VariCustom");
	}
}
