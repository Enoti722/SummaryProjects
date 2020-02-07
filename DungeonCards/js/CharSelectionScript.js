var charIndex = parseInt(window.localStorage.getItem("charIndex"));
var coins = parseInt(window.localStorage.getItem("coins"));

var charName = document.getElementById("CharName");
var charInfo = document.getElementById("CharInfo");
var leftBtn = document.getElementById("leftBtn");
var rightBtn = document.getElementById("rightBtn");
var charImg = document.getElementById("CharImg");
var menuBtn = document.getElementById("MenuBtn");
var coinsText = document.getElementById("Coins");
var charLvlText = document.getElementById("CharLvl");
var upgradeBtn = document.getElementById("UpgradeBtn");
var upgradePriceText = document.getElementById("UpgradePrice");

function indexChanged() {
	switch(charIndex) {
		case 0:
			charName.innerHTML = "D.Va";
			charInfo.innerHTML = "Персонаж Overwatch D.Va</br>Способность: При смерти выпрыгивает из робота";
			charImg.src = "src/DVaChar.gif";
			break;
		case 1:
			charName.innerHTML = "JunkRat";
			charInfo.innerHTML = "Персонаж Overwatch JunkRat</br>Способность: Если у него остаётся 1 очко здоровья - убивает врага без траты здоровья и распугивает всех остальных врагов";
			charImg.src = "src/JunkRatChar.gif";
			break;
	}
	var lStorage = window.localStorage;
	lStorage.setItem('charIndex', charIndex);

	charLvlText.innerHTML = "Уровень " + lStorage.getItem("Char" + charIndex + "Lvl");
}

function leftClicked() {
	if (charIndex-1 >= 0) {
		charIndex--;
		indexChanged();
	}
	else {
		charIndex = 1;
		indexChanged();
	}
}

function rightClicked() {
	if (charIndex+1 <= 1) {
		charIndex++;
		indexChanged();
	}
	else {
		charIndex = 0;
		indexChanged();
	}
}

menuBtn.addEventListener('click', function () {
	var lStorage = window.localStorage;
	lStorage.setItem('charIndex', charIndex);
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
	if (coins >= 10000 + 5000 * (parseInt(lStorage.getItem("Char" + charIndex + "Lvl")) - 1)) {
		coins -= 10000 + 5000 * (parseInt(lStorage.getItem("Char" + charIndex + "Lvl")) - 1);
		lStorage.setItem("Char" + charIndex + "Lvl", parseInt(lStorage.getItem("Char" + charIndex + "Lvl")) + 1);
		lStorage.setItem("Char" + charIndex + "Healths", parseInt(lStorage.getItem("Char" + charIndex + "Healths")) + 1);
		upgradePriceText.innerHTML = "Цена улучшения: " + (10000 + 5000 * (parseInt(lStorage.getItem("Char" + charIndex + "Lvl")) - 1));
		lStorage.setItem("coins", coins);
		coinsText.innerHTML = "Монеты: " + coins;
		indexChanged();
	}
});

upgradePriceText.innerHTML = "Цена улучшения: " + (10000 + 5000 * (parseInt(window.localStorage.getItem("Char" + charIndex + "Lvl")) - 1));