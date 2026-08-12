package caxecraft.app;

import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.content.RuntimeContentPack.RuntimeItemUseProfile;
import caxecraft.domain.World;
import caxecraft.gameplay.ItemKind;

/**
	Connects validated item mechanics to the current bounded hotbar.

	The content pack selects a mechanic, such as a light source or melee weapon.
	This adapter converts that mechanic to the matching engine inventory kind.
	Equipment stays outside the hotbar because the session equips it directly.
**/
/**
	Return the inventory kind for one runtime item, or null for direct equipment.

	A placeable block uses its validated terrain code. A no-use item needs its
	validated base-pack identity because its action profile cannot identify a
	hotbar kind.
**/
function inventoryKindForRuntimeItem(registry:RuntimeContentRegistry, storageCode:Int):Null<ItemKind> {
	return switch registry.itemUseProfile(storageCode) {
		case ConsumeOneHeart: ItemKind.Berries;
		case HaxeforgeTool: ItemKind.Haxeforge;
		case LightSource: ItemKind.Lantern;
		case MeleeSword: ItemKind.CopperSword;
		case PlaceBlock:
			switch World.kindFromCode(registry.itemPlacementBlockStorageCode(storageCode)) {
				case Grass: ItemKind.GrassBlock;
				case Dirt: ItemKind.DirtBlock;
				case Stone: ItemKind.StoneBlock;
				case Sand: ItemKind.SandBlock;
				case Air | Bedrock | Wood | Leaves | Snow | Ash: null;
			};
		case NoItemUse:
			switch registry.itemIdForStorageCode(storageCode) {
				case "caxecraft:bread": ItemKind.Bread;
				case _: null;
			};
		case EquipAquatic: null;
	};
}
