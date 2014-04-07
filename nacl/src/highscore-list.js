define([
	'ajax',
], function (
	ajax
) {
	function encodeQueryArgs(opts) {
		var queryArgs = "";
		for (key in opts) {
			queryArgs += "&" + encodeURIComponent(key) + "=" + encodeURIComponent(opts[key])
		}
		return queryArgs.substr(1);
	}

	function findIndex(array, predicate) {
		for (var index = 0; index < array.length; ++index) {
			if (predicate(array[index], index, array)) return index;
		}
		return -1;
	}


	function HighscoreList(dom, centralCouch, gravatarProvider) {
		if (!this instanceof HighscoreList) return new HighscoreList(dom, centralCouch);

		this.dom = dom;
		this.centralCouch = centralCouch;
		this.gravatarProvider = gravatarProvider;
		this.limit = 10;
		this.entries = [];
		this.activeLongPoll = null;
	}

	HighscoreList.prototype.clear = function () {
		var tbody = this.dom.getElementsByTagName("tbody")[0];
		while (tbody.firstChild) tbody.lastChild.remove();
	};

	HighscoreList.prototype.render = function () {
		this.clear();

		var tbody = this.dom.getElementsByTagName("tbody")[0];
		var prevScore = null;
		this.entries.slice(0, this.limit).forEach(function (value, i) {
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
		}.bind(this));
	};

	HighscoreList.prototype.load = function (board) {
		this.desiredBoard = board;
		this.clear();

		var opts = {
			startkey: JSON.stringify([board]),
			endkey: JSON.stringify([board, {}]),
			inclusive_end: false,
			limit: this.limit,
			update_seq: true
		};

		ajax({
			url: this.centralCouch + "personal_records/_design/snygg/_view/highscore_lists?" + encodeQueryArgs(opts)
		}, function (err, data) {
			if (this.desiredBoard !== board) return;
			this.clear();
			if (err) return console.error(err);

			this.entries = data.rows.map(function (row) {
				return {
					name: row.value[0],
					score: row.value[1],
					timestamp: row.value[2]
				};
			});
			this.render();

			if (data.update_seq) {
				// Waiting for Cloudant to merge in this feature
				this.longPoll(data.update_seq);
			} else {
				ajax({
					url: this.centralCouch + "personal_records"
				}, function (err, data) {
					if (err) {
						console.error("Failed to get update_seq for long polling:", err);
						return;
					}
					this.longPoll(data.update_seq);
				}.bind(this));
			}
		}.bind(this));
	};

	HighscoreList.prototype.applyScore = function (score) {
		var existingIndex = findIndex(this.entries, function (old) { return old.name === score.name; });
		if (existingIndex !== -1) {
			if (this.entries[existingIndex].score >= score.score) return -1;
			this.entries.splice(existingIndex, 1);
		}

		var newIndex = findIndex(this.entries, function (old) { return old.score < score.score; });
		if (newIndex === -1) newIndex = this.entries.length;

		if (newIndex >= this.limit) return -1;

		this.entries.splice(newIndex, 0, score);
		return newIndex;
	};

	HighscoreList.prototype.longPoll = function (update_seq) {
		var opts = {
			feed: "longpoll",
			filter: "snygg/highscores",
			board: this.desiredBoard,
			include_docs: true,
			since: update_seq,
			timeout: 60000
		};

		if (this.entries.length >= this.limit) {
			// Don't poll for scores that won't show up:
			opts.over = this.entries[this.limit-1].score;
		}

		if (this.activeLongPoll) this.activeLongPoll.abort();

	   this.activeLongPoll = ajax({
			url: this.centralCouch + "personal_records/_changes?" + encodeQueryArgs(opts),
			timeout: 70000
		}, function (err, data) {
			if (err) {
				if (err.status === 500) console.log("Long polling likely aborted: ", err);
				else console.error("Terminating long polling due to unexpected error:", err);
				return;
			}

			data.results.forEach(function (result) {
				if (result.doc.key[1] !== this.desiredBoard) return;
				var score = {
					name: result.doc.key[0],
					score: -result.doc.value[0],
					timestamp: result.doc.value[1]
				};
				this.applyScore(score);
			}.bind(this));
			this.render();

			this.activeLongPoll = null;
			this.longPoll(data.last_seq);
		}.bind(this));
	};

	return HighscoreList;
});
