// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemWidget.h"

#include "Styling/SlateBrush.h"


#include "QuantitySpliterWidget.h"
#include "InventoryWidget.h"

void UItemWidget::SetItem(FItem ItemInfoToSet) {
	//Set Item Info
	ItemInfo = ItemInfoToSet;

	//Set Quantitiy Text
	if(ItemInfo.bCombinable) {
		TxtQuantity->SetText(FText::AsNumber(ItemInfo.Quantity));
	}else {
		TxtQuantity->SetText(FText::GetEmpty());
	}

	//Set Image Style
	ImgItem->Brush.SetResourceObject(ItemInfo.Image);
}

void UItemWidget::SetItemStyleAsDragged() {
	FSlateBrush slateBrush = ImgItem->Brush;	
	FSlateColor newTintColor = FSlateColor(DraggedColour);
	slateBrush.TintColor = newTintColor;

	ImgItem->SetBrush(slateBrush);
}

void UItemWidget::SetItemStyleAsDefault() {
	FSlateBrush slateBrush = ImgItem->Brush;
	FSlateColor newTintColor = FSlateColor(DefaultColour);
	slateBrush.TintColor = newTintColor;

	ImgItem->SetBrush(slateBrush);
}

void UItemWidget::OnItemDrop(UItemWidget* DroppedItem) {
	if (SlotIndex == DroppedItem->ItemInfo.SlotIndex) {	return;	}

	DroppedItem->bIsItemDragging = false;

	if(bIsSlotHasItem) {
		if(ItemInfo.ItemName.EqualTo(DroppedItem->ItemInfo.ItemName)) {
			//Combine
			InventoryWidgetRef->CombineItems(ItemInfo, DroppedItem->ItemInfo);
		}else {
			//Swap
			OpenSplitItemPanel(DroppedItem);
			InventoryWidgetRef->SwapItemsBySlot(DroppedItem->ItemInfo, ItemInfo);
		}
	}else {
		if(InventoryWidgetRef->GetSplitFunctinalityEnabled()) {
			//Open split Item
			OpenSplitItemPanel(DroppedItem);
		}else {
			//Change Item Slot
			InventoryWidgetRef->ChangeItemSlot(DroppedItem->ItemInfo, SlotIndex);
		}
	}
}

void UItemWidget::OpenSplitItemPanel(UItemWidget* DroppedItem) {
	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) return;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController != NULL)) return;
	
	Spliter = CreateWidget<UQuantitySpliterWidget>(PlayerController, UQuantitySpliterWidget::StaticClass());
	if (!ensure(Spliter != NULL)) return;	
	Spliter->InitializeSpliter(InventoryWidgetRef, DroppedItem->ItemInfo,SlotIndex);
	Spliter->AddToViewport(1);
	
}

