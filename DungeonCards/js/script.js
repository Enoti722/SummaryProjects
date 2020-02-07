// Отдельные дефолтные классы

var gameField = document.getElementById("gameField");
var scoreText = document.getElementById("score");
var recordText = document.getElementById("record");
var gameOverText = document.getElementById("gameOverTextBox");
var gameOver = false;
var secretUsing = false;

var saveRecordBtn = document.getElementById("gameOverSaveBtn");
var ReloadBtn = document.getElementById("gameOverReloadBtn");
var NickInput = document.getElementById("gameOverNickName");

var lStorage = window.localStorage;

var fieldSize = parseInt(lStorage.getItem("curMode"));

var cards = new Array(5);
var charCard = null;
var score = 0;
var step = 0;
var moving = false;

var RecordTableSize = 10;

var mobHealthAdder = 0;
var helpPointsAdder = 0;
var poisonPointsAdder = 0;

let ability = null;

let inventory;

// Классы

class Inventory {
	constructor() {
		this.helmet = 0;
		this.breastplate = 0;
		this.leggins = 0;
		this.boots = 0;

		this.helmetSrc = "";
		this.breastplateSrc = "";
		this.legginsSrc = "";
		this.bootsSrc = "";
	}
	
	clearInventory() {
		if (charCard.healths > 0) {
			charCard.maxHealths = charCard.maxHealths - this.helmet;
			charCard.updateHealths();
		}
		this.helmet = 0;
		document.getElementById("HelmetSlot").src = "src/InvSlot.png";
		
		if (charCard.healths > 0) {
			charCard.maxHealths = charCard.maxHealths - this.breastplate;
			charCard.updateHealths();
		}
		this.breastplate = 0;
		document.getElementById("BreastplateSlot").src = "src/InvSlot.png";
		
		if (charCard.healths > 0) {
			charCard.maxHealths = charCard.maxHealths - this.leggins;
			charCard.updateHealths();
		}
		this.leggins = 0;
		document.getElementById("LegginsSlot").src = "src/InvSlot.png";
		
		if (charCard.healths > 0) {
			charCard.maxHealths = charCard.maxHealths - this.boots;
			charCard.updateHealths();
		}
		this.boots = 0;
		document.getElementById("BootsSlot").src = "src/InvSlot.png";
	}

	updateSlot(SlotNum, item) { // Продавать шмотки хуже
		switch(SlotNum) {
			case 0:
				if (item.armor > this.helmet) {
					charCard.maxHealths = charCard.maxHealths - this.helmet + item.armor;
					charCard.updateHealths();
					this.helmet = item.armor;
					document.getElementById("HelmetSlot").src = item.slotSrc;
				}
				break;
			case 1:
				if (item.armor > this.breastplate) {
					charCard.maxHealths = charCard.maxHealths - this.breastplate + item.armor;
					charCard.updateHealths();
					this.breastplate = item.armor;
					document.getElementById("BreastplateSlot").src = item.slotSrc;
				}
				break;
			case 2:
				if (item.armor > this.leggins) {
					charCard.maxHealths = charCard.maxHealths - this.leggins + item.armor;
					charCard.updateHealths();
					this.leggins = item.armor;
					document.getElementById("LegginsSlot").src = item.slotSrc;
				}
				break;
			case 3:
				if (item.armor > this.boots) {
					charCard.maxHealths = charCard.maxHealths - this.boots + item.armor;
					charCard.updateHealths();
					this.boots = item.armor;
					document.getElementById("BootsSlot").src = item.slotSrc;
				}
				break;
		}
	}
}

class Card { // Начальный класс пустой карты. От него наследуются все остальные

	constructor(xPos, yPos) {
		this.elem = document.createElement("div");
		this.elem.className = "card";
		this.cardSrc = "'src/EmptyCard.png'";
		this.name = "Пустая карта";
		this.elem.innerHTML = "<img src=" + this.cardSrc + " id='cardImg'></img><p id='name'>" + this.name + "</p>";

		if (fieldSize == 3) {
			xPos++;
			yPos++;
		}

		if (xPos == -1) {
			this.curX = -2 + xPos*150;
		}
		else {
			this.curX = 2 + xPos*150;
		}
		if (yPos == -1) {
			this.curY = 199 + yPos*150;
		}
		else {
			this.curY = 201 + yPos*150;
		}

		this.targetX = this.curX;
		this.targetY = this.curY;

		this.elem.style.left = this.curX + "px";
		this.elem.style.top = this.curY + "px";

		if (fieldSize == 3) {
			xPos--;
			yPos--;
		}
		
		this.x = xPos == fieldSize ? fieldSize-1 : xPos == -1 ? 0 : xPos;
		this.y = yPos == fieldSize ? fieldSize-1 : yPos == -1 ? 0 : yPos;
		
		gameField.appendChild(this.elem);
	}
	
	moveTo(xPos, yPos) {
		if (fieldSize == 3) {
			xPos++;
			yPos++;
		}

		this.targetX = 2 + xPos*150;
		this.targetY = 201 + yPos*150;

		if (fieldSize == 3) {
			xPos--;
			yPos--;
		}

		this.x = xPos;
		this.y = yPos;
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			cardDestroyed(this.x, this.y);
			worldTick();
		}
	}

	UpdateImg() {
		this.elem.childNodes[0].src = this.cardSrc;
	}

	UpdateName() {
		this.elem.childNodes[1].innerHTML = this.name;
	}
	
	Tick() { }
	
	acceptDamage(Damage) { }
}

class ArmorCard extends Card {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 0;

		this.slotNum = 0;

		this.slotSrc = "src/LeatherHelmet.png";
		this.cardSrc = "'src/LeatherHelmetCard.png'";

		this.name = "Карта армора";

		this.elem.innerHTML = "<img src=" + this.cardSrc + " draggable='false' id='cardImg'></img><div id='attackBox'><p id='attackText'>" + this.armor + "</p></div><p id='name'>" + this.name + "</p>";
	}
	
	Tick() { }

	cardClick (BySecret) {
		if (CanBeClickedCheck(this)) {
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			cardDestroyed(this.x, this.y);

			inventory.updateSlot(this.slotNum, this);

			worldTick();
		}
	}

	updateArmor() {
		this.elem.childNodes[1].childNodes[0].innerHTML = this.armor;
	}

	UpdateName() {
		this.elem.childNodes[2].innerHTML = this.name;
	}
	
	acceptDamage(Damage, Type) { }
}

class LHelmetCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 1;

		this.slotNum = 0;

		this.slotSrc = "src/LeatherHelmet.png";
		this.cardSrc = "src/LeatherHelmetCard.png";
		this.name = "Кожаный шлем";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class LBreastplateCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 2;

		this.slotNum = 1;

		this.slotSrc = "src/LeatherBreastplate.png";
		this.cardSrc = "src/LeatherBreastplateCard.png";
		this.name = "Кожаная кираса";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class LLegginsCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 1;

		this.slotNum = 2;

		this.slotSrc = "src/LeatherLeggins.png";
		this.cardSrc = "src/LeatherLegginsCard.png";
		this.name = "Кожаные поножи";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class LBootsCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 1;

		this.slotNum = 3;

		this.slotSrc = "src/LeatherBoots.png";
		this.cardSrc = "src/LeatherBootsCard.png";
		this.name = "Кожаные сапоги";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class IHelmetCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 2;

		this.slotNum = 0;

		this.slotSrc = "src/IronHelmet.png";
		this.cardSrc = "src/IronHelmetCard.png";
		this.name = "Железный шлем";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class IBreastplateCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 4;

		this.slotNum = 1;

		this.slotSrc = "src/IronBreastplate.png";
		this.cardSrc = "src/IronBreastplateCard.png";
		this.name = "Железная кираса";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class ILegginsCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 3;

		this.slotNum = 2;

		this.slotSrc = "src/IronLeggins.png";
		this.cardSrc = "src/IronLegginsCard.png";
		this.name = "Железные поножи";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class IBootsCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 2;

		this.slotNum = 3;

		this.slotSrc = "src/IronBoots.png";
		this.cardSrc = "src/IronBootsCard.png";
		this.name = "Железные сапоги";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class FHelmetCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 4;

		this.slotNum = 0;

		this.slotSrc = "src/FireHelmet.png";
		this.cardSrc = "src/FireHelmetCard.png";
		this.name = "Огненный шлем";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class FBreastplateCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 6;

		this.slotNum = 1;

		this.slotSrc = "src/FireBreastplate.png";
		this.cardSrc = "src/FireBreastplateCard.png";
		this.name = "Огненная кираса";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class FLegginsCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 5;

		this.slotNum = 2;

		this.slotSrc = "src/FireLeggins.png";
		this.cardSrc = "src/FireLegginsCard.png";
		this.name = "Огненные поножи";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class FBootsCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 4;

		this.slotNum = 3;

		this.slotSrc = "src/FireBoots.png";
		this.cardSrc = "src/FireBootsCard.png";
		this.name = "Огненные сапоги";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class DHelmetCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 8;

		this.slotNum = 0;

		this.slotSrc = "src/DemonicHelmet.png";
		this.cardSrc = "src/DemonicHelmetCard.png";
		this.name = "Демонический шлем";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class DBreastplateCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 10;

		this.slotNum = 1;

		this.slotSrc = "src/DemonicBreastplate.png";
		this.cardSrc = "src/DemonicBreastplateCard.png";
		this.name = "Демоническая кираса";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class DLegginsCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 9;

		this.slotNum = 2;

		this.slotSrc = "src/DemonicLeggins.png";
		this.cardSrc = "src/DemonicLegginsCard.png";
		this.name = "Демонические поножи";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class DBootsCard extends ArmorCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.armor = 8;

		this.slotNum = 3;

		this.slotSrc = "src/DemonicBoots.png";
		this.cardSrc = "src/DemonicBootsCard.png";
		this.name = "Демонические сапоги";

		this.updateArmor();
		this.UpdateImg();
		this.UpdateName();
	}
}

class SecretCard extends Card { // Рандомная карта
	constructor(xPos, yPos) {
		super(xPos, yPos);
		this.cardSrc = "src/SecretCard.png";
		this.name = "Секретная карта";
		this.UpdateName();
		this.UpdateImg();
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			setClickTimer(this.x, this.y);
			secretUsing = true;
			spawnCard(this.x, this.y);
		}
	}
}

function setClickTimer (xPos, yPos) {
	setTimeout(function () {
		cards[xPos][yPos].cardClick(true);
		//worldTick();
	}, 500);
}

class ShuffleCard extends Card {

	constructor(xPos, yPos) {
		super(xPos, yPos);
		this.cardSrc = "src/ShuffleCard.png";
		this.name = "Перемешивание";
		this.UpdateName();
		this.UpdateImg();
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			shuffleMatrix(cards);

			for (i = 0; i < fieldSize; i++) {
				for (j = 0; j < fieldSize; j++) {
					cards[i][j].moveTo(i, j);
				}
			}

			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			spawnCardFromShuffle(this.x, this.y, true);
			worldTick();
		}
	}
}

class ChestCard extends Card { // Начальный класс пустой карты. От него наследуются все остальные

	constructor(xPos, yPos) {
		super(xPos, yPos);
		this.cardSrc = "src/ChestCard.png";
		this.name = "Сундук";
		this.UpdateName();
		this.UpdateImg();
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			spawnCardFromChest(this.x, this.y, true);
			worldTick();
		}
	}
}

class BadChestCard extends ChestCard { // Начальный класс пустой карты. От него наследуются все остальные

	constructor(xPos, yPos) {
		super(xPos, yPos);
		this.cardSrc = "src/BadChestCard.png";
		this.name = "Плохой сундук";
		this.UpdateName();
		this.UpdateImg();
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			spawnCardFromChest(this.x, this.y, false);
			worldTick();
		}
	}
}

class WeaponChestCard extends Card { // Начальный класс пустой карты. От него наследуются все остальные

	constructor(xPos, yPos) {
		super(xPos, yPos);
		this.cardSrc = "src/WeaponChestCard.png";
		this.name = "Сундук экипировки";
		this.UpdateName();
		this.UpdateImg();
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			spawnCardFromWeaponChest(this.x, this.y);
			worldTick();
		}
	}
}

class FireGunCard extends Card {
	constructor(xPos, yPos) {
		super(xPos, yPos);
		
		this.attack = getRandomNum(1, 5) + mobHealthAdder;
		
		this.rotation = getRandomNum(1, 4) * 90;
		
		this.cardSrc = "'src/FireGunCard.png'";

		this.name = "Огнемёт";
		
		this.elem.innerHTML = "<img src=" + this.cardSrc + " draggable='false' id='cardImg'></img><div id='attackBox'><p id='attackText'>" + this.attack + "</p></div><p id='name'>" + this.name + "</p>";
		
		this.elem.childNodes[0].style.transform = 'rotate(' + this.rotation + 'deg)';

		this.firing = false;
	}
	
	Tick() {
		this.rotation += 90;
		if (this.rotation > 360) this.rotation -= 360;
		this.elem.childNodes[0].style.transform = 'rotate(' + this.rotation + 'deg)';
	}
	
	cardClick(BySecret) {
		if (!this.firing) {
			if (secretUsing || CanBeClickedCheck(this)) {
				var elem = document.createElement("img");
				elem.src = "src/Fire.gif";
				elem.id = "fire";
				if (this.rotation == 90 && this.y+1 < fieldSize) {
					cards[this.x][this.y+1].acceptDamage(this.attack, "FireGun");
					elem.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
					elem.style.top = (this.y+1)*150 - 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
					elem.style.transform = 'rotate(90deg)';
				}
				else if (this.rotation == 180 && this.x-1 > -1) {
					cards[this.x-1][this.y].acceptDamage(this.attack, "FireGun");
					elem.style.left = (this.x-1)*150 + 35 + (fieldSize == 3 ? 150 : 0) + "px";
					elem.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
					elem.style.transform = 'rotate(180deg)';
				}
				else if (this.rotation == 270 && this.y-1 > -1) {
					cards[this.x][this.y-1].acceptDamage(this.attack, "FireGun");
					elem.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
					elem.style.top = (this.y-1)*150 + 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
					elem.style.transform = 'rotate(270deg)';
				}
				else if (this.rotation == 360 && this.x+1 < fieldSize) {
					cards[this.x+1][this.y].acceptDamage(this.attack, "FireGun");
					elem.style.left = (this.x+1)*150 - 35 + (fieldSize == 3 ? 150 : 0) + "px";
					elem.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
					elem.style.transform = 'rotate(0deg)';
				}
				
				if (elem.style.top != "") {
					gameField.appendChild(elem);
				}
				setTimeout(this.afterFire, 500, elem, this);
				secretUsing = true;
				this.firing = true;
			}
		}
	}

	afterFire (elem, card) {
		card.elem.parentNode.removeChild(card.elem);
		if (elem.parentNode != undefined) {
			elem.parentNode.removeChild(elem);
		}
		cardDestroyed(card.x, card.y);
		worldTick();
	}
	
	acceptDamage(Damage) {
		this.attack -= Damage;
		if (this.attack <= 0) {
			this.attack = 0;
			this.updateAttack();
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			spawnEmptyCard(this.x, this.y);
		}
		this.updateAttack();
	}

	UpdateName() {
		this.elem.childNodes[2].innerHTML = this.name;
	}

	updateAttack() {
		this.elem.childNodes[1].childNodes[0].innerHTML = this.attack;
	}
}

class LFireGunCard extends FireGunCard {
	constructor(xPos, yPos) {
		super(xPos, yPos);
		
		this.cardSrc = "src/LFireGunCard.png";
		
		this.UpdateImg();
	}
	
	cardClick(BySecret) {
		if (!this.firing) {
			if (secretUsing || CanBeClickedCheck(this)) {
				var elem1 = document.createElement("img");
				elem1.src = "src/Fire.gif";
				elem1.id = "fire";
				var elem2 = document.createElement("img");
				elem2.src = "src/Fire.gif";
				elem2.id = "fire";
				if (this.rotation == 90) {
					if (this.y+1 < fieldSize) {
						cards[this.x][this.y+1].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y+1)*150 - 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(90deg)';
					}
					if (this.x-1 > -1) {
						cards[this.x-1][this.y].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x-1)*150 + 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(180deg)';
					}
				}
				else if (this.rotation == 180) {
					if (this.x-1 > -1) {
						cards[this.x-1][this.y].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x-1)*150 + 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(180deg)';
					}
					if (this.y-1 > -1) {
						cards[this.x][this.y-1].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y-1)*150 + 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(270deg)';
					}
				}
				else if (this.rotation == 270) {
					if (this.y-1 > -1) {
						cards[this.x][this.y-1].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y-1)*150 + 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(270deg)';
					}
					if (this.x+1 < fieldSize) {
						cards[this.x+1][this.y].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x+1)*150 - 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(0deg)';
					}
				}
				else if (this.rotation == 360) {
					if (this.x+1 < fieldSize) {
						cards[this.x+1][this.y].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x+1)*150 - 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(0deg)';
					}
					if (this.y+1 < fieldSize) {
						cards[this.x][this.y+1].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y+1)*150 - 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(90deg)';
					}
				}
				
				if (elem1.style.top != "") {
					gameField.appendChild(elem1);
				}
				if (elem2.style.top != "") {
					gameField.appendChild(elem2);
				}
				setTimeout(this.afterFire, 500, elem1, elem2, this);
				secretUsing = true;
				this.firing = true;
			}
		}
	}

	afterFire (elem1, elem2, card) {
		card.elem.parentNode.removeChild(card.elem);
		if (elem1.parentNode != undefined) {
			elem1.parentNode.removeChild(elem1);
		}
		if (elem2.parentNode != undefined) {
			elem2.parentNode.removeChild(elem2);
		}
		cardDestroyed(card.x, card.y);
		worldTick();
	}
}

class IFireGunCard extends FireGunCard {
	constructor(xPos, yPos) {
		super(xPos, yPos);
		
		this.cardSrc = "src/IFireGunCard.png";
		
		this.UpdateImg();
	}
	
	cardClick(BySecret) {
		if (!this.firing) {
			if (secretUsing || CanBeClickedCheck(this)) {
				var elem1 = document.createElement("img");
				elem1.src = "src/Fire.gif";
				elem1.id = "fire";
				var elem2 = document.createElement("img");
				elem2.src = "src/Fire.gif";
				elem2.id = "fire";
				if (this.rotation == 90 || this.rotation == 270) {
					if (this.y+1 < fieldSize) {
						cards[this.x][this.y+1].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y+1)*150 - 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(90deg)';
					}
					if (this.y-1 > -1) {
						cards[this.x][this.y-1].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y-1)*150 + 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(270deg)';
					}
				}
				else if (this.rotation == 180 || this.rotation == 360) {
					if (this.x-1 > -1) {
						cards[this.x-1][this.y].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x-1)*150 + 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(180deg)';
					}
					if (this.x+1 < fieldSize) {
						cards[this.x+1][this.y].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x+1)*150 - 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(0deg)';
					}
				}
				
				if (elem1.style.top != "") {
					gameField.appendChild(elem1);
				}
				if (elem2.style.top != "") {
					gameField.appendChild(elem2);
				}
				setTimeout(this.afterFire, 500, elem1, elem2, this);
				secretUsing = true;
				this.firing = true;
			}
		}
	}

	afterFire (elem1, elem2, card) {
		card.elem.parentNode.removeChild(card.elem);
		if (elem1.parentNode != undefined) {
			elem1.parentNode.removeChild(elem1);
		}
		if (elem2.parentNode != undefined) {
			elem2.parentNode.removeChild(elem2);
		}
		cardDestroyed(card.x, card.y);
		worldTick();
	}
}

class TFireGunCard extends FireGunCard {
	constructor(xPos, yPos) {
		super(xPos, yPos);
		
		this.cardSrc = "src/TFireGunCard.png";
		
		this.UpdateImg();
	}
	
	cardClick(BySecret) {
		if (!this.firing) {
			if (secretUsing || CanBeClickedCheck(this)) {
				var elem1 = document.createElement("img");
				elem1.src = "src/Fire.gif";
				elem1.id = "fire";
				var elem2 = document.createElement("img");
				elem2.src = "src/Fire.gif";
				elem2.id = "fire";
				var elem3 = document.createElement("img");
				elem3.src = "src/Fire.gif";
				elem3.id = "fire";
				if (this.rotation == 90) {
					if (this.y+1 < fieldSize) {
						cards[this.x][this.y+1].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y+1)*150 - 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(90deg)';
					}
					if (this.x-1 > -1) {
						cards[this.x-1][this.y].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x-1)*150 + 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(180deg)';
					}
					if (this.y-1 > -1) {
						cards[this.x][this.y-1].acceptDamage(this.attack, "FireGun");
						elem3.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem3.style.top = (this.y-1)*150 + 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem3.style.transform = 'rotate(270deg)';
					}
				}
				else if (this.rotation == 180) {
					if (this.x-1 > -1) {
						cards[this.x-1][this.y].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x-1)*150 + 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(180deg)';
					}
					if (this.y-1 > -1) {
						cards[this.x][this.y-1].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y-1)*150 + 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(270deg)';
					}
					if (this.x+1 < fieldSize) {
						cards[this.x+1][this.y].acceptDamage(this.attack, "FireGun");
						elem3.style.left = (this.x+1)*150 - 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem3.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem3.style.transform = 'rotate(0deg)';
					}
				}
				else if (this.rotation == 270) {
					if (this.y-1 > -1) {
						cards[this.x][this.y-1].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y-1)*150 + 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(270deg)';
					}
					if (this.x+1 < fieldSize) {
						cards[this.x+1][this.y].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x+1)*150 - 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(0deg)';
					}
					if (this.y+1 < fieldSize) {
						cards[this.x][this.y+1].acceptDamage(this.attack, "FireGun");
						elem3.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem3.style.top = (this.y+1)*150 - 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem3.style.transform = 'rotate(90deg)';
					}
				}
				else if (this.rotation == 360) {
					if (this.x+1 < fieldSize) {
						cards[this.x+1][this.y].acceptDamage(this.attack, "FireGun");
						elem1.style.left = (this.x+1)*150 - 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem1.style.transform = 'rotate(0deg)';
					}
					if (this.y+1 < fieldSize) {
						cards[this.x][this.y+1].acceptDamage(this.attack, "FireGun");
						elem2.style.left = (this.x)*150 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.top = (this.y+1)*150 - 35 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem2.style.transform = 'rotate(90deg)';
					}
					if (this.x-1 > -1) {
						cards[this.x-1][this.y].acceptDamage(this.attack, "FireGun");
						elem3.style.left = (this.x-1)*150 + 35 + (fieldSize == 3 ? 150 : 0) + "px";
						elem3.style.top = (this.y)*150 + 200 + (fieldSize == 3 ? 150 : 0) + "px";
						elem3.style.transform = 'rotate(180deg)';
					}
				}
				
				if (elem1.style.top != "") {
					gameField.appendChild(elem1);
				}
				if (elem2.style.top != "") {
					gameField.appendChild(elem2);
				}
				if (elem3.style.top != "") {
					gameField.appendChild(elem3);
				}
				setTimeout(this.afterFire, 500, elem1, elem2, elem3, this);
				secretUsing = true;
				this.firing = true;
			}
		}
	}

	afterFire (elem1, elem2, elem3, card) {
		card.elem.parentNode.removeChild(card.elem);
		if (elem1.parentNode != undefined) {
			elem1.parentNode.removeChild(elem1);
		}
		if (elem2.parentNode != undefined) {
			elem2.parentNode.removeChild(elem2);
		}
		if (elem3.parentNode != undefined) {
			elem3.parentNode.removeChild(elem3);
		}
		cardDestroyed(card.x, card.y);
		worldTick();
	}
}

// Сделать наконец персам абилки
class CharacterCard extends Card {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.healths = 10;
		this.maxHealths = 10;
		this.attack = 0;

		this.hitSrc = "";

		this.poisonInfluence = 0;

		this.cardSrc = "'src/DVaCharCard.png'";

		this.name = "Герой";

		this.elem.innerHTML = "<img src=" + this.cardSrc + " draggable='false' id='cardImg'></img><div id='attackBox'><p id='attackText'>" + this.attack + "</p></div><div id='healthBox'><p id='healthsText'>" + this.healths + "/" + this.maxHealths + "</p></div><p id='charName'>" + this.name + "</p>";
	}
	
	Tick() {
		if (this.healths != 0) {
			this.healths -= this.poisonInfluence;
			if (this.healths <= 1) {
				this.poisonInfluence = 0;
				this.healths = 1;
			}
			this.updateHealths();
		}
	}

	cardClick (BySecret) { }

	updateAttack() {
		this.elem.childNodes[1].childNodes[0].innerHTML = this.attack;
	}

	updateHealths() {
		this.elem.childNodes[2].childNodes[0].innerHTML = this.healths + "/" + this.maxHealths;
		if (this.healths <= 0) {
			gameOver = true;
			gameOverText.className = 'visible';

			document.getElementById("Inventory").className = 'hidden';
			
			if (fieldSize == 3) {
				if (lStorage.getItem('record3x3') < score) {
					lStorage.setItem('record3x3', score);	
				}
			}
			else {
				if (lStorage.getItem('record5x5') < score) {
					lStorage.setItem('record5x5', score);	
				}
			}
			lStorage.setItem('coins', parseInt(lStorage.getItem('coins')) + score);
		}
	}
	
	acceptDamage(Damage, Type) {
		if (Type == "FireGun") {
			if (Damage > 0) {
				if (Damage >= this.healths) {
					this.healths = 0;
					this.updateHealths();
				}
				else if (Damage < this.healths) {
					this.healths -= Damage;
					this.updateHealths();
				}
			}
		}
	}

	UpdateName() {
		this.elem.childNodes[3].innerHTML = this.name;
	}
}

class DVaCharacterCard extends CharacterCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.healths = parseInt(lStorage.getItem("Char0Healths"));
		this.maxHealths = this.healths;
		this.attack = 0;

		this.cardSrc = "src/DVaCharCard.gif";

		this.name = "Герой";

		this.UpdateImg();
		this.updateHealths();
	}

	updateHealths() {
		this.elem.childNodes[2].childNodes[0].innerHTML = this.healths + "/" + this.maxHealths;
		if (this.healths == 0) {
			inventory.clearInventory();
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			let card = new DVaCard(this.x, this.y);
			cards[this.x][this.y] = card;
			charCard = undefined;
			charCard = card;
				
			card.elem.onclick = function() {card.cardClick();};
			lStorage.setItem('coins', parseInt(lStorage.getItem('coins')) + score);
		}
	}
}

class DVaCard extends CharacterCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.healths = 5 + parseInt(lStorage.getItem("Char0Lvl"))-1;
		this.maxHealths = this.healths;
		this.attack = 0;

		this.cardSrc = "src/DVaCard.gif";

		this.name = "Герой";

		this.UpdateImg();
		this.updateHealths();
	}
}

class WinstonCharacterCard extends CharacterCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.healths = parseInt(lStorage.getItem("Char1Healths"));
		this.maxHealths = this.healths;
		this.attack = 0;

		this.cardSrc = "src/JunkRatCharCard.gif";

		this.name = "Герой";

		this.UpdateImg();
		this.updateHealths();

		this.abilityUsed = false;
	}

	updateHealths() {
		if (this.abilityUsed) {
			this.elem.childNodes[2].childNodes[0].innerHTML = this.healths + "/" + this.maxHealths;
			if (this.healths <= 0) {
				gameOver = true;
				gameOverText.className = 'visible';

				document.getElementById("Inventory").className = 'hidden';
				
				if (fieldSize == 3) {
					if (lStorage.getItem('record3x3') < score) {
						lStorage.setItem('record3x3', score);	
					}
				}
				else {
					if (lStorage.getItem('record5x5') < score) {
						lStorage.setItem('record5x5', score);	
					}
				}
				lStorage.setItem('coins', parseInt(lStorage.getItem('coins')) + score);
			}
		}
		else {
			if (this.healths < 1) {
				this.healths = 1;
				this.elem.childNodes[2].childNodes[0].innerHTML = this.healths + "/" + this.maxHealths;
				this.abilityUsed = true;
				for(i = 0; i < fieldSize; i++) {
					for(j = 0; j < fieldSize; j++) {
						if (cards[i][j] instanceof EnemyCard) {
							cards[i][j].elem.parentNode.removeChild(cards[i][j].elem);
							spawnEmptyCard(i, j);
						}
					}
				}
			}
			else {
				this.elem.childNodes[2].childNodes[0].innerHTML = this.healths + "/" + this.maxHealths;
			}
		}
	}
}

class EnemyCard extends CharacterCard { // Сделать выпадение монеток со врага
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.healths = getRandomNum(3, 5) + mobHealthAdder;
		this.maxHealths = this.healths; // Нинада

		this.cardSrc = "'src/EnemyCard.gif'";

		this.name = "Панда";

		this.elem.innerHTML = "<img src=" + this.cardSrc + " draggable='false' id='cardImg'></img><div id='healthBox'><p id='healthsText'>" + this.healths + "</p></div><p id='name'>" + this.name + "</p>";
	}
	
	Tick() { }

	cardClick (BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			if (charCard.hitSrc != "") {
				var elem = document.createElement("img");
				// elem.src = charCard.hitSrc;
				// elem.id = "fire";
				// elem.style.left = this.elem.style.left;
				// elem.style.top = this.elem.style.top;

				gameField.appendChild(elem);
				setTimeout(this.afterFire, 500, elem, this);
				secretUsing = true;
			}
			else {
				this.afterFire(undefined, this);
			}

			//worldTick();
		}
		
	}

	updateHealths() {
		if (this.healths < 0) {
			this.healths = 0;
		}
		this.elem.childNodes[2].childNodes[0].innerHTML = this.healths + "/" + this.maxHealths;
	}
	
	acceptDamage(Damage) {
		this.healths -= Damage;
		if (this.healths <= 0) {
			this.healths = 0;
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			spawnCardFromEnemy(this.x, this.y);
		}
		this.updateHealths();
	}

	afterFire (elem, card) {
		//card.elem.parentNode.removeChild(card.elem);
		if (elem != undefined && elem.parentNode != undefined) {
			elem.parentNode.removeChild(elem);
		}

		if (charCard.attack > 0) {
			if (charCard.attack >= card.healths) {
				charCard.attack -= card.healths;
				card.healths = 0;
				card.updateHealths();
				charCard.updateAttack();

				cards[card.x][card.y].elem.parentNode.removeChild(cards[card.x][card.y].elem);
			}
			else if (charCard.attack < card.healths) {
				card.healths -= charCard.attack;
				charCard.attack = 0;
				card.updateHealths();
				charCard.updateAttack();
			}
		}
		else if (charCard.attack == 0) {
			if (card.healths > charCard.healths) {
				card.healths -= charCard.healths
				charCard.healths = 0;
				card.updateHealths();
				charCard.updateHealths();
				// GAME OVER
			}
			else {
				charCard.healths -= card.healths;
				card.healths = 0;
				card.updateHealths();
				charCard.updateHealths();

				cards[card.x][card.y].elem.parentNode.removeChild(cards[card.x][card.y].elem);
			}
		}
		if (card.healths == 0) {
			spawnCardFromEnemy(card.x, card.y);
		}

		//cardDestroyed(card.x, card.y);
		worldTick();
	}

	updateAttack() { }

	updateHealths() {
		this.elem.childNodes[1].childNodes[0].innerHTML = this.healths;
	}

	UpdateName() {
		this.elem.childNodes[2].innerHTML = this.name;
	}
}

class JeromeEnemyCard extends EnemyCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.healths = getRandomNum(5, 7) + mobHealthAdder;
		this.maxHealths = this.healths;

		this.cardSrc = "src/JeromeEnemyCard.gif";

		this.name = "Тоторо";

		this.updateAttack();
		this.updateHealths();
		this.UpdateImg();
		this.UpdateName();
	}
}

class CatEnemyCard extends EnemyCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.healths = getRandomNum(1, 3) + mobHealthAdder;
		this.maxHealths = this.healths;

		this.cardSrc = "src/CatEnemyCard.gif";

		this.name = "Кошак";

		this.updateAttack();
		this.updateHealths();
		this.UpdateImg();
		this.UpdateName();
	}
}

class CoinCard extends Card { // Добавить кол-во монет и его рандом
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.money = getRandomNum(1, 8) + helpPointsAdder;

		this.cardSrc = "'src/CoinCard.png'";

		this.name = "Монетка";

		this.elem.innerHTML = "<img src=" + this.cardSrc + " draggable='false' id='cardImg'></img><div id='MoneyBox'><p id='MoneyText'>" + this.money + "</p></div><p id='name'>" + this.name + "</p>";
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			cardDestroyed(this.x, this.y);
			score += this.money;
			scoreText.innerHTML = "Score: " + score;
			IncStep(1);
			worldTick();
		}
	}
	
	acceptDamage(Damage) {
		if (this.money > 1) {
			this.money -= Damage;
			if (this.money <= 1) {
				this.money = 1;
			}
			this.updateMoney();
		}
	}
	
	updateMoney() {
		this.elem.childNodes[1].childNodes[0].innerHTML = this.money;
	}

	UpdateName() {
		this.elem.childNodes[2].innerHTML = this.name;
	}
}

class RubyCard extends CoinCard { // Добавить кол-во монет и его рандом
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.money = getRandomNum(5, 13) + mobHealthAdder;

		this.cardSrc = "src/RubyCard.png";

		this.name = "Рубин";

		this.UpdateImg();
		this.UpdateName();
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			cardDestroyed(this.x, this.y);
			score += this.money;
			scoreText.innerHTML = "Score: " + score;
			IncStep(3);
			worldTick();
		}
	}

	acceptDamage(Damage) {
		this.money -= Damage;
		if (this.money <= 0) {
			this.money = 0;
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			spawnCoinCard(this.x, this.y);
		}
		this.updateMoney();
	}
}

class WeaponCard extends Card {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		var d = Math.random();
		if (d < 0.5)
			this.attack = 3;
		else if (d < 0.7)
			this.attack = 5;
		else
			this.attack = 4;
		
		this.attack += helpPointsAdder;

		this.hitSrc = "src/Boom.gif";

		this.cardSrc = "src/MagicWandCard.png";

		this.name = "Волшебный посох";

		this.elem.innerHTML = "<img src=" + this.cardSrc + " draggable='false' id='cardImg'></img><div id='attackBox'><p id='attackText'>" + this.attack + "</p></div><p id='name'>" + this.name + "</p>";
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) {
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			cardDestroyed(this.x, this.y);
			if (charCard.attack < this.attack) {
				charCard.attack = this.attack;
				charCard.updateAttack();
			}

			charCard.hitSrc = this.hitSrc;

			worldTick();
		}
	}
	
	acceptDamage(Damage) {
		this.attack -= Damage;
		if (this.attack <= 0) {
			this.attack = 0;
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			spawnCoinCard(this.x, this.y);
		}
		this.updateAttack();
	}

	updateAttack() {
		this.elem.childNodes[1].childNodes[0].innerHTML = this.attack;
	}

	UpdateName() {
		this.elem.childNodes[2].innerHTML = this.name;
	}
}

class SwordCard extends WeaponCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.attack = getRandomNum(5, 7) + helpPointsAdder;

		this.updateAttack();

		this.hitSrc = "src/SwordLine.gif";

		this.cardSrc = "src/SwordCard.png";

		this.name = "Железный меч";

		this.UpdateName();
		this.UpdateImg();
	}
}

class OnimeSwordCard extends WeaponCard {
	constructor (xPos, yPos) {
		super(xPos, yPos);

		this.attack = getRandomNum(8, 10) + helpPointsAdder;

		this.updateAttack();

		this.hitSrc = "src/SwordLine90.gif";

		this.cardSrc = "src/OnimeSwordCard.png";

		this.name = "Алмазный меч";

		this.UpdateName();

		this.UpdateImg();
	}
}

class PotionCard extends Card { // Класс карты зелий

	constructor(xPos, yPos) {
		super(xPos, yPos);

		this.cardSrc = "src/PotionCard.png";

		this.healths = getRandomNum(1, 4) + helpPointsAdder;

		this.name = "Зелье здоровья";

		this.elem.innerHTML = "<img src=" + this.cardSrc + " draggable='false' id='cardImg'></img><div id='healthBox'><p id='healthsText'>" + this.healths + "</p></div><p id='name'>" + this.name + "</p>";
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) { // Можно сделать бул возвращаемое и тут в child'ах проверку
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			cardDestroyed(this.x, this.y);

			charCard.healths = charCard.healths + this.healths > charCard.maxHealths ? charCard.maxHealths : charCard.healths + this.healths; // Можно сделать функцию в карточке перса
			charCard.poisonInfluence = 0;
			charCard.updateHealths();
			worldTick();
		}
	}
	
	acceptDamage(Damage) {
		this.healths -= Damage;
		if (this.healths <= 0) {
			this.healths = 0;
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			spawnCoinCard(this.x, this.y);
		}
		this.updateHealths();
	}

	updateHealths() {
		this.elem.childNodes[1].childNodes[0].innerHTML = this.healths;
	}

	UpdateName() {
		this.elem.childNodes[2].innerHTML = this.name;
	}
}

class PoisonPotionCard extends PotionCard { // Класс карты зелий

	constructor(xPos, yPos) {
		super(xPos, yPos);

		this.cardSrc = "src/PoisonPotionCard.png";

		this.healths = getRandomNum(1, 2) + poisonPointsAdder;

		this.name = "Яд";

		this.UpdateName();

		this.updateHealths();

		this.UpdateImg();
	}
	
	cardClick(BySecret) {
		if (secretUsing || CanBeClickedCheck(this)) { // Можно сделать бул возвращаемое и тут в child'ах проверку
			cards[this.x][this.y].elem.parentNode.removeChild(cards[this.x][this.y].elem);
			cardDestroyed(this.x, this.y);

			charCard.healths -= this.healths;
			if (charCard.healths <= 1) {
				charCard.poisonInfluence = 0;
				charCard.healths = 1;
			}

			worldTick();
			charCard.poisonInfluence = 1; // Можно сделать функцию в карточке перса
			charCard.updateHealths();
		}
	}
}

class Ability {
	constructor () {
		this.neededSteps = 10;
	}

	useAbility() { }

	UpdateImg() {
		var AbilityImg = document.getElementById("AbilityImg");
		AbilityImg.src = this.abilitySrc;
	}
}

class HealthPotionAbility extends Ability {
	constructor () {
		super();

		this.healthsPerUse = parseInt(lStorage.getItem("Ability0Value"));

		this.abilitySrc = "src/HealthPotionEmpty.png";

		this.UpdateImg();
	}

	useAbility() {
		if (this.neededSteps - step <= 3 && this.neededSteps - step > 0) {
			this.abilitySrc = "src/HealthPotion.png";
			this.UpdateImg();
		}
		else {
			this.abilitySrc = "src/HealthPotionEmpty.png";
			this.UpdateImg();
		}
		if (step >= this.neededSteps) {
			step -= this.neededSteps;
			if(charCard.healths + this.healthsPerUse < charCard.maxHealths) {
				charCard.healths += this.healthsPerUse;
			}
			else {
				charCard.healths = charCard.maxHealths;
			}
			charCard.poisonInfluence = 0;
			charCard.updateHealths();
		}
	}
}

class SwordAbility extends Ability {
	constructor () {
		super();

		this.DamagePerUse = parseInt(lStorage.getItem("Ability1Value"));
		this.neededSteps = 14;

		this.abilitySrc = "src/Sword.png";

		this.UpdateImg();
	}

	useAbility() {
		if (this.neededSteps - step <= 3 && this.neededSteps - step > 0) {
			this.abilitySrc = "src/ActiveSword.png";
			this.UpdateImg();
		}
		else {
			this.abilitySrc = "src/Sword.png";
			this.UpdateImg();
		}

		if (step >= this.neededSteps) {
			step -= this.neededSteps;
			for (i = 0; i < fieldSize; i++) {
				for (j = 0; j < fieldSize; j++) {
					if (cards[i][j] instanceof EnemyCard) {
						cards[i][j].acceptDamage(this.DamagePerUse, "Ability");
					}
				}
			}
		}
	}
}

//Функции

function movingFunc () {
	moving = false;
	for (i = 0; i < fieldSize; i++) {
		for (j = 0; j < fieldSize; j++) {
			if (cards[i][j].targetX > cards[i][j].curX) {
				cards[i][j].curX+=2;
				cards[i][j].elem.style.left = cards[i][j].curX + "px";
				moving = true;
			}
			else if (cards[i][j].targetX < cards[i][j].curX) {
				cards[i][j].curX-=2;
				cards[i][j].elem.style.left = cards[i][j].curX + "px";
				moving = true;
			}
			if (cards[i][j].targetY > cards[i][j].curY) {
				cards[i][j].curY+=2;
				cards[i][j].elem.style.top = cards[i][j].curY + "px";
				moving = true;
			}
			else if (cards[i][j].targetY < cards[i][j].curY) {
				cards[i][j].curY-=2;
				cards[i][j].elem.style.top = cards[i][j].curY + "px";
				moving = true;
			}
			if (cards[i][j].targetY == cards[i][j].curY && cards[i][j].targetX == cards[i][j].curX) {
				cards[i][j].elem.className = "card";
			}
		}
	}
	
}

function cardDestroyed(xPos, yPos) {
	var spawnX = charCard.x;
	var spawnY = charCard.y;
	// проверка какая карта уничтожена
	if (spawnX > xPos) { // Клик слева от перса
		for (i = spawnX; i <= fieldSize; i++) {
			if (i == fieldSize) {
				spawnCard(fieldSize, yPos);

				cards[i-1][yPos].moveTo(i-1, yPos);
			}
			else {
				cards[i-1][yPos] = undefined;
				cards[i-1][yPos] = cards[i][yPos];
				
				cards[i-1][yPos].moveTo(i-1, yPos);
			}
		}
	}
	else if (spawnX < xPos) { // Клик справа от перса
		for (i = spawnX; i >= -1; i--) {
			if (i == -1) {
				spawnCard(-1, yPos);

				cards[i+1][yPos].moveTo(i+1, yPos);
			}
			else {
				cards[i+1][yPos] = undefined;
				cards[i+1][yPos] = cards[i][yPos];
				
				cards[i+1][yPos].moveTo(i+1, yPos);
			}
		}
	}
	else if (spawnY > yPos) { // Клик сверху
		for (i = spawnY; i <= fieldSize; i++) {
			if (i == fieldSize) {
				spawnCard(xPos, fieldSize);

				cards[xPos][i-1].moveTo(xPos, i-1);
			}
			else {
				cards[xPos][i-1] = undefined;
				cards[xPos][i-1] = cards[xPos][i];
				
				cards[xPos][i-1].moveTo(xPos, i-1);
			}
		}
	}
	else if (spawnY < yPos) { // Клик снизу
		for (i = spawnY; i >= -1; i--) {
			if (i == -1) {
				spawnCard(xPos, -1);

				cards[xPos][i+1].moveTo(xPos, i+1);
			}
			else {
				cards[xPos][i+1] = undefined;
				cards[xPos][i+1] = cards[xPos][i];
				
				cards[xPos][i+1].moveTo(xPos, i+1);
			}
		}
	}
}

function spawnCard(xPos, yPos) {
	let card = getNewRandomCard(xPos, yPos);
	cards[xPos == fieldSize ? fieldSize-1 : xPos == -1 ? 0 : xPos][yPos == fieldSize ? fieldSize-1 : yPos == -1 ? 0 : yPos] = card;
		
	card.elem.onclick = function() {card.cardClick();};
	
	card.elem.className = "hidden";

	return card;
}

function spawnCardFromShuffle(xPos, yPos) {
	let card = getNewRandomCard(xPos, yPos);
	cards[xPos][yPos] = card;
		
	card.elem.onclick = function() {card.cardClick();};
}

function spawnCardFromEnemy(xPos, yPos) {
	let card = null;
	if (Math.random() < 0.95) {
		card = new CoinCard(xPos, yPos);
	}
	else {
		card = new RubyCard(xPos, yPos);
	}
	cards[xPos][yPos] = card;
		
	card.elem.onclick = function() {card.cardClick();};
}

function spawnCardFromChest(xPos, yPos, goodChest) {
	let card = null;
	if (goodChest) {
		switch (getRandomNum(0, 2)) {
			case 0:
				if (Math.random() < 0.95) {
					card = new CoinCard(xPos, yPos);
				}
				else {
					card = new RubyCard(xPos, yPos);
				}
				break;
			case 1:
				card = new PotionCard(xPos, yPos);
				break;
		}
	}
	else {
		if (Math.random() < 0.7) {
			card = new PoisonPotionCard(xPos, yPos);
		}
		else {
			card = new ShuffleCard(xPos, yPos);
		}
	}

	cards[xPos][yPos] = card;
		
	card.elem.onclick = function() {card.cardClick();};
}

function spawnCardFromWeaponChest(xPos, yPos) {
	let card = null;
	var rand = Math.random();
	if (rand < 0.8) {
		switch (getRandomNum(0, 4)) {
			case 0:
				card = new LHelmetCard(xPos, yPos);
				break;
			case 1:
				card = new LBreastplateCard(xPos, yPos);
				break;
			case 2:
				card = new LLegginsCard(xPos, yPos);
				break;
			case 3:
				card = new LBootsCard(xPos, yPos);
				break;
		}
	}
	else if (rand < 0.96) {
		switch (getRandomNum(0, 4)) {
			case 0:
				card = new IHelmetCard(xPos, yPos);
				break;
			case 1:
				card = new IBreastplateCard(xPos, yPos);
				break;
			case 2:
				card = new ILegginsCard(xPos, yPos);
				break;
			case 3:
				card = new IBootsCard(xPos, yPos);
				break;
		}
	}
	else if (rand < 0.99) {
		switch (getRandomNum(0, 4)) {
			case 0:
				card = new FHelmetCard(xPos, yPos);
				break;
			case 1:
				card = new FBreastplateCard(xPos, yPos);
				break;
			case 2:
				card = new FLegginsCard(xPos, yPos);
				break;
			case 3:
				card = new FBootsCard(xPos, yPos);
				break;
		}
	}
	else {
		switch (getRandomNum(0, 4)) {
			case 0:
				card = new DHelmetCard(xPos, yPos);
				break;
			case 1:
				card = new DBreastplateCard(xPos, yPos);
				break;
			case 2:
				card = new DLegginsCard(xPos, yPos);
				break;
			case 3:
				card = new DBootsCard(xPos, yPos);
				break;
		}
	}

	cards[xPos][yPos] = card;
		
	card.elem.onclick = function() {card.cardClick();};
}

function spawnEmptyCard(xPos, yPos) {
	let card = new Card(xPos, yPos);
	cards[xPos][yPos] = card;
		
	card.elem.onclick = function() {card.cardClick();};
}

function spawnCoinCard(xPos, yPos) {
	let card = new CoinCard(xPos, yPos);
	cards[xPos][yPos] = card;
		
	card.elem.onclick = function() {card.cardClick();};
}

function getRandomNum(min, max)
{
  return Math.floor(Math.random() * (max - min) + min);
}

function getNewRandomCard (xPos, yPos) {
    var s = Math.random();
    if (s < 0.15) { // Монетка
    	return new CoinCard(xPos, yPos);
    }
    else if (s < 0.216) { // Оружие
    	return new WeaponCard(xPos, yPos);
    }
    else if (s < 0.283) {// Волшебная палочка
    	return new SwordCard(xPos, yPos);
    }
    else if (s < 0.35) {// Ониме меч
    	return new OnimeSwordCard(xPos, yPos);
    }
    else if (s < 0.40) {//Кот
    	return new CatEnemyCard(xPos, yPos);
    }
    else if (s < 0.45) {//Панда
    	return new EnemyCard(xPos, yPos);
    }
    else if (s < 0.50) {//Тоторо
    	return new JeromeEnemyCard(xPos, yPos);
    }
    else if (s < 0.63) {//хил
    	return new PotionCard(xPos, yPos);
    }
    else if (s < 0.65) {//яд
    	return new PoisonPotionCard(xPos, yPos);
    }
    else if (s < 0.675) {//1 огнемёт
    	return new FireGunCard(xPos, yPos);
    }
    else if (s < 0.7) {//2 огнемёт
    	return new LFireGunCard(xPos, yPos);
    }
    else if (s < 0.725) {//3 огнемёт
    	return new TFireGunCard(xPos, yPos);
    }
    else if (s < 0.75) {//4 огнемёт
    	return new IFireGunCard(xPos, yPos);
    }
    else if (s < 0.85) {//сундук
    	return new ChestCard(xPos, yPos);
    }
    else if (s < 0.88) {//плохой сундук
    	return new BadChestCard(xPos, yPos);
    }
    else if (s < 0.92) {//оружейный сундук
    	return new WeaponChestCard(xPos, yPos);
    }
    else {//секретная карта
    	return new SecretCard(xPos, yPos);
    }
}

function IncStep (steps) { // Если будет выбор способностей - будет в классе способности
	step += steps;
	ability.useAbility();
}

function worldTick() {
	for (i = 0; i < fieldSize; i++) {
		for (j = 0; j < fieldSize; j++) {
			cards[i][j].Tick();
		}
	}
	
	helpPointsAdder = Math.floor(score / 100);
	mobHealthAdder = Math.floor(score / 40);
	poisonPointsAdder = Math.floor(score / 80);

	secretUsing = false;
}

function CanBeClickedCheck(card) {
	if (!gameOver) {
		if (!moving) {
			if (!secretUsing) {
				if ((Math.abs(card.x-charCard.x) == 1 && Math.abs(card.y-charCard.y) == 0) || (Math.abs(card.x-charCard.x) == 0 && Math.abs(card.y-charCard.y) == 1)) {
					return true;
				}
			}
		}
	}
	return false;
}

function getCharacterCard(x, y) {
	switch(parseInt(lStorage.getItem('charIndex'))) {
		case 0:
			return new DVaCharacterCard(x, y);
			break;
		case 1:
			return new WinstonCharacterCard(x, y);
			break;
	}
}

function shuffleMatrix(matrix) {
	for (i = 0; i < fieldSize; i++) 
    {
        for (j = 0; j < fieldSize; j++) 
        { 
        	ri = getRandomNum(0, fieldSize);
        	rj = getRandomNum(0, fieldSize);
 			var temp = matrix[i][j]
 			matrix[i][j] = matrix[ri][rj];
 			matrix[ri][rj] = temp;
        }
    }
    return matrix;
}

// Основной код

for (i = 0; i < fieldSize; i++) {
	var arr = new Array(5);
	for (j = 0; j < fieldSize; j++) {
		let nCard = null;
		if (i == 1 && j == 1 && fieldSize == 3) {
			nCard = getCharacterCard(i, j);
			charCard = nCard;
		}
		else if (i == 2 && j == 2 && fieldSize == 5) {
			nCard = getCharacterCard(i, j);
			charCard = nCard;
		}
		else {
			nCard = getNewRandomCard(i, j);
		}
		arr[j] = nCard
		
		nCard.elem.onclick = function() {nCard.cardClick();};
	}
	cards[i] = arr;
}

switch (parseInt(lStorage.getItem("abilityIndex"))) {
	case 0: 
		ability = new HealthPotionAbility();
		break;
	case 1: 
		ability = new SwordAbility();
		break;
}

inventory = new Inventory();

setInterval(function() { movingFunc(); }, 1);

onkeydown = function(key) {
	if (key.keyCode == 38 || key.keyCode == 87) { // Вверх
		if (charCard.y-1 > -1) {
			if (CanBeClickedCheck(cards[charCard.x][charCard.y-1])) {
				cards[charCard.x][charCard.y-1].cardClick();
			}
		}
	}
	else if (key.keyCode == 65 || key.keyCode == 37) { // Влево
		if (charCard.x-1 > -1) {
			if (CanBeClickedCheck(cards[charCard.x-1][charCard.y])) {
				cards[charCard.x-1][charCard.y].cardClick();
			}
		}
	}
	else if (key.keyCode == 83 || key.keyCode == 40) { // Вниз
		if (charCard.y+1 < fieldSize) {
			if (CanBeClickedCheck(cards[charCard.x][charCard.y+1])) {
				cards[charCard.x][charCard.y+1].cardClick();
			}
		}
	}
	else if (key.keyCode == 39 || key.keyCode == 68) { // Вправо
		if (charCard.x+1 < fieldSize) {
			if (CanBeClickedCheck(cards[charCard.x+1][charCard.y])) {
				cards[charCard.x+1][charCard.y].cardClick();
			}
		}
	}
}

recordText.innerHTML = "Record: " + (fieldSize == 3 ? lStorage.getItem('record3x3') : lStorage.getItem('record5x5')); // Сделать отдельные рекорды для обоих режимов

if (fieldSize == 3) {
	saveRecordBtn.addEventListener("click", function() {
		var flag = false;
		var Nick;
		var value;
		for (i = 0; i < RecordTableSize; i++) {
			if (flag) {
				var temp = lStorage.getItem("Record3x3Nick_" + i);
				lStorage.setItem("Record3x3Nick_" + i, Nick);
				Nick = temp;
				temp = lStorage.getItem("Record3x3Value_" + i);
				lStorage.setItem("Record3x3Value_" + i, value);
				value = temp;
			}
			else if (lStorage.getItem("Record3x3Value_" + i) < score) {
				Nick = lStorage.getItem("Record3x3Nick_" + i);
				value = lStorage.getItem("Record3x3Value_" + i);
				lStorage.setItem("Record3x3Nick_" + i, NickInput.value);
				lStorage.setItem("Record3x3Value_" + i, score);
				flag = true;
			}
		}
		saveRecordBtn.disabled = true;
	});
}
else {
	saveRecordBtn.addEventListener("click", function() {
		var flag = false;
		var Nick;
		var value;
		for (i = 0; i < RecordTableSize; i++) {
			if (flag) {
				var temp = lStorage.getItem("Record5x5Nick_" + i);
				lStorage.setItem("Record5x5Nick_" + i, Nick);
				Nick = temp;
				temp = lStorage.getItem("Record5x5Value_" + i);
				lStorage.setItem("Record5x5Value_" + i, value);
				value = temp;
			}
			else if (lStorage.getItem("Record5x5Value_" + i) < score) {
				Nick = lStorage.getItem("Record5x5Nick_" + i);
				value = lStorage.getItem("Record5x5Value_" + i);
				lStorage.setItem("Record5x5Nick_" + i, NickInput.value);
				lStorage.setItem("Record5x5Value_" + i, score);
				flag = true;
			}
		}
		saveRecordBtn.disabled = true;
	});
}

ReloadBtn.addEventListener("click", function () {
	location.reload();
});