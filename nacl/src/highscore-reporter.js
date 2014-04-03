define([
	'deps/pouchdb',
], function(
	PouchDB
) {
	function initializeDb(centralCouch) {
		var highscores = new PouchDB('highscores', { cache: true });
		var localHighscores = new PouchDB('localHighscores');

		var remoteDatabase = centralCouch + "snygg/";

		function syncError() {
			console.log("syncError");
		}

		function sync() {
			var opts = { continuous: true, complete: syncError, cache:true };
			highscores.replicate.to(remoteDatabase, opts);
		}

		sync();

		return {
			"highscores": highscores,
			"localHighscores": localHighscores
		};
	}

	function HighscoreReporter(centralCouch) {
		if (!this instanceof HighscoreReporter) return new HighscoreReporter(centralCouch);
		this.db = initializeDb(centralCouch);
		this.usernameMapper = null;
	}

	HighscoreReporter.prototype.registerScore = function (board, score) {
		this.db.localHighscores.post({
			"timestamp": (new Date()).toISOString(),
			"board": board,
			"score": score
		});
	};

	HighscoreReporter.prototype.loggedIn = function (username) {
		if (this.usernameMapper) {
			console.error("HighscoreReporter.loggedIn called when already logged in!");
			this.loggedOut();
		}

		this.usernameMapper = this.db.localHighscores.changes({
			continuous: true,
			include_docs: true,
			onChange: function (change) {
				var doc = change.doc;
				if (doc._deleted) return;

				this.db.highscores.put({
					_id: doc._id,
					name: username,
					board: doc.board,
					score: doc.score,
					timestamp: doc.timestamp
				}, function (err, status) {
					if (err && err.status !== 409) return console.error(err);
					this.db.localHighscores.remove(doc);
				}.bind(this));
			}.bind(this)
		});
	};

	HighscoreReporter.prototype.loggedOut = function () {
		if (this.usernameMapper) this.usernameMapper.cancel();
		this.usernameMapper = null;
	};

	return HighscoreReporter;
});
