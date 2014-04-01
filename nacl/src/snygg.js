require({
	shim: {
		"md5": { exports: "md5" },
		"sha1": { exports: "CryptoJS" }
	}
}, [
	'login',
	'highscore-reporter',
	'board-selector',
	'fullscreen',
	'deps/pouchdb',
	'deps/domReady!'
], function(
	Login,
	HighscoreReporter,
	BoardSelector,
	initializeFullscreen,
	PouchDB
) {
	var centralCouch = 'https://mag.cloudant.com/';

	function installApp() {
		var highscoreReporter = new HighscoreReporter(centralCouch);
		var login = new Login(document.getElementById("login-interaction"), centralCouch, highscoreReporter);
		var boardSelector =
			new BoardSelector(
				document.getElementById("board"),
				{
					boardChanged: function (board) { if (snygg) snygg.postMessage(board); }
				}
			);

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
			boardSelector.loadBoard();
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
				if (msg.score > 0) highscoreReporter.registerScore(msg.board, msg.score);
			} else if (msg.what === "score_updated") {
				document.getElementById("score").textContent = msg.score;
			}
		});

		var keyCodeForF = 70;
		initializeFullscreen(keyCodeForF, document.getElementById("fullscreenbox"));
	}

	installApp();
});
