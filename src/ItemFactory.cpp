#include "ItemFactory.h"

void ItemFactory::loadItemBean(ItemBean& bean, ItemID id)
{
	switch (id)
	{
	case ItemID::Equipment_head_wizardhat_blue:
		bean.description = Texts::Item_description_wizardhat;
		bean.name = Texts::Item_name_wizardhat_blue;
		bean.type = ItemType::Equipment_head;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Head_wizardhat_blue;
		bean.value = 20;
		bean.attributes.damageIce = 5;
		bean.attributes.resistanceIce = 2;
		bean.attributes.resistancePhysical = 1;
		bean.attributes.manaRegenerationPerS = 2;
		bean.attributes.maxManaPoints = 5;
		break;
	case ItemID::Equipment_head_wizardhat_black:
		bean.description = Texts::Item_description_wizardhat;
		bean.name = Texts::Item_name_wizardhat_grey;
		bean.type = ItemType::Equipment_head;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Head_wizardhat_grey;
		bean.value = 20;
		bean.attributes.manaRegenerationPerS = 4;
		bean.attributes.resistanceFire = 2;
		bean.attributes.resistancePhysical = 1;
		bean.attributes.maxManaPoints = 5;
		bean.attributes.maxHealthPoints = 5;
		break;
	case ItemID::Equipment_weapon_staff_ice:
		bean.description = Texts::Item_description_staff_ice;
		bean.name = Texts::Item_name_staff_ice;
		bean.type = ItemType::Equipment_weapon;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Weapon_icestaff;
		bean.value = 120;
		bean.attributes.manaRegenerationPerS = 4;
		bean.attributes.maxManaPoints = 10;
		bean.attributes.maxHealthPoints = 10;
		bean.attributes.damageIce = 30;
		bean.attributes.damagePhysical = 15;
		break;
	case ItemID::Equipment_weapon_rustysword:
		bean.description = Texts::Item_description_weapon_rustysword;
		bean.name = Texts::Item_name_weapon_rustysword;
		bean.type = ItemType::Equipment_weapon;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Weapon_rustysword;
		bean.value = 15;
		bean.attributes.damagePhysical = 5;
		break;
	case ItemID::Food_Cheese:
		bean.description = Texts::Item_description_Food_Cheese;
		bean.name = Texts::Item_name_food_cheese;
		bean.type = ItemType::Food;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.maxHealthPoints = 40;
		bean.value = 3;
		break;
	case ItemID::Food_Bread:
		bean.description = Texts::Item_description_Food_Bread;
		bean.name = Texts::Item_name_food_bread;
		bean.type = ItemType::Food;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.maxHealthPoints = 30;
		bean.value = 2;
		break;
	case ItemID::Food_Water:
		bean.description = Texts::Item_description_Food_Water;
		bean.name = Texts::Item_name_food_water;
		bean.type = ItemType::Food;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.maxHealthPoints = 10;
		bean.attributes.maxManaPoints = 50;
		bean.value = 2;
		break;
	default:
		bean.id = ItemID::Void;
		return;
	}

	bean.id = id;
}