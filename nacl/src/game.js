define([], function () {
	function Game(dom, listener) {
		if (!this instanceof Game) return new Game(dom, listener);

		this.dom = dom;
		this.listener = listener;

		dom.addEventListener('loadstart', this.loadstart.bind(this));
		dom.addEventListener('progress', this.progress.bind(this));
		dom.addEventListener('load', this.load.bind(this));

		dom.addEventListener('error', this.displayLastError.bind(this));
		dom.addEventListener('abort', this.displayLastError.bind(this));

		dom.addEventListener('crash', this.crash.bind(this));

		dom.addEventListener('message', this.message.bind(this));

		console.log("Initial readyState for nacl object:", dom.readyState);
		if (dom.readyState >= 1) this.loadstart();
		if (dom.readyState === 4) setTimeout(this.load.bind(this), 0);
	}

	Game.prototype.loadstart = function () {
		this.listener.status("loading");
	};

	Game.prototype.progress = function (ev) {
		var status = "loading";
		if (!ev.lengthComputable && this.dom.getAttribute("data-size")) {
			ev = {
				loaded: ev.loaded,
				lengthComputable: true,
				total: parseInt(this.dom.getAttribute("data-size"), 10)
			};
		}
		if (ev.lengthComputable) {
			var progressPercentage = ev.loaded / ev.total * 100;
			status = "<span class='progressbar'><span class='progress' style='width: " + progressPercentage + "%'></span></span>";
		}
		this.listener.status(status, true);
	};

	Game.prototype.load = function () {
		console.log("nacl object finished loading");
		this.listener.status("loading");
		this.listener.didLoad();
	};

	Game.prototype.displayLastError = function () {
		this.listener.status(this.dom.lastError);
	}

	Game.prototype.crash = function () {
		var status;
		if (this.dom.exitStatus === 0) status = "terminated";
		else status = "crashed with exit code " + this.dom.exitStatus;
		this.listener.status(status);
	};

	Game.prototype.message = function (ev) {
		var msg = JSON.parse(ev.data);
		if (msg.what === "status") {
			if (msg.status === "running") {
				this.listener.status('by <a href="http://magnushoff.com/">Magnus Hoff</a>', true);
			}
		} else if (msg.what === "died") {
			this.listener.died(msg.board, msg.score);
		} else if (msg.what === "score_updated") {
			this.listener.score(msg.score);
		}
	};

	Game.prototype.loadBoard = function (boardName) {
		this.dom.postMessage(boardName);
	};

	return Game;
});
