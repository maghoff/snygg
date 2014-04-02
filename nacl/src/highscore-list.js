define([
	'ajax',
], function (
	ajax
) {
	function HighscoreList(dom, centralCouch) {
		if (!this instanceof HighscoreList) return new HighscoreList(dom, centralCouch);

		this.dom = dom;
		this.centralCouch = centralCouch;
	}

	HighscoreList.prototype.clear = function () {
		var tbody = this.dom.getElementsByTagName("tbody")[0];
		while (tbody.firstChild) tbody.lastChild.remove();
	};

	HighscoreList.prototype.load = function (board) {
		this.desiredBoard = board;
		this.clear();

		var opts = {
			startkey: JSON.stringify([board]),
			endkey: JSON.stringify([board, {}]),
			inclusive_end: false,
			reduce: true,
			group_level: 2,
			limit: 10
		};

		var queryArgs = "";
		for (key in opts) {
			queryArgs += "&" + encodeURIComponent(key) + "=" + encodeURIComponent(opts[key])
		}
		queryArgs = queryArgs.substr(1);

		ajax({
			url: this.centralCouch + "snygg/_design/snygg/_view/highscores?" + queryArgs
		}, function (err, data) {
			if (this.desiredBoard !== board) return;
			this.clear();
			if (err) return console.error(err);

			var tbody = this.dom.getElementsByTagName("tbody")[0];
			for (var i = 0; i < data.rows.length; ++i) {
				var row = document.createElement("tr");

				var rank = document.createElement("td");
				rank.textContent = (i+1) + ".";
				row.appendChild(rank);

				var player = document.createElement("td");
				player.textContent = data.rows[i].value.name;
				row.appendChild(player);

				var score = document.createElement("td");
				score.textContent = data.rows[i].value.score;
				row.appendChild(score);

				tbody.appendChild(row);
			}
		}.bind(this));
	};

	return HighscoreList;
});
