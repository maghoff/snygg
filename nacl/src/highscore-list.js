define([
	'ajax',
], function (
	ajax
) {
	function HighscoreList(dom, centralCouch, gravatarProvider) {
		if (!this instanceof HighscoreList) return new HighscoreList(dom, centralCouch);

		this.dom = dom;
		this.centralCouch = centralCouch;
		this.gravatarProvider = gravatarProvider;
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
			limit: 10
		};

		var queryArgs = "";
		for (key in opts) {
			queryArgs += "&" + encodeURIComponent(key) + "=" + encodeURIComponent(opts[key])
		}
		queryArgs = queryArgs.substr(1);
		ajax({
			url: this.centralCouch + "personal_records/_design/snygg/_view/highscore_lists?" + queryArgs
		}, function (err, data) {
			if (this.desiredBoard !== board) return;
			this.clear();
			if (err) return console.error(err);

			var tbody = this.dom.getElementsByTagName("tbody")[0];
			var prevScore = null;
			for (var i = 0; i < data.rows.length; ++i) {
				var value = {
					name: data.rows[i].value[0],
					score: data.rows[i].value[1],
					timestamp: data.rows[i].value[2]
				};

				var row = document.createElement("tr");

				var rank = document.createElement("td");
				rank.classList.add("rank-col");
				if (value.score !== prevScore) {
					rank.textContent = (i+1) + ".";
					prevScore = value.score;
				}
				row.appendChild(rank);

				var player = document.createElement("td");
				player.classList.add("player-col");
				player.textContent = value.name;
				var gravatar = (function () {
					var gravatar = document.createElement("img");
					gravatar.classList.add("highscore-gravatar");
					gravatar.setAttribute("src", "http://www.gravatar.com/avatar/?d=mm&s=50");
					this.gravatarProvider.get(value.name, function (gravatarUrl) {
						gravatar.setAttribute("src", gravatarUrl + "&s=50");
					});
					return gravatar;
				}.bind(this)());
				player.insertBefore(gravatar, player.firstChild);
				row.appendChild(player);

				var score = document.createElement("td");
				score.classList.add("score-col");
				score.textContent = value.score;
				row.appendChild(score);

				tbody.appendChild(row);
			}
		}.bind(this));
	};

	return HighscoreList;
});
