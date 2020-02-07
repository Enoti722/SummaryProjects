var table3 = document.getElementById("Table3x3");
var table5 = document.getElementById("Table5x5");

var TableSize = 10;

var lStorage = window.localStorage;

table3.innerHTML = "";
table5.innerHTML = "";

table3.innerHTML += "<tr><th>NickName</th><th>Record</th></tr>"
table5.innerHTML += "<tr><th>NickName</th><th>Record</th></tr>"
for (i = 0; i < TableSize; i++) {
	table3.innerHTML += "<tr><td>" + lStorage.getItem("Record3x3Nick_" + i) + "</td><td>" + lStorage.getItem("Record3x3Value_" + i) + "</td></tr>"
	table5.innerHTML += "<tr><td>" + lStorage.getItem("Record5x5Nick_" + i) + "</td><td>" + lStorage.getItem("Record5x5Value_" + i) + "</td></tr>"
}