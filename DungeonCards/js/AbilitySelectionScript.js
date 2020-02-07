var abilityIndex = parseInt(window.localStorage.getItem("abilityIndex"));
var coins = parseInt(window.localStorage.getItem("coins"));

var charName = document.getElementById("CharName");
var charInfo = document.getElementById("CharInfo");
var leftBtn = document.getElementById("leftBtn");
var rightBtn = document.getElementById("rightBtn");
var charImg = document.getElementById("CharImg");
var menuBtn = document.getElementById("MenuBtn");
var coinsText = document.getElementById("Coins");
var abilityLvlText = document.getElementById("CharLvl");
var upgradeBtn = document.getElementById("UpgradeBtn");
var upgradePriceText = document.getElementById("UpgradePrice");

function indexChanged() { // Сделать для этой части свои картинки
	var lStorage = window.localStorage;
	switch(abilityIndex) {
		case 0:
			charName.innerHTML = "Зелье здоровья";
			charInfo.innerHTML = "Добавляет " + lStorage.getItem("Ability0Value") + " здоровья</br>Перезарядка: 10</br>Монетки перезаряжают на 1 и рубины на 3";
			charImg.src = "src/HealthPotion.png";
			break;
		case 1:
			charName.innerHTML = "Урон всем врагам";
			charInfo.innerHTML = "Наносит " + lStorage.getItem("Ability1Value") + " урона всем врагам на карте</br>Перезарядка: 14</br>Монетки перезаряжают на 1 и рубины на 3";
			charImg.src = "src/ActiveSword.png";
			break;
	}
	upgradePriceText.innerHTML = "Цена улучшения: " + (10000 + 5000 * (parseInt(lStorage.getItem("Ability" + abilityIndex + "Lvl")) - 1));

	abilityLvlText.innerHTML = "Уровень " + lStorage.getItem("Ability" + abilityIndex + "Lvl");

	lStorage.setItem('abilityIndex', abilityIndex);
}

function leftClicked() {
	if (abilityIndex-1 >= 0) {
		abilityIndex--;
		indexChanged();
	}
	else {
		abilityIndex = 1;
		indexChanged();
	}
}

function rightClicked() {
	if (abilityIndex+1 <= 1) {
		abilityIndex++;
		indexChanged();
	}
	else {
		abilityIndex = 0;
		indexChanged();
	}
}

menuBtn.addEventListener('click', function () {
	var lStorage = window.localStorage;
	lStorage.setItem('abilityIndex', abilityIndex);
});

leftBtn.addEventListener('click', leftClicked);

rightBtn.addEventListener('click', rightClicked);

onkeydown = function(key) {
	if (key.keyCode == 65 || key.keyCode == 37) { // Влево
		leftClicked();
	}
	else if (key.keyCode == 39 || key.keyCode == 68) { // Вправо
		rightClicked();
	}
}

indexChanged();
coinsText.innerHTML = "Монеты: " + coins;

upgradeBtn.addEventListener("click", function () {
	var lStorage = window.localStorage;
	if (coins >= 10000 + 5000 * (parseInt(lStorage.getItem("Ability" + abilityIndex + "Lvl")) - 1)) {
		coins -= 10000 + 5000 * (parseInt(lStorage.getItem("Ability" + abilityIndex + "Lvl")) - 1);
		lStorage.setItem("Ability" + abilityIndex + "Lvl", parseInt(lStorage.getItem("Ability" + abilityIndex + "Lvl"))+1);
		lStorage.setItem("Ability" + abilityIndex + "Value", parseInt(lStorage.getItem("Ability" + abilityIndex + "Value"))+1);
		lStorage.setItem("coins", coins);
		coinsText.innerHTML = "Монеты: " + coins;
		indexChanged();
	}
});

upgradePriceText.innerHTML = "Цена улучшения: " + (10000 + 5000 * (parseInt(window.localStorage.getItem("Ability" + abilityIndex + "Lvl")) - 1));