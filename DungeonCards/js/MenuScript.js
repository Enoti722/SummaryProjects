var lStorage = window.localStorage;

var RecordsTableSize = 10;

if (lStorage.getItem("firstRun") == null) {
	lStorage.setItem("firstRun", false);
	lStorage.setItem("charIndex", 0);
	lStorage.setItem("abilityIndex", 0);
	lStorage.setItem("record3x3", 0);
	lStorage.setItem("record5x5", 0);
	lStorage.setItem("coins", 0);

	for (i = 0; i < RecordsTableSize; i++) {
		lStorage.setItem("Record3x3Nick_" + i, "NoBody");
		lStorage.setItem("Record3x3Value_" + i, "0");
		lStorage.setItem("Record5x5Nick_" + i, "NoBody");
		lStorage.setItem("Record5x5Value_" + i, "0");
	}

	lStorage.setItem("Ability0Lvl", 1);
	lStorage.setItem("Ability0Value", 10);
	lStorage.setItem("Ability1Lvl", 1);
	lStorage.setItem("Ability1Value", 3);
	lStorage.setItem("Char0Lvl", 1);
	lStorage.setItem("Char0Healths", 10);
	lStorage.setItem("Char1Lvl", 1);
	lStorage.setItem("Char1Healths", 10);
}

var PlayBtn3 = document.getElementById("playBtn3");
var PlayBtn5 = document.getElementById("playBtn5");

PlayBtn3.addEventListener("click", function () {
	lStorage.setItem("curMode", 3);
	document.location.href = "index.html";
});

PlayBtn5.addEventListener("click", function () {
	lStorage.setItem("curMode", 5);
	document.location.href = "index.html";
});