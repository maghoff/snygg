function initializeDb() {
	var highscores = new PouchDB('highscores', { cache: true });

	var remoteCouch = 'http://db.magnushoff.com/snygg/';

	function syncError() {
		console.log("syncError");
	}

	function sync() {
		var opts = { continuous: true, complete: syncError, cache:true };
		highscores.replicate.to(remoteCouch, opts);
	}

	sync();

	return {
		"highscores": highscores
	};
}


function installApp() {
	var db = initializeDb();

	var boardSelector = document.getElementById("board");

	function loadBoard() {
		var board = window.location.hash.substr(1);
		if (!boardSelector.namedItem(board)) {
			if (board) {
				window.history.pushState(null, "Snygg: Pill", "#pill");
			}
			board = "pill";
		}
		boardSelector.namedItem(board).setAttribute("selected", "selected");
		if (snygg) snygg.postMessage(board);
	}
	loadBoard();
	window.addEventListener('hashchange', loadBoard);

	boardSelector.addEventListener('change', function () {
		var boardItem = boardSelector.options[boardSelector.selectedIndex];
		window.history.pushState(
			null,
			"Snygg: " + boardItem.name,
			"#" + boardItem.value
		);
		loadBoard();
	});


	var snygg = document.getElementById("snygg");

	snygg.addEventListener('loadstart', function () {
		document.getElementById('statusField').textContent = "loading";
	});

	snygg.addEventListener('progress', function (ev) {
		var status = "loading";
		if (ev.lengthComputable) {
			var progressPercentage = ev.loaded / ev.total * 100;
			status = "loading<br/><span class='progressbar'><span class='progress' style='width: " + progressPercentage + "%'></span></span>";
		}
		document.getElementById('statusField').innerHTML = status;
	});

	snygg.addEventListener('load', function () {
		document.getElementById('statusField').textContent = "loading";
		loadBoard();
	});

	function displayLastError() {
		document.getElementById('statusField').textContent = snygg.lastError;
	}

	snygg.addEventListener('error', displayLastError);
	snygg.addEventListener('abort', displayLastError);

	snygg.addEventListener('crash', function () {
		var status;
		if (snygg.exitStatus === 0) status = "terminated";
		else status = "crashed with exit code " + snygg.exitStatus;
		document.getElementById('statusField').textContent = status;
	});

	snygg.addEventListener('message', function (ev) {
		var msg = JSON.parse(ev.data);
		if (msg.what === "status") {
			if (msg.status === "running") {
				document.getElementById('statusField').innerHTML = 'by <a href="http://magnushoff.com/">Magnus Hoff</a>';
			}
		} else if (msg.what === "died") {
			// TODO: Commit highscore to database
			/*db.highscores.post({
				"userId": "mag",
				"board": msg.board,
				"score": msg.score
			});*/
		} else if (msg.what === "score_updated") {
			document.getElementById("score").textContent = msg.score;
		}
	});

	function initializeFullscreen(keyCode, box) {
		function toggleFullscreen() {
			var isFullscreen =
				document.fullscreenEnabled ||
				(document.fullScreenElement && document.fullScreenElement !== null) ||
				document.mozFullScreen ||
				document.webkitIsFullScreen;

			if (!isFullscreen) {
				var requestFullscreen =
					box.requestFullscreen ||
					box.mozRequestFullScreen ||
					box.webkitRequestFullscreen;
				requestFullscreen.call(box);
			} else {
				var exitFullscreen =
					document.exitFullscreen ||
					document.mozCancelFullScreen ||
					document.webkitExitFullscreen;
				exitFullscreen.call(document);
			}
		}

		box.addEventListener('keydown', function (ev) {
			if (ev.keyCode === keyCode) {
				ev.preventDefault();
				toggleFullscreen();
			}
		});
	}

	initializeFullscreen(70, document.getElementById("fullscreenbox"));
}

document.addEventListener("DOMContentLoaded", installApp);
